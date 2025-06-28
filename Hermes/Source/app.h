#pragma once
#include <memory>

#include "systray.h"
#include "SDLCPP/SDLCPP.h"

namespace Hermes {

class Application {
public:
    Application();
    ~Application();
    void Run();
    void Stop() { running = false; }
    bool IsRunning() const { return running; }
private:
    void Initialize();
    void HandleEvents(const SDL::Event& event);
private:
    bool running = false;
private:
    std::unique_ptr<Systray::Icon> icon;
};

};