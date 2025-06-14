#pragma once
#include "surface.h"
#include "tray.h"

namespace afk {

//////////////////// App ////////////////////

class App {
public:
    App() = default;
    ~App() = default;
    void Run();
    void Stop();
protected:
    virtual void Update() {}
    virtual void Events(SDL_Event* event) {}
private:
    bool running = false;
};


//////////////////// AFK ////////////////////

class AFK : public App {
public:
    AFK();
    ~AFK();
private:
    Surface* icon_image = nullptr;
    TrayIcon* icon = nullptr;
private:
    void Update() override;
};

};