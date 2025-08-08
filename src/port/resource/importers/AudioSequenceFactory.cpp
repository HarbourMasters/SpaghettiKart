#include "AudioSequenceFactory.h"
#include "../type/AudioBank.h"
#include "port/Engine.h"
#include <nlohmann/json.hpp>

std::vector<std::string> extension = {".wav", ".ogg", ".mp3", ".flac", ".WAV", ".OGG", ".MP3", ".FLAC"};

std::shared_ptr<Ship::IResource>
SM64::AudioSequenceFactoryV0::ReadResource(std::shared_ptr<Ship::File> file,
                                           std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    std::shared_ptr<AudioSequence> bank = std::make_shared<AudioSequence>(initData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    uint8_t id = reader->ReadUInt32();
    size_t bankCount = reader->ReadUInt32();
    for(size_t i = 0; i < bankCount; i++){
        std::string bankName = reader->ReadString();
        bank->banks.push_back(GameEngine::GetBankIdByName(bankName));
    }

    size_t sampleSize = reader->ReadUInt32();
    for(size_t i = 0; i < sampleSize; i++){
        bank->sampleData.push_back(reader->ReadUByte());
    }

    bank->mData.bankCount = bankCount;
    bank->mData.banks = bank->banks.data();
    bank->mData.data = bank->sampleData.data();
    bank->mData.id = id;

    for (const auto& ext : extension) {
        auto custom = Ship::Context::GetInstance()->GetResourceManager()->LoadFileProcess(initData->Path + ext);

        if (custom != nullptr) {
            bank->sampleData = std::vector<uint8_t>(custom->Buffer->begin(), custom->Buffer->end());
            uint8_t* data = bank->sampleData.data();
            size_t size = bank->sampleData.size();

            auto metadata = Ship::Context::GetInstance()->GetResourceManager()->LoadFileProcess(initData->Path + ".json");
            if (metadata != nullptr) {
                auto json = nlohmann::json::parse(std::string(metadata->Buffer->begin(), metadata->Buffer->end()));
                HMAS_Info info;

                info.name = json.value("name", "");
                info.author = json.value("author", "");
                info.date = json.value("date", "");
                if (json.contains("loop")) {
                    info.loop.start = json["loop"].value("start", -1);
                    info.loop.end = json["loop"].value("end", -1);
                }

                GameEngine::Instance->gHMAS->RegisterSound(id, data, size, info);
            } else {
                GameEngine::Instance->gHMAS->RegisterSound(id, data, size);
            }
            break;
        }
    }

    return bank;
}
