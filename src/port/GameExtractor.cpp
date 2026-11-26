#ifdef _WIN32
#include <Windows.h>
#include <winuser.h>
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#endif
#include "GameExtractor.h"
#include <cstdio>
#include <unordered_map>
#include <utility>

#include <fstream>

#include "Companion.h"
#include "ship/Context.h"
#include "spdlog/spdlog.h"
#include <port/Engine.h>
#include "engine/mods/ModManager.h"

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

static std::string sGamePath;

void GameExtractor::SetGamePath(const std::string& path) {
    sGamePath = path;
}

bool GameExtractor::HasGamePath() {
    return !sGamePath.empty();
}

// .z64 is the byte order the rest of the code expects. .v64 swaps every pair of bytes and .n64 reverses every
// 4 bytes, so put those back in .z64 order. The first word of a ROM says which one it is.
static void ToZ64ByteOrder(std::vector<uint8_t>& data) {
    if (data.size() < 4) {
        return;
    }
    if (data[0] == 0x37 && data[1] == 0x80) {
        for (size_t i = 0; i + 1 < data.size(); i += 2) {
            std::swap(data[i], data[i + 1]);
        }
    } else if (data[0] == 0x40 && data[1] == 0x12) {
        for (size_t i = 0; i + 3 < data.size(); i += 4) {
            std::swap(data[i], data[i + 3]);
            std::swap(data[i + 1], data[i + 2]);
        }
    }
}

bool GameExtractor::SelectGameFromUI() {
    std::vector<std::string> roms;
    GetRoms(roms);

    // The ROM given with --game is tried first
    if (HasGamePath()) {
        roms.insert(roms.begin(), sGamePath);
    }

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
        ToZ64ByteOrder(data);
        std::string hash = Companion::CalculateHash(data);

        if (mGameList.find(hash) != mGameList.end()) {
            romPath = rom;
            romData = std::move(data);
            foundGame = true;
            break;
        }

        if (HasGamePath() && rom == sGamePath) {
            SPDLOG_ERROR("The ROM given with --game is not a supported Mario Kart 64 (US) ROM: {}", rom);
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

        auto selection = pfd::open_file("Select a file", ".", { "N64 Roms", "*.z64 *.n64 *.v64" }).result();
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
        ToZ64ByteOrder(romData);
    }

    this->mGamePath = romPath;
    this->mGameData = std::move(romData);

    return true;
}

bool GameExtractor::SelectGameFromPath(const std::string& path) {
    std::ifstream inFile(path, std::ios::binary);
    if (!inFile.is_open()) {
        return false;
    }

    std::vector<uint8_t> data = std::vector<uint8_t>(std::istreambuf_iterator<char>(inFile), {});
    inFile.close();
    ToZ64ByteOrder(data);

    this->mGamePath = path;
    this->mGameData = std::move(data);

    return true;
}

// The number is written as text: printf would use the player's decimal comma (setlocale in main)
static void PrintProgress(const char* progress, const char* text) {
    printf("progress %s %s\n", progress, text);
    fflush(stdout);
}

// Writes the ROM to path through a .part file, so a cut-short copy never replaces a good one. On failure nothing is
// left behind, not even an empty folder.
static bool KeepRomCopy(const std::vector<uint8_t>& data, const std::string& dir, const std::string& path) {
    std::error_code ec;
    std::filesystem::create_directories(dir, ec);
    const std::string partPath = path + ".part";
    bool ok = !ec;
    if (ok) {
        std::ofstream out(partPath, std::ios::binary | std::ios::trunc);
        out.write(reinterpret_cast<const char*>(data.data()), data.size());
        out.close();
        ok = out.good();
    }
    if (ok) {
        std::filesystem::rename(partPath, path, ec);
        ok = !ec;
    }
    if (!ok) {
        std::filesystem::remove(partPath, ec);
        if (std::filesystem::is_directory(dir, ec) && std::filesystem::is_empty(dir, ec)) {
            std::filesystem::remove(dir, ec);
        }
    }
    return ok;
}

int GameExtractor::Prepare() {
    const std::string o2rPath = Ship::Context::GetPathRelativeToAppDirectory(game_asset_file);
    // Written before mk64.o2r and removed once it's complete, so a run that was cut short is redone
    const std::string unfinishedPath = o2rPath + ".unfinished";

    // A .z64 copy of the player's ROM, kept so a new release can make mk64.o2r again without asking for it. It has a
    // folder of its own (the first folders.game in recomplauncher.json): the launcher takes a folder that isn't empty
    // as a kept copy.
    const std::string keptDir = Ship::Context::GetPathRelativeToAppDirectory("kept");
    const std::string keptRomPath = keptDir + "/mk64.z64";

    // The same check the game makes at startup, so an mk64.o2r from an older release is made again
    const bool finished = std::filesystem::exists(o2rPath) && !std::filesystem::exists(unfinishedPath);
    if (finished && IsMK64O2RCurrent(o2rPath)) {
        PrintProgress("1", "Already set up");
        return 0;
    }

    std::string romPath = sGamePath;
    if (romPath.empty() && std::filesystem::exists(keptRomPath)) {
        romPath = keptRomPath;
    }
    if (romPath.empty()) {
        printf(finished ? "The game's files are from an older version. Pick your Mario Kart 64 (USA) ROM to make "
                          "them again.\n"
                        : "The game isn't set up yet. Pick your Mario Kart 64 (USA) ROM.\n");
        return 1;
    }

    PrintProgress("0.05", "Reading your ROM");
    GameExtractor extractor;
    if (!extractor.SelectGameFromPath(romPath)) {
        printf("Couldn't open the ROM: %s\n", romPath.c_str());
        return 2;
    }

    if (extractor.mGameData.size() < 0x1000 || !extractor.ValidateChecksum().has_value()) {
        printf("This ROM isn't a supported Mario Kart 64. SpaghettiKart needs the USA version.\n");
        return 3;
    }

    // Kept before anything is changed, so a copy that can't be kept leaves the game as it was
    if (romPath != keptRomPath && !KeepRomCopy(extractor.mGameData, keptDir, keptRomPath)) {
        printf("Couldn't keep a copy of the ROM in %s\n", keptDir.c_str());
        return 5;
    }

    std::ofstream(unfinishedPath).close();
    std::error_code ec;
    std::filesystem::remove(o2rPath, ec);

    PrintProgress("0.1", "Making the game's files from your ROM");
    if (!extractor.GenerateOTR()) {
        std::filesystem::remove(o2rPath, ec);
        printf("Making the game's files from the ROM failed.\n");
        return 4;
    }

    std::filesystem::remove(unfinishedPath, ec);
    PrintProgress("1", "Done");
    return 0;
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
