#include <vector>
#include <exception>
#include <string>
#include <filesystem>

#include "archiver.h"
#include "../archiver/archiver.h"

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

        constexpr uint8_t buf_size = 1024;
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

        auto path{user_path.append(relative_path)};
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

        for (int i = 0; i < files_count; i++)
        {
            auto ptr = (FileMemoryDefinition *)control + i; // TODO: it was ARCHIVE here. I consider it an error

            BackUpFile(); // TODO: backup each file in a temp folder with the same directory structure as the pached files

            auto path = HandleFile(ptr->offset, ptr->compressedSize, ptr->relative_path);

            if (!IsFileCorrect(path, ptr))
            {
                throw std::runtime_error("File size or crc32 of original file and uncompressed file are not agree.");
            }
        }
    }

    bool Unpacker::IsFileCorrect(const std::filesystem::path &path, const FileMemoryDefinition *compressedFile)
    {

        return archiver::archiver::crc32b(path) == compressedFile->crc32 &&
               std::filesystem::file_size(path) == compressedFile->originSize;
    }

} // Unpacker