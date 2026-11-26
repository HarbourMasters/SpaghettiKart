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
    // --prepare: makes mk64.o2r from the --game ROM (or the copy it kept) with no window or questions, unless the one
    // there is current. Prints "progress <0-1> <text>" lines. Returns the exit code.
    static int Prepare();
    std::optional<std::string> ValidateChecksum() const;
    bool SelectGameFromUI();
    bool SelectGameFromPath(const std::string& path);
    void GetRoms(std::vector<std::string>& roms);
    bool GenerateOTR() const;
private:
    fs::path mGamePath;
    std::vector<uint8_t> mGameData;
};
