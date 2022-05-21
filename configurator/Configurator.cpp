#include <filesystem>

#include "../shared/liblog.h"
#include "../shared/path_lib.h"
#include "../shared/utils.h"
#include "config/Config.h"
#include "run/Run.h"


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
        LibLog::LogEngine::LogConsoleWarn("Program runs with admin privileges. Hope you know what you doing");
    }

    std::filesystem::path config_path;
    /* first arg may be json config file or dir
     *  if dir was specified sfp_config.json appends
     *  if no arg was specified ./sfp_config.json assumes as config
     */
    if (argc > 1)
    {
        config_path = std::filesystem::path(argv[1]);
        if (std::filesystem::is_directory(config_path))
        {
            config_path = PathUtils::Path::Append(config_path, "sfp_config.json");
        }
    }
    else
    {
        config_path = std::filesystem::path("./sfp_config.json");
    }

    if (!std::filesystem::exists(config_path))
    {
        LibLog::LogEngine::LogConsoleError("no config file was found.\n Use syntax:\n configurator <path_to_config>\n or create sfp_config.json in program dir");
        return -1;
    }

    LibLog::LogEngine::LogConsoleInfo("config file found at ", config_path);
    LibLog::LogEngine::LogConsoleInfo("Reading config");

    SFPConfig::JSONConfigFiller parser(config_path);
    auto config = parser.Create();

    SFPConfig::RuleChecker checker;
    if(!checker.Check(config)) {
         LibLog::LogEngine::LogConsoleError("Invalid config. Please check and try again.");
         return -1;
    }

    LibLog::LogEngine::LogConsoleInfo("Config successfully read");
    LibLog::LogEngine::LogConsoleInfo("Packing");

    archiver::ArchiveBuilder builder;
    try {
        builder.AddFiles(config.files);
    } catch(const std::exception& err) {
        LibLog::LogEngine::LogConsoleError("Some errors during archivation occurred. Error:", err.what());
        return -1;
    }

    LibLog::LogEngine::LogConsoleInfo("Packing successfully done");
    LibLog::LogEngine::LogConsoleInfo("Building patch");

    Runner::Script script(PathUtils::Path::Append(PathUtils::Path::GetDir(argv[0]), "cmd.cmd"));
    script.AddCommand("cmake -S./patcher -B./patcher/build");
    script.AddCommand("cmake --build  ./patcher/build --config Debug");    
    if(script.Execute()) {
        LibLog::LogEngine::LogConsoleInfo("Building successful");
        return 0;
    } else {
        LibLog::LogEngine::LogConsoleError("error");
        return -1;
    }
}