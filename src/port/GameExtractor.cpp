#include "GameExtractor.h"
#include <unordered_map>

#include <fstream>

#include "Context.h"
#include "spdlog/spdlog.h"
#include <port/Engine.h>

#if !defined(__IOS__) && !defined(__ANDROID__) && !defined(__SWITCH__)
#include "portable-file-dialogs.h"
#endif

std::unordered_map<std::string, std::string> mGameList = {
    { "579c48e211ae952530ffc8738709f078d5dd215e", "Mario Kart 64 (US)" },
};

bool GameExtractor::SelectGameFromUI() {
#if !defined(__IOS__) && !defined(__ANDROID__) && !defined(__SWITCH__)
    auto selection = pfd::open_file("Select a file", ".", { "N64 Roms", "*.z64" }).result();

    if (selection.empty()) {
        return false;
    }

    this->mGamePath = selection[0];
#else
    this->mGamePath = Ship::Context::GetPathRelativeToAppDirectory("baserom.us.z64");
#endif

    std::ifstream file(this->mGamePath, std::ios::binary);
    this->mGameData = std::vector<uint8_t>( std::istreambuf_iterator( file ), {} );
    file.close();
    return true;
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
    Companion::Instance = new Companion(this->mGameData, ArchiveType::O2R, false);

    try {
        Companion::Instance->Init(ExportType::Binary);
    } catch (const std::exception& e) {
        return false;
    }

    return true;
}
