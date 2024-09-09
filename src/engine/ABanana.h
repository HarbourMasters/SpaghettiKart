#ifndef ABANANA_H
#define ABANANA_H

#include <libultraship.h>
#include "structs.h"
#include "GameActor.h"

class ABanana : public GameActor {
public:
    uint8_t uuid[16];
    struct BananaActor a;

    // Constructor
    ABanana(uint16_t playerId, const float pos[3], const s16 rot[3], const float velocity[3]);

    virtual ~ABanana() override = default;

    // Virtual functions to be overridden by derived classes
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(Camera*) override;
    virtual void Collision() override;
    virtual void Expire() override;
    virtual void Destroy() override;
};

#endif // ABANANA_H
