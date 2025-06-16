#pragma once
#include <string>
#include <filesystem>

namespace hermes {
    
/**
    * @brief Pauses the program for the specified duration in miliseconds.
    * 
    * Uses a while loop, rather than sleeping the current thread,
    */
void WaitMiliseconds(int duration);

bool IsDisplaySleepEnabled();
bool IsDisplaySleepDisabled();
void EnableDisplaySleep();
void DisableDisplaySleep();

// Toggles the display's ability to sleep.
// Upon error, displays a messagebox and exits the program with a negative error code.
void ToggleDisplaySleep();


class ResourceManager {
public:
    // Creates a new ResourceManager
    ResourceManager(const std::filesystem::path& relative_path);

    // Returns the path to the manager's resources directory
    std::filesystem::path Dir() { return _directory; }

    // Retrieves the path to a given resource in the Resources directory.
    std::filesystem::path GetPath(const std::string& filename);
private:
    std::filesystem::path _directory;
};

};