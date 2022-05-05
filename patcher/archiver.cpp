#include <vector>

#include "archiver.h"
#include "../archiver/archiver.h"

namespace Unpacker
{
    Unpacker::Unpacker(const std::filesystem::path &user_path) : user_path{user_path}
    {
    }

    std::vector<UnCompressedDataDefinition> Unpacker::Unpack(const std::vector<archiver::archiver::CompressedDataDefinition> &packedFilesHandler)
    {
        std::vector<UnCompressedDataDefinition> out;

        size_t files_count{packedFilesHandler.size()};

        /// сделай как надо!!!
        char *offset = (char *)const_cast<uint8_t *>(data);
        for (size_t i = 0; i < files_count; i++)
        {
            UnCompressedDataDefinition Data;

            size_t CompressedSize{packedFilesHandler[i].CompressedSize};
            size_t OriginSize{packedFilesHandler[i].OriginSize};

            Data.data =
                archiver::archiver::decompress(offset, CompressedSize, OriginSize);
            offset += CompressedSize;
            Data.crc32 = archiver::archiver::crc32b(Data.data.get(), OriginSize);
            Data.OriginSize = OriginSize;

            if (!packedFilesHandler[i].crc32 == Data.crc32)
            {
                Data.IsCrcGood = false;
                throw std::runtime_error("File crc32 value does not match original value!");
            }
            else
                Data.IsCrcGood = true;

            out.push_back(std::move(Data));
        }
    }
} // Unpacker