#include "../../archiver/archiver.h"
#include <filesystem>
#include <cstdio>

int main() {
    std::filesystem::create_directory("./patch");

    auto archive = archiver::ArchiveBuilder();

    std::vector<archiver::ArchiveBuilder::FileReplacmentConfig> vec;

    for(int i = 0; i < 10; i++) {
        std::string path = std::tmpnam(nullptr);
        std::ofstream stream(path);
        stream << "just test string";
        
        archiver::ArchiveBuilder::FileReplacmentConfig conf;
        conf.OnCreatorPath = path;
        conf.RelativeUserPath = "/just/some/path";
        vec.push_back(conf);
    }

    archive.AddFiles(vec);

    std::cout << "Pass";
    return 0;
}