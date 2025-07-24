#pragma once

#include <optional>
#include <string>
#include <vector>

namespace SDL
{

class Surface;

}

using SDL_TrayCallback = void (*)(void* userdata, SDL_TrayEntry* entry);
struct SDL_TrayEntry;
struct SDL_TrayMenu;
struct SDL_Tray;

namespace Hermes::Systray
{

struct EntryData
{
    std::optional<std::string> label;
    SDL_TrayCallback callback = nullptr;
    unsigned int flags = 0;

    EntryData& Disable();
    EntryData& SetCallback(const SDL_TrayCallback cb)
    {
	this->callback = cb;
	return *this;
    }

    const char* CStr() const
    {
	return label.has_value() ? label->c_str() : nullptr;
    }
};

EntryData Separator();
EntryData Label(const std::string& label);
EntryData Button(const std::string& label);
EntryData Checkbox(const std::string& label, bool checked);
EntryData Submenu(const std::string& label);

class Entry
{
public:
    using handle_type = SDL_TrayEntry;

    explicit Entry(SDL_TrayEntry* handle) : _handle(handle) {}

    handle_type* Handle() const { return _handle; }

    bool Checked() const;

private:
    handle_type* _handle;
};

class TMenu
{
public:
    using handle_type = SDL_TrayMenu;

    explicit TMenu(handle_type* handle) : _menu{handle} {}

    handle_type* Handle() const { return _menu; }

    Entry& Insert(std::size_t pos, const EntryData& entry);

    Entry& At(std::size_t pos);
    void Remove(std::size_t pos);
    std::size_t Length(std::size_t pos) const;

private:
    handle_type* _menu;
    std::vector<Entry> _entries;
};

class TIcon
{
public:
    using handle_type = SDL_Tray;

    explicit TIcon(handle_type* handle);
    TIcon(SDL::Surface& icon, const std::string& tooltip, bool with_menu = true);
    ~TIcon();

    handle_type* Handle() const { return _icon; }
    TMenu& Menu() { return _menu; }

private:
    handle_type* _icon;
    TMenu _menu;
};

}; // namespace Hermes::Systray