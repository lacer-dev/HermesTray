#include "error.h"
#include "tray.h"
#include <utility>
#include <cassert>
#include <stdexcept>

namespace { 
    
constexpr int TRAY_POS_APPEND = -1; 

};

namespace afk {

////////// TrayEntry //////////

TrayEntry::TrayEntry(SDL_TrayEntry* entry)
: _entry(entry) {
    if (!_entry) {
        throw std::invalid_argument(MakeErr("tray entry can't be constructed from null\n"));
    }
}

TrayEntry::TrayEntry(TrayMenu* menu, int pos, const char* label, SDL_TrayEntryFlags flags) {
    _entry = SDL_InsertTrayEntryAt(menu->Handle(), pos, label, flags);
    if (!_entry) {
        throw std::out_of_range(MakeErr("tray entry position is out of bounds\n"));
    }
}

SDL_TrayEntry* TrayEntry::Handle() {
    return _entry;
}

void TrayEntry::SetCallback(SDL_TrayCallback callback) {
    SDL_SetTrayEntryCallback(Handle(), callback, nullptr);
}

bool TrayEntry::IsChecked() {
    return SDL_GetTrayEntryChecked(Handle());
}

void TrayEntry::SetLabel(const std::string& label) {
    SDL_SetTrayEntryLabel(Handle(), label.c_str());
}


////////// TrayMenu //////////

TrayMenu::TrayMenu(TrayIcon& icon) {
    _menu = SDL_CreateTrayMenu(icon.Handle());
    if (!_menu) {
        throw std::out_of_range(MakeErr("there was a problem creating this TrayMenu\n"));
    }
}

SDL_TrayMenu* TrayMenu::Handle() { 
    return _menu; 
}

TrayEntry& TrayMenu::AddEntryToVector(int pos, TrayEntry& entry) {
    if (pos == TRAY_POS_APPEND) {
        _entries.push_back(entry);
        return _entries.back();
    } else {
        return *_entries.insert(_entries.begin() + pos, entry);
    }
}

TrayEntry& TrayMenu::InsertEntry(int pos, const std::string& label, SDL_TrayEntryFlags flags) {
    TrayEntry entry = TrayEntry(this, pos, label.c_str(), flags);
    return AddEntryToVector(pos, entry);
}

TrayEntry& TrayMenu::InsertSeparator(int pos, SDL_TrayEntryFlags flags) {
    TrayEntry entry = TrayEntry(this, pos, nullptr, flags);
    return AddEntryToVector(pos, entry);
}

TrayEntry& TrayMenu::InsertCheckbox(int pos, const std::string& label, bool checked, SDL_TrayEntryFlags flags) {
    flags |= SDL_TRAYENTRY_CHECKBOX;
    if (checked) {
        flags |= SDL_TRAYENTRY_CHECKED;
    }
    return InsertEntry(pos, label, flags);
}

TrayEntry& TrayMenu::AddEntry(const std::string& label, SDL_TrayEntryFlags flags) {
    return InsertEntry(TRAY_POS_APPEND, label, flags);
}
    
TrayEntry& TrayMenu::AddSeparator(SDL_TrayEntryFlags flags) {
    return InsertSeparator(TRAY_POS_APPEND, flags);
}

TrayEntry& TrayMenu::AddCheckbox(const std::string& label, bool checked, SDL_TrayEntryFlags flags) {
    return InsertCheckbox(TRAY_POS_APPEND, label, checked);
}

TrayEntry& TrayMenu::Get(int pos) {
    return _entries.at(pos);
}

////////// TrayIcon //////////

TrayIcon::TrayIcon(Surface& icon, const std::string& tooltip) 
:   _icon{SDL_CreateTray(icon.Handle(), tooltip.c_str())} {
}

TrayIcon::~TrayIcon() {
    delete _menu;
    SDL_DestroyTray(_icon);
}

SDL_Tray* TrayIcon::Handle() { 
    return _icon; 
}

TrayMenu& TrayIcon::CreateMenu() {
    _menu = new TrayMenu(*this);
    return *_menu;
}

TrayMenu& TrayIcon::Menu() { 
    return *_menu; 
}

};