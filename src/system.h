#pragma once
#include <expected>
#include <filesystem>
#include <string>

namespace Hermes
{

bool ScreenSaverEnabled();
std::expected<void, std::string> EnableScreenSaver();
std::expected<void, std::string> DisableScreenSaver();

class ResourceManager
{
public:
    // Creates a new ResourceManager
    explicit ResourceManager(const std::filesystem::path& relative_path);
    // Returns the path to the manager's resources directory
    std::filesystem::path Directory() { return _directory; }
    // Retrieves the path to a given resource in the Resources directory.
    std::filesystem::path GetResource(const std::string& filename);

private:
    std::filesystem::path _directory;
};

std::filesystem::path processpath();

}; // namespace Hermes