#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
    #include "assets/models/tracks/choco_mountain/choco_mountain_vertices.h"
    #include "assets/models/tracks/choco_mountain/choco_mountain_displaylists.h"
    #include "assets/models/tracks/choco_mountain/choco_mountain_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    extern const course_texture choco_mountain_textures[];
}

class ChocoMountain : public Course {
public:
    virtual ~ChocoMountain() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit ChocoMountain();

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
    virtual void SomeCollisionThing(Player *player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) override;
    virtual void Destroy() override;
};
