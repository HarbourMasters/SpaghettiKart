#pragma once

#include <libultraship.h>
#include <vector>
#include "World.h"
#include "CoreMath.h"
#include "Mole.h"

class OMole;

class OMoleGroup : public OObject {
public:
    struct MoleEntry {
        OMole* Mole;
        FVector Pos;
        bool Active;
    };

    /**
     * TickRate:
     * How many moles can pop out per frame.
     * The mole must also be ready to jump to be considered.
     */
    explicit OMoleGroup(std::vector<FVector>& moles, size_t tickRate);
    ~OMoleGroup() {
        _count--;
    }

    virtual void Tick() override;

    void func_80081FF4(s32 objectIndex);


    std::vector<MoleEntry> _moles;
    size_t _tickRate;
private:
    static size_t _count;
    size_t _idx;
};
