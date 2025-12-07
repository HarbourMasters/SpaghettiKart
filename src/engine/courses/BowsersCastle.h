#pragma once

#include <libultraship.h>
#include "Course.h"

extern "C" {
    #include "assets/models/tracks/bowsers_castle/bowsers_castle_vertices.h"
    #include "assets/models/tracks/bowsers_castle/bowsers_castle_displaylists.h"
    #include "assets/models/tracks/bowsers_castle/bowsers_castle_data.h"
    #include "course_offsets.h"
    #include "camera.h"
    #include "data/some_data.h"
    #include "objects.h"
    #include "path_spawn_metadata.h"
    extern const course_texture bowsers_castle_textures[];
}

class BowsersCastle : public Course {
public:
    virtual ~BowsersCastle() = default;  // Virtual destructor for proper cleanup in derived classes

    // Constructor
    explicit BowsersCastle();

//    virtual void Load(const char* courseVtx, 
//                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void UnLoad() override;
    void SpawnStockThwomp();
    virtual void BeginPlay() override;
    //virtual void InitClouds() override;
    virtual void InitCourseObjects() override;
    virtual void UpdateCourseObjects() override;
    virtual void RenderCourseObjects(s32 cameraId) override;
    virtual void SomeSounds() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void Render(struct UnkStruct_800DC5EC*) override;
    virtual void RenderCredits() override;
    virtual void SomeCollisionThing(Player *player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) override;
    virtual void Waypoints(Player*, int8_t) override;
    virtual void DrawWater(struct UnkStruct_800DC5EC* screen, uint16_t pathCounter, uint16_t cameraRot, uint16_t playerDirection);
    virtual void CreditsSpawnActors() override;
    virtual void Destroy() override;
};
