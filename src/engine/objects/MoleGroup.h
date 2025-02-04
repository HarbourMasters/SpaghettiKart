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

    explicit OMoleGroup(std::vector<FVector> moles);

    virtual void Tick() override;

    void func_80081FF4(s32 objectIndex);


    std::vector<MoleEntry> _moles;
};
