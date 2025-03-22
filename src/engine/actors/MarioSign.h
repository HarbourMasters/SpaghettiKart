#pragma once

#include <libultraship.h>
#include "engine/Actor.h"
#include "CoreMath.h"

extern "C" {
#include "common_structs.h"
}

class AMarioSign : public AActor {
public:

    virtual ~AMarioSign() = default;
    explicit AMarioSign(FVector pos);

    virtual void Tick() override;
    virtual void Draw(Camera*) override;
};
