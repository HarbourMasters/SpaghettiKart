#include "ModMetadata.h"
#include <spdlog/spdlog.h>

std::string ModMetadata::ToString() const {
    std::string result = "Mod Name: " + name + "\n";
    result += "Version: " + version.to_string() + "\n";
    result += "Dependencies:\n";
    for (const auto& [depName, depVersion] : dependencies) {
        result.append("  - ").append(depName).append(": ").append(depVersion.second).append("\n");
    }
    return result;
}