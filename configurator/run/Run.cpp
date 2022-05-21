#include "Run.h"
#if defined(_WIN32) || defined(__CYGWIN__)


bool Runner::RunScript::Run(const std::filesystem::path &PathToScript)
{
    std::string line = "cmd.exe /c \"" + PathToScript.string() + "\" > log.txt";
    if(system(line.c_str())) {
        return true;
    }
    return false;
}
#endif

#if defined(unix) || defined(__unix__) || defined(__unix)

#include <stdlib.h>

bool Runner::RunScript::Run(const std::filesystem::path &PathToScript)
{
    if(system(PathToScript.c_str())) {
        return true;
    }
    return false;
}

#endif

bool Runner::Script::Execute() {
    if(!stream.is_open())
        return false;
    stream.close();
    //on linux set +x perm
    std::filesystem::permissions(_Path, 
                                std::filesystem::perms::owner_exec,
                                std::filesystem::perm_options::add);
    return  Runner::RunScript::Run(_Path) && std::filesystem::remove(_Path);
}