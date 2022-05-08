#include "../../archiver/archiver.h"
#include <sstream>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <filesystem>

int main()
{
    std::stringstream str; //< this is the file content

    //< generate a very random string for file
    str << "Its a highly random string which is produced like d!@#$%^&*()_+q w e r t y u i o [   ]{}|//?/.,MJDGFNEf  LEJFVJ    rrksneng'kn4g[0909u54pinrgb'omngb'-o=o```[3pk//\\mdfv;lmlfv";
    //< add even more randomness
    for (int i = 0; i < 256; i++)
        str << (char)i;
    //< size of file
    const size_t str_size = str.str().size();
    //< generate a temporary file
    std::string path = std::tmpnam(nullptr);
    std::ofstream stream{path, std::ios::binary};
    if (stream.is_open())
    {
        stream << str.rdbuf();
        stream.close();
    }
    else
    {
        std::cout << "Fail. Unable to open stream";
        return -1;
    }
    size_t crc_val_array = archiver::archiver::crc32b(str.str().data(), str_size);
    std::cout << "Array based crc32 value: " << crc_val_array << '\n'
              << std::flush;
    size_t crc_val_file = archiver::archiver::crc32b(path);
    std::cout << "File based crc32 value:  " << crc_val_file << '\n'
              << std::flush;

    if (crc_val_array == crc_val_file)
    {
        std::cout << "Pass";
        return 0;
    }
    else
    {
        std::cout << "Fail";
        return -1;
    }
}