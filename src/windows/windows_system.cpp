// #ifdef HM_PLATFORM_WINDOWS
#include <expected>
#ifdef _WIN32

#    include "../system.h"

static bool _is_screensaver_on = true;

namespace Hermes
{

std::expected<void, std::string> EnableScreenSaver()
{
    if (!SetThreadExecutionState(ES_CONTINUOUS))
    {
	return std::unexpected("could not enable screensaver");
    }

    _is_screensaver_on = true;
    return {};
}

std::expected<void, std::string> DisableScreenSaver()
{
    if (!SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED))
    {
	return std::unexpected("could not disable screensaver");
    }

    _is_screensaver_on = false;
    return {};
}

bool ScreenSaverEnabled()
{
    return _is_screensaver_on;
}

} // namespace Hermes

#endif