#include "app.h"
#include "SDLCPP/SDLCPP.h"
#include "error.h"
#include "globals.h"
#include "pch.h"
#include "system.h"
#include <SDL3/SDL.h>

namespace
{

using namespace Hermes;

void ShowErrorMessageBox(const std::string& message)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, messagebox_title_error().c_str(), message.c_str(), nullptr);
}

void OnClickToggleSleep(void*, SDL_TrayEntry* p_entry)
{
    auto result = ScreenSaverEnabled() ? DisableScreenSaver() : EnableScreenSaver();

    if (!result)
    {
	ShowErrorMessageBox("could not toggle sleep");
    }

#ifndef NDEBUG
    Hermes_Assert((SDL_GetTrayEntryChecked(p_entry) == !ScreenSaverEnabled()));
#endif
}

void OnClickQuit(void*, SDL_TrayEntry*)
{
    if (SDL_Event event{SDL_EVENT_QUIT}; !SDL_PushEvent(&event))
    {
	log_last_sdl_error();
    }
}

void OnClickAbout(void*, SDL_TrayEntry*)
{
    if (!SDL_OpenURL(METADATA_WEBSITE_URL.c_str()))
    {
	ShowErrorMessageBox("could not open url");
	log_last_sdl_error();
    }
}

SDL_TrayEntry* InsertTrayEntryWithCallback(SDL_TrayMenu* menu, int pos, std::string_view label = {}, SDL_TrayCallback callback = nullptr, int flags = 0)
{
    SDL_TrayEntry* entry = SDL_InsertTrayEntryAt(menu, pos, label.data(), flags);
    if (callback)
    {
	SDL_SetTrayEntryCallback(entry, callback, nullptr);
    }
    return entry;
}

}; // namespace

namespace Hermes
{

Application::Application()
{
    // Set metadata for app
    if (std::atexit(SDL_Quit) != 0)
    {
	logerror("failed to register function with std::atexit");
    }

    if (std::atexit([] { EnableScreenSaver(); }) != 0)
    {
	logerror("failed to register function with std::atexit");
    }

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, METADATA_NAME.c_str()))
    {
	log_last_sdl_error();
    }

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, METADATA_VERSION.c_str()))
    {
	log_last_sdl_error();
    }

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, METADATA_CREATOR.c_str()))
    {
	log_last_sdl_error();
    }

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, METADATA_COPYRIGHT.c_str()))
    {
	log_last_sdl_error();
    }

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, METADATA_WEBSITE_URL.c_str()))
    {
	log_last_sdl_error();
    }

    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, "application"))
    {
	log_last_sdl_error();
    }

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
	log_last_sdl_error();
    }

    Init();
}

Application::~Application()
{
    SDL_Quit();
}

void Application::Init()
{
    using namespace Systray;

    if (auto result = DisableScreenSaver(); !result)
    {
	logerror(result.error());
    }

    auto p_image = IMG_Load((processpath().parent_path() / "hermes32.png").string().c_str());
    if (!p_image)
    {
	log_last_sdl_error();
    }

    SDL_Tray* p_icon = SDL_CreateTray(p_image, "Hermes");
    SDL_TrayMenu* p_menu = SDL_CreateTrayMenu(p_icon);

    InsertTrayEntryWithCallback(p_menu, 0, "Quit", OnClickQuit);
    InsertTrayEntryWithCallback(p_menu, 0);
    InsertTrayEntryWithCallback(p_menu, 0, "Disable Sleep", OnClickToggleSleep, SDL_TRAYENTRY_CHECKED);
    InsertTrayEntryWithCallback(p_menu, 0);
    InsertTrayEntryWithCallback(p_menu, 0, "About Hermes", OnClickAbout);
}

void Application::Run()
{
    running = true;
    while (running)
    {
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
	    HandleEvents(event);
	}

	SDL_Delay(1000 / 10);
    }
}

void Application::HandleEvents(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_QUIT)
    {
	running = false;
    }
}

}; // namespace Hermes