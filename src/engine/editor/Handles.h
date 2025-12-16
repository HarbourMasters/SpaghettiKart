#pragma once

#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include "GameObject.h"

namespace TrackEditor {
    class Handles : public GameObject {

        Handles();

        virtual void Tick() override;
        virtual void Draw() override;
        virtual void Load() override;

    };
}
