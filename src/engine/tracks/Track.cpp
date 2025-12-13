#include <libultraship.h>
#include <unordered_set>
#include <string>
#include <cstring>

#include "Track.h"
#include "MarioRaceway.h"
#include "ChocoMountain.h"
#include "port/Game.h"
#include "engine/editor/SceneManager.h"
#include "Registry.h"
#include "libultraship/bridge/resourcebridge.h"
#include "align_asset_macro.h"

extern "C" {
#include "main.h"
#include "memory.h"
#include "common_structs.h"
#include "course_offsets.h"
#include "some_data.h"
#include "code_8006E9C0.h"
#include "code_8003DC40.h"
#include "assets/models/common_data.h"
#include "render_objects.h"
#include "save.h"
#include "replays.h"
#include "code_800029B0.h"
#include "render_courses.h"
#include "collision.h"
#include "actors.h"
#include "math_util.h"
#include "code_80005FD0.h"
extern StaffGhost* d_mario_raceway_staff_ghost;
}

void ResizeMinimap(MinimapProps* minimap) {
    if (minimap->Height < minimap->Width) {
        minimap->Width = (minimap->Width * 64) / minimap->Height;
        minimap->Height = 64;
    } else {
        minimap->Height = (minimap->Height * 64) / minimap->Width;
        minimap->Width = 64;
    }
}

static std::unordered_set<std::string> gModifiedGfxSet;

static void SwapTriangleVertices(Gfx* cmd) {
    int8_t opcode = cmd->words.w0 >> 24;
    
    if (opcode == G_TRI1) {
        uint32_t w1 = cmd->words.w1;
        uint8_t v0 = (w1 >> 16) & 0xFF;
        uint8_t v1 = (w1 >> 8) & 0xFF;
        uint8_t v2 = w1 & 0xFF;
        
        cmd->words.w1 = (v0 << 16) | (v2 << 8) | v1;
    }
    else if (opcode == G_TRI1_OTR) {
        uint32_t w0 = cmd->words.w0;
        uint32_t w1 = cmd->words.w1;
        uint16_t v0 = w0 & 0xFFFF;
        uint16_t v1 = (w1 >> 16) & 0xFFFF;
        uint16_t v2 = w1 & 0xFFFF;
        
        cmd->words.w0 = (opcode << 24) | v0;
        cmd->words.w1 = (v2 << 16) | v1;
    }
    else if (opcode == G_TRI2 || opcode == G_QUAD) {
        uint32_t w0 = cmd->words.w0;
        uint8_t v0_a = (w0 >> 16) & 0xFF;
        uint8_t v1_a = (w0 >> 8) & 0xFF;
        uint8_t v2_a = w0 & 0xFF;
        
        uint32_t w1 = cmd->words.w1;
        uint8_t v0_b = (w1 >> 16) & 0xFF;
        uint8_t v1_b = (w1 >> 8) & 0xFF;
        uint8_t v2_b = w1 & 0xFF;
        
        cmd->words.w0 = (opcode << 24) | (v0_a << 16) | (v2_a << 8) | v1_a;
        cmd->words.w1 = (v0_b << 16) | (v2_b << 8) | v1_b;
    }
}

// Remove all occurrences of the marker "__OTR__" from a name string.
static std::string StripOtrPrefix(const char* s) {
    if (s == nullptr) {
        return std::string();
    }

    std::string out(s);
    const std::string marker = "__OTR__";
    size_t pos = 0;
    while ((pos = out.find(marker, pos)) != std::string::npos) {
        out.erase(pos, marker.length());
    }
    return out;
}

