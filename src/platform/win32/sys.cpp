#include "../../pch.h"

#include "../../sys.h"

namespace hermes {

namespace process {

	std::filesystem::path procpath()
	{
		// consider using _get_pgmptr instead of GetModuleFileNameA

		// reset last error so we can check for if this function fails
		SetLastError(0);

		static constexpr std::size_t _size = 1024;
		auto _buffer = std::unique_ptr<char[]>{new char[_size]};

		std::size_t _len = GetModuleFileNameA(nullptr, _buffer.get(), _size);
		if (_len == 0)
		{
			throw std::length_error("failed to retrieve path to this executable");
		}
		else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			throw std::length_error(std::format("length of executable path (which is greater than {}) exceeds limit", _len));
		}

		return std::filesystem::path{_buffer.get()};
	}

} // namespace process

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