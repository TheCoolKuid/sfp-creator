#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>
#include <vector>
#include <regex>
#include <array>
#include "../../archiver/archiver.h"
#include "../../shared/json.h"
#include "../../shared/liblog.h"

#define _SZ_ALLOC_DEBUG //  define it if you want to debug alloc/free operations to stderr.

namespace SFPConfig
{
    struct Config
    {
        std::string executable_name;
        std::string add_build_command;
        std::string cmake_path;
        std::string gcc_path;
        std::string fileReplacement_Destination;
        std::vector<archiver::ArchiveBuilder::FileReplacmentConfig> files;
    };

    class IConfigFiller
    {
    public:
        virtual Config Create() = 0;
    };

    class JSONConfigFiller : public IConfigFiller
    {
    public:
        JSONConfigFiller(const std::filesystem::path &path);
        JSONConfigFiller(std::string &&content);

        virtual Config Create();

    private:
        std::unique_ptr<JSON::IJObject<char>> config;
    };

    class IConfigRule
    {
    public:
        constexpr IConfigRule() {}
        bool Check(const Config &conf) const;
    protected:
        virtual bool _Check(const Config &conf) const = 0;
        virtual std::string ErrMsg() const = 0;
    };

    class ExeNameRule : public IConfigRule
    {
    protected:
        virtual bool _Check(const Config &conf) const;
        virtual std::string ErrMsg() const;
    };

    class PathCheckRule : public IConfigRule
    {
    protected:
        virtual bool _Check(const Config &conf) const;
        virtual std::string ErrMsg() const;
    };

    class RuleChecker
    {
    public:
        bool Check(const Config &conf);
    };
}

#endif