static void InvertTriangleWindingInternal(Gfx* gfx, const char* gfxName, bool shouldSwap) {
    if (gfx == nullptr) {
        return;
    }
    
    if (GameEngine_OTRSigCheck((char*)gfx)) {
        gfx = (Gfx*)LOAD_ASSET(gfx);
    }
    
    if (gfxName != nullptr) {
        std::string nameStr = StripOtrPrefix(gfxName);
        if (gModifiedGfxSet.find(nameStr) != gModifiedGfxSet.end()) {
            return;
        }
        gModifiedGfxSet.insert(nameStr);
    }
    
    Gfx* cmd = gfx;
    
    for (int i = 0; i < 0x1FFF; i++) {
        int8_t opcode = cmd->words.w0 >> 24;
        
        switch (opcode) {
            case G_TRI1:
            case G_TRI1_OTR:
            case G_TRI2:
            case G_QUAD:
                if (shouldSwap) {
                    SwapTriangleVertices(cmd);
                }
                break;
            case G_DL: {
                Gfx* subDL = (Gfx*)(uintptr_t)cmd->words.w1;
                InvertTriangleWindingInternal(subDL, nullptr, shouldSwap);
                break;
            }
            case G_DL_OTR_HASH: {
                cmd++;
                i++;
                uint64_t hash = ((uint64_t)cmd->words.w0 << 32) | cmd->words.w1;
                const char* name = ResourceGetNameByCrc(hash);
                if (name != nullptr) {
                    bool subShouldSwap = (strstr(name, "packed") != nullptr);
                    Gfx* subDL = (Gfx*)ResourceGetDataByCrc(hash);
                    InvertTriangleWindingInternal(subDL, name, subShouldSwap);
                }
                break;
            }
            case G_DL_OTR_FILEPATH: {
                const char* name = (const char*)(uintptr_t)cmd->words.w1;
                if (name != nullptr) {
                    bool subShouldSwap = (strstr(name, "packed") != nullptr);
                    Gfx* subDL = (Gfx*)ResourceGetDataByName(name);
                    InvertTriangleWindingInternal(subDL, name, subShouldSwap);
                }
                break;
            }
            case G_VTX_OTR_FILEPATH:
            case G_VTX_OTR_HASH:
                cmd++;
                i++;
                break;
            case G_MARKER: {
                cmd++;
                i++;
                uint64_t hash = ((uint64_t)cmd->words.w0 << 32) | cmd->words.w1;
                const char* name = ResourceGetNameByCrc(hash);
                if (name != nullptr && gfxName == nullptr) {
                    std::string nameStr = StripOtrPrefix(name);
                    if (gModifiedGfxSet.find(nameStr) != gModifiedGfxSet.end()) {
                        return;
                    }
                    gModifiedGfxSet.insert(nameStr);
                    shouldSwap = (strstr(name, "packed") != nullptr);
                }
                break;
            }
            case G_MTX_OTR:
            case G_SETTIMG_OTR_HASH:
                cmd++;
                i++;
                break;
            case G_ENDDL:
                return;
            default:
                break;
        }
        
        cmd++;
    }
}

