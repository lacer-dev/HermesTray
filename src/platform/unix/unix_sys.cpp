#include "../../pch.h"

#include <filesystem>

#include "../../sys.h"

namespace hermes::this_process {
	std::filesystem::path path() { return std::filesystem::read_symlink("/proc/self/exe"); }
} // namespace hermes::this_process