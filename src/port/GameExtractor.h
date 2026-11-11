#pragma once

#include "Companion.h"
#include <filesystem>
#include <vector>
#include <cstdint>

class GameExtractor {
public:
    static bool GenAssetFile();
    // A ROM passed on the command line with --game <path>, used instead of searching for one.
    static void SetGamePath(const std::string& path);
    static bool HasGamePath();
    std::optional<std::string> ValidateChecksum() const;
    bool SelectGameFromUI();
    void GetRoms(std::vector<std::string>& roms);
    bool GenerateOTR() const;
private:
    fs::path mGamePath;
    std::vector<uint8_t> mGameData;
};
