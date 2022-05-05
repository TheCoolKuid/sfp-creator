#include <cstdint>
#include <filesystem>
#include <vector>

#include "../archiver/archiver.h"

extern const std::uint8_t data[]; // vector of compressed bytes --- stack of compressed files
extern const uint8_t control[];   // vector of structs FileMemoryDefinition_t
extern const uint8_t archive[];   // vector of FileArchiveDefinition_t

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
        std::vector<UnCompressedDataDefinition> Unpack(const std::vector<archiver::archiver::CompressedDataDefinition> &packedFilesHandler);

    private:
        std::filesystem::path user_path{};
    };
} // Unpacker