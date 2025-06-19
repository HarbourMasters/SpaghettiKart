#include <libultraship.h>

#include "Course.h"
#include "MarioRaceway.h"
#include "ChocoMountain.h"
#include "port/Game.h"
#include "port/resource/type/TrackPathPointData.h"
#include "port/resource/type/TrackSections.h"

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
#include "save.h"
#include "staff_ghosts.h"
#include "code_800029B0.h"
#include "render_courses.h"
#include "collision.h"
#include "actors.h"
#include "math_util.h"
extern StaffGhost* d_mario_raceway_staff_ghost;
}

Course::Course() {
    Props.SetText(Props.Name, "Blank Track", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "blnktrck", sizeof(Props.DebugName));
    Props.SetText(Props.CourseLength, "100m", sizeof(Props.CourseLength));
    // Props.Cup = FLOWER_CUP;
    // Props.CupIndex = 3;
    Id = "";
    Props.Minimap.Texture = gTextureCourseOutlineMarioRaceway;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 257;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 0;
    Props.Minimap.PlayerY = 0;
    Props.Minimap.PlayerScaleFactor = 0.22f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    Props.Minimap.Colour = {255, 255, 255};
    Props.WaterLevel = FLT_MAX;

    Props.LakituTowType = (s32) OLakitu::LakituTowType::NORMAL;
    Props.AIBehaviour = D_0D008F28;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.3f;
    Props.AIDistance = gMarioRacewayAIDistances;
    Props.AISteeringSensitivity = 48;

    Props.NearPersp = 3.0f;
    Props.FarPersp = 6800.0f;

    Props.PathSizes = { 600, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 };

    Props.CurveTargetSpeed[0] = 4.1666665f;
    Props.CurveTargetSpeed[1] = 5.5833334f;
    Props.CurveTargetSpeed[2] = 6.1666665f;
    Props.CurveTargetSpeed[3] = 6.75f;

    Props.NormalTargetSpeed[0] = 3.75f;
    Props.NormalTargetSpeed[1] = 5.1666665f;
    Props.NormalTargetSpeed[2] = 5.75f;
    Props.NormalTargetSpeed[3] = 6.3333334f;

    Props.D_0D0096B8[0] = 3.3333332f;
    Props.D_0D0096B8[1] = 3.9166667f;
    Props.D_0D0096B8[2] = 4.5f;
    Props.D_0D0096B8[3] = 5.0833334f;

    Props.OffTrackTargetSpeed[0] = 3.75f;
    Props.OffTrackTargetSpeed[1] = 5.1666665f;
    Props.OffTrackTargetSpeed[2] = 5.75f;
    Props.OffTrackTargetSpeed[3] = 6.3333334f;

    Props.PathTable[0] = NULL;
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = NULL;
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = NULL;
    Props.CloudList = NULL;
    Props.Sequence = MusicSeq::MUSIC_SEQ_UNKNOWN;
}

// Load custom track from code
void Course::Load(Vtx* vtx, Gfx* gfx) {
    gSegmentTable[4] = reinterpret_cast<uintptr_t>(&vtx[0]);
    gSegmentTable[7] = reinterpret_cast<uintptr_t>(&gfx[0]);

    Course::Init();
}

void Course::LoadO2R(std::string trackPath) {
    if (!trackPath.empty()) {
        TrackSectionsPtr = (trackPath + "/data_track_sections");

        std::string path_file = (trackPath + "/data_paths").c_str();

        auto res = std::dynamic_pointer_cast<MK64::Paths>(ResourceLoad(path_file.c_str()));

        if (res != nullptr) {
            auto& paths = res->PathList;

            size_t i = 0;
            for (auto& path : paths) {
                if (i == 0) {
                    Props.PathTable[0] = (TrackPathPoint*)path.data();
                    Props.PathTable[1] = NULL;
                    Props.PathTable[2] = NULL;
                    Props.PathTable[3] = NULL;
                    Props.PathTable2[0] = (TrackPathPoint*)path.data();
                    Props.PathTable2[1] = NULL;
                    Props.PathTable2[2] = NULL;
                    Props.PathTable2[3] = NULL;
                }

                i += 1;
            }
        }

    } else {
        printf("Course.cpp: LoadO2R: trackPath str is empty\n");
    }
}

