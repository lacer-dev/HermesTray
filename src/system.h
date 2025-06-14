#pragma once
#include <string>
#include <filesystem>

namespace afk {

/**
 * @brief Displays an error message box to the screen with the given message.
 */
void ShowErrorMessageBox(const std::string& message);

/**
 * @brief Toggles the system's screensaver.
 * 
 * Returns `true` if the screensaver was enabled, or `false` if disabled.
 * Upon error, displays a messagebox and exits the program with a negative error code.
 */
bool ToggleScreensaver();

/**
 * @brief Retrieves the path to a given resource in the Resources directory.
 */
std::filesystem::path GetFileResource(const std::string& filename);

};