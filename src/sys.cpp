#include "sys.h"
#include "error.h"
#include "pch.h"
// #include <unistd.h>

namespace Hermes
{

std::filesystem::path processpath()
{
    // std::string buffer;
    // buffer.reserve(1024);

    // int bytes_written = readlink("/proc/self/exe", buffer.data(),
    // buffer.size()); if (bytes_written == -1)
    // {
    //     std::perror("readlink()");
    // }
    // buffer.data()[bytes_written] == '\0';

    // return {buffer};

    return std::filesystem::path{SDL_GetBasePath()} / "hermes.exe";
}

namespace Meta {
        
std::expected<void, std::string> SetAppName(const std::string& value)
{
	if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, value.c_str()))
    {
		return std::unexpected(SDL_GetError());
	}

	return {};
}

std::expected<void, std::string> SetAppVersion(const std::string& value)
{
	if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, value.c_str()))
    {
		return std::unexpected(SDL_GetError());
	}

	return {};
}

std::expected<void, std::string> SetAppCreator(const std::string& value)
{
	if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, value.c_str()))
    {
		return std::unexpected(SDL_GetError());
	}

	return {};
}

std::expected<void, std::string> SetAppCopyright(const std::string& value)
{
	if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, value.c_str()))
    {
		return std::unexpected(SDL_GetError());
	}

	return {};
}

std::expected<void, std::string> SetAppURL(const std::string& value)
{
	if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, value.c_str()))
    {
		return std::unexpected(SDL_GetError());
	}

	return {};
}

std::expected<void, std::string> SetAppType(const std::string& value)
{
	if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, value.c_str()))
    {
		return std::unexpected(SDL_GetError());
	}

	return {};
}

}

}; // namespace Hermes