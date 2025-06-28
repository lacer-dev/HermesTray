#pragma once
#include <SDL3/SDL.h>

namespace SDL {

inline void Delay(int ms) { 
    SDL_Delay(ms); 
}

};