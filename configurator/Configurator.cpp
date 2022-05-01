#include "../shared/liblog.h"
#include <filesystem>

int main(
    int argc,
    #ifdef __GNUC__
        char** argv
    #endif
    #ifdef _MSC_BUILD
        wchar_t** argv
    #endif
) {
    std::filesystem::path config_path;
    /* first arg may be json config file or dir
    *  if dir was specified sfp_config.json appends 
    *  if no arg was specified ./sfp_config.json assumes as config
    */
    if(argc > 1)
    {
        config_path = std::filesystem::path(argv[1]);
        if (std::filesystem::is_directory(config_path)) {
            config_path.append("sfp_config.json");
        }
    }else{
        config_path = std::filesystem::path("./sfp_config.json");
    }

    if(!std::filesystem::exists(config_path))
    {
        LOG_CONSOLE_ERROR("no config file was found.\n Use syntax:\n configurator <path_to_config>\n or create sfp_config.json in program dir");
        return -1;
    }

    LOG_CONSOLE_INFO("config file has found at " + config_path.string());

}