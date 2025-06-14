#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_tray.h>
#include <string>
#include <vector>
#include "surface.h"

class TrayIcon;
class TrayMenu;
class TrayEntry;

class TrayEntry {
public:
    // using Callback = void(* const)(TrayEntry&);

    static TrayEntry FromHandle(SDL_TrayEntry*);

    SDL_TrayEntry* Handle();

    void SetCallback(SDL_TrayCallback);
    bool IsChecked();
    void SetLabel(const std::string& label);
private:
    friend TrayMenu;
private:
    SDL_TrayEntry* _entry;
    
    TrayEntry(TrayMenu* menu, int pos, const char* label, SDL_TrayEntryFlags flags);
    TrayEntry(SDL_TrayEntry*);
};

class TrayMenu {
public:
    TrayMenu(TrayIcon& icon);

    SDL_TrayMenu* Handle();

    TrayEntry& InsertEntry(int pos, const std::string& label, SDL_TrayEntryFlags flags = 0);
    TrayEntry& InsertSeparator(int pos, SDL_TrayEntryFlags flags = 0);
    TrayEntry& InsertCheckbox(int pos, const std::string& label, bool checked = false, SDL_TrayEntryFlags flags = 0);
    TrayEntry& AddEntry(const std::string& label, SDL_TrayEntryFlags flags = 0);
    TrayEntry& AddSeparator(SDL_TrayEntryFlags flags = 0);
    TrayEntry& AddCheckbox(const std::string& label, bool checked = false, SDL_TrayEntryFlags flags = 0);

    TrayEntry& Get(int pos);
private:
    SDL_TrayMenu* _menu;
    std::vector<TrayEntry> _entries;

    TrayEntry& AddEntryToVector(int pos, TrayEntry& entry);
};

class TrayIcon {
public:
    TrayIcon(Surface& icon, const std::string& tooltip);
    ~TrayIcon();
    
    SDL_Tray* Handle();

    TrayMenu& CreateMenu();
    TrayMenu& Menu();
private:
    SDL_Tray* _icon;
    TrayMenu* _menu = nullptr;
};