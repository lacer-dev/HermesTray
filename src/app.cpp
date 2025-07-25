#include "app.h"
#include "error.h"
#include "pch.h"
#include "sys.h"
#include <expected>

namespace
{

struct
{
    const std::string name = "HermesTray";
    const std::string version = "0.1.1";
    const std::string author = "Leon Allotey";
    const std::string copyright = "Copyright (c) 2025 Leon Allotey";
    const std::string website_url = "https://github.com/lacer-dev/HermesTray";
    const std::string type = "application";
} g_hermes_metadata;

using namespace Hermes;

std::expected<void, std::string> printunexpectedstr(const std::string& error)
{
    printerror("{}", error);
    return std::unexpected(error);
}

void ShowErrorMessageBox(const std::string& message)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
			     messagebox_title_error().c_str(), message.c_str(),
			     nullptr);
}

void OnClickToggleSleep(void*, SDL_TrayEntry* p_entry)
{
    auto result = ScreenSaverEnabled() ? DisableScreenSaver() : EnableScreenSaver();
    if (!result)
    {
        ShowErrorMessageBox(result.error());
    }

#ifndef NDEBUG
    hermes_assert((SDL_GetTrayEntryChecked(p_entry) == !ScreenSaverEnabled()));
#endif  
}

void OnClickQuit(void*, SDL_TrayEntry*)
{
    if (SDL_Event event{SDL_EVENT_QUIT}; !SDL_PushEvent(&event))
    {
        eprintln("{}: {}", colors(Ansi::Teal, "SDL"), SDL_GetError());
    }
}

void OnClickAbout(void*, SDL_TrayEntry*)
{
    if (!SDL_OpenURL(g_hermes_metadata.website_url.c_str()))
    {
	ShowErrorMessageBox("could not open url");
        eprintln("{}: {}", colors(Ansi::Teal, "SDL"), SDL_GetError());
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
        printmsg(ERR_PREFIX, "failed to register 'SDL_Quit' with std::atexit");
    }

    if (std::atexit([] { EnableScreenSaver(); }) != 0)
    {
        printmsg(ERR_PREFIX, "failed to register 'EnableScreenSaver' with std::atexit");
    }

    Meta::SetAppName(g_hermes_metadata.name).or_else(printunexpectedstr);
    Meta::SetAppVersion(g_hermes_metadata.version).or_else(printunexpectedstr);
    Meta::SetAppCreator(g_hermes_metadata.author).or_else(printunexpectedstr);
    Meta::SetAppCopyright(g_hermes_metadata.copyright).or_else(printunexpectedstr);
    Meta::SetAppURL(g_hermes_metadata.website_url).or_else(printunexpectedstr);
    Meta::SetAppType(g_hermes_metadata.type).or_else(printunexpectedstr);

    Init();
}

Application::~Application() 
{
    SDL_Quit();
}

void Application::Init()
{
    DisableScreenSaver().or_else(printunexpectedstr);

    auto p_image =
	IMG_Load((processpath().parent_path() / "hermes32.png").string().c_str());
    if (!p_image)
    {
        printmsg(SDL_PREFIX, "{}", SDL_GetError());
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

    SDL_Event last_event;
    while (running)
    {
        while (SDL_PollEvent(&last_event))
        {
            if (last_event.type == SDL_EVENT_QUIT)
            {
            running = false;
            }
        }

	    SDL_Delay(1000 / 10);
    }
}

}; // namespace Hermes