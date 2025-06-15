#pragma once
#include <string>
#include <filesystem>
#include <system_error>

namespace afk::sys {
    
struct MessageBoxes {
public:
    MessageBoxes() = delete;
    
    /**
     * @brief Displays an error message box to the screen with the given message.
     */
    static void ShowError(const std::string& message = "", const std::string& title = "hermes - error");
};

struct Timing {
public:
    Timing() = delete;
    
    /**
     * @brief Pauses the program for the specified duration in miliseconds.
     * 
     * Uses a while loop, rather than sleeping the current thread,
     */
    static void WaitMiliseconds(int duration);
};


struct Display {
public:
    Display() = delete;

    static bool IsSleepEnabled() { return _allow_sleep; }
    static void EnableSleeping();
    static void DisableSleeping();
    
    /**
     * @brief Toggles the display's ability to sleep.
     * 
     * Upon error, displays a messagebox and exits the program with a negative error code.
     */
    static void ToggleSleep();
private:
    static bool _allow_sleep;
};


class SDLManager {
public:
    SDLManager() = default;
    ~SDLManager();
    void Init(int systems);
    void Quit();
private:
    std::string GetError();
    void SetAppMetadataProperty(const char*, const char*);
};


class ResourceManager {
public:
    ResourceManager(const std::string& path_to_res);

    /**
     * @brief Retrieves the path to a given resource in the Resources directory.
     */
    std::filesystem::path GetPathToResource(const std::string& filename);
private:
    std::filesystem::path _res_dir;
};

};