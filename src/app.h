#pragma once

union SDL_Event;

namespace Hermes
{

class Application
{
public:
    Application();
    ~Application();
    
    void Run();
    bool Running() const { return running; }
private:
    void Init();
    bool running = false;
};

}; // namespace Hermes