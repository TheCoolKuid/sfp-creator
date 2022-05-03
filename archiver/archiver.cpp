
#include <filesystem>
#include <exception>
#include <memory>
#include <fstream>

#include "archiver.h"
#include "../lz4/lz4/lib/lz4.h"

namespace archiver
{
    void archiver::compress(const std::filesystem::path &f_name, std::ostream &o)
    {
        namespace fs = std::filesystem;

        // check whether file exists or not
        if (!fs::exists(f_name))
        {
            throw std::invalid_argument(
                "File " +
                f_name.generic_string() +
                " does not exist.");
        }
        // file size in bytes
        size_t f_size{fs::file_size(f_name)}; 

        auto f_content = std::make_unique<char[]>(f_size);

        std::ifstream stream{f_name};
        if (!stream.is_open())
        {
            throw std::runtime_error(
                "Could not open stream to read "
                "file " +
                f_name.generic_string() +
                ".");
        }

        stream.read(f_content.get(), f_size);

        stream.close(); // close stream

        const int max_dist_size = LZ4_compressBound(f_size); // the maximum possible theoretical size after compression

        auto compressed_data = std::make_unique<char[]>(max_dist_size);

        // assume we are good with memory

        // now we compress and return the final size
        const int compressed_data_size = LZ4_compress_default(f_content.get(), compressed_data.get(), f_size, max_dist_size);
        f_content.release();

        o << '0x' << std::hex << (int)compressed_data[0];
        for (int i = 0; i < compressed_data_size; i++)
            o << ',' << '0x' << std::hex << (int)compressed_data[i];
    }
}