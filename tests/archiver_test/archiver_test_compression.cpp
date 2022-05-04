#include "../../archiver/archiver.h"
#include <sstream>
#include <cstdio>
#include <fstream>
#include <iostream>

int main() {
    std::stringstream test;

    std::string path = std::tmpnam(nullptr);

    const char* str = "Its a test file for compression";

    std::ofstream stream(path);
    if(stream.is_open()) {
        stream.write(str, 31);
        stream.close();
    }else{
        std::cout << "Fail. Unable to open stream";
        return -1;
    }

    auto compress = archiver::archiver::compress(path);
    auto decompress = archiver::archiver::decompress(compress.data.get(), compress.CompressedSize, compress.OriginSize);

    if (std::memcmp(str, decompress.get(), 31) == 0) {
        std::cout << "Pass";
        return 0;
    }else{
        std::cout << "Fail";
        return -1;
    }
}