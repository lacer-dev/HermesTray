// #ifdef HM_PLATFORM_WINDOWS
#ifdef _WIN32

#include "../system.h"

namespace {

bool _screensaver_on = true;

}

namespace Hermes {

bool ScreenSaverEnabled() { 
    return _screensaver_on; 
}

bool EnableScreenSaver() {
    if (!SetThreadExecutionState(ES_CONTINUOUS))
        return false;

    _screensaver_on = true;
    return true;
}

bool DisableScreenSaver() {
    if (!SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED))
        return false;

    _screensaver_on = false;
    return true;
}

}

#endif