// Load stock and o2r tracks
void Course::Load() {

    // Load from O2R
    if (!TrackSectionsPtr.empty()) {
        bIsMod = true;
        //auto res = std::dynamic_pointer_cast<MK64::TrackSectionsO2RClass>(ResourceLoad(TrackSectionsPtr.c_str()));

        TrackSectionsO2R* sections = (TrackSectionsO2R*) LOAD_ASSET_RAW(TrackSectionsPtr.c_str());
        size_t size = ResourceGetSizeByName(TrackSectionsPtr.c_str());

        if (sections != nullptr) {
            Course::Init();
            ParseCourseSections(sections, size);
            func_80295C6C();

            if (Props.WaterLevel == FLT_MAX) {
                Props.WaterLevel = gCourseMinY - 10.0f;
            }
        } else {
            printf("Course.cpp: Custom track sections are invalid\n");
        }
        return;
    }

    // Stock
    size_t vtxSize = (ResourceGetSizeByName(this->vtx) / sizeof(CourseVtx)) * sizeof(Vtx);
    size_t texSegSize;

    // Convert course vtx to vtx
    Vtx* vtx = reinterpret_cast<Vtx*>(allocate_memory(vtxSize));
    gSegmentTable[4] = reinterpret_cast<uintptr_t>(&vtx[0]);
    func_802A86A8(reinterpret_cast<CourseVtx*>(LOAD_ASSET_RAW(this->vtx)), vtx, vtxSize / sizeof(Vtx));

    // Load and allocate memory for course textures
    const course_texture* asset = this->Props.textures;
    u8* freeMemory = NULL;
    u8* texture = NULL;
    size_t size = 0;
    texSegSize = 0;
    while (asset->addr) {
        size = ResourceGetTexSizeByName(asset->addr);
        freeMemory = (u8*) allocate_memory(size);

        texture = (u8*) (asset->addr);
        if (texture) {
            if (asset == &this->Props.textures[0]) {
                gSegmentTable[5] = reinterpret_cast<uintptr_t>(&freeMemory[0]);
            }
            strcpy(reinterpret_cast<char*>(freeMemory), asset->addr);
            // memcpy(freeMemory, texture, size);
            texSegSize += size;
            // printf("Texture Addr: 0x%llX, size 0x%X\n", &freeMemory[0], size);
        }
        asset++;
    }

    // Extract packed DLs
    u8* packed = reinterpret_cast<u8*>(LOAD_ASSET_RAW(this->gfx));
    Gfx* gfx = (Gfx*) allocate_memory(sizeof(Gfx) * this->gfxSize); // Size of unpacked DLs
    if (gfx == NULL) {
        printf("Failed to allocate course displaylist memory\n");
    }

    gSegmentTable[7] = reinterpret_cast<uintptr_t>(&gfx[0]);
    displaylist_unpack(reinterpret_cast<uintptr_t*>(gfx), reinterpret_cast<uintptr_t>(packed), 0);

    Course::Init();
}

// C++ version of parse_course_displaylists()
void Course::ParseCourseSections(TrackSectionsO2R* sections, size_t size) {
    for (size_t i = 0; i < (size / sizeof(TrackSectionsO2R)); i++) {
        if (sections[i].flags & 0x8000) {
            D_8015F59C = 1;
        } else {
            D_8015F59C = 0;
        }
        if (sections[i].flags & 0x2000) {
            D_8015F5A0 = 1;
        } else {
            D_8015F5A0 = 0;
        }
        if (sections[i].flags & 0x4000) {
            D_8015F5A4 = 1;
        } else {
            D_8015F5A4 = 0;
        }
        printf("LOADING DL %s\n",  sections[i].addr.c_str());
        generate_collision_mesh((Gfx*)LOAD_ASSET_RAW(sections[i].addr.c_str()), sections[i].surfaceType, sections[i].sectionId);
    }
}

void Course::TestPath() {
    // DEBUG ONLY TO VISUALIZE PATH
    return;
    s16 x;
    s16 y;
    s16 z;
    Vec3s rot = {0, 0, 0};
    Vec3f vel = {0, 0, 0};

    for (size_t i = 0; i < gPathCountByPathIndex[0]; i++) {
        x = gTrackPaths[0][i].posX;
        y = gTrackPaths[0][i].posY;
        z = gTrackPaths[0][i].posZ;

        if (((x & 0xFFFF) == 0x8000) && ((y & 0xFFFF) == 0x8000) && ((z & 0xFFFF) == 0x8000)) {
            break;
        }

        f32 height = spawn_actor_on_surface(x, 2000.0f, z);
        Vec3f itemPos = {x, height, z};
        add_actor_to_empty_slot(itemPos, rot, vel, ACTOR_ITEM_BOX);
    }
}

