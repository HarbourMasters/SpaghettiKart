#pragma once

#include <libultraship.h>
#include "engine/Actor.h"
#include "CoreMath.h"

extern "C" {
#include "common_structs.h"
}

class AWarioSign : public AActor {
public:

    virtual ~AWarioSign() = default;
    explicit AWarioSign(FVector pos);

    virtual void Tick() override;
    virtual void Draw(Camera*) override;
};
