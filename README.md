# HermesTray
A systray utility for AFK grinding.
### Features
* Toggle display from system tray (automatically reenabled upon exit)
## Supported Platforms
* MSYS2 / MinGW
## Dependencies
With MSYS2:
```
sudo pacman -S mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-sdl3 mingw-w64-ucrt-x86_64-sdl3-image
```
## Build
### Prerequisites:
* CMake
```
# in source root directory
./scripts/build.sh --config <config>
```
Allowed values for \<config> are "debug", "release", "relwithdebinfo", and "minsizerel". Run `./scripts/build.sh --help` for additional build options.