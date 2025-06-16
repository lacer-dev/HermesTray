#pragma once
#include <string>
#include <vector>
#include "surface.h"

struct SDL_TrayEntry;
struct SDL_TrayMenu;
struct SDL_TrayIcon;
typedef uint32_t SDL_TrayEntryFlags;

namespace SDL {

class Tray;
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
    TrayMenu(Tray& icon);

    SDL_TrayMenu* Handle();

    TrayEntry InsertEntry(int pos, const std::string& label);
    TrayEntry InsertSeparator(int pos);
    TrayEntry InsertCheckbox(int pos, const std::string& label, bool checked);

    int NumEntries();
private:
    SDL_TrayMenu* _menu;
};


class Tray {
public:
    Tray(Surface& icon, const std::string& tooltip);
    ~Tray();
    
    SDL_Tray* Handle();

    TrayMenu& CreateMenu();
private:
    SDL_Tray* _icon;
    TrayMenu* _menu = nullptr;
};

};