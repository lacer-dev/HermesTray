#pragma once
#include <chrono>
#include <SDL3/SDL_timer.h>

namespace afk {

void WaitMiliseconds(int duration) {
    uint64_t start = SDL_GetTicks();
    uint64_t end = start + duration;
    while (SDL_GetTicks() < end) {
        
    }
}

};