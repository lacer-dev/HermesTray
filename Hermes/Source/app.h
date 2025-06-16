#pragma once
#include <memory>
#include "SDL/tray.h"

namespace hermes {

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

private:
    std::unique_ptr<SDL::Tray> icon;
};

};