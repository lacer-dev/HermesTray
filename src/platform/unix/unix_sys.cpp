#include "../../pch.h"

#include <filesystem>

#include "../../sys.h"

namespace hermes {
	namespace this_process {
		const std::filesystem::path& path() {
			namespace fs = std::filesystem;
			static fs::path process_path = fs::read_symlink("/proc/self/exe");
			return process_path;
		}
	} // namespace this_process
} // namespace hermes