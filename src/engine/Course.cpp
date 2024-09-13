#include <libultraship.h>

#include "Course.h"
#include "MarioRaceway.h"

extern "C" {
    #include "main.h"
    #include "memory.h"
    #include "common_structs.h"
    #include "course_offsets.h"
    #include "some_data.h"
    #include "code_8006E9C0.h"
    #include "code_8003DC40.h"
    #include "assets/common_data.h"
    #include "render_objects.h"
    extern StaffGhost* d_mario_raceway_staff_ghost;
}

Course::Course() {}

void Course::Load(const char* vtx, const char* gfx) {
    gSegmentTable[4] = reinterpret_cast<uintptr_t>(&vtx[0]);
    gSegmentTable[7] = reinterpret_cast<uintptr_t>(&gfx[0]);
}

void Course::Load(const char* courseVtx, course_texture* textures, const char* displaylists, size_t dlSize) {

    size_t vtxSize = (ResourceGetSizeByName(courseVtx) / sizeof(CourseVtx)) * sizeof(Vtx);
    size_t texSegSize;

    // Convert course vtx to vtx
    Vtx* vtx = reinterpret_cast<Vtx*>(allocate_memory(vtxSize));
    gSegmentTable[4] = reinterpret_cast<uintptr_t>(&vtx[0]);
    printf("\nVtxsize: 0x%X\n\n",vtxSize);
    func_802A86A8(reinterpret_cast<CourseVtx*>(LOAD_ASSET_RAW(courseVtx)), vtx, vtxSize / sizeof(Vtx));

    // Load and allocate memory for course textures
    course_texture* asset = textures;
    u8* freeMemory = NULL;
    u8* texture = NULL;
    size_t size = 0;
    texSegSize = 0;
    while (asset->addr) {
        size = ResourceGetTexSizeByName(asset->addr);
        freeMemory = (u8*) allocate_memory(size);

        texture = reinterpret_cast<u8*>(LOAD_ASSET_RAW(asset->addr));
        if (texture) {
            if (asset == &textures[0]) {
                gSegmentTable[5] = reinterpret_cast<uintptr_t>(&freeMemory[0]);
            }
            memcpy(freeMemory, texture, size);
            texSegSize += size;
            // printf("Texture Addr: 0x%llX, size 0x%X\n", &freeMemory[0], size);
        }
        asset++;
    }

    // Extract packed DLs
    u8* packed = reinterpret_cast<u8*>(LOAD_ASSET_RAW(displaylists));
    Gfx* gfx = (Gfx*) allocate_memory(sizeof(Gfx) * dlSize); // Size of unpacked DLs
    assert(gfx != NULL);
    gSegmentTable[7] = reinterpret_cast<uintptr_t>(&gfx[0]);
    displaylist_unpack(reinterpret_cast<uintptr_t *>(gfx), reinterpret_cast<uintptr_t>(packed), 0);
}

void Course::LoadTextures() { }

void Course::SpawnActors() {

}

void Course::Init() {
}
void Course::InitClouds() {
    if (this->clouds) {
        init_clouds(this->clouds);
    }
}

void Course::UpdateClouds(s32 arg0, Camera* camera) {
    s32 cloudIndex;
    s32 objectIndex;
    CloudData* cloud;

    for (cloudIndex = 0; cloudIndex < D_8018D1F0; cloudIndex++) {
        cloud = &this->cloudList[cloudIndex];
        objectIndex = D_8018CC80[arg0 + cloudIndex];
        func_800788F8(objectIndex, cloud->rotY, camera);
    }
}

