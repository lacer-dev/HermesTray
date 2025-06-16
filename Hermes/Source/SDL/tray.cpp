#include "../pch.h"
#include "../error.h"
#include "tray.h"

namespace SDL {


//////////////////// TrayEntry ////////////////////

TrayEntry::TrayEntry(SDL_TrayEntry* entry)
: _entry(entry) {
    if (!_entry)
        throw std::invalid_argument(err::make_error("tray entry can't be constructed from null\n"));
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


//////////////////// TrayMenu ////////////////////

TrayMenu::TrayMenu(Tray& icon) {
    _menu = SDL_CreateTrayMenu(icon.Handle());
    if (!_menu)
        throw std::out_of_range(err::make_error("there was a problem creating this TrayMenu\n"));
}

SDL_TrayMenu* TrayMenu::Handle() { 
    return _menu; 
}

SDL_TrayEntryFlags GetDisabledFlag(bool disabled) {
    return (disabled) ? SDL_TRAYENTRY_DISABLED: 0;
}

TrayEntry TrayMenu::InsertEntry(int pos, const std::string& label) {
    return {SDL_InsertTrayEntryAt(Handle(), pos, label.c_str(), 0)};
}

TrayEntry TrayMenu::InsertSeparator(int pos) {
    return {SDL_InsertTrayEntryAt(Handle(), pos, nullptr, 0)};
}

TrayEntry TrayMenu::InsertCheckbox(int pos, const std::string& label, bool checked) {
    int flags = SDL_TRAYENTRY_CHECKBOX;
    if (checked)
        flags |= SDL_TRAYENTRY_CHECKED;
    return SDL_InsertTrayEntryAt(Handle(), pos, label.c_str(), flags);
}

int TrayMenu::NumEntries() {
    int count;
    SDL_GetTrayEntries(Handle(), &count);
    return count;
}

//////////////////// Tray ////////////////////

Tray::Tray(Surface& icon, const std::string& tooltip) 
:   _icon{SDL_CreateTray(icon.Handle(), tooltip.c_str())} {
}

Tray::~Tray() {
    delete _menu;
    SDL_DestroyTray(_icon);
}

SDL_Tray* Tray::Handle() { 
    return _icon; 
}

TrayMenu& Tray::CreateMenu() {
    _menu = new TrayMenu(*this);
    return *_menu;
}

};