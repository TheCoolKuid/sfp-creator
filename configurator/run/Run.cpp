#include "Run.h"
#if defined(_WIN32) || defined(__CYGWIN__)

bool Runner::RunScript::Run(const std::filesystem::path &PathToScript)
{
    std::string line = "cmd.exe /c \"" + PathToScript.string() + "\" > log.txt";
    int ret = 0;
    if (!(ret = system(line.c_str())))
    {
        return true;
    }
    LOG_CONSOLE_DEBUG("error during system call(). errno set to " + std::to_string(errno) + " ret was " + std::to_string(ret));
    return false;
}
#endif

#if defined(unix) || defined(__unix__) || defined(__unix)

#include <stdlib.h>

bool Runner::RunScript::Run(const std::filesystem::path &PathToScript)
{
    if (system(PathToScript.c_str()))
    {
        return true;
    }
    return false;
}

#endif

bool Runner::Script::Execute()
{
    if (!stream.is_open())
        return false;
    stream.close();
    // on linux set +x perm
    std::filesystem::permissions(_Path,
                                 std::filesystem::perms::owner_all,
                                 std::filesystem::perm_options::add);
    return Runner::RunScript::Run(_Path) && std::filesystem::remove(_Path);
}

void Runner::BuildSystemScript::Exec()
{
    Runner::Script script(PathUtils::Path::Append(ScriptFolder, "cmd.cmd"));
    script.AddCommand("cmake -S./patcher -B./patcher/build");
    script.AddCommand("cmake --build  ./patcher/build --config Debug");
    if (!script.Execute())
    {
        throw std::runtime_error("Some error during compilation was ocurred. Log file is available at ./log.txt");
    }
}