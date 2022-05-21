#ifndef RUN_H
#define RUN_H
#include <fstream>
#include <filesystem>
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
}
#endif

