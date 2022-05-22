#ifndef CONFIGURATOR_UTILS_H
#define CONFIGURATOR_UTILS_H

#include <filesystem>

#include "../../shared/path_lib.h"

namespace ConfiguratorUtils
{
    class FSUtis
    {
        public:
            static bool CopyExecutable(
                const std::filesystem::path& exec_path,
                const std::filesystem::path& dest_dir,
                const std::filesystem::path& dest_name,
                std::error_code& perr);
    };    
} // namespace ConfiguratorUtils

#endif
