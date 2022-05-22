#ifndef RUN_H
#define RUN_H
#include <fstream>
#include <filesystem>
#include <string>

#include "../../shared/liblog.h"
#include "../../shared/path_lib.h"

namespace Runner {
    class RunScript {
        public:
        static bool Run(const std::filesystem::path& PathToScript);
    };

    class Script {
        public:
            Script(const std::filesystem::path& PathToScript) : stream{PathToScript}, _Path{PathToScript} {}
            template<typename T> void AddCommand(T&& command) {
                stream << command << '\n';
            }
            bool Execute();
        private:
            std::ofstream stream;
            std::filesystem::path _Path;
    };

    class ISystemScript {
        public:
            virtual void Exec() = 0;
    };

    class BuildSystemScript : public ISystemScript {
        public:
            BuildSystemScript(const std::filesystem::path& Folder) : ScriptFolder{Folder} {}
            virtual void Exec();
        private:
            std::filesystem::path ScriptFolder;
    };
}
#endif

