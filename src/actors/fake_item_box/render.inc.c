#include <actors.h>
#include <code_800029B0.h>
#include <libultra/gbi.h>
#include <main.h>
#include "port/interpolation/FrameInterpolation.h"

/**
 * @brief Renders the fake item box actor.
 *
 * @param camera
 * @param fakeItemBox
 */
void render_actor_fake_item_box(Camera* camera, struct FakeItemBox* fakeItemBox) {
    Vec3s someRot;
    UNUSED s32 pad[3];
    Vec3f someVec;
    Mat4 someMatrix2;
    Mat4 someMatrix3;
    UNUSED s32 pad2[12];
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f12;
    f32 temp_f2;
    f32 thing;
    f32 temp_f2_2;
    f32 someMultiplier;

    // @port: Tag the transform.
    FrameInterpolation_RecordOpenChild("Fake Item Box", TAG_ITEM_ADDR(fakeItemBox));

    if (is_within_render_distance(camera->pos, fakeItemBox->pos, camera->rot[1], 2500.0f, gCameraFOV[camera - camera1],
                                  1000000.0f) < 0 &&
        CVarGetInteger("gNoCulling", 0) == 0) {
        actor_not_rendered(camera, (struct Actor*) fakeItemBox);
        return;
    }
    if (((f32) gCourseMaxY + 800.0f) < fakeItemBox->pos[1]) {
        actor_not_rendered(camera, (struct Actor*) fakeItemBox);
        return;
    }
    if (fakeItemBox->pos[1] < ((f32) gCourseMinY - 800.0f)) {
        actor_not_rendered(camera, (struct Actor*) fakeItemBox);
        return;
    }

    actor_rendered(camera, (struct Actor*) fakeItemBox);
    someRot[0] = 0;
    someRot[1] = fakeItemBox->rot[1];
    someRot[2] = 0;
    mtxf_pos_rotation_xyz(someMatrix2, fakeItemBox->pos, someRot);
    mtxf_scale(someMatrix2, fakeItemBox->sizeScaling);
    if (fakeItemBox->state != 2) {

        if (!render_set_position(someMatrix2, 0)) {
            return;
        }

        gSPDisplayList(gDisplayListHead++, common_model_fake_itembox);
        mtxf_pos_rotation_xyz(someMatrix2, fakeItemBox->pos, fakeItemBox->rot);
        mtxf_scale(someMatrix2, fakeItemBox->sizeScaling);

        if (!render_set_position(someMatrix2, 0)) {
            return;
        }

        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
        gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);

        /*
         * In the original game, the question mark texture would become corrupted. Thus, this code
         * makes it disappear to hide the issue. Since the texture no longer becomes corrupted, this
         * fix can be removed.
         */
#ifdef TARGET_N64
        if ((fakeItemBox->rot[1] < 0xAA1) && (fakeItemBox->rot[1] > 0)) {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else if ((fakeItemBox->rot[1] >= 0x6AA5) && (fakeItemBox->rot[1] < 0x754E)) {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else if ((fakeItemBox->rot[1] >= 0x38E1) && (fakeItemBox->rot[1] < 0x438A)) {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else if ((fakeItemBox->rot[1] >= 0xC711) && (fakeItemBox->rot[1] < 0xD1BA)) {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else {
#endif
            gDPSetBlendMask(gDisplayListHead++, 0xFF);
            gDPSetRenderMode(gDisplayListHead++, G_RM_ZB_CLD_SURF, G_RM_ZB_CLD_SURF2);
#ifdef TARGET_N64
        }
#endif
        gSPDisplayList(gDisplayListHead++, D_0D003090);
    } else {
        gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
        gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
        gDPSetBlendMask(gDisplayListHead++, 0xFF);
        thing = fakeItemBox->someTimer;
        mtxf_pos_rotation_xyz(someMatrix2, fakeItemBox->pos, fakeItemBox->rot);
        if (thing < 10.0f) {
            someMultiplier = 1.0f;
        } else {
            someMultiplier = 1.0f - ((thing - 10.0f) * 0.1f);
        }
        mtxf_scale(someMatrix2, someMultiplier);
        if (fakeItemBox->someTimer & 1) {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
        }
        temp_f2 = 2.0f * thing;
        someVec[0] = 0.0f;
        someVec[1] = temp_f2;
        someVec[2] = thing;
        add_translate_mat4_vec3f(someMatrix2, someMatrix3, someVec);

        if (!render_set_position(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(gDisplayListHead++, D_0D003158);
        temp_f2_2 = 0.8f * thing;
        temp_f12 = 0.5f * thing;
        someVec[0] = temp_f2_2;
        someVec[1] = 2.3f * thing;
        someVec[2] = temp_f12;
        add_translate_mat4_vec3f(someMatrix2, someMatrix3, someVec);

        if (!render_set_position(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(gDisplayListHead++, D_0D0031B8);
        temp_f0_2 = -0.5f * thing;
        someVec[0] = temp_f2_2;
        someVec[1] = 1.2f * thing;
        someVec[2] = temp_f0_2;
        add_translate_mat4_vec3f(someMatrix2, someMatrix3, someVec);

        if (!render_set_position(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(gDisplayListHead++, D_0D003128);
        if (!(fakeItemBox->someTimer & 1)) {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        } else {
            gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_XLU_INTER, G_RM_NOOP2);
        }
        someVec[0] = 0.0f;
        someVec[1] = 1.8f * thing;
        someVec[2] = -1.0f * thing;
        add_translate_mat4_vec3f(someMatrix2, someMatrix3, someVec);

        if (!render_set_position(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(gDisplayListHead++, D_0D0031E8);
        temp_f0_3 = -0.8f * thing;
        someVec[0] = temp_f0_3;
        someVec[1] = 0.6f * thing;
        someVec[2] = temp_f0_2;
        add_translate_mat4_vec3f(someMatrix2, someMatrix3, someVec);

        if (!render_set_position(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(gDisplayListHead++, D_0D003188);
        someVec[0] = temp_f0_3;
        someVec[1] = temp_f2;
        someVec[2] = temp_f12;
        add_translate_mat4_vec3f(someMatrix2, someMatrix3, someVec);

        if (!render_set_position(someMatrix3, 0)) {
            return;
        }

        gSPDisplayList(gDisplayListHead++, D_0D0030F8);
        gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    }
    // @port Pop the transform id.
    FrameInterpolation_RecordCloseChild();
}
