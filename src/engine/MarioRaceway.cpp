#include <libultraship.h>
#include <libultra/gbi.h>

#include "MarioRaceway.h"

extern "C" {
    #include "main.h"
    #include "camera.h"
    #include "course_offsets.h"
    #include "code_800029B0.h"
    #include "render_courses.h"
    //#include "all_course_offsets.h"
    #include "memory.h"
    extern const char *mario_raceway_dls[];
}

CourseMarioRaceway::CourseMarioRaceway() {}

    // Virtual functions to be overridden by derived classes
// void CourseMarioRaceway::Load(const char* courseVtx, 
//                   course_texture* textures, const char* displaylists, size_t dlSize) {
//     StockCourse::Load(courseVtx, textures, displaylists, dlSize);
// }
void CourseMarioRaceway::LoadTextures() {}
void CourseMarioRaceway::Init() {  }
void CourseMarioRaceway::BeginPlay() {  }
void CourseMarioRaceway::Render(struct UnkStruct_800DC5EC* arg0) {
    u16 sp22 = arg0->pathCounter;
    u16 temp_t0 = arg0->playerDirection;

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_mario_raceway_packed_dl_3050
        gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07003050))));
    }

    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gDPPipeSync(gDisplayListHead++);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);

    switch (sp22) {
        case 1:
        case 2:
        case 17:
            if ((temp_t0 == 2) || (temp_t0 == 1)) {
                func_802911C4();
            }
            break;
        case 3:
            if (temp_t0 != 0) {
                func_802911C4();
            }
            break;
        case 4:
            if (temp_t0 == 0) {
                func_80291198();
            } else {
                if (temp_t0 == 1) {
                    func_80291198();
                }
                func_802911C4();
            }
            break;
        case 5:
        case 6:
            if ((temp_t0 == 2) || (temp_t0 == 3)) {
                func_802911C4();
            } else {
                func_80291198();
            }
            break;
        case 7:
            func_80291198();
            if ((temp_t0 == 2) || (temp_t0 == 3)) {
                func_802911C4();
            }
            break;
        case 8:
        case 9:
            if (temp_t0 != 1) {
                func_802911C4();
            }
            /* fallthrough */
        case 10:
            if (temp_t0 != 2) {
                func_80291198();
            }
            break;
        case 11:
            if (temp_t0 == 0) {
                func_802911C4();
                func_80291198();
            } else if (temp_t0 == 3) {
                func_802911C4();
            }
            break;
        case 12:
            if ((temp_t0 == 0) || (temp_t0 == 3)) {
                func_802911C4();
            }
            break;
        case 13:
        case 14:
            if (temp_t0 != 1) {
                case 15:
                case 16:
                    func_802911C4();
            }
            break;
    }
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07003508))));
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07003240))));
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070014A0))));
    render_course_segments(mario_raceway_dls, arg0);
    gDPSetCombineMode(gDisplayListHead++, G_CC_MODULATEIDECALA, G_CC_MODULATEIDECALA);
    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2);
    gSPClearGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000450))));
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000240))));
    gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK);
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x070000E0))));
    gSPDisplayList(gDisplayListHead++, (segmented_gfx_to_virtual(reinterpret_cast<void*>(0x07000160))));
}
void CourseMarioRaceway::Collision() {}
void CourseMarioRaceway::Destroy() { }
