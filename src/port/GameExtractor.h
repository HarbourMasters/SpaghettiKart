#pragma once

#include "Companion.h"
#include <filesystem>
#include <vector>
#include <cstdint>

class GameExtractor {
public:
    static bool GenAssetFile();
    std::optional<std::string> ValidateChecksum() const;
    bool SelectGameFromUI();
    bool GenerateOTR() const;
private:
    fs::path mGamePath;
    std::vector<uint8_t> mGameData;
};
