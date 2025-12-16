#pragma once

#include <libultraship.h>
#include "Track.h"

extern "C" {
#include "assets/models/tracks/luigi_raceway/luigi_raceway_vertices.h"
#include "assets/models/tracks/luigi_raceway/luigi_raceway_displaylists.h"
#include "assets/models/tracks/luigi_raceway/luigi_raceway_data.h"
#include "course_offsets.h"
#include "camera.h"
#include "data/some_data.h"
#include "objects.h"
#include "path_spawn_metadata.h"
extern const course_texture luigi_raceway_textures[];
}

class LuigiRaceway : public Track {
    void CopyJumbotron(s32 ulx, s32 uly, s16 portionToDraw, u16* source);

  public:
    // Constructor
    explicit LuigiRaceway();

    //    virtual void Load(const char* courseVtx,
    //                  course_texture* textures, const char* displaylists, size_t dlSize);
    virtual void Load() override;
    virtual void BeginPlay() override;
    virtual void InitTrackObjects() override;
    virtual void SomeSounds() override;
    virtual void WhatDoesThisDo(Player* player, int8_t playerId) override;
    virtual void WhatDoesThisDoAI(Player* player, int8_t playerId) override;
    virtual void SetStaffGhost() override;
    virtual void Draw(ScreenContext*) override;
    virtual void DrawCredits() override;
    virtual void SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6,
                                    f32* arg7) override;
};