static void InvertTriangleWindingModdedInternal(Gfx* gfx, const char* gfxName) {
    if (gfx == nullptr) {
        return;
    }
    
    if (GameEngine_OTRSigCheck((char*)gfx)) {
        gfx = (Gfx*)LOAD_ASSET(gfx);
    }
    
    if (gfxName != nullptr) {
        std::string nameStr = StripOtrPrefix(gfxName);
        if (gModifiedGfxSet.find(nameStr) != gModifiedGfxSet.end()) {
            return;
        }
        gModifiedGfxSet.insert(nameStr);
    }
    
    Gfx* cmd = gfx;
    
    for (int i = 0; i < 0x1FFF; i++) {
        int8_t opcode = cmd->words.w0 >> 24;
        
        switch (opcode) {
            case G_TRI1:
            case G_TRI1_OTR:
            case G_TRI2:
            case G_QUAD:
                SwapTriangleVertices(cmd);
                break;
            case G_DL: {
                Gfx* subDL = (Gfx*)(uintptr_t)cmd->words.w1;
                InvertTriangleWindingModdedInternal(subDL, nullptr);
                break;
            }
            case G_DL_OTR_HASH: {
                cmd++;
                i++;
                uint64_t hash = ((uint64_t)cmd->words.w0 << 32) | cmd->words.w1;
                const char* name = ResourceGetNameByCrc(hash);
                if (name != nullptr) {
                    Gfx* subDL = (Gfx*)ResourceGetDataByCrc(hash);
                    InvertTriangleWindingModdedInternal(subDL, name);
                }
                break;
            }
            case G_DL_OTR_FILEPATH: {
                const char* name = (const char*)(uintptr_t)cmd->words.w1;
                if (name != nullptr) {
                    Gfx* subDL = (Gfx*)ResourceGetDataByName(name);
                    InvertTriangleWindingModdedInternal(subDL, name);
                }
                break;
            }
            case G_VTX_OTR_FILEPATH:
            case G_VTX_OTR_HASH:
                cmd++;
                i++;
                break;
            case G_MARKER: {
                cmd++;
                i++;
                uint64_t hash = ((uint64_t)cmd->words.w0 << 32) | cmd->words.w1;
                const char* name = ResourceGetNameByCrc(hash);
                if (name != nullptr) {
                    std::string nameStr = StripOtrPrefix(name);
                    if (gModifiedGfxSet.find(nameStr) != gModifiedGfxSet.end()) {
                        return;
                    }
                    gModifiedGfxSet.insert(nameStr);
                }
                break;
            }
            case G_MTX_OTR:
            case G_SETTIMG_OTR_HASH:
                cmd++;
                i++;
                break;
            case G_ENDDL:
                return;
            default:
                break;
        }
        
        cmd++;
    }
}

void InvertTriangleWinding(Gfx* gfx) {
    InvertTriangleWindingInternal(gfx, nullptr, false);
}

void InvertTriangleWindingByName(const char* name) {
    if (name == nullptr) {
        return;
    }
    Gfx* gfx = (Gfx*)ResourceGetDataByName(name);
    bool shouldSwap = (strstr(name, "packed") != nullptr);
    InvertTriangleWindingInternal(gfx, name, shouldSwap);
}

void InvertTriangleWindingModdedByName(const char* name) {
    if (name == nullptr) {
        return;
    }
    Gfx* gfx = (Gfx*)ResourceGetDataByName(name);
    InvertTriangleWindingModdedInternal(gfx, name);
}

void RestoreTriangleWinding() {
    for (const std::string& name : gModifiedGfxSet) {
        if (strstr(name.c_str(), "packed") == nullptr) {
            continue;
        }
        
        Gfx* gfx = (Gfx*)ResourceGetDataByName(name.c_str());
        if (gfx == nullptr) {
            continue;
        }
        
        Gfx* cmd = gfx;
        
        for (int i = 0; i < 0x1FFF; i++) {
            int8_t opcode = cmd->words.w0 >> 24;
            
            switch (opcode) {
                case G_TRI1:
                case G_TRI1_OTR:
                case G_TRI2:
                case G_QUAD:
                    SwapTriangleVertices(cmd);
                    break;
                    
                case G_DL_OTR_HASH:
                case G_VTX_OTR_FILEPATH:
                case G_VTX_OTR_HASH:
                case G_MARKER:
                case G_MTX_OTR:
                case G_SETTIMG_OTR_HASH:
                    cmd++;
                    i++;
                    break;
                
                case G_ENDDL:
                    goto next_gfx;
                    
                default:
                    break;
            }
            
            cmd++;
        }
        next_gfx:;
    }
    
    gModifiedGfxSet.clear();
}

bool IsTriangleWindingInverted() {
    return !gModifiedGfxSet.empty();
}


