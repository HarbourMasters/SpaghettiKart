#ifndef VEHICLE_H
#define VEHICLE_H

#include <libultraship.h>

extern "C" {
#include "common_structs.h"
}

// Base class
class AVehicle {
  public:
    virtual ~AVehicle() = default; // Virtual destructor for proper cleanup in derived classes

    explicit AVehicle();

    const char* Type;
    uint8_t uuid[16];

    virtual void Spawn();
    virtual void BeginPlay();
    virtual void Tick();
    virtual void Draw(s32 playerId);
    virtual void Collision(s32 playerId, Player* player);
};

#endif // VEHICLE_H
