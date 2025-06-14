#include "app.h"

namespace afk {

void App::Run() {
    SDL_Event* last_event = nullptr;
    running = true;
    while (running) {
        while (SDL_PollEvent(last_event)) {
            if (last_event->type == SDL_EVENT_QUIT) {
                Stop();
            }
            Events(last_event);
        }
        Update();
    }    
}


void App::Stop() {
    running = false;    
}

};