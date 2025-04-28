#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "MooMooFarm.h"
#include "World.h"
#include "engine/actors/Finishline.h"
#include "engine/objects/BombKart.h"
#include "assets/moo_moo_farm_data.h"
#include "engine/objects/MoleGroup.h"
#include "engine/objects/Mole.h"

extern "C" {
    #include "main.h"
    #include "camera.h"
    #include "course_offsets.h"
    #include "code_800029B0.h"
    #include "render_courses.h"
    #include "code_8006E9C0.h"
    #include "code_80057C60.h"
    #include "defines.h"
    #include "math_util.h"
    #include "external.h"
    #include "code_80005FD0.h"
    #include "spawn_players.h"
    #include "render_objects.h"
    #include "assets/common_data.h"
    #include "save.h"
    #include "staff_ghosts.h"
    #include "actors.h"
    #include "collision.h"
    #include "memory.h"
    #include "code_80086E70.h"
    #include "course.h"
    extern const char *moo_moo_farm_dls[];
    extern s16 currentScreenSection;
    extern s8 gPlayerCount;
}

const course_texture moo_moo_farm_textures[] = {
    { gTextureWoodDoor0, 0x0294, 0x1000, 0x0 },
    { gTextureGrass2, 0x0415, 0x0800, 0x0 },
    { gTexture64AF50, 0x0140, 0x0800, 0x0 },
    { gTexture64B090, 0x0365, 0x0800, 0x0 },
    { gTexture64B54C, 0x038C, 0x0800, 0x0 },
    { gTexture64B3F8, 0x0153, 0x0800, 0x0 },
    { gTextureSignNintendo0, 0x0541, 0x1000, 0x0 },
    { gTextureSignNintendo1, 0x0512, 0x1000, 0x0 },
    { gTexture6684F8, 0x010D, 0x0800, 0x0 },
    { gTextureSignLuigis0, 0x0287, 0x1000, 0x0 },
    { gTextureSignLuigis1, 0x02AF, 0x1000, 0x0 },
    { gTextureSignMarioStar0, 0x02D2, 0x1000, 0x0 },
    { gTextureSignMarioStar1, 0x02B1, 0x1000, 0x0 },
    { gTexture674D58, 0x030C, 0x1000, 0x0 },
    { gTexture675064, 0x01BB, 0x0800, 0x0 },
    { gTexture675220, 0x0212, 0x0800, 0x0 },
    { gTexture6775EC, 0x0233, 0x1000, 0x0 },
    { gTexture683314, 0x02DC, 0x1000, 0x0 },
    { gTexture68CDA0, 0x0110, 0x0800, 0x0 },
    { gTexture6442D4, 0x0138, 0x0800, 0x0 },
    { gTexture64440C, 0x029D, 0x1000, 0x0 },
    { gTexture6446AC, 0x0116, 0x0800, 0x0 },
    { gTextureMooMooFarmSignLeft, 0x0A66, 0x1000, 0x0 },
    { gTextureMooMooFarmSignRight, 0x0A64, 0x1000, 0x0 },
    { gTexture64ACAC, 0x02A3, 0x0800, 0x0 },
    { gTexture66D698, 0x0370, 0x0800, 0x0 },
    { gTexture66EBF0, 0x0146, 0x0800, 0x0 },
    { gTextureWheelSteamEngineReal, 0x022F, 0x1000, 0x0 },
    { 0x00000000, 0x0000, 0x0000, 0x0 },
};

