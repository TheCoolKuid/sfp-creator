#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>
#include <vector>

#define _SZ_ALLOC_DEBUG //  define it if you want to debug alloc/free operations to stderr.

namespace SFPConfig {
    /// this struct describes a single file that should be replaced
    struct FileReplacmentConfig {
        // get a file from here
        std::filesystem::path OnCreatorPath; // Creator creates a patch and describes where files are at Creator side
        // put the file at this address
        std::string RelativeUserPath; // where to store the file on unpack
    };


    struct CommonReplacementSetConfig
    {
        std::vector<FileReplacmentConfig> Files;

        /*
        1. loop through Files
        2. zip
        3. make a single file *.c


        */
       // Compression method          : LZMA (default), LZMA2, PPMD, BCJ, BCJ2, BZip2, Deflate    // https://www.7-zip.org/7z.html
       // Desired compression ratio   ?
       // Conversion method     
       // Encryption method           : AES-256 algorithm
       // File size                   ?
       // Archive headers compressing ?
       // File sorting order          : by type, by name(default - ispecially good for NTFS, where default sorting is by name)  
       // Dictionary size             : below 2Gb in x32, unlimited in x64. increasing the size one can improve compression ratio
       // Correct headers             ? ??           
       // Partly solid archive        ? If you have big number of files in archive, and you need fast extracting, you can use partly-solid archive                  

       // 7z can support any new compression method



    }
}

#endif