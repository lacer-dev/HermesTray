#pragma once
#include <string>

namespace SDL {
    
constexpr int INIT_VIDEO = SDL_INIT_VIDEO;
bool Init(int systems);
void Quit();
int WasInit(int flags = 0);
bool SetMetadataName(const std::string& value);
bool SetMetadataVersion(const std::string& value);
bool SetMetadataCreator(const std::string& value);
bool SetMetadataCopyright(const std::string& value);
bool SetMetadataUrl(const std::string& value);
bool SetMetadataType(const std::string& value);
std::string GetError();

};