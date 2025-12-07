#ifdef _WIN32
#include <Windows.h>
#include <winuser.h>
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#endif
#include "GameExtractor.h"
#include <cstdio>
#include <unordered_map>
#include <Companion.h>

#include <fstream>

#include "Companion.h"
#include "Context.h"
#include "spdlog/spdlog.h"
#include <port/Engine.h>

#ifdef unix
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#if !defined(__IOS__) && !defined(__ANDROID__) && !defined(__SWITCH__)
#include "portable-file-dialogs.h"
#endif

std::unordered_map<std::string, std::string> mGameList = {
    { "579c48e211ae952530ffc8738709f078d5dd215e", "Mario Kart 64 (US)" },
};

bool GameExtractor::SelectGameFromUI() {
    std::vector<std::string> roms;
    GetRoms(roms);

    bool foundGame = false;

    // Store both path and already-read data
    std::string romPath;
    std::vector<uint8_t> romData;

    // Auto detect first baserom with valid hash
    for (const auto& rom : roms) {
        if (!std::filesystem::exists(rom)) continue;

        std::ifstream inFile(rom, std::ios::binary);
        if (!inFile.is_open()) {
            SPDLOG_INFO("Failed to open ROM at path: {}, continuing", rom);
            continue;
        }

        inFile.seekg(0, std::ios::end);
        size_t fileSize = inFile.tellg();
        inFile.seekg(0, std::ios::beg);

        std::vector<uint8_t> data(fileSize);
        if (!inFile.read(reinterpret_cast<char*>(data.data()), fileSize)) {
            SPDLOG_INFO("Failed to read ROM at path: {}, continuing", rom);
            continue;
        }

        inFile.close();
        std::string hash = Companion::CalculateHash(data);

        if (mGameList.find(hash) != mGameList.end()) {
            romPath = rom;
            romData = std::move(data);
            foundGame = true;
            break;
        }
    }

#if !defined(__IOS__) && !defined(__ANDROID__) && !defined(__SWITCH__)
    // Desktop: fallback to file dialogue if no baserom found
    if (!foundGame) {
        if (!pfd::settings::available()) {
            SPDLOG_ERROR(
                "portable-file-dialogs is not available on this system."
            );
            return false;
        }

        auto selection = pfd::open_file("Select a file", ".", { "N64 Roms", "*.z64" }).result();
        if (selection.empty()) return false;

        romPath = selection[0];
    }
#else
    // Mobile: fallback to baserom.us.z64
    if (!foundGame && !std::filesystem::exists(Ship::Context::GetPathRelativeToAppDirectory("baserom.us.z64"))) {
        SPDLOG_ERROR("baserom not found");
        return false;
    }

    if (!foundGame) {
        romPath = Ship::Context::GetPathRelativeToAppDirectory("baserom.us.z64");
    }
#endif

    // Load file if it is not already open
    if (romData.empty()) {
        if (!std::filesystem::exists(romPath)) {
            SPDLOG_ERROR("Failed to find ROM at path: {}", romPath);
            return false;
        }

        std::ifstream inFile(romPath, std::ios::binary);
        if (!inFile.is_open()) return false;

        romData = std::vector<uint8_t>(std::istreambuf_iterator<char>(inFile), {});
        inFile.close();
    }

    this->mGamePath = romPath;
    this->mGameData = std::move(romData);

    return true;
}

void GameExtractor::GetRoms(std::vector<std::string>& roms) {
#ifdef _WIN32
    WIN32_FIND_DATAA ffd;
    HANDLE h = FindFirstFileA(".\\*", &ffd);

    do {
        if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char* ext = PathFindExtensionA(ffd.cFileName);

            // Check for any standard N64 rom file extensions.
            if ((strcmp(ext, ".z64") == 0))
                roms.push_back(ffd.cFileName);
        }
    } while (FindNextFileA(h, &ffd) != 0);
    // if (h != nullptr) {
    //    CloseHandle(h);
    //}
#elif unix
    // Open the directory of the app.
    DIR* d = opendir(".");
    struct dirent* dir;

    if (d != NULL) {
        // Go through each file in the directory
        while ((dir = readdir(d)) != NULL) {
            struct stat path;

            auto fullPath = std::filesystem::path(".") / dir->d_name;
            auto fullPathString = fullPath.string();
            const char* fullPathCStr = fullPathString.c_str();

            // Check if current entry is not folder
            stat(fullPathCStr, &path);
            if (S_ISREG(path.st_mode)) {

                // Get the position of the extension character.
                char* ext = strrchr(dir->d_name, '.');
                if (ext != NULL && (strcmp(ext, ".z64") == 0)) {
                    roms.push_back(fullPathCStr);
                }
            }
        }
    }
    closedir(d);
#else
    for (const auto& file : std::filesystem::directory_iterator(".")) {
        if (file.is_directory())
            continue;
        if (file.path().extension() == ".z64") {
            roms.push_back((file.path()));
        }
    }
#endif
}

std::optional<std::string> GameExtractor::ValidateChecksum() const {
    const auto rom = new N64::Cartridge(this->mGameData);
    rom->Initialize();
    auto hash = rom->GetHash();
    
    if (mGameList.find(hash) == mGameList.end()) {
        return std::nullopt;
    }

    return mGameList[hash];
}

bool GameExtractor::GenerateOTR() const {
    const std::string assets_path = Ship::Context::GetAppBundlePath();
    const std::string game_path = Ship::Context::GetAppDirectoryPath();

    Companion::Instance = new Companion(this->mGameData, ArchiveType::O2R, false, assets_path, game_path);
    Companion::Instance->SetAdditionalFiles({ "meta/mods.toml" });

    try {
        Companion::Instance->Init(ExportType::Binary);
    } catch (const std::exception& e) {
        return false;
    }

    return true;
}
