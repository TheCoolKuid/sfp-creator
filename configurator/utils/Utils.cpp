#include "Utils.h"

bool ConfiguratorUtils::FSUtis::CopyExecutable(
    const std::filesystem::path &exec_path,
    const std::filesystem::path &dest_dir,
    const std::filesystem::path &dest_name,
    std::error_code &perr)
{
    std::filesystem::path dest;
    dest = PathUtils::Path::Append(dest_dir, dest_name
#if defined(_WIN32) || defined(__CYGWIN__)
                                   ,
                                   ".exe"
#endif
    );

    auto result = std::filesystem::copy_file(
        exec_path,
        dest,
        std::filesystem::copy_options::overwrite_existing,
        perr);

#if defined(unix) || defined(__unix__) || defined(__unix)
    // on linux set +x perm
    std::filesystem::permissions(dest,
                                 std::filesystem::perms::owner_all |
                                     std::filesystem::perms::group_all |
                                     std::filesystem::perms::others_exec,
                                 std::filesystem::perm_options::add);
#endif

    return !perr;
}