#ifndef LIBLOG_H
#define LIBLOG_H

#include <ostream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <assert.h>

#define LOG_CONSOLE_INFO(string)                                                                \
do {                                                                                            \
    LibLog::LogEngine::Log(std::cout, "[",  LibLog::LogEngine::GetISOTime(), "] ", string);        \
} while(0)

#define LOG_CONSOLE_WARNING(string)                                                             \
do {                                                                                            \
    LibLog::LogEngine::Log(std::cout, "[",  LibLog::LogEngine::GetISOTime(), "] [WARN] ", string);  \
} while(0)

#define LOG_CONSOLE_ERROR(string)                                                               \
do {                                                                                            \
    LibLog::LogEngine::Log(std::cerr, "[",  LibLog::LogEngine::GetISOTime(), "] [ERROR] ", string); \
} while(0)

#ifdef NDEBUG
    #define LOG_CONSOLE_DEBUG(string) ((void)0)
#else
    #define LOG_CONSOLE_DEBUG(string)                       \
    do {                                                    \
        LibLog::LogEngine::Log(std::cout,                   \
        "[",  LibLog::LogEngine::GetISOTime(), "] [DEBUG] ",\
        "File ", __FILE__ ,                                 \
        " Line ", __LINE__,                                 \
        " Msg: ", string);                                  \
    } while (0)
#endif

namespace LibLog{
    class LogEngine {
        public:
            template<typename T, typename... TArgs> static void Log(std::ostream& stream, T val, TArgs... args) {
                stream << val;
                Log(stream, args...);
            }   

            static std::string GetISOTime() {
                auto now_t = std::time(nullptr);;
                auto now_tm = std::localtime(&now_t);
                char buffer[256];
                std::strftime(buffer, 256, "%F %T", now_tm);
                return buffer;
            }

        private:
             static void Log(std::ostream& stream) {
                 stream << "\r\n";
             }
    };
}
#endif //LIBLOG_H