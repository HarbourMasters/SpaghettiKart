#pragma once

#ifdef __cplusplus
#include "spdlog/spdlog.h"
#include <string>
#include <map>
#include <toml++/toml.hpp>
#include "semver.hpp"

struct ModMetadata {
    std::string name;
    semver::version<int, int, int> version;
    std::map<std::string, std::pair<semver::range_set<int, int, int>, std::string>> dependencies;

    ModMetadata() {}
    static ModMetadata LoadFromTOML(const std::string& tomlContent) {
        ModMetadata metadata;
        try {
            auto table = toml::parse(tomlContent);
            auto mods_property = table["mod"];

            if (auto nameValue = mods_property["name"].value<std::string>()) {
                metadata.name = *nameValue;
            }

            if (auto versionValue = mods_property["version"].value<std::string>()) {
                semver::parse(*versionValue, metadata.version);
            }

            if (auto *depsTable = table["dependencies"].as_table()) {
                for (const auto& [key, value] : *depsTable) {
                    if (auto depVersion = value.value<std::string>()) {
                        semver::range_set<int, int, int> parsedVersion;
                        semver::parse(*depVersion, parsedVersion);
                        metadata.dependencies[std::string(key)] = std::make_pair(parsedVersion, *depVersion);
                    }
                }
            }
        } catch (const toml::parse_error& err) {
            SPDLOG_ERROR("Failed to parse mods.toml: {}", err.what());
        }

        return metadata;
    }

    std::string ToString() const;
};
#endif