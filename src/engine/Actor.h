#pragma once

#include <libultraship.h>

extern "C" {
#include "macros.h"
#include "main.h"
#include "camera.h"
#include "common_structs.h"

class AActor {
  public:
    /* 0x00 */ s16 Type = 0;
    /* 0x02 */ s16 Flags;
    /* 0x04 */ s16 Unk_04;
    /* 0x06 */ s16 State;
    /* 0x08 */ f32 Unk_08;
    /* 0x0C */ f32 BoundingBoxSize;
    /* 0x10 */ Vec3s Rot = { 0, 0, 0 };
    /* 0x16 */ s16 Unk_16;
    /* 0x18 */ Vec3f Pos;
    /* 0x24 */ Vec3f Velocity = { 0, 0, 0 };
    /* 0x30 */ Collision Unk30;
    uint8_t uuid[16];

    virtual ~AActor() = default; // Virtual destructor for proper cleanup in derived classes

    explicit AActor();

    virtual void Tick();
    virtual void Draw(Camera*);
    virtual void Collision(Player* player, AActor* actor);
    virtual void Destroy();
    virtual bool IsMod();
};
}
