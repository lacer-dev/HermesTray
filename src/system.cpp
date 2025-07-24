#include "system.h"
#include "SDLCPP/SDLCPP.h"
#include "error.h"
#include "pch.h"
#include <memory>
#include <unistd.h>

namespace Hermes
{

//////////////////// ResourceManager ////////////////////

ResourceManager::ResourceManager(const std::filesystem::path& resources)
{
    using namespace std::filesystem;

    if (!exists(resources))
    {
	SDL::ShowSimpleMessageBoxError(messagebox_title_error(), directory_not_found(resources.string()));
    }

    _directory = resources;
}

std::filesystem::path ResourceManager::GetResource(const std::string& filename)
{
    using namespace std::filesystem;

    const path file = Directory() / filename;
    if (!exists(file))
    {
	SDL::ShowSimpleMessageBoxError(messagebox_title_error(), file_not_found(file.string()));
    }

    return file;
}

std::filesystem::path processpath()
{
    // std::unique_ptr<char[]> buffer{new char[256]};
    // const int bytes_written = readlink("/proc/self/exe", buffer.get(), 256);
    // if (bytes_written == -1)
    // {
	// std::perror("readlink()");
    // }
    // buffer[bytes_written] == '\0';
    // return std::filesystem::path{buffer.get()};

    return std::filesystem::path{SDL_GetBasePath()} / "hermes.exe";
}

}; // namespace Hermes