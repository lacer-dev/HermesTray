#pragma once
#include <expected>
#include <filesystem>
#include <string>

namespace Hermes
{

bool ScreenSaverEnabled();
std::expected<void, std::string> EnableScreenSaver();
std::expected<void, std::string> DisableScreenSaver();

std::filesystem::path processpath();

namespace Meta {
        
std::expected<void, std::string> SetAppName(const std::string& value);
std::expected<void, std::string> SetAppVersion(const std::string& value);
std::expected<void, std::string> SetAppCreator(const std::string& value);
std::expected<void, std::string> SetAppCopyright(const std::string& value);
std::expected<void, std::string> SetAppURL(const std::string& value);
std::expected<void, std::string> SetAppType(const std::string& value);

};

}; // namespace Hermes