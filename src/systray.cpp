#include "pch.h"

#include "systray.h"

namespace hermes {
namespace systray {

	void entry::set_callback(const callback_type& callback)
	{
		_callback = callback;
		SDL_SetTrayEntryCallback(_handle, _invoke_entry_callback, this);
	}

	bool entry::is_checked() { return SDL_GetTrayEntryChecked(_handle); }

	void entry::_invoke_entry_callback(void* ptr_entry, SDL_TrayEntry*)
	{
		auto& ref_entry = *static_cast<entry*>(ptr_entry);
		ref_entry._callback(ref_entry);
	}

	entry menu::insert_label(int pos, const std::string& label)
	{
		return _insert_or_throw(pos, label.c_str());
	}

	entry menu::insert_checkbox(int pos, const std::string& label, bool checked)
	{
		return _insert_or_throw(pos, label.c_str(),
								SDL_TRAYENTRY_CHECKBOX |
									(checked ? SDL_TRAYENTRY_CHECKED : 0));
	}

	entry menu::insert_separator(int pos) { return _insert_or_throw(pos); }

	entry menu::_insert_or_throw(int pos, const char* label, int flags)
	{
		auto entry_handle = SDL_InsertTrayEntryAt(_handle, pos, label, flags);
		if (entry_handle == nullptr)
		{
			std::out_of_range(
				std::format("entry position (which is {}) is out of bounds", pos));
		}

		entry entry;
		entry._handle = entry_handle;
		return entry;
	}

	tray_object::tray_object(SDL_Surface* icon, const std::string& tooltip)
		: _handle{
			  SDL_CreateTray(icon, (tooltip.empty() ? nullptr : tooltip.c_str()))}
	{}

	tray_object::~tray_object() { SDL_DestroyTray(_handle); }

	menu tray_object::new_menu()
	{
		menu menu;
		menu._handle = SDL_CreateTrayMenu(_handle);
		return menu;
	}

} // namespace systray
} // namespace hermes