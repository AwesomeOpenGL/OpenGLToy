#pragma once

#include <filesystem>
#include <string>

namespace Alice::Foundation::IO
{
    std::filesystem::path GetProjectRoot();
    std::filesystem::path GetAssetRoot();
    std::filesystem::path GetAssetPath(const std::string& relativePath);
    std::string ReadAllText(const std::filesystem::path& path);
    bool Exists(const std::filesystem::path& path);
}
