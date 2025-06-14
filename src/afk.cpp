#include "app.h"
#include "error.h"
#include "timing.h"
#include "system.h"

namespace {
    
constexpr std::string SCREENSAVER_ON = "Screensaver On";
constexpr std::string SCREENSAVER_OFF = "Screensaver Off";

void OnQuit(void*, SDL_TrayEntry* p_entry) {
    SDL_Event quit_event = {.type = SDL_EVENT_QUIT};
    bool ok = SDL_PushEvent(&quit_event);
    if (!ok) {
        PrintErr("SDL failed to send event \'SDL_EVENT_QUIT\'\n");
    }
}


void OnToggle(void*, SDL_TrayEntry* p_entry) {
    afk::TrayEntry entry{p_entry};

    Assert(entry.IsChecked() == SDL_ScreenSaverEnabled());
    
    if (afk::ToggleScreensaver()) {
        entry.SetLabel(SCREENSAVER_ON);
    } else {
        entry.SetLabel(SCREENSAVER_OFF);
    }
}

};

namespace afk {

AFK::AFK() {
    icon_image = new Surface{GetFileResource("icon128.png")};
    icon = new TrayIcon{*icon_image, "AFK Window"};
    auto& menu = icon->CreateMenu();

    auto& toggleButton = menu.AddCheckbox(SCREENSAVER_ON, true);
    toggleButton.SetCallback(OnToggle);

    menu.AddSeparator();
    
    auto& quitButton = menu.AddEntry("Quit");
    quitButton.SetCallback(OnQuit);
}


AFK::~AFK() {
    delete icon_image, icon;
}


void AFK::Update() {
    WaitMiliseconds(1000 / 10);
}

};