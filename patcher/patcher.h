#ifndef PATCHER_H
#define PATCHER_H

#include <filesystem>
#include <vector>
#include <string>

#include "../archiver/archiver.h"
#include "../external_libs/lib_lz4/lz4file.h"
#include "../shared/path_lib.h"
#include "archive.h"

namespace Unpacker
{
    class Unpacker
    {
    public:
        /**
         * @brief Structure describing uncompressed file and success of uncompress
         *
         */
        struct UnCompressedDataDefinition
        {
            size_t OriginSize = 0;        ///< uncompressed size
            std::unique_ptr<char[]> data; ///< uncompressed data
            unsigned int crc32;           ///< crc32 value (after decompression)
            bool IsCrcGood = false;       ///< is new crc32 equal to the known value (from compression step)
        };
        /**
         * @brief instantiates Unpacker object
         * @param user_path user path to the directory where relative path(coded with the data) is stored
         */
        Unpacker(const std::filesystem::path &user_path); // complete_path = user_path{C:/.../MatLab/} + relative_path{bin/...}

        /**
         * @brief unpacks compressed files
         * @param packedFilesHandler vector of structs describing compressed data-files
         */
        void Unpack();

    private:
        std::filesystem::path user_path{};

        /**
         * @brief decompress temprorary file to its new location
         * @param tmp compressed temporary file descriptor
         * @param file output stream binded to the decompressed location
         */
        void DecompressFile(std::ofstream &file, const char* src, size_t compressedSize, size_t originSize);

        /**
         * @brief handles a single compressed file: copies it from archive.c to temp and
         * calls DecompressFile()
         *
         * @param offset compressed file position at control[] of archive.c
         * @param size compressed file size
         * @param relative_path relative path to the patch file destination
         * @return complete path to the patched file on user computer
         */
        std::filesystem::path HandleFile(size_t offset, size_t size, size_t originSize, const char *relative_path);

        bool IsFileCorrect(const std::filesystem::path &path, const FileMemoryDefinition *compressedfile);

        void BackUpFile(const char *relative_path);
        void RestoreFile(const char *relative_path);
    };
} // Unpacker
#endif