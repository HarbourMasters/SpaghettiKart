#pragma once

#include <cstdint>
#include "AudioSample.h"
#include "resource/Resource.h"

struct AudioBankSound {
    AudioBankSample* sample;
    float tuning; // frequency scale factor
}; // size = 0x8

struct AdsrEnvelope {
    int16_t delay;
    int16_t arg;
}; // size = 0x4

struct Instrument {
    /*0x00*/ uint8_t loaded;
    /*0x01*/ uint8_t normalRangeLo;
    /*0x02*/ uint8_t normalRangeHi;
    /*0x03*/ uint8_t releaseRate;
    /*0x04*/ AdsrEnvelope* envelope;
    /*0x08*/ AudioBankSound lowNotesSound;
    /*0x10*/ AudioBankSound normalNotesSound;
    /*0x18*/ AudioBankSound highNotesSound;
}; // size = 0x20

struct Drum {
    uint8_t releaseRate;
    uint8_t pan;
    uint8_t loaded;
    AudioBankSound sound;
    AdsrEnvelope* envelope;
};

struct CtlEntry {
    uint8_t bankId;
    uint8_t numInstruments;
    uint8_t numDrums;
    struct Instrument** instruments;
    struct Drum** drums;
}; // size = 0xC

namespace SM64 {
class AudioBank : public Ship::Resource<CtlEntry> {
  public:
    using Resource::Resource;

    AudioBank() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {
    }

    CtlEntry* GetPointer();
    size_t GetPointerSize();

    CtlEntry mData;

    std::vector<Instrument*> instruments;
    std::vector<Drum*> drums;
};
} // namespace SM64
