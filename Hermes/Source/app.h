#pragma once
#include <memory>
#include "surface.h"
#include "system.h"
#include "tray.h"

namespace afk {

class App {
public:
    App();
    ~App();

    /**
     * Runs the app.
     */
    void Run();

    /**
     * Stops the main loop.
     */
    void Stop() { running = false; }
private:
    bool running = false;
    void Init();
    void Events(SDL_Event* event);

    sys::SDLManager sdl_manager;
    sys::ResourceManager resource_manager{"Resources"};
private:
    std::unique_ptr<sdl::TrayIcon> icon;
};

};