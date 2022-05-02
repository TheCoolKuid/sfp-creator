#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>
namespace SFPConfig {
    /// 
    struct FileReplacmentConfig {
        // get a file from here
        std::filesystem::path OnCreatorPath; // Creator creates a patch and describes where files are at Creator side
        // put the file at this address
        std::string RelativeUserPath; // where to store the file on unpack
    };
}

#endif