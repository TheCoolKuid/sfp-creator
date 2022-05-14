#include "../../shared/path_lib.h"
#include <iostream>
#include <cstring>
int main(int argc, char** argv)
{
    std::filesystem::path path(argv[1]);
    std::filesystem::path true_path(argv[2]);
    for(int i = 4; i < argc; i++) {
        if(!strcmp(argv[3], "c"))
            path = PathUtils::Path::Append(path, argv[i]);
        if(!strcmp(argv[3], "s"))
            path = PathUtils::Path::Append(path, std::string(argv[i]));
        if(!strcmp(argv[3], "p"))
            path = PathUtils::Path::Append(path, std::filesystem::path(argv[i]));
    }

    if(path == true_path) {
        std::cout << "Pass";
        return 0;
    } else {
        std::cout << "Fail. True was:" << true_path << " Return:" << path;
        return -1;
    }
}