MooMooFarm::MooMooFarm() {
    this->vtx = d_course_moo_moo_farm_vertex;
    this->gfx = d_course_moo_moo_farm_packed_dls;
    this->gfxSize = 3304;
    Props.textures = moo_moo_farm_textures;
    Props.Minimap.Texture = gTextureCourseOutlineMooMooFarm;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 271;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 18;
    Props.Minimap.PlayerY = 36;
    Props.Minimap.PlayerScaleFactor = 0.0155f;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;

    Props.SetText(Props.Name, "moo moo farm", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "farm", sizeof(Props.DebugName));
    Props.SetText(Props.CourseLength, "527m", sizeof(Props.CourseLength));

    Props.AIBehaviour = D_0D009210;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.5f;
    Props.AISteeringSensitivity = 48;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = {0x230, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

    Props.D_0D009418[0] = 4.1666665f;
    Props.D_0D009418[1] = 5.5833334f;
    Props.D_0D009418[2] = 6.1666665f;
    Props.D_0D009418[3] = 6.75f;

    Props.D_0D009568[0] = 3.75f;
    Props.D_0D009568[1] = 5.1666665f;
    Props.D_0D009568[2] = 5.75f;
    Props.D_0D009568[3] = 6.3333334f;

    Props.D_0D0096B8[0] = 3.3333332f;
    Props.D_0D0096B8[1] = 3.9166667f;
    Props.D_0D0096B8[2] = 4.5f;
    Props.D_0D0096B8[3] = 5.0833334f;

    Props.D_0D009808[0] = 3.75f;
    Props.D_0D009808[1] = 5.1666665f;
    Props.D_0D009808[2] = 5.75f;
    Props.D_0D009808[3] = 6.3333334f;

    Props.PathTable[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_moo_moo_farm_unknown_waypoints);
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = (TrackWaypoint*)LOAD_ASSET_RAW(d_course_moo_moo_farm_track_waypoints);
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.CloudTexture = (u8*) LOAD_ASSET_RAW(gTextureExhaust0);
    Props.Clouds = gYoshiValleyMooMooFarmClouds;
    Props.CloudList = gYoshiValleyMooMooFarmClouds;

    Props.Skybox.TopRight = {0, 18, 255};
    Props.Skybox.BottomRight = {197, 211, 255};
    Props.Skybox.BottomLeft = {197, 211, 255};
    Props.Skybox.TopLeft = {0, 18, 255};
    Props.Skybox.FloorTopRight = {255, 184, 99};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {255, 184, 99};
    Props.Sequence = MusicSeq::MUSIC_SEQ_MOO_MOO_FARM_YOSHI_VALLEY;
}

void MooMooFarm::Load() {
    Course::Load();

    parse_course_displaylists((TrackSections*)LOAD_ASSET_RAW(d_course_moo_moo_farm_addr));
    func_80295C6C();
    Props.WaterLevel = gCourseMinY - 10.0f;
}

void MooMooFarm::LoadTextures() {
    dma_textures(gTextureTrees4Left, 0x000003E8U, 0x00000800U);
    dma_textures(gTextureTrees4Right, 0x000003E8U, 0x00000800U);
    dma_textures(gTextureCow01Left, 0x00000400U, 0x00000800U);
    dma_textures(gTextureCow01Right, 0x00000400U, 0x00000800U);
    dma_textures(gTextureCow02Left, 0x00000400U, 0x00000800U);
    dma_textures(gTextureCow02Right, 0x00000400U, 0x00000800U);
    dma_textures(gTextureCow03Left, 0x00000400U, 0x00000800U);
    dma_textures(gTextureCow03Right, 0x00000400U, 0x00000800U);
    dma_textures(gTextureCow04Left, 0x00000400U, 0x00000800U);
    dma_textures(gTextureCow04Right, 0x00000400U, 0x00000800U);
    dma_textures(gTextureCow05Left, 0x00000400U, 0x00000800U);
    dma_textures(gTextureCow05Right, 0x00000400U, 0x00000800U);
}

// These are full arrays that are not used in the original game
static std::vector<FVector> sMoleSpawns1 = {
    { FVector(771, 20, -2022) },
    { FVector(807, 15, -2063) },
    { FVector(847, 18, -2040) },
    { FVector(913, 14, -2054) },
    { FVector(939, 21, -1997) },
    { FVector(941, 17, -2024) },
    { FVector(994, 17, -1994) },
    { FVector(863, 22, -2010) },
};

static std::vector<FVector> sMoleSpawns2 = {
    { FVector(1500, 2, 1140) },
    { FVector(1510, 15, 1050) },
    { FVector(1609, 21, 935) },
    { FVector(1289, 3, 1269) },
    { FVector(1468, 22, 1046) },
    { FVector(1380, 12, 1154) },
    { FVector(1297, 19, 1170) },
    { FVector(1589, 11, 1004) },
    { FVector(1414, 3, 1185) },
    { FVector(1405, 4, 1254) },
    { FVector(1463, 8, 1118) },
};

static std::vector<FVector> sMoleSpawns3 = {
    { FVector(701, 2, 1279) },
    { FVector(811, 8, 1278) },
    { FVector(791, 16, 1229) },
    { FVector(876, 15, 1266) },
    { FVector(984, 23, 1248) },
    { FVector(891, 20, 1242) },
    { FVector(920, 15, 1304) },
    { FVector(823, 6, 1327) },
    { FVector(717, 8, 1239) },
    { FVector(695, 19, 1176) },
    { FVector(628, 8, 1191) },
    { FVector(724, 4, 1339) },
};

void MooMooFarm::BeginPlay() {
    if (gPlayerCountSelection1 != 4) {
        spawn_foliage((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_moo_moo_farm_tree_spawn));
    }
    spawn_all_item_boxes((struct ActorSpawnData*)LOAD_ASSET_RAW(d_course_moo_moo_farm_item_box_spawns));

    for (size_t i = 0; i < gObjectParticle2_SIZE; i++) {
        find_unused_obj_index(&gObjectParticle2[i]);
    }

    if (gGamestate != CREDITS_SEQUENCE) {
        switch(gCCSelection) {
            case CC_50: {
                std::vector<FVector> moleSpawns1_50 = { { FVector(771, 20, -2022) },
                                                        { FVector(807, 15, -2063) },
                                                        { FVector(847, 18, -2040) },
                                                        { FVector(913, 14, -2054) } };

                gWorldInstance.AddObject(new OMoleGroup(moleSpawns1_50));

                std::vector<FVector> moleSpawns2_50 = { { FVector(1500, 2, 1140) },  { FVector(1510, 15, 1050) },
                                                        { FVector(1609, 21, 935) },  { FVector(1289, 3, 1269) },
                                                        { FVector(1468, 22, 1046) }, { FVector(1380, 12, 1154) } };

                gWorldInstance.AddObject(new OMoleGroup(moleSpawns2_50));

                std::vector<FVector> moleSpawns3_50 = { { FVector(701, 2, 1279) },  { FVector(811, 8, 1278) },
                                                        { FVector(791, 16, 1229) }, { FVector(876, 15, 1266) },
                                                        { FVector(984, 23, 1248) }, { FVector(891, 20, 1242) } };

                gWorldInstance.AddObject(new OMoleGroup(moleSpawns3_50));
                break;
            }
            case CC_100: {
                std::vector<FVector> moleSpawns1_100 = { { FVector(771, 20, -2022) },
                                                         { FVector(807, 15, -2063) },
                                                         { FVector(847, 18, -2040) },
                                                         { FVector(913, 14, -2054) },
                                                         { FVector(939, 21, -1997) } };

                gWorldInstance.AddObject(new OMoleGroup(moleSpawns1_100));

                std::vector<FVector> moleSpawns2_100 = { { FVector(1500, 2, 1140) },  { FVector(1510, 15, 1050) },
                                                         { FVector(1609, 21, 935) },  { FVector(1289, 3, 1269) },
                                                         { FVector(1468, 22, 1046) }, { FVector(1380, 12, 1154) },
                                                         { FVector(1297, 19, 1170) }, { FVector(1589, 11, 1004) } };

                gWorldInstance.AddObject(new OMoleGroup(moleSpawns2_100));

                std::vector<FVector> moleSpawns3_100 = { { FVector(701, 2, 1279) },  { FVector(811, 8, 1278) },
                                                         { FVector(791, 16, 1229) }, { FVector(876, 15, 1266) },
                                                         { FVector(984, 23, 1248) }, { FVector(891, 20, 1242) },
                                                         { FVector(920, 15, 1304) }, { FVector(823, 6, 1327) } };

                gWorldInstance.AddObject(new OMoleGroup(moleSpawns3_100));
                break;
            }
            case CC_150: {
                std::vector<FVector> moleSpawns1_150 = { { FVector(771, 20, -2022) },
                                                         { FVector(807, 15, -2063) },
                                                         { FVector(847, 18, -2040) },
                                                         { FVector(913, 14, -2054) },
                                                         { FVector(939, 21, -1997) } };

                gWorldInstance.AddObject(new OMoleGroup(moleSpawns1_150));

                std::vector<FVector> moleSpawns2_150 = { { FVector(1500, 2, 1140) },  { FVector(1510, 15, 1050) },
                                                         { FVector(1609, 21, 935) },  { FVector(1289, 3, 1269) },
                                                         { FVector(1468, 22, 1046) }, { FVector(1380, 12, 1154) },
                                                         { FVector(1297, 19, 1170) }, { FVector(1589, 11, 1004) } };

                gWorldInstance.AddObject(new OMoleGroup(moleSpawns2_150));

                std::vector<FVector> moleSpawns3_150 = { { FVector(701, 2, 1279) },  { FVector(811, 8, 1278) },
                                                         { FVector(791, 16, 1229) }, { FVector(876, 15, 1266) },
                                                         { FVector(984, 23, 1248) }, { FVector(891, 20, 1242) },
                                                         { FVector(920, 15, 1304) }, { FVector(823, 6, 1327) },
                                                         { FVector(717, 8, 1239) },  { FVector(695, 19, 1176) } };

                gWorldInstance.AddObject(new OMoleGroup(moleSpawns3_150));
                break;
            }
            case CC_EXTRA: {
                std::vector<FVector> moleSpawns1_extra = { { FVector(771, 20, -2022) },
                                                           { FVector(807, 15, -2063) },
                                                           { FVector(847, 18, -2040) },
                                                           { FVector(913, 14, -2054) },
                                                           { FVector(939, 21, -1997) } };

                gWorldInstance.AddObject(new OMoleGroup(moleSpawns1_extra));

                std::vector<FVector> moleSpawns2_extra = { { FVector(1500, 2, 1140) },  { FVector(1510, 15, 1050) },
                                                           { FVector(1609, 21, 935) },  { FVector(1289, 3, 1269) },
                                                           { FVector(1468, 22, 1046) }, { FVector(1380, 12, 1154) },
                                                           { FVector(1297, 19, 1170) }, { FVector(1589, 11, 1004) } };

                gWorldInstance.AddObject(new OMoleGroup(moleSpawns2_extra));

                std::vector<FVector> moleSpawns3_extra = { { FVector(701, 2, 1279) },  { FVector(811, 8, 1278) },
                                                           { FVector(791, 16, 1229) }, { FVector(876, 15, 1266) },
                                                           { FVector(984, 23, 1248) }, { FVector(891, 20, 1242) },
                                                           { FVector(920, 15, 1304) }, { FVector(823, 6, 1327) } };

                gWorldInstance.AddObject(new OMoleGroup(moleSpawns3_extra));
                break;
            }
        }
    }

    if (gModeSelection == VERSUS) {
        FVector pos = { 0, 0, 0 };

        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][50], 50, 3, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][140], 140, 3, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][225], 225, 3, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][316], 316, 3, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][434], 434, 3, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][0], 0, 0, 0.8333333f));
        gWorldInstance.AddObject(new OBombKart(pos, &D_80164550[0][0], 0, 0, 0.8333333f));
    }
}

