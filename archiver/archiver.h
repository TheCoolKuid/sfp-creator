#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <filesystem>
#include <ostream>

namespace archiver
{
    class archiver
    {
        public:
        /// converts a set of bytes into hexidecimal representation 0x11,0x12,0xA3 separated with commas 
        /// to intialize a const char data[] array
        static void compress(const std::filesystem::path& f_name, std::ostream& o);
    };
} // archiver

#endif ARCHIVER_H