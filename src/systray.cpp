#include "systray.h"
#include "SDLCPP/SDLCPP.h"
#include "pch.h"

namespace Hermes::Systray
{

//////////////////////////////////////// EntryData ////////////////////////////////////////

EntryData& EntryData::Disable()
{
    flags |= SDL_TRAYENTRY_DISABLED;
    return *this;
}

//////////////////////////////////////// EntryData Factories ////////////////////////////////////////

EntryData Separator()
{
    return {};
}

EntryData Label(const std::string& label)
{
    return {.label = label};
}

EntryData Button(const std::string& label)
{
    return {
	.label = label,
	.flags = SDL_TRAYENTRY_BUTTON};
}

EntryData Checkbox(const std::string& label, const bool checked)
{
    return {
	.label = label,
	.flags = SDL_TRAYENTRY_CHECKBOX | (checked ? SDL_TRAYENTRY_CHECKED : 0)};
}

EntryData Submenu(const std::string& label)
{
    return {
	.label = label,
	.flags = SDL_TRAYENTRY_SUBMENU};
}

//////////////////////////////////////// Entry ////////////////////////////////////////

bool Entry::Checked() const
{
    return SDL_GetTrayEntryChecked(Handle());
}

//////////////////////////////////////// TrayMenu ////////////////////////////////////////

Entry& TMenu::Insert(const std::size_t pos, const EntryData& entry)
{
    if (pos > std::numeric_limits<int>::max())
    {
	throw std::out_of_range{std::format("menu entry position (which is {}) is out of range", pos)};
    }

    auto entry_handle = SDL_InsertTrayEntryAt(
	Handle(),
	static_cast<int>(pos),
	entry.CStr(),
	entry.flags);

    SDL_SetTrayEntryCallback(entry_handle, entry.callback, nullptr);

    return *_entries.emplace(_entries.begin() + static_cast<int>(pos), entry_handle);
}

Entry& TMenu::At(const std::size_t pos)
{
    return _entries.at(pos);
}

void TMenu::Remove(const std::size_t pos)
{
    const auto it = _entries.erase(_entries.begin() + pos);
    SDL_RemoveTrayEntry(it->Handle());
}

std::size_t TMenu::Length(std::size_t pos) const
{
    // int len;
    // SDL_GetTrayEntries(Handle(), &len);
    // return static_cast<std::size_t>(len);
    return _entries.size();
}

//////////////////////////////////////// Icon ////////////////////////////////////////

TIcon::TIcon(handle_type* handle) : _icon{handle}, _menu{SDL_GetTrayMenu(_icon)}
{
}

TIcon::TIcon(SDL::Surface& icon, const std::string& tooltip, const bool with_menu)
    : _icon{SDL_CreateTray(icon.Handle(), tooltip.c_str())},
      _menu{with_menu ? SDL_CreateTrayMenu(Handle()) : nullptr}
{
}

TIcon::~TIcon()
{
    SDL_DestroyTray(_icon);
}

}; // namespace Hermes::Systray