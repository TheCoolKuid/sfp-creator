#ifndef PATH_LIB_H
#define PATH_LIB_H
#include <filesystem>
#include <type_traits>
namespace PathUtils
{
    class Path
    {
    public:
        template <typename T, typename... TArgs>
        static std::filesystem::path Append(const std::filesystem::path &origin, T add, TArgs... rest)
        {
            auto new_path(origin);
            RecursiveAppend(new_path, add, rest...);
            return new_path;
        }

    private:
        const static char WinSep = '\\';
        const static char UnixSep = '/';

        template <typename T, typename... TArgs>
        static void RecursiveAppend(std::filesystem::path &origin, T add, TArgs... rest)
        {
            RecursiveAppend(Add(origin, add), rest...);
        }
        static void RecursiveAppend(std::filesystem::path &origin) {}

        static bool hasEndSeparator(const std::filesystem::path &origin)
        {
            auto str = origin.string();
            return str.back() == WinSep || str.back() == UnixSep;
        }

        static bool IsExt(const std::filesystem::path &p) { return false; }
        static bool IsExt(const char *p) { return p[0] == '.'; }
        static bool IsExt(char *p) { return p[0] == '.'; }
        static bool IsExt(const std::string& p) { return p[0] == '.'; }

        template <typename T>
        static bool hasBeginSeparator(const T &origin)
        {
            if constexpr (std::is_same<T, std::filesystem::path>::value)
            {
                return origin.c_str()[0] == WinSep || origin.c_str()[0] == UnixSep;
            }
            else
            {
                return origin[0] == WinSep || origin[0] == UnixSep;
            }
        }

        static void AddWS(std::filesystem::path &p, const std::string &s)
        {
            p.concat(s.begin() + 1, s.end());
        }
        static void AddWS(std::filesystem::path &p, const std::filesystem::path &s)
        {
            auto ptr = s.c_str();
            p.concat(++ptr);
        }
        static void AddWS(std::filesystem::path &p, char *s)
        {
            p.concat(++s);
        }
        static void AddWS(std::filesystem::path &p, const char *s)
        {
            p.concat(++s);
        }

        template <typename T>
        static std::filesystem::path &Add(std::filesystem::path &origin, T &&add)
        {
            if (!IsExt(add))
            {
                // add separator at the end to origin path
                if (!hasEndSeparator(origin))
                {
                    origin += "/";
                }
            }
            // check is add has begin separator
            if (hasBeginSeparator(add))
            {
                AddWS(origin, add);
            }
            else
            {
                origin += add;
            }
            return origin;
        }
    };
}
#endif