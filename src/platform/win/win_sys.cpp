#include "../../pch.h"

#include <errhandlingapi.h>
#include <libloaderapi.h>
#include <windows.h>

#include <array>
#include <filesystem>
#include <system_error>

#include "../../sys.h"

namespace hermes::this_process {
	[[nodiscard]] std::filesystem::path get_path_to_this_process() {
		std::array<char, MAX_PATH> buffer;

		// output a null-terminated string
		if (!GetModuleFileNameA(nullptr, buffer.data(), MAX_PATH)) {
			static std::string error_message = "Failed to obtain path to executable";
			throw std::system_error(GetLastError(), std::generic_category(), error_message);
		}

		return buffer.data();
	}

	std::filesystem::path path() {
		static std::filesystem::path process_path = get_path_to_this_process();
		return process_path;
	}
} // namespace hermes::this_process