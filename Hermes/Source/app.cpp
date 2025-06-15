#include "pch.h"
#include "app.h"
#include "error.h"
#include "system.h"

namespace {
    
const std::string& GetSleepStatusLabel() {
    static const std::string SLEEP_ENABLED_STRING = "Sleep Enabled";
    static const std::string SLEEP_DISABLED_STRING = "Sleep Disabled";

    return afk::sys::Display::IsSleepEnabled()
        ? SLEEP_ENABLED_STRING : SLEEP_DISABLED_STRING;
}

void OnClickToggleSleep(void*, SDL_TrayEntry* p_entry) {
    sdl::TrayEntry entry{p_entry};
    afk::sys::Display::ToggleSleep();
    entry.SetLabel(GetSleepStatusLabel());
    
    Assert(entry.IsChecked() == afk::sys::Display::IsSleepEnabled());

    #ifndef NDEBUG
    if (afk::sys::Display::IsSleepEnabled()) {
        DebugPrintLn("afk: Sleep Enabled");
    } else {
        DebugPrintLn("afk: Sleep Disabled");  
    }
    #endif
}

void OnClickQuit(void*, SDL_TrayEntry* p_entry) {
    using namespace std::literals;

    SDL_Event event = {.type = SDL_EVENT_QUIT};
    if (!SDL_PushEvent(&event)) {
        err::note(err::get_sdl_error());
    }
}

void OnClickAbout(void*, SDL_TrayEntry* p_entry) {
    const char* url = SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING);    
    if (!SDL_OpenURL(url)) {
        afk::sys::MessageBoxes::ShowError(SDL_GetError());
    }
}

};

namespace afk {

App::App() {
    DebugPrint("afk: Creating App\n");
    DebugPrintLn("afk: Initializing SDL...");
    sdl_manager.Init(SDL_INIT_VIDEO);
    DebugPrintLn("afk: Initializing SDL (done)");
}

App::~App() {
    // Uninitialize
    DebugPrintLn("afk: Uninitializing SDL...");
    sdl_manager.Quit();
    DebugPrintLn("afk: Uninitializing SDL (done)");
    DebugPrint("afk: Destroying App\n");
}

void App::Run() {
    // Initialize
    Init();

    // Run
    DebugPrintLn("afk: App Running");
    SDL_Event* event = nullptr;
    running = true;
    while (running) {
        while (SDL_PollEvent(event))
            Events(event);
        sys::Timing::WaitMiliseconds(1000 / 10);
    }
    DebugPrintLn("afk: Quitting (done)");
}

void App::Init() {
    DebugPrintLn("afk: Loading Resources...");
    sdl::Surface icon_image{resource_manager.GetPathToResource("icon128.png")};
    DebugPrintLn("afk: Loading Resources (done)");

    DebugPrintLn("afk: Creating Tray Icon...");
    
    icon = std::make_unique<sdl::TrayIcon>(icon_image, "Hermes");
    auto& menu = icon->CreateMenu();

    sys::Display::DisableSleeping();
    auto toggle = menu.InsertCheckbox(0, GetSleepStatusLabel(), false);
    toggle.SetCallback(OnClickToggleSleep);
    
    menu.InsertSeparator(1);

    auto quit = menu.InsertEntry(2, "Quit");
    quit.SetCallback(OnClickQuit);

    auto about_button = menu.InsertEntry(3, "About Hermes");
    about_button.SetCallback(OnClickAbout);
    
    DebugPrintLn("afk: Creating Tray Icon (done)");
}

void App::Events(SDL_Event* event) {
    switch (event->type) {
    case SDL_EVENT_QUIT:
        DebugPrintLn("afk: Quitting");
        Stop();
    }
}

};