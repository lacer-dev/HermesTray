#pragma once
#include <memory>

#include "SDLCPP/SDLCPP.h"
#include "systray.h"

union SDL_Event;

namespace Hermes
{

class Application
{
public:
    Application();
    ~Application();
    void Run();
    [[nodiscard]]
    bool IsRunning() const
    {
	return running;
    }

private:
    void Init();
    void HandleEvents(const SDL_Event& event);

private:
    bool running = false;

private:
    std::unique_ptr<Systray::TIcon> icon;
};

}; // namespace Hermes