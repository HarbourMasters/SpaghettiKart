#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
    #include "assets/models/tracks/royal_raceway/royal_raceway_vertices.h"
    #include "assets/models/tracks/royal_raceway/royal_raceway_displaylists.h"
    #include "assets/models/tracks/royal_raceway/royal_raceway_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    extern const course_texture royal_raceway_textures[];
}

class RoyalRaceway : public Course {
public:
    virtual ~RoyalRaceway() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit RoyalRaceway();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void UnLoad() override;
    virtual void BeginPlay() override;
    virtual void InitCourseObjects() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void SetStaffGhost() override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void RenderCredits() override;    
    virtual void ScrollingTextures() override;
    virtual void Waypoints(Player* player, int8_t playerId) override;
};
