#include "AudioSequenceFactory.h"
#include "../type/AudioSequence.h"
#include "../type/AudioBank.h"
#include "spdlog/spdlog.h"
#include "port/Engine.h"

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
    for (size_t i = 0; i < bankCount; i++) {
        std::string bankName = reader->ReadString();
        bank->banks.push_back(GameEngine::GetBankIdByName(bankName));
    }

    size_t sampleSize = reader->ReadUInt32();
    for (size_t i = 0; i < sampleSize; i++) {
        bank->sampleData.push_back(reader->ReadUByte());
    }

    bank->mData.bankCount = bankCount;
    bank->mData.banks = bank->banks.data();
    bank->mData.data = bank->sampleData.data();
    bank->mData.id = id;

    return bank;
}
