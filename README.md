# HermesTray 0.1.1
A systray utility for AFK grinding.
## Features
* Toggle screensaver from system tray (automatically reenabled upon exit)
## Build
### Prerequisites:
* CMake
```
# in source root directory
./scripts/build.sh --config <config>
```
Allowed values for \<config> are "build", "release", and "relwithdebinfo". Run `./scripts/build.sh --help` for additional build options.
## Supported Platforms
* Windows