#pragma once

#include <functional>
#include <string>

struct SDL_Tray;
struct SDL_TrayEntry;
struct SDL_TrayMenu;
struct SDL_Surface;

namespace hermes::systray {

class entry
{
public:
	using callback_type = std::function<void(entry&)>;

	void set_callback(const callback_type& callback);

	bool is_checked();

private:
	friend class menu;

	SDL_TrayEntry* _handle = nullptr;
	callback_type _callback = nullptr;

	static void _invoke_entry_callback(void* ptr_entry, SDL_TrayEntry*);
};

class menu
{
public:
	entry insert_label(int pos, const std::string& label);
	entry insert_checkbox(int pos,
						  const std::string& label,
						  bool checked = false);
	entry insert_separator(int pos);

private:
	friend class tray_object;

	SDL_TrayMenu* _handle;

	entry _insert_or_throw(int pos, const char* = nullptr, int flags = 0);
};

class tray_object
{
public:
	tray_object(SDL_Surface* icon = nullptr, const std::string& tooltip = {});
	~tray_object();
	menu new_menu();

private:
	SDL_Tray* _handle;
};

} // namespace hermes::systray