Track::Track() {
    Props.SetText(Props.Name, "Blank Track", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "blnktrck", sizeof(Props.DebugName));
    Props.SetText(Props.TrackLength, "100m", sizeof(Props.TrackLength));
    // Props.Cup = FLOWER_CUP;
    // Props.CupIndex = 3;
    ResourceName = "";
    Props.Minimap.Texture = minimap_mario_raceway;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 257;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 0;
    Props.Minimap.PlayerY = 0;
    Props.Minimap.PlayerScaleFactor = 0.22f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;
    Props.Minimap.Colour = { 255, 255, 255 };

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
    Props.PathTable2[4] = NULL;

    Props.Clouds = NULL;
    Props.CloudList = NULL;
    Props.Sequence = MusicSeq::MUSIC_SEQ_UNKNOWN;

    bFog = false;
    gFogColour.r = 0;
    gFogColour.g = 0;
    gFogColour.b = 0;
    gFogColour.a = 255;
    gFogMin = 995;
    gFogMax = 1000;
}

// Load custom track from code
void Track::Load(Vtx* vtx, Gfx* gfx) {
    Track::Init();
}

// Load stock and o2r tracks
void Track::Load() {
    printf("[Track] Loading... %s\n", ResourceName.c_str());
    const TrackInfo* info = gTrackRegistry.GetInfo(ResourceName);
    if (nullptr == info) {
        printf("Could not find TrackInfo for %s\n", ResourceName.c_str());
        return;
    }
    const std::string& trackPath = info->Path;

    if (trackPath.empty()) {
        // Load stock track
        Track::Init();
    } else { // Load custom track
        bIsMod = true;

        Editor::LoadTrackDataFromJson(this, trackPath);
        
        const std::string trackSectionPath = (trackPath + "/data_track_sections");
        TrackSections* sections = (TrackSections*) LOAD_ASSET_RAW(trackSectionPath.c_str());
        size_t size = ResourceGetSizeByName(trackSectionPath.c_str());

        size_t totalSections = size / sizeof(TrackSections);

        if (sections != nullptr) {
            Track::Init();
            if (gIsMirrorMode != 0) {
                for (size_t i = 0; i < totalSections; i++) {
                    auto name = ResourceGetNameByCrc(sections[i].crc);
                    InvertTriangleWindingModdedByName(name);
                }
            }
            ParseTrackSections(sections, size);
            func_80295C6C();

            if (Props.WaterLevel == FLT_MAX) {
                Props.WaterLevel = gTrackMinY - 10.0f;
            }
        } else {
            printf("Track.cpp: Custom track sections are invalid\n");
        }
    }
}

// C++ version of parse_track_displaylists()
void Track::ParseTrackSections(TrackSections* sections, size_t size) {
    printf("\n[Track] Generating Collision Meshes...\n");
    for (size_t i = 0; i < (size / sizeof(TrackSections)); i++) {
        if (sections[i].flags & 0x8000) {
            D_8015F59C = 1; // single-sided wall
        } else {
            D_8015F59C = 0;
        }
        if (sections[i].flags & 0x2000) {
            D_8015F5A0 = 1; // surface
        } else {
            D_8015F5A0 = 0;
        }
        if (sections[i].flags & 0x4000) {
            D_8015F5A4 = 1; // double-sided wall
        } else {
            D_8015F5A4 = 0;
        }
        const char* name = ResourceGetNameByCrc(sections[i].crc);
        printf("  %s\n", name);
        generate_collision_mesh((Gfx*) ResourceGetDataByCrc(sections[i].crc), sections[i].surfaceType,
                                sections[i].sectionId);
    }
    printf("[Track] Collision Mesh Generation Complete!\n\n");
}

void Track::TestPath() {
    // DEBUG ONLY TO VISUALIZE PATH
    return;
    s16 x;
    s16 y;
    s16 z;
    Vec3s rot = { 0, 0, 0 };
    Vec3f vel = { 0, 0, 0 };

    for (size_t i = 0; i < gPathCountByPathIndex[0]; i++) {
        x = gTrackPaths[0][i].x;
        y = gTrackPaths[0][i].y;
        z = gTrackPaths[0][i].z;

        if (((x & 0xFFFF) == 0x8000) && ((y & 0xFFFF) == 0x8000) && ((z & 0xFFFF) == 0x8000)) {
            break;
        }

        f32 height = spawn_actor_on_surface(x, 2000.0f, z);
        Vec3f itemPos = { (f32) x, height, (f32) z };
        add_actor_to_empty_slot(itemPos, rot, vel, ACTOR_ITEM_BOX);
    }
}

