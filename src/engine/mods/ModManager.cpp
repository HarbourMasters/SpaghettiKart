#include "ship/resource/archive/Archive.h"
#include "ModMetadata.h"
#include "ship/resource/archive/FolderArchive.h"
#include "ship/resource/archive/O2rArchive.h"
#include "port/Engine.h"
#include "semver.hpp"
#include "utils/StringHelper.h"
#include <memory>
#include <optional>
#include <string>

#include "ModManager.h"

void CheckMK64O2RExists();
void FindAndLoadMods();
void PrintModInfo();
void DetectCyclicDependencies();
void DetectOutdatedDependencies();
void SortModsByDependencies();

std::vector<std::tuple<ModMetadata, std::shared_ptr<Ship::Archive>>> Mods = {};

void InitModsSystem() {
    CheckMK64O2RExists();

    FindAndLoadMods();

    PrintModInfo();

    DetectCyclicDependencies();

    DetectOutdatedDependencies();

    SortModsByDependencies();

    std::vector<std::shared_ptr<Ship::Archive>> loadedArchives;
    loadedArchives.reserve(Mods.size());
    for (const auto& [_, archive] : Mods) {
        if (archive == nullptr) {
            continue;
        }
        loadedArchives.push_back(archive);
    }
    auto context = GameEngine::Instance->context;
    auto resourceManager = context->GetResourceManager();
    auto archiveManager = resourceManager->GetArchiveManager();
    archiveManager->SetArchives(std::make_shared<std::vector<std::shared_ptr<Ship::Archive>>>(loadedArchives));
}

void UnloadMods() {
    Mods.clear();
}

void GenerateAssetsMods() {
    if (GameEngine::ShowYesNoBox("No O2R Files", "No O2R files found. Generate one now?") == IDYES) {
        if (!GameEngine::GenAssetFile()) {
            GameEngine::ShowMessage("Error", "An error occured, no O2R file was generated.\n\nExiting...");
            exit(1);
        }
    } else {
        exit(1);
    }
}

std::vector<std::string> ListMods() {
    const std::string main_path = Ship::Context::GetPathRelativeToAppDirectory(game_asset_file);
    const std::string assets_path = Ship::Context::LocateFileAcrossAppDirs(engine_asset_file);

    std::vector<std::string> archiveFiles;
    if (std::filesystem::exists(main_path)) {
        archiveFiles.push_back(main_path);
    } else { // should not happen, but just in case
        GenerateAssetsMods();
        archiveFiles.push_back(main_path);
    }

    if (std::filesystem::exists(assets_path)) {
        archiveFiles.push_back(assets_path);
    }

    const std::string mods_path = Ship::Context::GetPathRelativeToAppDirectory("mods");

    // Create mods folder if it doesn't exist
    if (!std::filesystem::exists(mods_path)) {
        std::filesystem::create_directories(mods_path);
        SPDLOG_INFO("Created mods folder at: {}", mods_path);
    }

    if (std::filesystem::exists(mods_path) && std::filesystem::is_directory(mods_path)) {
        for (const auto& p : std::filesystem::directory_iterator(mods_path)) {
            auto ext = p.path().extension().string();
            if (StringHelper::IEquals(ext, ".zip") || StringHelper::IEquals(ext, ".o2r") || std::filesystem::is_directory(p.path())) {
                archiveFiles.push_back(p.path().generic_string());
            }
        }
    }

    return archiveFiles;
}

std::optional<std::vector<std::string>> CheckCyclicDependencies() {
    auto list = std::vector<std::string>{};
    for (const auto& [metaA, _] : Mods) {
        for (const auto& [metaB, _] : Mods) {
            if (metaA.name != metaB.name) {
                // If A depends on B and B depends on A, we have a cycle
                if (metaA.dependencies.contains(metaB.name) &&
                    metaB.dependencies.contains(metaA.name)) {
                    list.push_back(metaA.name + " <-> " +  metaB.name);
                }
            }
        }
    }
    if (!list.empty()) {
        return list;
    }
    return std::nullopt;
}

std::optional<std::vector<std::string>> CheckOutdatedDependencies(const ModMetadata& mod) {
    auto list = std::vector<std::string>{};
    for (const auto& [depName, depVersion] : mod.dependencies) {
        bool found = false;
        for (const auto& [otherMeta, _] : Mods) {
            if (otherMeta.name == depName) {
                found = true;
                auto range = depVersion.first;
                if (!range.contains(otherMeta.version)) {
                    list.push_back(depName + " (required: " + depVersion.second + ", found: " + otherMeta.version.to_string() + ")");
                }
                break;
            }
        }
        if (!found) {
            list.push_back(depName + " (required: " + depVersion.second + ", found: none)");
        }
    }
    if (!list.empty()) {
        return list;
    }
    return std::nullopt;
}

