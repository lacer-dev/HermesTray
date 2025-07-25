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

template<class E>
std::expected<void, std::string> printunex(const E& error)
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

class Entry
{
private:
    SDL_TrayEntry* _handle;
    friend class Menu;
public:
    void set_callback(SDL_TrayCallback callback)
    {
        SDL_SetTrayEntryCallback(_handle, callback, nullptr);
    }
};

class Menu
{
private:
    SDL_TrayMenu* _handle;
    friend class Tray;
public:
    auto insert_label(int pos, const std::string& label) -> std::expected<Entry, std::string>
    {
        return insert_or_unexpected(pos, label.c_str());
    }

    auto insert_checkbox(int pos, const std::string& label, bool checked = false) -> std::expected<Entry, std::string>
    {
        return insert_or_unexpected(pos, label.c_str(), SDL_TRAYENTRY_CHECKBOX | ((checked) ? SDL_TRAYENTRY_CHECKED: 0));
    }

    auto insert_separator(int pos) -> std::expected<Entry, std::string>
    {
        return insert_or_unexpected(pos);
    }
private:
    auto insert_or_unexpected(int pos, const char* = nullptr, int flags = 0) -> std::expected<Entry, std::string>
    {
        auto entry_handle = SDL_InsertTrayEntryAt(_handle, pos, nullptr, flags);
        if (entry_handle == nullptr)
        {
            return std::unexpected("entry position is out of bounds");
        }
        else
        {
            Entry entry;
            entry._handle = entry_handle;
            return entry;
        }
    }
};

class Tray
{
private:
    SDL_Tray* _handle;
public:
    Tray(SDL_Surface* icon = nullptr, const std::string& tooltip = {})
    : _handle{SDL_CreateTray(icon, (tooltip.empty() ? nullptr : tooltip.c_str()))} {}

    ~Tray() { SDL_DestroyTray(_handle); }
    
    Menu new_menu()
    {
        Menu menu;
        menu._handle = SDL_CreateTrayMenu(_handle);
        return menu;
    }
};

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

    Meta::SetAppName(g_hermes_metadata.name).or_else(printunex<std::string>);
    Meta::SetAppVersion(g_hermes_metadata.version).or_else(printunex<std::string>);
    Meta::SetAppCreator(g_hermes_metadata.author).or_else(printunex<std::string>);
    Meta::SetAppCopyright(g_hermes_metadata.copyright).or_else(printunex<std::string>);
    Meta::SetAppURL(g_hermes_metadata.website_url).or_else(printunex<std::string>);
    Meta::SetAppType(g_hermes_metadata.type).or_else(printunex<std::string>);

    Init();
}

Application::~Application() 
{
    SDL_Quit();
}

void Application::Init()
{
    DisableScreenSaver().or_else(printunex<std::string>);

    auto p_image =
	IMG_Load((processpath().parent_path() / "hermes32.png").string().c_str());
    if (!p_image)
    {
        printmsg(SDL_PREFIX, "{}", SDL_GetError());
    }

    // SDL_Tray* p_icon = SDL_CreateTray(p_image, "Hermes");
    // SDL_TrayMenu* p_menu = SDL_CreateTrayMenu(p_icon);

    // SDL_TrayEntry* entry = SDL_InsertTrayEntryAt(p_menu, 0, "Quit", 0x0);
	// SDL_SetTrayEntryCallback(entry, OnClickQuit, nullptr);
    // SDL_TrayEntry* entry = SDL_InsertTrayEntryAt(p_menu, 0, nullptr, 0x0);
    // SDL_TrayEntry* entry = SDL_InsertTrayEntryAt(p_menu, 0, "Disable Sleep", SDL_TRAYENTRY_CHECKED);
    // SDL_SetTrayEntryCallback(entry, OnClickToggleSleep, nullptr);
    // SDL_TrayEntry* entry = SDL_InsertTrayEntryAt(p_menu, 0, nullptr, 0x0);
    // SDL_TrayEntry* entry = SDL_InsertTrayEntryAt(p_menu, 0, "About Hermes", 0x0);
    // SDL_SetTrayEntryCallback(entry, OnClickAbout, nullptr);

    Tray tray{p_image, "Hermes"};
    Menu menu = tray.new_menu();

    menu.insert_label(0, "Quit")->set_callback(OnClickQuit);
    menu.insert_separator(0);
    menu.insert_checkbox(0, "Disable Sleep")->set_callback(OnClickToggleSleep);
    menu.insert_separator(0);
    menu.insert_label(0, "About Hermes")->set_callback(OnClickAbout);
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