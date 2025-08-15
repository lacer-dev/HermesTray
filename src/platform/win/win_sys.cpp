#include "../../pch.h"

#include <array>
#include <errhandlingapi.h>
#include <expected>
#include <filesystem>
#include <libloaderapi.h>
#include <memory>
#include <system_error>
#include <windows.h>

#include "../../error.h"
#include "../../sys.h"

namespace hermes {
	namespace this_process {
		namespace {
			std::filesystem::path _path_to_this_processess() {
				std::array<char, MAX_PATH> buffer;
				if (!GetModuleFileNameA(nullptr, buffer.data(), MAX_PATH)) {
					throw std::system_error(
						GetLastError(), std::generic_category(), "failed to obtain path to executable");
				}
				return buffer.data();
			}
		} // namespace

		const std::filesystem::path& path() {
			static std::filesystem::path process_path = _path_to_this_processess();
			return process_path;
		}
	} // namespace this_process
} // namespace hermes