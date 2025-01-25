#include "AudioSampleFactory.h"
#include <iostream>
#include "../type/AudioSample.h"
#include "spdlog/spdlog.h"

std::shared_ptr<Ship::IResource> SM64::AudioSampleFactoryV0::ReadResource(std::shared_ptr<Ship::File> file) {
    if (!FileHasValidFormatAndReader(file)) {
        return nullptr;
    }

    std::shared_ptr<AudioSample> bank = std::make_shared<AudioSample>(file->InitData);
    auto reader = std::get<std::shared_ptr<Ship::BinaryReader>>(file->Reader);

    bank->loop.start = reader->ReadUInt32();
    bank->loop.end = reader->ReadUInt32();
    bank->loop.count = reader->ReadInt32();
    bank->loop.pad = reader->ReadInt32();

    uint32_t stateSize = reader->ReadUInt32();
    std::vector<int16_t> state;
    if (stateSize > 0) {
        bank->loop.state = new int16_t[stateSize];
        reader->Read((char*) bank->loop.state, stateSize * sizeof(int16_t));
    } else {
        bank->loop.state = nullptr;
    }

    bank->book.order = reader->ReadInt32();
    bank->book.npredictors = reader->ReadInt32();

    uint32_t tableSize = reader->ReadUInt32();
    bank->book.book = new int16_t[tableSize];
    reader->Read((char*) bank->book.book, tableSize * sizeof(int16_t));

    int32_t sampleSize = reader->ReadInt32();
    char* sampleData = new char[sampleSize];
    reader->Read(sampleData, sampleSize);

    bank->mData.unused = 0;
    bank->mData.loaded = 1;
    bank->mData.loop = &bank->loop;
    bank->mData.book = &bank->book;
    bank->mData.sampleAddr = (uint8_t*) sampleData;
    bank->mData.sampleSize = sampleSize;

    return bank;
}
