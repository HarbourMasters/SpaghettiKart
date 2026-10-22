#pragma once

#ifdef __cplusplus
#include <string>
#include <map>
#include <stdexcept>
#include <toml++/toml.hpp>
#include "semver.hpp"

struct ModMetadata {
    std::string name;
    semver::version<> version;
    std::map<std::string, std::pair<semver::range_set<>, std::string>> dependencies;

    ModMetadata() {}
    // Throws std::runtime_error for invalid TOML, versions or dependency ranges.
    static ModMetadata LoadFromTOML(const std::string& tomlContent) {
        ModMetadata metadata;
        try {
            auto table = toml::parse(tomlContent);
            auto mods_property = table["mod"];

            if (auto nameValue = mods_property["name"].value<std::string>()) {
                metadata.name = *nameValue;
            }

            if (auto versionValue = mods_property["version"].value<std::string>()) {
                const auto result = semver::parse(*versionValue, metadata.version);
                if (!result || result.ptr != versionValue->data() + versionValue->size()) {
                    throw std::runtime_error("Invalid mod.version: '" + *versionValue + "'");
                }
            }

            if (auto* depsTable = table["dependencies"].as_table()) {
                for (const auto& [key, value] : *depsTable) {
                    if (auto depVersion = value.value<std::string>()) {
                        semver::range_set<> parsedVersion;
                        const auto result = semver::parse(*depVersion, parsedVersion);
                        if (!result || result.ptr != depVersion->data() + depVersion->size()) {
                            throw std::runtime_error("Invalid dependency '" + std::string(key) + "' range: '" +
                                                     *depVersion + "'");
                        }
                        metadata.dependencies[std::string(key)] = std::make_pair(parsedVersion, *depVersion);
                    }
                }
            }
        } catch (const toml::parse_error& err) { throw std::runtime_error(std::string("Invalid TOML: ") + err.what()); }

        return metadata;
    }

    std::string ToString() const;
};
#endif
