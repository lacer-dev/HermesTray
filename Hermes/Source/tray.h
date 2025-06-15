#pragma once
#include <string>
#include <vector>
#include "surface.h"

struct SDL_TrayEntry;
struct SDL_TrayMenu;
struct SDL_TrayIcon;
typedef uint32_t SDL_TrayEntryFlags;

namespace sdl {

class TrayIcon;
class TrayMenu;
class TrayEntry;


class TrayEntry {
public:
    TrayEntry(SDL_TrayEntry* handle);

    SDL_TrayEntry* Handle();

    void SetCallback(SDL_TrayCallback);
    bool IsChecked();
    void SetLabel(const std::string& label);
private:
    SDL_TrayEntry* _entry;    
};


class TrayMenu {
public:
    TrayMenu(TrayIcon& icon);

    SDL_TrayMenu* Handle();

    TrayEntry InsertEntry(int pos, const std::string& label);
    TrayEntry InsertSeparator(int pos);
    TrayEntry InsertCheckbox(int pos, const std::string& label, bool checked);
private:
    SDL_TrayMenu* _menu;
};


class TrayIcon {
public:
    TrayIcon(sdl::Surface& icon, const std::string& tooltip);
    ~TrayIcon();
    
    SDL_Tray* Handle();

    TrayMenu& CreateMenu();
private:
    SDL_Tray* _icon;
    TrayMenu* _menu = nullptr;
};

};