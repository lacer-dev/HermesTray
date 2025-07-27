#pragma once

#include <expected>
#include <filesystem>
#include <string>

namespace hermes {

namespace screensaver {

	std::expected<void, std::string> enable();
	std::expected<void, std::string> disable();
	bool is_enabled();

} // namespace screensaver

namespace process {

	std::filesystem::path procpath();

	inline std::filesystem::path procdir() { return procpath().parent_path(); }

	inline std::string procname() { return procpath().filename().string(); }

} // namespace process

namespace Meta {

	std::expected<void, std::string> SetAppName(const std::string& value);
	std::expected<void, std::string> SetAppVersion(const std::string& value);
	std::expected<void, std::string> SetAppCreator(const std::string& value);
	std::expected<void, std::string> SetAppCopyright(const std::string& value);
	std::expected<void, std::string> SetAppURL(const std::string& value);
	std::expected<void, std::string> SetAppType(const std::string& value);

}; // namespace Meta

}; // namespace hermes