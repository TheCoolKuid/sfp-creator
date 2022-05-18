#ifndef LIBLOG_H
#define LIBLOG_H

#include <ostream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <assert.h>

#ifdef NDEBUG
#define LOG_CONSOLE_DEBUG(string) ((void)0)
#else
#define LOG_CONSOLE_DEBUG(string)                                                  \
    do                                                                             \
    {                                                                              \
        LibLog::LogEngine::Log(std::cout,                                          \
                               "[DEBUG] ", \
                               "File ", __FILE__,                                  \
                               " Line ", __LINE__,                                 \
                               " Msg: ", string);                                  \
    } while (0)
#endif

namespace LibLog
{
    class LogEngine
    {
    public:
        template <typename T, typename... TArgs>
        static void Log(std::ostream &stream, T val, TArgs... args)
        {
            stream << val;
            Log(stream, args...);
        }

        template <typename T, typename... TArgs>
        static void LogConsoleInfo(T val, TArgs... rest)
        {
            LogTag(std::cout, "info", val, rest...);
        }

        template <typename T, typename... TArgs>
        static void LogConsoleWarn(T val, TArgs... rest)
        {
            LogTag(std::cout, "Warn", val, rest...);
        }

        template <typename T, typename... TArgs>
        static void LogConsoleError(T val, TArgs... rest)
        {
            LogTag(std::cerr, "Warn", val, rest...);
        }

    private:
        static void Log(std::ostream &stream)
        {
            stream << "\r\n";
        }

        template <typename T, typename... TArgs>
        static void LogTag(std::ostream &stream, std::string tag, T val, TArgs... rest)
        {
            stream << "[" << tag << "] ";
            Log(stream, val, rest...);
            stream << "\n";
        }
    };
}
#endif // LIBLOG_H