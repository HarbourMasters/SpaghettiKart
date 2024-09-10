#include <actors.h>
#include <main.h>
#include <assets/choco_mountain_data.h>

/**
 * @brief Renders the falling rock actor.
 * Actor used in Choco Mountain.
 *
 * @param camera
 * @param rock
 */
void render_actor_falling_rock(Camera* camera, struct FallingRock* rock) {
    Vec3s sp98;
    Vec3f sp8C;
    Mat4 sp4C;
    f32 height;
    UNUSED s32 pad[4];

    if (rock->respawnTimer != 0) {
        return;
    }

    height = is_within_render_distance(camera->pos, rock->pos, camera->rot[1], 400.0f, gCameraZoom[camera - camera1],
                                       4000000.0f);

    if (CVarGetInteger("gNoCulling", 0) == 1) {
        height = CLAMP(height, 0.0f, 250000.0f);
    }

    if (height < 0.0f) {
        return;
    }

    if (height < 250000.0f) {

        if (rock->unk30.unk34 == 1) {
            sp8C[0] = rock->pos[0];
            sp8C[2] = rock->pos[2];
            height = calculate_surface_height(sp8C[0], rock->pos[1], sp8C[2], rock->unk30.meshIndexZX);
            sp98[0] = 0;
            sp98[1] = 0;
            sp98[2] = 0;
            sp8C[1] = height + 2.0f;
            mtxf_pos_rotation_xyz(sp4C, sp8C, sp98);
            if (render_set_position(sp4C, 0) == 0) {
                return;
            }
            gSPDisplayList(gDisplayListHead++, d_course_choco_mountain_dl_6F88);
        }
    }
    mtxf_pos_rotation_xyz(sp4C, rock->pos, rock->rot);
    if (render_set_position(sp4C, 0) == 0) {
        return;
    }
    // gSPDisplayList(gDisplayListHead++, d_course_choco_mountain_dl_falling_rock);
    //! @todo: Placeholder DLs weird vtx issues
    gSPSetGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCycleType(gDisplayListHead++, G_CYC_2CYCLE);
    gDPPipeSync(gDisplayListHead++);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPSetGeometryMode(gDisplayListHead++, G_FOG);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEI, G_CC_PASS2);
    gDPSetRenderMode(gDisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPPipeSync(gDisplayListHead++);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0,
               G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, 5, G_TX_NOLOD);
    gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, d_course_choco_mountain_wall_texture);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP,
               G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gDisplayListHead++);
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 1023, 256);
    gSPVertex(gDisplayListHead++, d_course_choco_mountain_falling_rock_model, 15, 0);
    gSP1Triangle(gDisplayListHead++, 0, 1, 2, 0);
    gSP1Triangle(gDisplayListHead++, 3, 4, 5, 0);
    gSP1Triangle(gDisplayListHead++, 6, 7, 8, 0);
    gSP1Triangle(gDisplayListHead++, 9, 10, 11, 0);
    gSP1Triangle(gDisplayListHead++, 12, 13, 14, 0);
    gSPVertex(gDisplayListHead++, d_course_choco_mountain_6006C28, 5, 0);
    gSPVertex(gDisplayListHead++, d_course_choco_mountain_6006D08, 10, 5);
    gSP1Triangle(gDisplayListHead++, 0, 5, 6, 0);
    gSP1Triangle(gDisplayListHead++, 1, 7, 8, 0);
    gSP1Triangle(gDisplayListHead++, 2, 9, 10, 0);
    gSP1Triangle(gDisplayListHead++, 3, 11, 12, 0);
    gSP1Triangle(gDisplayListHead++, 4, 13, 14, 0);
    gSPVertex(gDisplayListHead++, d_course_choco_mountain_6006C78, 5, 0);
    gSPVertex(gDisplayListHead++, d_course_choco_mountain_6006DA8, 10, 5);
    gSP1Triangle(gDisplayListHead++, 0, 5, 6, 0);
    gSP1Triangle(gDisplayListHead++, 1, 7, 8, 0);
    gSP1Triangle(gDisplayListHead++, 2, 9, 10, 0);
    gSP1Triangle(gDisplayListHead++, 3, 11, 12, 0);
    gSP1Triangle(gDisplayListHead++, 4, 13, 14, 0);
    gSPVertex(gDisplayListHead++, d_course_choco_mountain_6006CC8, 4, 0);
    gSPVertex(gDisplayListHead++, d_course_choco_mountain_6006E48, 8, 4);
    gSP1Triangle(gDisplayListHead++, 0, 4, 5, 0);
    gSP1Triangle(gDisplayListHead++, 1, 6, 7, 0);
    gSP1Triangle(gDisplayListHead++, 2, 8, 9, 0);
    gSP1Triangle(gDisplayListHead++, 3, 10, 11, 0);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0x0000, G_TX_RENDERTILE, 0, G_TX_MIRROR | G_TX_WRAP,
               5, G_TX_NOLOD, G_TX_MIRROR | G_TX_WRAP, 5, G_TX_NOLOD);
    gDPSetTileSize(gDisplayListHead++, G_TX_RENDERTILE, 0, 0, 0x007C, 0x007C);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, d_course_choco_mountain_rock_texture);
    gDPTileSync(gDisplayListHead++);
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0x0000, G_TX_LOADTILE, 0, G_TX_NOMIRROR | G_TX_WRAP,
               G_TX_NOMASK, G_TX_NOLOD, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOLOD);
    gDPLoadSync(gDisplayListHead++);
    gDPLoadBlock(gDisplayListHead++, G_TX_LOADTILE, 0, 0, 1023, 256);
    gSPVertex(gDisplayListHead++, d_course_choco_mountain_6006EC8, 3, 0);
    gSP1Triangle(gDisplayListHead++, 0, 1, 2, 0);
    gSPClearGeometryMode(gDisplayListHead++, G_FOG | G_LIGHTING);
    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 1, 1, G_OFF);
    gDPPipeSync(gDisplayListHead++);
}
