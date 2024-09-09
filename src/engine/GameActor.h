#ifndef GAMEACTOR_H
#define GAMEACTOR_H

#include <libultraship.h>
#include "structs.h"

class GameActor {
public:
    uint8_t uuid[16];
    Actor *a;

    virtual ~GameActor() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit GameActor();  // UUID should be passed in constructor

    // Virtual functions to be overridden by derived classes
    virtual void Init();
    virtual void Update();
    virtual void Render(Camera*);
    virtual void Collision();
    virtual void Expire();
    virtual void Destroy();
};

#endif // GAMEACTOR_H
