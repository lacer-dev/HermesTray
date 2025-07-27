#include "pch.h"

#include "app.h"

#include <expected>
#include <memory>

#include "aliases.h"
#include "error.h"
#include "sys.h"
#include "systray.h"

using namespace hermes;

namespace {

struct
{
	const std::string name = "HermesTray";
	const std::string version = "0.1.1";
	const std::string author = "Leon Allotey";
	const std::string copyright = "Copyright (c) 2025 Leon Allotey";
	const std::string website_url = "https://github.com/lacer-dev/HermesTray";
	const std::string type = "application";
} g_hermes_metadata;

void ShowErrorMessageBox(const std::string& message)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
							 messagebox_title_error().c_str(), message.c_str(),
							 nullptr);
}

void OnClickToggleSleep(systray::entry& entry)
{
	auto result = screensaver::is_enabled() ? screensaver::disable() : screensaver::enable();
	if (!result)
	{
		ShowErrorMessageBox(result.error());
	}

	hermes_assert((entry.is_checked() == screensaver::is_enabled()));
}

void OnClickQuit(systray::entry&)
{
	// !SDL_PushEvent(std::unique_ptr<SDL_Event>{new SDL_Event{.type = SDL_EVENT_QUIT}});
	if (SDL_Event event{SDL_EVENT_QUIT}; !SDL_PushEvent(&event))
	{
		eprintln("{}: {}", prefixes::SDL, SDL_GetError());
	}
}

void OnClickAbout(systray::entry&)
{
	if (!SDL_OpenURL(g_hermes_metadata.website_url.c_str()))
	{
		ShowErrorMessageBox("could not open url");
		eprintln("{}: {}", prefixes::SDL, SDL_GetError());
	}
}

}; // namespace

namespace hermes {

application::application()
{
	// Set metadata for app
	if (std::atexit(SDL_Quit) != 0)
	{
		printerror("failed to register 'SDL_Quit' with std::atexit");
	}

	if (std::atexit([] { screensaver::enable(); }) != 0)
	{
		printerror("failed to register 'screensaver::enable' with std::atexit");
	}

	v_expec_str res;

	if (!(res = Meta::SetAppName(g_hermes_metadata.name)))
	{
		printerror("{}", res.error());
	}

	if (!(res = res = Meta::SetAppVersion(g_hermes_metadata.version)))
	{
		printerror("{}", res.error());
	}

	if (!(res = res = Meta::SetAppCreator(g_hermes_metadata.author)))
	{
		printerror("{}", res.error());
	}

	if (!(res = res = Meta::SetAppCopyright(g_hermes_metadata.copyright)))
	{
		printerror("{}", res.error());
	}

	if (!(res = res = Meta::SetAppURL(g_hermes_metadata.website_url)))
	{
		printerror("{}", res.error());
	}

	if (!(res = res = Meta::SetAppType(g_hermes_metadata.type)))
	{
		printerror("{}", res.error());
	}

	init();
}

application::~application()
{
	SDL_Quit();
}

void application::init()
{

	if (auto res = screensaver::disable(); !res)
	{
		printerror("{}", res.error());
	}

	auto p_icon = IMG_Load((process::procdir() / "hermes32.png").string().c_str());
	if (!p_icon)
	{
		eprintln("{}: {}", prefixes::SDL, SDL_GetError());
	}

	systray::tray_object tray{p_icon, "Hermes"};
	systray::menu menu = tray.new_menu();

	menu.insert_label(0, "Quit").set_callback(OnClickQuit);
	menu.insert_separator(0);
	menu.insert_checkbox(0, "Disable Sleep").set_callback(OnClickToggleSleep);
	menu.insert_separator(0);
	menu.insert_label(0, "About Hermes").set_callback(OnClickAbout);
}

void application::Run()
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

}; // namespace hermes