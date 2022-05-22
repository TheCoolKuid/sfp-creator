#include "patcher.h"

namespace Unpacker
{
    Unpacker::Unpacker(const std::filesystem::path &user_path) : user_path{user_path}
    {
    }

    void Unpacker::DecompressFile(FILE *tmp, std::ofstream &file)
    {
        if (!file.is_open()) //< the output file strem must be opened before writing into it
            throw std::runtime_error("The outpu file stream is not open. Cannot write to it.");

        LZ4F_errorCode_t ret = LZ4F_OK_NoError;
        LZ4_readFile_t *lz4fRead;

        constexpr uint16_t buf_size = 1024;
        auto buffer = std::make_unique<char[]>(buf_size);

        if (LZ4F_isError(LZ4F_readOpen(&lz4fRead, tmp)))
            throw std::invalid_argument("Cannot open buffer file in read mode.");

        size_t read_size = buf_size;
        size_t read_count = 0;
        while (read_size > 0) //< there is still something to read
        {
            read_size = LZ4F_read(lz4fRead, buffer.get(), buf_size);
            if (LZ4F_isError(ret))
                throw std::invalid_argument("Cannot read from the buffer file at position " + std::to_string(read_count) + ".");

            read_count += read_size;             //< this amont of bytes is already read
            file.write(buffer.get(), read_size); //< write the next chunk of bytes
        }
        // file.close(); //< it is not the responsibility of this methd to close the output stream
    }

    std::filesystem::path Unpacker::HandleFile(size_t offset, size_t size, const char *relative_path)
    {
        auto tmp = std::tmpfile(); //< create temprary file to move compressed data from archive.c
        if (fwrite(&data[0] + offset, 1, size, tmp) != size)
        {
            throw std::runtime_error("Unable to write compressed data at offset position = " + std::to_string(offset) + ".");
        }

        auto path = PathUtils::Path::Append(user_path, relative_path);
        //create all dir on path if its necessary
        auto dir_path = PathUtils::Path::GetDir(path);
        if(!std::filesystem::exists(dir_path)) {
            std::error_code perr;
            if(!std::filesystem::create_directories(dir_path, perr)) {
                throw std::runtime_error(
                    "unable to create directories at path:" + dir_path.string() + " Error:" + perr.message());
            }
        }

        std::ofstream out(path); //< where to write the decompssed file on disk

        DecompressFile(tmp, out);

        fclose(tmp);
        out.close();

        return path;
    }

    void Unpacker::Unpack()
    {
        namespace fs = std::filesystem;
        auto _arch = *(FileArchiveDefinition_t *)archive;
        auto files_count = _arch.SizeOfFMMArray; // number of compressed files
        auto size = _arch.SizeOfData;

        LibLog::LogEngine::LogConsoleInfo("Replacement begins");

        for (int i = 0; i < files_count; i++)
        {
            auto ptr = (FileMemoryDefinition *)control + i; // TODO: it was ARCHIVE here. I consider it an error

            LibLog::LogEngine::LogConsoleInfo("Processing file ", ptr->relative_path, " Creating backup");
            try
            {
                BackUpFile(ptr->relative_path); // TODO: backup each file in a temp folder with the same directory structure as the pached files
            }
            catch (std::exception err)
            {
                LibLog::LogEngine::LogConsoleError(
                    "File ",
                    ptr->relative_path,
                    err.what());
            }

            auto path = HandleFile(ptr->offset, ptr->compressedSize, ptr->relative_path);

            if (!IsFileCorrect(path, ptr))
            {
                try
                {
                    RestoreFile(ptr->relative_path);
                }
                catch (std::exception err)
                {
                    LibLog::LogEngine::LogConsoleError(
                        "File ",
                        ptr->relative_path,
                        err.what());
                }
                LibLog::LogEngine::LogConsoleError(
                    "File ",
                    ptr->relative_path,
                    " size or crc32 of original file and uncompressed file are not agree.");
            }
        }

        LibLog::LogEngine::LogConsoleInfo("Replacement completed");
    }

    bool Unpacker::IsFileCorrect(const std::filesystem::path &path, const FileMemoryDefinition *compressedFile)
    {

        return archiver::archiver::crc32b(path) == compressedFile->crc32 &&
               std::filesystem::file_size(path) == compressedFile->originSize;
    }

    void Unpacker::BackUpFile(const char *relative_path)
    {
        auto old_path = PathUtils::Path::Append(user_path, relative_path);
        auto new_path = PathUtils::Path::Append(old_path, ".bal");
        if (!std::filesystem::copy_file(old_path, new_path))
        {
            throw std::runtime_error("unable to create backup file");
        }
    }

    void Unpacker::RestoreFile(const char *relative_path)
    {
        auto origin = PathUtils::Path::Append(user_path, relative_path);
        auto backup = PathUtils::Path::Append(origin, ".bal");
        if (std::filesystem::exists(backup))
        {
            std::filesystem::copy_file(backup, origin);
            if (!std::filesystem::remove(backup))
            {
                throw std::runtime_error("unable to delete backup file");
            }
        }
        else
        {
            throw std::runtime_error("backup file not found");
        }
    }
} // Unpacker