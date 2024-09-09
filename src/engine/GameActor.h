#ifndef GAMEACTOR_H
#define GAMEACTOR_H

#include <libultraship.h>
#include "structs.h"

class GameActor {
public:
    uint8_t uuid[16];
    int16_t type;
    int16_t flags;
    int16_t unk_04;
    int16_t state;
    float unk_08;
    float boundingBoxSize;
    s16 rot[3];
    int16_t unk_16;
    float pos[3];
    float velocity[3];
    Collision unk30;

    virtual ~GameActor() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
   // explicit GameActor(uint8_t* actorUuid);  // UUID should be passed in constructor

    // Virtual functions to be overridden by derived classes
    virtual void Init();
    virtual void Update();
    virtual void Render();
    virtual void Expire();
    virtual void Destroy();
};

#endif // GAMEACTOR_H
