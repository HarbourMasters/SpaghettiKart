#pragma once

#include <libultraship.h>
#include "engine/Actor.h"

extern "C" {
#include "common_structs.h"
}

class AWarioSign : public AActor {
public:


    virtual ~AWarioSign() = default;
    explicit AWarioSign(Vec3f pos);

    virtual void Tick() override;
    virtual void Draw(Camera*) override;
};
