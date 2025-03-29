#include "AudioBankFactory.h"
#include "../type/AudioBank.h"
#include "spdlog/spdlog.h"
#include "resourcebridge.h"
#include "ResourceUtil.h"

std::shared_ptr<Ship::IResource>
SM64::AudioBankFactoryV0::ReadResource(std::shared_ptr<Ship::File> file,
                                       std::shared_ptr<Ship::ResourceInitData> initData) {
    if (!FileHasValidFormatAndReader(file, initData)) {
        return nullptr;
    }

    std::shared_ptr<AudioBank> bank = std::make_shared<AudioBank>(initData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    uint8_t bankId = reader->ReadUInt32();
    uint32_t instrumentCount = reader->ReadUInt32();

    for(size_t i = 0; i < instrumentCount; i++){
        auto* instrument = new Instrument();
        bool valid = reader->ReadUByte();
        if(!valid){
            bank->instruments.push_back(nullptr);
            continue;
        }
        instrument->loaded = 1;
        instrument->releaseRate = reader->ReadUByte();
        instrument->normalRangeLo = reader->ReadUByte();
        instrument->normalRangeHi = reader->ReadUByte();

        uint32_t envelopeSize = reader->ReadUInt32();
        if(envelopeSize != 0){
            instrument->envelope = new AdsrEnvelope[envelopeSize];
            for(size_t j = 0; j < envelopeSize; j++){
                instrument->envelope[j].delay = BSWAP16(reader->ReadInt16());
                instrument->envelope[j].arg = BSWAP16(reader->ReadInt16());
            }
        }

        uint32_t soundFlags = reader->ReadUInt32();
        bool hasLo = soundFlags & (1 << 0);
        bool hasMed = soundFlags & (1 << 1);
        bool hasHi = soundFlags & (1 << 2);

        if(hasLo){
            std::string lowSampleName = reader->ReadString();
            instrument->lowNotesSound.sample = LoadChild<AudioBankSample*>(lowSampleName.c_str());
            instrument->lowNotesSound.tuning = reader->ReadFloat();
        }

        if(hasMed){
            std::string normalSampleName = reader->ReadString();
            instrument->normalNotesSound.sample = LoadChild<AudioBankSample*>(normalSampleName.c_str());
            instrument->normalNotesSound.tuning = reader->ReadFloat();
        }

        if(hasHi){
            std::string highSampleName = reader->ReadString();
            instrument->highNotesSound.sample = LoadChild<AudioBankSample*>(highSampleName.c_str());
            instrument->highNotesSound.tuning = reader->ReadFloat();
        }

        bank->instruments.push_back(instrument);
    }

    uint32_t drumCount = reader->ReadUInt32();

    for(size_t i = 0; i < drumCount; i++){
        auto* drum = new Drum();
        drum->releaseRate = reader->ReadUByte();
        drum->pan = reader->ReadUByte();
        drum->loaded = 1;

        uint32_t envelopeSize = reader->ReadUInt32();
        if(envelopeSize != 0){
            drum->envelope = new AdsrEnvelope[envelopeSize];
            for(size_t j = 0; j < envelopeSize; j++){
                drum->envelope[j].delay = BSWAP16(reader->ReadInt16());
                drum->envelope[j].arg = BSWAP16(reader->ReadInt16());
            }
        }

        std::string sampleName = reader->ReadString();
        drum->sound.sample = LoadChild<AudioBankSample*>(sampleName.c_str());
        drum->sound.tuning = reader->ReadFloat();

        bank->drums.push_back(drum);
    }

    bank->mData.bankId = bankId;
    bank->mData.numInstruments = instrumentCount;
    bank->mData.numDrums = drumCount;
    bank->mData.instruments = bank->instruments.data();
    bank->mData.drums = bank->drums.data();

    return bank;
}