#pragma once

#include <libultraship.h>
#include "CoreMath.h"
#include "Course.h"
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
    extern const course_texture kalimari_desert_textures[];
}

class KalimariDesert : public Course {
public:
    virtual ~KalimariDesert() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit KalimariDesert();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void UnLoad() override;
    virtual void BeginPlay() override;
    virtual void InitCourseObjects() override;
    virtual void SomeSounds() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void RenderCredits() override;
    virtual void Destroy() override;

private:
    size_t _numTrains = 2;
    size_t _numCarriages = 5;
    ATrain::TenderStatus _tender = ATrain::TenderStatus::HAS_TENDER;
};
