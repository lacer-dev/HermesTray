#include "pch.h"

#include "sys.h"

#include "error.h"

namespace hermes {

namespace Meta {

	std::expected<void, std::string> SetAppName(const std::string& value)
	{
		if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING,
										value.c_str()))
		{
			return std::unexpected(SDL_GetError());
		}

		return {};
	}

	std::expected<void, std::string> SetAppVersion(const std::string& value)
	{
		if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING,
										value.c_str()))
		{
			return std::unexpected(SDL_GetError());
		}

		return {};
	}

	std::expected<void, std::string> SetAppCreator(const std::string& value)
	{
		if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING,
										value.c_str()))
		{
			return std::unexpected(SDL_GetError());
		}

		return {};
	}

	std::expected<void, std::string> SetAppCopyright(const std::string& value)
	{
		if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING,
										value.c_str()))
		{
			return std::unexpected(SDL_GetError());
		}

		return {};
	}

	std::expected<void, std::string> SetAppURL(const std::string& value)
	{
		if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING,
										value.c_str()))
		{
			return std::unexpected(SDL_GetError());
		}

		return {};
	}

	std::expected<void, std::string> SetAppType(const std::string& value)
	{
		if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING,
										value.c_str()))
		{
			return std::unexpected(SDL_GetError());
		}

		return {};
	}

} // namespace Meta

}; // namespace hermes