void Course::Init() {
    gNumActors = 0;
    gCourseMinX = 0;
    gCourseMinY = 0;
    gCourseMinZ = 0;

    gCourseMaxX = 0;
    gCourseMaxY = 0;
    gCourseMaxZ = 0;

    D_8015F59C = 0;
    D_8015F5A0 = 0;
    func_80295D6C();
    D_8015F58C = 0;
    gCollisionMeshCount = 0;
    gCollisionMesh = (CollisionTriangle*) gNextFreeMemoryAddress;
    D_800DC5BC = 0;
    D_800DC5C8 = 0;
}

void Course::LoadTextures() {
}

void Course::BeginPlay() {
    TestPath();
}

void Course::InitClouds() {
    if (this->Props.Clouds) {
        init_clouds(this->Props.Clouds);
    }
}

void Course::UpdateClouds(s32 arg0, Camera* camera) {
    s32 cloudIndex;
    s32 objectIndex;
    CloudData* cloud;

    if (this->Props.CloudList) {
        for (cloudIndex = 0; cloudIndex < D_8018D1F0; cloudIndex++) {
            cloud = &this->Props.CloudList[cloudIndex];
            objectIndex = D_8018CC80[arg0 + cloudIndex];
            func_800788F8(objectIndex, cloud->rotY, camera);
        }
    }
}

// Adjusts player speed on steep hills
void Course::SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6,
                                f32* arg7) {
    func_8003E048(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

void Course::InitCourseObjects() {
}

void Course::UpdateCourseObjects() {
}

void Course::RenderCourseObjects(s32 cameraId) {
}

// Implemented for the first cup of each course plus Koopa Beach
void Course::SomeSounds() {
}

void Course::CreditsSpawnActors() {
}

void Course::WhatDoesThisDo(Player* player, int8_t playerId) {
}

void Course::WhatDoesThisDoAI(Player* player, int8_t playerId) {
}

void Course::SetStaffGhost() {
    D_80162DD6 = 1;
    D_80162DF4 = 1;
}

void Course::Waypoints(Player* player, int8_t playerId) {
    player->nearestPathPointId = gNearestPathPointByPlayerId[playerId];
    if (player->nearestPathPointId < 0) {
        player->nearestPathPointId = gPathCountByPathIndex[0] + player->nearestPathPointId;
    }
}

void Course::Render(struct UnkStruct_800DC5EC* arg0) {
    if (!TrackSectionsPtr.empty()) {
        gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
        set_track_light_direction(D_800DC610, D_802B87D4, 0, 1);
        gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
        gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);

        if (func_80290C20(arg0->camera) == 1) {
            gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
            // d_course_big_donut_packed_dl_DE8
        }

        TrackSectionsO2R* sections = (TrackSectionsO2R*)LOAD_ASSET_RAW(TrackSectionsPtr.c_str());
        size_t size = ResourceGetSizeByName(TrackSectionsPtr.c_str());
        for (size_t i = 0; i < (size / sizeof(TrackSectionsO2R)); i++) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)LOAD_ASSET_RAW(sections[i].addr.c_str()));
        }
    }
}

void Course::RenderCredits() {
}

f32 Course::GetWaterLevel(FVector pos, Collision* collision) {
    float highestWater = -FLT_MAX;
    bool found = false;

    for (const auto& volume : gWorldInstance.CurrentCourse->WaterVolumes) {
        if (pos.x >= volume.MinX && pos.x <= volume.MaxX &&
            pos.z >= volume.MinZ && pos.z <= volume.MaxZ) {
            // Choose the highest water volume the player is over
            if (!found || volume.Height > highestWater) {
                highestWater = volume.Height;
                found = true;
            }
        }
    }

    // If player is not over-top of a water volume then return the courses default water level
    return found ? highestWater : gWorldInstance.CurrentCourse->Props.WaterLevel;
}

void Course::ScrollingTextures() {
}
void Course::DrawWater(struct UnkStruct_800DC5EC* screen, uint16_t pathCounter, uint16_t cameraRot,
                       uint16_t playerDirection) {
}

void Course::Destroy() {
}

bool Course::IsMod() {
    return bIsMod;
}

Course* currentCourse = nullptr;