void SortModsByDependencies() {
    // Core assets that should always be loaded first (at the bottom of the priority stack)
    static const std::vector<std::string> coreAssets = { "mk64-assets", "extended-assets" };

    std::sort(Mods.begin(), Mods.end(),
              [](const std::tuple<ModMetadata, std::shared_ptr<Ship::Archive>>& a,
                 const std::tuple<ModMetadata, std::shared_ptr<Ship::Archive>>& b) {
                  const ModMetadata& metaA = std::get<0>(a);
                  const ModMetadata& metaB = std::get<0>(b);

                  // Check if either mod is a core asset
                  auto itA = std::find(coreAssets.begin(), coreAssets.end(), metaA.name);
                  auto itB = std::find(coreAssets.begin(), coreAssets.end(), metaB.name);
                  bool isACoreAsset = itA != coreAssets.end();
                  bool isBCoreAsset = itB != coreAssets.end();

                  // Core assets should come first (lower priority index)
                  if (isACoreAsset && !isBCoreAsset) {
                      return true;
                  }
                  if (!isACoreAsset && isBCoreAsset) {
                      return false;
                  }
                  // If both are core assets, sort by their order in coreAssets
                  if (isACoreAsset && isBCoreAsset) {
                      return itA < itB;
                  }

                  // If A depends on B, A should come after B
                  if (metaA.dependencies.contains(metaB.name)) {
                      return false;
                  }
                  // If B depends on A, B should come after A
                  if (metaB.dependencies.contains(metaA.name)) {
                      return true;
                  }
                  // Otherwise, maintain original order
                  return false;
              });
}

void AddModMetadata(const ModMetadata& metadata, const std::shared_ptr<Ship::Archive>& archive) {
    Mods.push_back(std::make_tuple(metadata, archive));
}

void AddCoreDependencies() {
    ModMetadata meta;
    meta.name = "spaghettikart-core";
    semver::parse(SPAGHETTI_VERSION, meta.version);

    semver::range_set<int, int, int> mk64Ver;
    semver::parse("1.0.0-alpha1", mk64Ver);
    semver::range_set<int, int, int> assetsVer;
    semver::parse("1.0.0-alpha1", assetsVer);
    meta.dependencies = {
        {"mk64-assets", {mk64Ver, "1.0.0-alpha1"}},
        {"extended-assets", {assetsVer, "1.0.0-alpha1"}},
    };
    AddModMetadata(meta, nullptr);
}

void CheckMK64O2RExists() {
    const std::string main_path = Ship::Context::GetPathRelativeToAppDirectory(game_asset_file);

    if (!std::filesystem::exists(main_path)) {
        GenerateAssetsMods();
    }
}

void FindAndLoadMods() {
    Mods.clear();
    AddCoreDependencies();
    auto list = ListMods();
    for (const auto& path : list) {
        const std::string extension = std::filesystem::path(path).extension().string();
        std::shared_ptr<Ship::Archive> archive = nullptr;
        if (StringHelper::IEquals(extension, ".o2r") || StringHelper::IEquals(extension, ".zip")) {
            archive = dynamic_pointer_cast<Ship::Archive>(std::make_shared<Ship::O2rArchive>(path));
        } else if (StringHelper::IEquals(extension, "") && std::filesystem::is_directory(path)) {
            archive = dynamic_pointer_cast<Ship::Archive>(std::make_shared<Ship::FolderArchive>(path));
        } else if (StringHelper::IEquals(extension, ".disabled")) {
            // Skip disabled mods
            continue;
        } else {
            SPDLOG_WARN("The file {} has an unrecognized extension ({}), ignoring it.", path, extension);
            continue;
        }

        int nb_unknown_files = 0;
        archive->Load();
        ModMetadata metadata;
        auto mods_file = archive->LoadFile("mods.toml");
        if (mods_file != nullptr) {
            metadata = ModMetadata::LoadFromTOML(std::string(mods_file->Buffer->data(), mods_file->Buffer->size()));
            SPDLOG_INFO("Loaded mod: {}\n{}", metadata.name, metadata.ToString());
        } else {
            const std::string msg = "The archive at path " + path +
                                    " is missing a mods.toml file. The Mod are likely incompatible.\n\n"
                                    "Do you want to continue loading the mods?";
            if (GameEngine::ShowYesNoBox("Missing mods.toml", msg.c_str()) == IDNO) {
                exit(1);
            }
            metadata.name = std::filesystem::path(path).stem().string();
            semver::parse("0.0.0", metadata.version);
            SPDLOG_WARN("The mod at path {} is missing a mods.toml file. Using default metadata:\n{}", path, metadata.ToString());
        }

        AddModMetadata(metadata, archive);
    }
}

void DetectCyclicDependencies() {
    if (auto cyclicDeps = CheckCyclicDependencies(); cyclicDeps.has_value()) {
        std::string msg = "Cyclic dependencies detected between the following mods:\n";
        for (const auto& cycle : cyclicDeps.value()) {
            msg += " - " + cycle + "\n";
        }
        msg += "\nPlease resolve these cyclic dependencies before continuing.\n";
        GameEngine::ShowMessage("Cyclic Dependency Issues", msg.c_str());
        exit(1);
    }
}

void DetectOutdatedDependencies() {
    bool exitDueToErrors = false;
    std::string allDepIssues;

    for (const auto& [meta, _] : Mods) {
        if (auto toUpdate = CheckOutdatedDependencies(meta); toUpdate.has_value()) {
            exitDueToErrors = true;
            allDepIssues += "The mod \"" + meta.name + "\" has the following missing or outdated dependencies:\n";
            for (const auto& dep : toUpdate.value()) {
                allDepIssues += " - " + dep + "\n";
            }
        }
    }

    if (exitDueToErrors) {
        allDepIssues += "\nPlease resolve these dependency issues before continuing.\n";
        GameEngine::ShowMessage("Dependency Issues", allDepIssues.c_str());
        exit(1);
    }
}

void PrintModInfo() {
    SPDLOG_INFO("[ModManager] Detected Mods:");
    for (const auto& [meta, _] : Mods) {
        SPDLOG_INFO(" - {} v{}", meta.name, meta.version.to_string());
    }
}
