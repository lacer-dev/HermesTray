#include "../../pch.h"

#include "../../sys.h"

namespace hermes {

namespace screensaver {

	static bool _is_screensaver_on = [] { return enable().has_value(); }();

} // namespace screensaver

std::expected<void, std::string> screensaver::enable()
{
	if (!SetThreadExecutionState(ES_CONTINUOUS))
	{
		return std::unexpected("could not enable screensaver");
	}

	_is_screensaver_on = true;
	return {};
}

std::expected<void, std::string> screensaver::disable()
{
	if (!SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED))
	{
		return std::unexpected("could not disable screensaver");
	}

	_is_screensaver_on = false;
	return {};
}

bool screensaver::is_enabled() { return _is_screensaver_on; }

} // namespace hermes