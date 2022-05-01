#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>
namespace SFPConfig {
    struct FileReplacmentConfig {
        std::filesystem::path OnCreatorPath;
        std::string RelativeUserPath;
    };
}

#endif