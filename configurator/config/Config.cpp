#include "Config.h"
SFPConfig::JSONConfigFiller::JSONConfigFiller(const std::filesystem::path &path)
{
    JSON::JSONParser<char> parser(path);
    config = std::move(parser.Parse());
}

SFPConfig::JSONConfigFiller::JSONConfigFiller(std::string &&content)
{
    config = std::make_unique<JSON::JObject<char>>(std::move(content));
}

SFPConfig::Config SFPConfig::JSONConfigFiller::Create()
{
    SFPConfig::Config out;

    const auto &value = config->Value();

    out.executable_name = value.at("executable_name")->GetSrc();
    out.add_build_command = value.at("add_build_command")->GetSrc();
    out.cmake_path = value.at("cmake_path")->GetSrc();
    out.gcc_path = value.at("gcc_path")->GetSrc();

    const auto &files = value.at("fileReplacement")->Value();

    out.fileReplacement_Destination = files.at("Destination")->GetSrc();

    const auto &files_arr = files.at("Files")->Value();

    for (const auto &obj : files_arr)
    {
        archiver::ArchiveBuilder::FileReplacmentConfig file;
        const auto &val = obj.second->Value();

        file.OnCreatorPath = val.at("OnCreatorPath")->GetSrc();
        file.RelativeUserPath = val.at("RelativeUserPath")->GetSrc();

        out.files.push_back(file);
    }

    return out;
}

bool SFPConfig::ExeNameRule::_Check(const SFPConfig::Config &conf) const
{
    const std::regex exe_name_regex("^[^<>:;,?\"*|/]+$");
    std::smatch match;
    return std::regex_match(conf.executable_name, match, exe_name_regex);
}

std::string SFPConfig::ExeNameRule::ErrMsg() const
{
    return "[ExeNameRule] executable_name is not a right name for file";
}

bool SFPConfig::PathCheckRule::_Check(const SFPConfig::Config &conf) const
{
    if (conf.fileReplacement_Destination == "AskUser")
        return true;
    if (conf.fileReplacement_Destination == "Registry")
        return true;

    const std::regex win_path_regex("^\\w:\\\\([^<>:;,?\"*|]+$)");
    const std::regex unix_path_regex("^/([^<>:;,?\"*|]+/*$)");
    const std::regex essential_regex("(windows|bin|boot|lib|proc|root|sys)", std::regex_constants::icase);
    std::smatch match;
    return (std::regex_match(conf.fileReplacement_Destination, match, win_path_regex) ||
            std::regex_match(conf.fileReplacement_Destination, match, unix_path_regex)) &&
           std::regex_match(conf.fileReplacement_Destination, match, essential_regex);
}

std::string SFPConfig::PathCheckRule::ErrMsg()  const
{
    return "[PathCheckRule] specified path is not a right path";
}

bool SFPConfig::RuleChecker::Check(const Config &conf)
{
    constexpr auto exe = ExeNameRule();
    constexpr auto path = PathCheckRule();
    
    return exe.Check(conf) && path.Check(conf);
}

bool SFPConfig::IConfigRule::Check(const SFPConfig::Config &conf) const
{
    if (_Check(conf))
    {
        return true;
    }
    else
    {
        LibLog::LogEngine::LogConsoleError(ErrMsg());
        return false;
    }
}