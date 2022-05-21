#include <iostream>
#include <filesystem>

#include "patcher.h"

int main(int argc, char **argv)
{
    std::filesystem::path path{argv[1]};

    std::cout << path << '\n'
              << std::flush;

    Unpacker::Unpacker ArchiveHandler{path};
    ArchiveHandler.Unpack();

    std::cout << "Pass";
    return 0;
}