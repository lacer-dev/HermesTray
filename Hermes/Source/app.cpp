#include "pch.h"
#include "SDLCPP/SDLCPP.h"
#include "app.h"
#include "error.h"
#include "system.h"
#include "globals.h"
#include "errormessage.h"

namespace {

using namespace Hermes;

void OnClickToggleSleep(void*, SDL_TrayEntry* p_entry) {
    if (!ToggleScreenSaver())
        SDL::ShowSimpleMessageBoxError(messagebox_title_error(), SDL::GetError());

    #ifndef NDEBUG
        const Systray::Entry entry{p_entry};
        Hermes_Assert((entry.Checked() == !ScreenSaverEnabled()));
    #endif
}

void OnClickQuit(void*, SDL_TrayEntry*) {
    if (SDL::Event event{SDL::EventType::QUIT}; !SDL::PushEvent(event))
        SDL::ShowSimpleMessageBoxError(EXECUTABLE_NAME, SDL::GetError());
}

void OnClickAbout(void*, SDL_TrayEntry*) {
    if (!SDL::OpenURL(METADATA_WEBSITE_URL))
        SDL::ShowSimpleMessageBoxError(EXECUTABLE_NAME, SDL::GetError());
}

};

namespace Hermes {

Application::Application() {
    // Set metadata for app

    if (std::atexit(SDL::Quit) != 0)
        LogError("failed to register function with std::atexit");

    if (std::atexit([]{EnableScreenSaver();}) != 0)
        LogError("failed to register function with std::atexit");

    if (!SDL::SetMetadataName(METADATA_NAME))
        LogSDLError(SDL::GetError());

    if (!SDL::SetMetadataVersion(METADATA_VERSION))
        LogSDLError(SDL::GetError());

    if (!SDL::SetMetadataCreator(METADATA_CREATOR))
        LogSDLError(SDL::GetError());

    if (!SDL::SetMetadataCopyright(METADATA_COPYRIGHT))
        LogSDLError(SDL::GetError());

    if (!SDL::SetMetadataUrl(METADATA_WEBSITE_URL))
        LogSDLError(SDL::GetError());

    if (!SDL::SetMetadataType("application"))
        LogSDLError(SDL::GetError());

    if (!SDL::Init(SDL::INIT_VIDEO))
        LogSDLError(SDL::GetError());

    Initialize();
}

Application::~Application() {
    SDL::Quit();
}

void Application::Initialize() {
    using namespace Systray;

    DisableScreenSaver();

    // Load icon image from
    SDL::Surface image = SDL::IMG::Load(gResourceManager.GetResource("hermes32.png"));
    if (!image)
        LogSDLError(SDL::GetError());

    // Create systray icon and its menu
    icon = std::make_unique<TIcon>(image, "Hermes");
    TMenu& menu = icon->Menu();

    // Create menu entries
    menu.Insert(0, Label("Quit").SetCallback(OnClickQuit));
    menu.Insert(0, Separator());
    menu.Insert(0, Checkbox("Disable Sleep", true).SetCallback(OnClickToggleSleep));
    menu.Insert(0, Separator());
    menu.Insert(0, Label("About Hermes").SetCallback(OnClickAbout));
}

void Application::Run() {
    running = true;
    while (IsRunning()) {
        SDL::Event event{};
        while (SDL::PollEvent(event))
            HandleEvents(event);

        SDL::Delay(1000 / 10);
    }
}

void Application::HandleEvents(const SDL::Event& event) {
    switch (event.Type()) {
        case SDL::EventType::QUIT: Stop();
        default: ;
    }
}

};