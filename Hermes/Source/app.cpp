#include "SDL/init.h"
#include "SDL/misc.h"
#include "pch.h"
#include "app.h"
#include "error.h"
#include "system.h"
#include "globals.h"
#include "SDL/SDLcpp.h"

namespace {

void OnClickToggleSleep(void*, SDL_TrayEntry* p_entry) {
    SDL::TrayEntry entry{p_entry};
    hermes::ToggleDisplaySleep();
    Assert(entry.IsChecked() == hermes::IsDisplaySleepDisabled());

    #ifndef NDEBUG
    if (hermes::IsDisplaySleepEnabled()) {
        DebugPrintLn("hermes: Sleep Enabled");
    } else {
        DebugPrintLn("hermes: Sleep Disabled");  
    }
    #endif
}

void OnClickQuit(void*, SDL_TrayEntry* p_entry) {
    using namespace std::literals;

    SDL_Event event = {.type = SDL_EVENT_QUIT};
    if (!SDL_PushEvent(&event)) {
        SDL::ShowSimpleMessageBoxError(hermes::APP_NAME, SDL_GetError());
    }
}

void OnClickAbout(void*, SDL_TrayEntry* p_entry) {
    const std::string url = hermes::WEBSITE_URL;    
    if (!SDL::OpenURL(url)) {
        SDL::ShowSimpleMessageBoxError(hermes::APP_NAME, SDL_GetError());
    }
}

};

namespace hermes {

App::App() {
    DebugPrint("hermes: Creating App\n");
    DebugPrintLn("hermes: Initializing SDL...");        
    SDL::Init(SDL::INIT_VIDEO);
    DebugPrintLn("hermes: Initializing SDL... (done)");
    std::atexit(SDL::Quit);
}

App::~App() {
    DebugPrint("hermes: Destroying App\n");
    DebugPrintLn("hermes: Uninitializing SDL...");
    SDL::Quit();
    DebugPrintLn("hermes: Uninitializing SDL (done)");
}

void App::Run() {
    Init();
    
    DebugPrintLn("hermes: App Running");
    SDL_Event* event = nullptr;
    running = true;
    while (running) {
        while (SDL_PollEvent(event))
            Events(event);
        WaitMiliseconds(1000 / 10);
    }
    DebugPrintLn("hermes: Quitting (done)");
}

void App::Init() {
    SDL::SetMetadataName("hermes Window");
    SDL::SetMetadataVersion("0.0.1");
    SDL::SetMetadataCreator("Leon Allotey");
    SDL::SetMetadataCopyright("Copyright (c) 2025 Leon Allotey");
    SDL::SetMetadataUrl("https://github.com/lacer-dev/HermesTray");
    SDL::SetMetadataType("application");

    DebugPrintLn("hermes: Loading Resources...");
    SDL::Surface icon_image = SDL::IMG::Load(gResourceManager.GetPath("icon128.png"));
    DebugPrintLn("hermes: Loading Resources (done)");

    DebugPrintLn("hermes: Creating Tray Icon...");
    
    icon = std::make_unique<SDL::Tray>(icon_image, "Hermes");
    auto& menu = icon->CreateMenu();

    DisableDisplaySleep();
    std::atexit(EnableDisplaySleep);
    auto quit = menu.InsertEntry(0, "Quit");
    menu.InsertSeparator(0);
    auto toggle = menu.InsertCheckbox(0, "Disable Sleep", true);
    menu.InsertSeparator(0);
    auto about = menu.InsertEntry(0, "About Hermes");
    
    quit.SetCallback(OnClickQuit);
    toggle.SetCallback(OnClickToggleSleep);
    about.SetCallback(OnClickAbout);
    
    DebugPrintLn("hermes: Creating Tray Icon (done)");
}

void App::Events(SDL_Event* event) {
    switch (event->type) {
    case SDL_EVENT_QUIT:
        DebugPrintLn("hermes: Quitting");
        Stop();
    }
}

};