void Course::SomeCollisionThing(Player *player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
    func_8003E048(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}


void Course::MinimapSettings() {

}

void Course::InitCourseObjects() {

}

void Course::UpdateCourseObjects() {

}

void Course::RenderCourseObjects() {

}

// Implemented for the first cup of each course plus Koopa Beach
void Course::SomeSounds() {

}

void Course::SetCourseVtxColours() {

}

void Course::WhatDoesThisDo(Player* player, int8_t playerId) {

}

void Course::WhatDoesThisDoAI(Player* player, int8_t playerId) {

}

// Positions the finishline on the minimap
void Course::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->MinimapFinishlineX, this->MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void Course::SetStaffGhost() {

}

void Course::BeginPlay() {  }
void Course::Render(struct UnkStruct_800DC5EC* arg0) { }
void Course::Collision() {}
void Course::Destroy() { }

Course* currentCourse = nullptr;

extern "C" {
    void CourseManager_LoadCourse(const char* courseVtx, course_texture* textures, const char* displaylists, size_t dlSize) {

    delete currentCourse;

    //switch (0) {
      //  case COURSE_MARIO_RACEWAY:
            currentCourse = new CourseMarioRaceway();
      //      break;

        // case COURSE_CHOCO_MOUNTAIN:
        //     currentCourse = new CourseChocoMountain();
        //     break;

        // case COURSE_BOWSER_CASTLE:
        //     currentCourse = new CourseBowsersCastle();
        //     break;

        // case COURSE_BANSHEE_BOARDWALK:
        //     currentCourse = new CourseBansheeBoardwalk();
        //     break;

        // case COURSE_YOSHI_VALLEY:
        //     currentCourse = new CourseYoshiValley();
        //     break;

        // case COURSE_FRAPPE_SNOWLAND:
        //     currentCourse = new CourseFrappeSnowland();
        //     break;

        // case COURSE_KOOPA_BEACH:
        //     currentCourse = new CourseKoopaBeach();
        //     break;

        // case COURSE_ROYAL_RACEWAY:
        //     currentCourse = new CourseRoyalRaceway();
        //     break;

        // case COURSE_LUIGI_RACEWAY:
        //     currentCourse = new CourseLuigiRaceway();
        //     break;

        // case COURSE_MOO_MOO_FARM:
        //     currentCourse = new CourseMooMooFarm();
        //     break;

        // case COURSE_TOADS_TURNPIKE:
        //     currentCourse = new CourseToadsTurnpike();
        //     break;

        // case COURSE_KALAMARI_DESERT:
        //     currentCourse = new CourseKalamariDesert();
        //     break;

        // case COURSE_SHERBET_LAND:
        //     currentCourse = new CourseSherbetLand();
        //     break;

        // case COURSE_RAINBOW_ROAD:
        //     currentCourse = new CourseRainbowRoad();
        //     break;

        // case COURSE_WARIO_STADIUM:
        //     currentCourse = new CourseWarioStadium();
        //     break;

        // case COURSE_BLOCK_FORT:
        //     currentCourse = new CourseBlockFort();
        //     break;

        // case COURSE_SKYSCRAPER:
        //     currentCourse = new CourseSkyscraper();
        //     break;

        // case COURSE_DOUBLE_DECK:
        //     currentCourse = new CourseDoubleDeck();
        //     break;

        // case COURSE_DK_JUNGLE:
        //     currentCourse = new CourseDKJungle();
        //     break;

        // case COURSE_BIG_DONUT:
        //     currentCourse = new CourseBigDonut();
        //     break;

      //  default:
            // Handle invalid course ID
   //         return;
   // }

        currentCourse->Load(courseVtx, textures, displaylists, dlSize);
    }

    void CourseManager_LoadTextures() {
        if (currentCourse) {
            currentCourse->LoadTextures();
        }
    }

    void CourseManager_RenderCourse(struct UnkStruct_800DC5EC* arg0) {
        if (currentCourse) {
            currentCourse->Render(arg0);
        }
    }

    void CourseManager_SpawnActors() {
        if (currentCourse) {
            currentCourse->SpawnActors();
        }
    }

    void CourseManager_InitClouds() {
        if (currentCourse) {
            currentCourse->InitClouds();
        }
    }

    void CourseManager_UpdateClouds(s32 arg0, Camera* camera) {
        if (currentCourse) {
            currentCourse->UpdateClouds(arg0, camera);
        }
    }

    void CourseManager_SomeCollisionThing(Player *player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7) {
        if (currentCourse) {
            currentCourse->SomeCollisionThing(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
    }

    void CourseManager_MinimapSettings() {
        if (currentCourse) {
            currentCourse->MinimapSettings();
        }
    }

    void CourseManager_InitCourseObjects() {
        if (currentCourse) {
            currentCourse->InitCourseObjects();
        }
    }

    void CourseManager_UpdateCourseObjects() {
        if (currentCourse) {
            currentCourse->UpdateCourseObjects();
        }
    }

    void CourseManager_RenderCourseObjects() {
        if (currentCourse) {
            currentCourse->RenderCourseObjects();
        }
    }

    void CourseManager_SomeSounds() {
        if (currentCourse) {
            currentCourse->SomeSounds();
        }
    }

    void CourseManager_SetCourseVtxColours() {
        if (currentCourse) {
            currentCourse->SetCourseVtxColours();
        }
    }

    void CourseManager_WhatDoesThisDo(Player* player, int8_t playerId) {
        if (currentCourse) {
            currentCourse->WhatDoesThisDo(player, playerId);
        }
    }

    void CourseManager_WhatDoesThisDoAI(Player* player, int8_t playerId) {
        if (currentCourse) {
            currentCourse->WhatDoesThisDoAI(player, playerId);
        }
    }

    void CourseManager_MinimapFinishlinePosition() {
        if (currentCourse) {
            currentCourse->MinimapFinishlinePosition();
        }
    }

    void CourseManager_SetStaffGhost() {
        if (currentCourse) {
            currentCourse->SetStaffGhost();
        }
    }
}