#ifndef BOMBKART_H
#define BOMBKART_H

#include <libultraship.h>
#include "GameObject.hpp"


#ifdef __cplusplus
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
    explicit OBombKart();

    BombKart vehicle;
    virtual void Init();
    virtual void Update();
    virtual void Render();
    virtual void Explode();
};

#endif

#endif // BOMBKART_H