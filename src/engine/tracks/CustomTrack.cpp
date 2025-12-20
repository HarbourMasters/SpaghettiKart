#include "CustomTrack.h"
#include "Track.h"

#include <libultraship/libultraship.h>
#include "libultraship/bridge/resourcebridge.h"
#include "align_asset_macro.h"
#include <unordered_set>
#include <string>
#include <cstring>
#include <algorithm>

#include "engine/editor/SceneManager.h"

extern "C" {
#include "collision.h"
#include "render_courses.h"
#include "actors.h"
#include "math_util.h"
}

CustomTrack::CustomTrack() {
    Props.SetText(Props.Name, "Blank Track", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "blnktrck", sizeof(Props.DebugName));
    Props.SetText(Props.TrackLength, "100m", sizeof(Props.TrackLength));
    ResourceName = "mod:blank_track";
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

void CustomTrack::Load() {
    /** Get TrackInfo **/
    printf("[CustomTrack] Loading... %s\n", ResourceName.c_str());
    const TrackInfo* info = gTrackRegistry.GetInfo(ResourceName);
    if (nullptr == info) {
        printf("[CustomTrack] Could not find TrackInfo for %s\n", ResourceName.c_str());
        return;
    }

    /** Get Directory to Track files **/
    const std::string& path = info->Path;
    if (path.empty()) {
        printf("[CustomTrack] No track path\n");
        return;
    }

    /** Get Props, Minimap, Actors, Environment settings from scene.json **/
    TrackEditor::LoadTrackDataFromJson(this, path);

    /** Get Track Mesh **/
    const std::string meshPath = (path + "/data_track_sections");

    TrackSections* meshData = (TrackSections*) LOAD_ASSET_RAW(meshPath.c_str());
    if (!meshData) {
        printf("[CustomTrack] No valid mesh found!\n");
        return;
    }

    size_t numMesh = ( ResourceGetSizeByName(meshPath.c_str()) / sizeof(TrackSections) );

    /** Iterate over meshes **/
    for (size_t i = 0; i < numMesh; i++) {
        if (gIsMirrorMode != 0) {
            CustomTrack::InvertTriangleWindingModdedByName(ResourceGetNameByCrc(meshData[i].crc));
        }

        /** Sort meshes into DrawLayer queues **/
        switch (static_cast<DrawLayer>(meshData[i].layer)) {
            case DrawLayer::DRAW_INVISIBLE:
                break;
            default:
            case DrawLayer::DRAW_OPAQUE:
                mOpaqueItems.push_back(meshData[i].crc);
                break;
            case DrawLayer::DRAW_TRANSLUCENT:
                mTranslucentItems.push_back(meshData[i].crc);
                break;
            case DrawLayer::DRAW_TRANSLUCENT_NO_ZBUFFER:
                mTranslucentNoZBufferItems.push_back(meshData[i].crc);
                break;
        }
    }

    Track::Init(); // Base class

    /** Generate Collision Mesh **/
    CustomTrack::ParseMeshForCollision(meshData, numMesh);
    func_80295C6C(); // generate collision grid

    /** Dumb hack that needs to be removed **/
    if (Props.WaterLevel == FLT_MAX) {
        Props.WaterLevel = gTrackMinY - 10.0f;
    }
}

void CustomTrack::InvertTriangleWindingModdedByName(const char* name) {
    if (name == nullptr) {
        return;
    }
    Gfx* gfx = (Gfx*)ResourceGetDataByName(name);
    InvertTriangleWindingModdedInternal(gfx, name);
}

// C++ version of parse_track_displaylists()
void CustomTrack::ParseMeshForCollision(TrackSections* sections, size_t numMesh) {
    printf("\n[Track] Generating Collision Meshes...\n");

    // Set configurations to zero
    D_8015F59C = 0;
    D_8015F5A0 = 0;
    D_8015F5A4 = 0;

    for (size_t i = 0; i < numMesh; i++) {
        switch(static_cast<SurfaceClip>(sections[i].clip)) {
            case SurfaceClip::CLIP_NONE:
                continue;
            case SurfaceClip::CLIP_SINGLE_SIDED_WALL:
                D_8015F59C = 1;
                break;
            case SurfaceClip::CLIP_SURFACE:
                D_8015F5A0 = 1;
                break;
            case SurfaceClip::CLIP_DOUBLE_SIDED_WALL:
                D_8015F5A4 = 1;
                break;
        }

        const char* name = ResourceGetNameByCrc(sections[i].crc);
        printf("  %s\n", name);
        generate_collision_mesh((Gfx*) ResourceGetDataByCrc(sections[i].crc), sections[i].surfaceType,
                                sections[i].sectionId);
    }
    printf("[Track] Collision Mesh Generation Complete!\n\n");
}

void CustomTrack::TestPath() {
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

void CustomTrack::BeginPlay() {
    printf("[Track] BeginPlay\n");
    CustomTrack::TestPath();
    this->SpawnActors();
}

void CustomTrack::Draw(ScreenContext* arg0) {
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (bFog) {
        gDPSetCycleType(gDisplayListHead++, G_CYC_2CYCLE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);
        gSPSetGeometryMode(gDisplayListHead++, G_FOG);

        gDPSetFogColor(gDisplayListHead++, gFogColour.r, gFogColour.g, gFogColour.b, gFogColour.a);
        gSPFogPosition(gDisplayListHead++, gFogMin, gFogMax);
        gDPPipeSync(gDisplayListHead++);
    } else {
        gSPClearGeometryMode(gDisplayListHead++, G_FOG);
    }

    set_track_light_direction(D_800DC610, D_802B87D4, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    }

    const TrackInfo* info = gTrackRegistry.GetInfo(ResourceName);
    if (nullptr == info) {
        printf("[Track] [Draw] Resource name did not return a valid TrackInfo %s\n", ResourceName.c_str());
        return;
    }
    std::string res = info->Path + "/data_track_sections";

    TrackSections* sections = (TrackSections*) LOAD_ASSET_RAW(res.c_str());
    size_t size = ResourceGetSizeByName(res.c_str());
    size_t totalSections = size / sizeof(TrackSections);
    for (uint64_t item : mOpaqueItems) {
        gSPDisplayList(gDisplayListHead++, (Gfx*) ResourceGetDataByCrc(item));
    }
}

// This should be a step for tick material
void CustomTrack::ScrollingTextures() {
    // D_802B87BC -= 20;
    // if (D_802B87BC < 0) {
    //     D_802B87BC = 0xFF;
    // }
    // // d_course_royal_raceway_packed_dl_A6A8
    // find_and_set_tile_size((uintptr_t) d_course_royal_raceway_packed_dl_A6A8, 0, D_802B87BC);
}

void CustomTrack::DrawWater(ScreenContext* screen, uint16_t pathCounter, uint16_t cameraRot, uint16_t playerDirection) {
    //FVector position = { screen->camera->pos[0], screen->camera->pos[1], screen->camera->pos[2] };

    // Sorting objects might matter in the future. But it doesn't really work for static geometry which
    // does not have a position other than 0,0,0. And if it did, the geometry can be big making sorting inaccurate.
    // std::sort(mTranslucentItems.begin(), mTranslucentItems.end(),
    //         [&](uint64_t a, uint64_t b) {
    //     FVector posA = GetItemPosition(a);
    //     FVector posB = GetItemPosition(b);

    //     // Distance squared is enough, avoids sqrt
    //     float distA = (posA - screen->camera->pos).LengthSquared();
    //     float distB = (posB - screen->camera->pos).LengthSquared();

    //     return distA > distB; // farthest first
    // });

    for (uint64_t item : mTranslucentItems) {
        gSPDisplayList(gDisplayListHead++, (Gfx*) ResourceGetDataByCrc(item));
    }

    for (uint64_t item : mTranslucentNoZBufferItems) {
        gSPDisplayList(gDisplayListHead++, (Gfx*) ResourceGetDataByCrc(item));
    }
}