void MooMooFarm::WhatDoesThisDo(Player* player, int8_t playerId) {
    if (((s16) gNearestWaypointByPlayerId[playerId] >= 0x145) &&
        ((s16) gNearestWaypointByPlayerId[playerId] < 0x18B)) {
        if (D_80165300[playerId] != 1) {
            func_800CA288(playerId, 0x55);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA2B8(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

void MooMooFarm::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (((s16) gNearestWaypointByPlayerId[playerId] >= 0x145) &&
        ((s16) gNearestWaypointByPlayerId[playerId] < 0x18B)) {
        if (D_80165300[playerId] != 1) {
            func_800CA2E4(playerId, 0x55);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA30C(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

void MooMooFarm::Render(struct UnkStruct_800DC5EC* arg0) {
    s16 temp_s0 = arg0->pathCounter;
    s16 temp_s1 = arg0->playerDirection;

    set_track_light_direction(D_800DC610, D_802B87D4, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    // d_course_moo_moo_farm_packed_dl_4DF8
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual((void*)0x07004DF8));
    // d_course_moo_moo_farm_packed_dl_5640
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual((void*)0x07005640));
    gSPFogPosition(gDisplayListHead++, D_802B87B0, D_802B87B4);

    render_course_segments(moo_moo_farm_dls, arg0);

    if ((temp_s0 < 14) && (temp_s0 > 10)) {
        if ((temp_s1 == 2) || (temp_s1 == 3) || (temp_s1 == 1)) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_13FF8);
        }

    } else if (temp_s0 < 16) {
        gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_13FF8);
    } else if (temp_s0 < 19) {
        if (temp_s1 != 2) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_13FF8);
        }

    } else if (temp_s0 < 20) {
        if (temp_s1 == 0) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_13FF8);
        }
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_MODULATEI);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);

    if ((temp_s0 >= 16) && (temp_s0 < 24)) {
        if ((temp_s1 == 2) || (temp_s1 == 3)) {
            // d_course_moo_moo_farm_packed_dl_5410
            gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual((void*)0x07005410));
        }

    } else if (temp_s0 < 9) {
        if (temp_s1 == 2) {
            // d_course_moo_moo_farm_packed_dl_5410
            gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual((void*)0x07005410));
        }
    }
    if (temp_s0 < 4) {
        if (temp_s1 != 0) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_14060);
        }

    } else if (temp_s0 < 8) {
        if (temp_s1 == 2) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_14060);
        }

    } else if (temp_s0 >= 22) {
        gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_14060);
    } else if (temp_s0 >= 18) {
        if ((temp_s1 == 0) || (temp_s1 == 3)) {
            gSPDisplayList(gDisplayListHead++, (Gfx*)d_course_moo_moo_farm_dl_14060);
        }
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    // d_course_moo_moo_farm_packed_dl_10C0
    gSPDisplayList(gDisplayListHead++, segmented_gfx_to_virtual((void*)0x070010C0));
}

void MooMooFarm::RenderCredits() {
    gSPDisplayList(gDisplayListHead++, (Gfx*)(d_course_moo_moo_farm_dl_14088));
}

void MooMooFarm::CreditsSpawnActors() {
    dma_textures(gTextureTrees4Left, 0x3E8, 0x800);
    dma_textures(gTextureTrees4Right, 0x3E8, 0x800);
    dma_textures(gTextureCow01Left, 0x400, 0x800);
    dma_textures(gTextureCow01Right, 0x400, 0x800);
    dma_textures(gTextureCow02Left, 0x400, 0x800);
    dma_textures(gTextureCow02Right, 0x400, 0x800);
    dma_textures(gTextureCow03Left, 0x400, 0x800);
    dma_textures(gTextureCow03Right, 0x400, 0x800);
    dma_textures(gTextureCow04Left, 0x400, 0x800);
    dma_textures(gTextureCow04Right, 0x400, 0x800);
    dma_textures(gTextureCow05Left, 0x400, 0x800);
    dma_textures(gTextureCow05Right, 0x400, 0x800);
    spawn_foliage((struct ActorSpawnData*) LOAD_ASSET_RAW(d_course_moo_moo_farm_tree_spawn));
}

void MooMooFarm::Destroy() { }
