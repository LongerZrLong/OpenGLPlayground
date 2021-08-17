#include "NameUtils.h"

std::string ParseNameFromPath(const std::string &path)
{
    std::string name;

    // parse shader name from filePath
    // e.g. assets/shaders/Texture.glsl
    size_t lastSlash = path.find_last_of("/\\");
    size_t lastDot = path.rfind('.');

    if (lastSlash == std::string::npos && lastDot == std::string::npos)
    {
        name = path;
    }
    else if (lastSlash == std::string::npos)    // lastDot != std::string::npos
    {
        name = path.substr(0, lastSlash);
    }
    else if (lastDot == std::string::npos)      // lastSlash != std::string::npos
    {
        name = path.substr(lastSlash + 1, path.size() - lastSlash - 1);
    }
    else
    {
        if (lastSlash < lastDot)
        {
            name = path.substr(lastSlash + 1, lastDot - lastSlash - 1);
        }
        else
        {
            name = path.substr(lastSlash + 1, path.size() - lastSlash - 1);
        }
    }

    return name;
}
