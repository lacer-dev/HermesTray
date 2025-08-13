# HermesTray
A systray utility for AFK grinding.

Allows the user to disable/enable thescreensaver from system tray (automatically reenabled upon exit).
## Supported (Tested) Platforms
* MSYS2 / MinGW
## Dependencies
With MSYS2 (in the UCRT environment):
```
pacman -S mingw-w64-ucrt-x86_64-cmake \
    mingw-w64-ucrt-x86_64-make \
    mingw-w64-ucrt-x86_64-sdl3 \
    mingw-w64-ucrt-x86_64-sdl3-image
```
## Build
Run the build script in the root directory: 
```
chmod +x ./build.sh
./build.sh --config <config>
```
Allowed values for \<config> are "debug", "release", "relwithdebinfo", and "minsizerel". Run `./build.sh --help` for additional build options.

Only debug works at the moment. 
## 