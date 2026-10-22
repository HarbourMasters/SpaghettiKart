#pragma once

#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include "GameObject.h"

namespace TrackEditor {
    class Handles : public GameObject {

        Handles();

        void Tick() override;
        void Draw() override;
        void Load() override;
    };
}
