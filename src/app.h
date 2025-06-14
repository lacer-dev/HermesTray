#pragma once
#include "surface.h"
#include "tray.h"
#include "error.h"
#include "timing.h"
#include <csignal>

//////////////////// App ////////////////////

inline void PushQuitEvent() {
    SDL_Event quit_event = {.type = SDL_EVENT_QUIT};
    bool ok = SDL_PushEvent(&quit_event);
    if (!ok) {
        PrintErr("SDL failed to send event \'SDL_EVENT_QUIT\'\n");
    }
}

class App {
public:
    App() = default;
    ~App() = default;
    
    void Run() {
        SDL_Event* last_event = nullptr;
        running = true;
        while (running) {
            while (SDL_PollEvent(last_event)) {
                Events(last_event);
            }
            Update();
            
        }    
    }
    
    void Stop() {
        running = false;    
    }

protected:
    virtual void Update() {}
    virtual void Events(SDL_Event* event) {}
private:
    bool running = false;
};

//////////////////// AFK ////////////////////

class AFK : public App {
public:
    AFK() {
        icon_image = new Surface{32, 32};

        
        icon = new TrayIcon{*icon_image, "AFK Window"};
        TrayMenu& menu = icon->CreateMenu();

        toggleButton = &menu.AddCheckbox(SCREENSAVER_ON, true);
        menu.AddSeparator();
        quitButton = &menu.AddEntry("Quit");
    
        toggleButton->SetCallback(ToggleScreensaver);
        quitButton->SetCallback(OnQuit);
    }

    ~AFK() {
        delete icon_image;
        delete icon;
    }
private:
    Surface* icon_image = nullptr;
    TrayIcon* icon = nullptr;
    TrayEntry* quitButton = nullptr;
    TrayEntry* toggleButton = nullptr;

    static constexpr std::string SCREENSAVER_ON = "Screensaver On";
    static constexpr std::string SCREENSAVER_OFF = "Screensaver Off";
private:
    void Update() override {
        WaitMiliseconds(1000 / 10);
    }
    
    void Events(SDL_Event* event) override {
        if (event->type == SDL_EVENT_QUIT) {
            Stop();
        }
    }

    static void OnQuit(void*, SDL_TrayEntry* p_entry) {
        PushQuitEvent();
    }

    static void ToggleScreensaver(void*, SDL_TrayEntry* p_entry) {
        TrayEntry entry = TrayEntry::FromHandle(p_entry);

        // Check if screensaver is off
        if (entry.IsChecked()) {
            entry.SetLabel(SCREENSAVER_ON);
        } else {
            entry.SetLabel(SCREENSAVER_OFF);
        }
    }
};