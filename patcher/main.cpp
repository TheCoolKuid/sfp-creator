#include <iostream>
#include <filesystem>

#include "patcher.h"
#include "../shared/utils.h"

int
#if defined(unix) || defined(__unix__) || defined(__unix)
    main
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
    wmain
#endif
    (
        int argc,
#if defined(unix) || defined(__unix__) || defined(__unix)
        char
#endif
#if defined(_WIN32) || defined(__CYGWIN__)
        wchar_t
#endif
        **argv)
{
    if(HasAdminRights()) {
        LibLog::LogEngine::LogConsoleWarn("Run with admin priviledged is forbidden.");
        return -1;
    }
    if(argc != 2) {
        LibLog::LogEngine::LogConsoleError("Not enough arguments. usage : patcher <relative path>");
        return -1;
    }

    std::filesystem::path path{argv[1]};

    LibLog::LogEngine::LogConsoleInfo("Relative path is ", path);
    LibLog::LogEngine::LogConsoleInfo("Unpacking");

    Unpacker::Unpacker ArchiveHandler{path};
    ArchiveHandler.Unpack();

    LibLog::LogEngine::LogConsoleInfo("Unpacking done");
    return 0;
}