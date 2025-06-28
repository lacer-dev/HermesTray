#pragma once
#include <SDL3/SDL.h>

namespace SDL {

// Constants
enum EventType : unsigned {
    QUIT = SDL_EVENT_QUIT
};

class Event {
public:
    Event() = default;
    explicit Event(EventType type) : _event{.type = static_cast<unsigned>(type)} {}
    explicit Event(const SDL_Event& event) : _event{event} {}

    SDL_Event& operator * () { return _event; }
    SDL_Event* operator -> () { return &_event; }

    SDL_Event& Underlying() { return _event; }
    EventType Type() const { return static_cast<EventType>(_event.type); }
private:
    SDL_Event _event;
};

// Functions
inline bool PollEvent(Event& event) {
    return SDL_PollEvent(&event.Underlying());
}

inline bool PushEvent(Event& event) {
    return SDL_PushEvent(&event.Underlying());
}

}