void Track::Init() {
    gNumActors = 0;
    gTrackMinX = 0;
    gTrackMinY = 0;
    gTrackMinZ = 0;

    gTrackMaxX = 0;
    gTrackMaxY = 0;
    gTrackMaxZ = 0;

    D_8015F59C = 0;
    D_8015F5A0 = 0;
    func_80295D6C();
    D_8015F58C = 0;
    gCollisionMeshCount = 0;
    gCollisionMesh = (CollisionTriangle*) gNextFreeMemoryAddress;
    D_800DC5C8 = 0;
}

void Track::BeginPlay() {
    printf("[Track] BeginPlay\n");
    TestPath();
    this->SpawnActors();
}

// Spawns actors from SpawnParams set by the scene file in SceneManager.cpp
void Track::SpawnActors() {
    for (const auto& params : SpawnList) {
        gActorRegistry.Invoke(params.Name, params);
    }
}

void Track::InitClouds() {
    if (this->Props.Clouds) {
        init_clouds(this->Props.Clouds);
    }
}

void Track::TickClouds(s32 arg0, Camera* camera) {
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
void Track::SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6,
                                f32* arg7) {
    func_8003E048(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

void Track::InitTrackObjects() {
}

void Track::TickTrackObjects() {
}

void Track::DrawTrackObjects(s32 cameraId) {
}

// Implemented for the first cup of each track plus Koopa Beach
void Track::SomeSounds() {
}

void Track::CreditsSpawnActors() {
}

void Track::WhatDoesThisDo(Player* player, int8_t playerId) {
}

void Track::WhatDoesThisDoAI(Player* player, int8_t playerId) {
}

void Track::SetStaffGhost() {
    bCourseGhostDisabled = 1;
    D_80162DF4 = 1;
}

void Track::Waypoints(Player* player, int8_t playerId) {
    player->nearestPathPointId = gNearestPathPointByPlayerId[playerId];
    if (player->nearestPathPointId < 0) {
        player->nearestPathPointId = gPathCountByPathIndex[0] + player->nearestPathPointId;
    }
}

void Track::Draw(ScreenContext* arg0) {
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

    const TrackInfo* info = gTrackRegistry.GetInfo(ResourceName);
    std::string res = info->Path + "/data_track_sections";

    TrackSections* sections = (TrackSections*) LOAD_ASSET_RAW(res.c_str());
    size_t size = ResourceGetSizeByName(res.c_str());
    size_t totalSections = size / sizeof(TrackSections);
    for (size_t i = 0; i < totalSections; i++) {
        gSPDisplayList(gDisplayListHead++, (Gfx*) ResourceGetDataByCrc(sections[i].crc));
    }
}

void Track::DrawCredits() {
}

f32 Track::GetWaterLevel(FVector pos, Collision* collision) {
    float highestWater = -FLT_MAX;
    bool found = false;

    for (const auto& volume : GetWorld()->GetTrack()->WaterVolumes) {
        if (pos.x >= volume.MinX && pos.x <= volume.MaxX && pos.z >= volume.MinZ && pos.z <= volume.MaxZ) {
            // Choose the highest water volume the player is over
            if (!found || volume.Height > highestWater) {
                highestWater = volume.Height;
                found = true;
            }
        }
    }

    // If player is not over-top of a water volume then return the tracks default water level
    return found ? highestWater : GetWorld()->GetTrack()->Props.WaterLevel;
}

void Track::ScrollingTextures() {
}
void Track::DrawWater(ScreenContext* screen, uint16_t pathCounter, uint16_t cameraRot, uint16_t playerDirection) {
}

void Track::Destroy() {
}

bool Track::IsMod() {
    return bIsMod;
}
