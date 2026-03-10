#include "foundation/IO/FileSystem.h"

#include "foundation/Diagnostics/Assert.h"

#include <fstream>
#include <sstream>

namespace Alice::Foundation::IO
{
    std::filesystem::path GetProjectRoot()
    {
        return std::filesystem::path(Alice_PROJECT_ROOT);
    }

    std::filesystem::path GetAssetRoot()
    {
        return std::filesystem::path(Alice_ASSET_ROOT);
    }

    std::filesystem::path GetAssetPath(const std::string& relativePath)
    {
        return GetAssetRoot() / relativePath;
    }

    std::string ReadAllText(const std::filesystem::path& path)
    {
        LEOCPP_ASSERT(std::filesystem::exists(path), "Requested file does not exist.");

        std::ifstream input(path, std::ios::in | std::ios::binary);
        LEOCPP_ASSERT(input.is_open(), "Failed to open file.");

        std::ostringstream stream;
        stream << input.rdbuf();
        return stream.str();
    }

    bool Exists(const std::filesystem::path& path)
    {
        return std::filesystem::exists(path);
    }
}
