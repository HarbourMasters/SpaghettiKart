#ifndef BOMBKART_H
#define BOMBKART_H

#include <libultraship.h>


#ifdef __cplusplus
#include "GameObject.h"
extern "C" {
#endif

    #include "bomb_kart.h"
    #include "Engine.h"

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class OBombKart : public GameObject {

public:
    virtual ~OBombKart() = default;

    // Constructor
    explicit OBombKart(uint16_t waypoint, uint16_t state, float unk_04, float x, float z, float unk_10, float unk_14);

    BombKart vehicle;
    virtual void Init();
    virtual void Update();
    virtual void Render();
    virtual void Explode();
};

#endif

#endif // BOMBKART_H