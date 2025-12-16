#pragma once

#include <libultraship.h>
#include "CoreMath.h"
#include "Track.h"
#include "engine/vehicles/Train.h"

extern "C" {
    #include "assets/models/tracks/kalimari_desert/kalimari_desert_vertices.h"
    #include "assets/models/tracks/kalimari_desert/kalimari_desert_displaylists.h"
    #include "assets/models/tracks/kalimari_desert/kalimari_desert_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    #include "code_800029B0.h"
    extern const course_texture kalimari_desert_textures[];
}

class KalimariDesert : public Track {
public:
    // Constructor
    explicit KalimariDesert();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void BeginPlay() override;
    virtual void InitTrackObjects() override;
    virtual void SomeSounds() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void Draw(ScreenContext*) override;
    virtual void DrawCredits() override;
    virtual void Destroy() override;

private:
    size_t _numTrains = 2;
    size_t _numCarriages = 5;
    ATrain::TenderStatus _tender = ATrain::TenderStatus::HAS_TENDER;
};
