#include <libultraship.h>
#include <common_structs.h>
#include <macros.h>
#include <mk64.h>
#include <defines.h>
#include "camera.h"
#include "math_util_2.h"
#include <sounds.h>
#include "audio/external.h"
#include "audio/load.h"
#include "audio/data.h"
#include "audio/port_eu.h"
#include "code_800029B0.h"
#include "code_80005FD0.h"
#include "menu_items.h"
#include <stdbool.h>
#include "port/Game.h"
#include "port/audio/HMAS.h"

s8 D_8018EF10;
UnkStruct8018EF18 D_8018EF18[16];
// chained list
struct Unk_8018EFD8 D_8018EFD8[50];
u8 D_8018FB90;
u8 D_8018FB91;
Camera* gCopyCamera[4];
Vec3f gVelocityCamera[4];
Vec3f gCameraLastPos[4];
u8 D_8018FC08;
s16 D_8018FC10[4][2];
struct Sound sSoundRequests[0x100];
struct SoundCharacteristics sSoundBanks[SOUND_BANK_COUNT][20];
u8 sSoundBankUsedListBack[SOUND_BANK_COUNT];
u8 sSoundBankFreeListFront[SOUND_BANK_COUNT];
u8 sNumSoundsInBank[SOUND_BANK_COUNT];
u8 D_80192AB8[SOUND_BANK_COUNT][8][8];
u8 D_80192C38;
bool sSoundBankDisabled[SOUND_BANK_COUNT];
struct ChannelVolumeScaleFade D_80192C48[SOUND_BANK_COUNT];
struct_D_80192CA8_entry D_80192CA8[3][5];
u8 D_80192CC6[3];
u32 D_80192CD0[256];
struct_D_801930D0_entry D_801930D0[3];

u8 D_800E9DA0 = 0;
UNUSED s32 D_800E9DA4[] = { 0, 0, 0, 0 };
s32 D_800E9DB4[] = { 0, 0, 0, 0 };
f32 D_800E9DC4[] = { 1.0f, 1.0f, 1.0f, 1.0f };
f32 D_800E9DD4[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9DE4[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9DF4[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
s32 D_800E9E14[] = { 0, 0, 0, 0 };
s32 D_800E9E24[] = { 0, 0, 0, 0 };
s32 D_800E9E34[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
f32 D_800E9E54[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9E64[] = { 0.0f, 0.0f, 0.0f, 0.0f };
s32 D_800E9E74[] = { 0, 0, 0, 0 };
s32 D_800E9E84[] = { 0, 0, 0, 0 };
u32 D_800E9E94[] = { 0, 0, 0, 0 };
s32 D_800E9EA4[] = { 0, 0, 0, 0 };
f32 D_800E9EB4[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9EC4[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9ED4[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9EE4[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9EF4[] = { 1.0f, 1.0f, 1.0f, 1.0f };
f32 D_800E9F04[] = { 1.0f, 1.0f, 1.0f, 1.0f };
f32 D_800E9F14[] = { 1.0f, 1.0f, 1.0f, 1.0f };
u8 D_800E9F24[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
u8 D_800E9F2C[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
f32 D_800E9F34[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800E9F54[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
u8 D_800E9F74[] = { 0, 0, 0, 0 };
u8 D_800E9F78[] = { 0, 0, 0, 0 };
struct Unk_800E9F7C D_800E9F7C[] = {
    { { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 0, 3800.0f, 3.4f, 0.4f, -1.0f, 0.4f, 1100.0f, 630.0f, 3600.0f, 1.0f },
    { { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 0, 3800.0f, 3.4f, 0.4f, -1.0f, 0.4f, 1100.0f, 630.0f, 3600.0f, 1.0f },
    { { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 0, 3800.0f, 3.4f, 0.4f, -1.0f, 0.4f, 1100.0f, 630.0f, 3600.0f, 1.0f },
    { { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 0, 3800.0f, 3.4f, 0.4f, -1.0f, 0.4f, 1100.0f, 630.0f, 3600.0f, 1.0f }
};
struct Unk_800EA06C D_800EA06C[] = { { { 0.0f, 1.0f, 1.0f }, 0 }, { { 0.0f, 1.0f, 1.0f }, 0 },
                                     { { 0.0f, 1.0f, 1.0f }, 0 }, { { 0.0f, 1.0f, 1.0f }, 0 },
                                     { { 0.0f, 1.0f, 1.0f }, 0 }, { { 0.0f, 1.0f, 1.0f }, 0 },
                                     { { 0.0f, 1.0f, 1.0f }, 0 }, { { 0.0f, 1.0f, 1.0f }, 0 } };
u8 D_800EA0EC[] = { 0, 0, 0, 0 };
u8 D_800EA0F0 = 0;
u8 D_800EA0F4 = 0;
UNUSED Vec3f D_800EA0F8 = { 0.0f, 0.0f, 1.0f };
u8 D_800EA104 = 0;
u8 D_800EA108 = 0;
u8 D_800EA10C[] = { 0, 0, 0, 0 };
f32 D_800EA110[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800EA120[] = { 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800EA130[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
f32 D_800EA150 = 1.4f;
u8 D_800EA154[] = { 2, 2, 88, 90, 3, 48, 88, 48 };
u16 gCurrentMusicSeq = 0;
u16 D_800EA160 = 0;
u8 D_800EA164 = 0;
s8 D_800EA168 = 0;
s8 D_800EA16C = 0;
u8 D_800EA170[] = { 0, 0, 0, 0 };
u16 D_800EA174 = 0;
f32 D_800EA178 = 1.0f;
f32 D_800EA17C = 0.85f;
u16 D_800EA180 = 0;
u16 D_800EA184 = 0;
u8 D_800EA188[][6] = { { 4, 2, 2, 2, 2, 1 }, { 6, 2, 2, 2, 2, 1 }, { 8, 2, 2, 0, 1, 1 }, { 8, 2, 2, 0, 1, 1 } };
u8 D_800EA1A0[][6] = { { 4, 1, 1, 2, 2, 1 }, { 3, 1, 1, 2, 2, 1 }, { 3, 1, 1, 0, 1, 1 }, { 3, 1, 1, 0, 1, 1 } };
u8 sSoundRequestCount = 0;
u8 sNumProcessedSoundRequests = 0;
u8 D_800EA1C0 = 0;
u16 D_800EA1C4 = 0;
Vec3f D_800EA1C8 = { 0.0f, 0.0f, 0.0f };
f32 D_800EA1D4 = 1.0f;
u32 external_unused_u32_0 = 0x00000000;
u8 D_800EA1DC = 0;
u32 external_unused_u32_1 = 0x00000000;
u8 D_800EA1E4 = 0;
u8 D_800EA1E8 = 0;
u8 D_800EA1EC = 0;
u8 D_800EA1F0[] = { 0, 1, 2, 3 };
u8 D_800EA1F4[] = { 0, 0, 0, 0 };

char external_unused_string00[] = "Error : Queue is not empty ( %x ) \n";
char external_unused_string01[] = "specchg error\n";
char external_unused_string02[] = "***** CAM MAX %d *****\n";
u8 D_800EA244 = 0;
char external_unused_string03[] = "entryout !!! %d\n";
char external_unused_string04[] = "AFTER GOAL VOICE FLAME %d\n";
char external_unused_string05[] = "*** Pause On ***\n";
char external_unused_string06[] = "*** Pause Off ***\n";
char external_unused_string07[] = "CALLED!! Na_ChangeSoundMode player %d\n";
char external_unused_string08[] = "CALLED!! Na_ChangeSoundMode spec   %d\n";
char external_unused_string09[] = "Interfaced Spec Change player %d\n";
char external_unused_string10[] = "Interfaced Spec Change spec   %d\n";
UNUSED u32 external_unused_u32s[] = { 0xff000000, 0xff000000, 0x00000000 };
char external_unused_string11[] = "FX MIX %d\n";
char external_unused_string12[] = "************** Seq Fadeout ***************\n";
char external_unused_string13[] = "SEQ FADE OUT TIME %d\n";
#ifdef VERSION_EU
char external_unused_string_eu_02[] = "************** SE Fadeout ***************\n";
char external_unused_string_eu_03[] = "SE FADE OUT TIME %d\n";
#endif

#ifdef VERSION_EU
#define AUDIO_LEFT_TYRE FRONT_LEFT
#define AUDIO_RIGHT_TYRE FRONT_RIGHT
#else
#define AUDIO_LEFT_TYRE BACK_LEFT
#define AUDIO_RIGHT_TYRE BACK_RIGHT
#endif

// Requires void in the argument list to match properly.
void func_800C13F0(void) {
}

void audio_reset_session_eu(OSMesg presetId) {
    OSMesg mesg;
    osRecvMesg(D_800EA3B4, &mesg, 0);
    osSendMesg(D_800EA3B0, presetId, 0);
    osRecvMesg(D_800EA3B4, &mesg, 1);
    if (mesg.data32 != presetId.data32) {
        osRecvMesg(D_800EA3B4, &mesg, 1);
    }
}

f32 func_800C1480(u8 bank, u8 soundId) {
    f32 temp_f0;
    f32 var_f2;
    s32 var_v0;
    struct SoundCharacteristics* temp_v0;

    temp_v0 = &sSoundBanks[bank][soundId];
    if (temp_v0->soundBits & 0x400000) {
        return 1.0f;
    }
    temp_f0 = temp_v0->distance;
    if (temp_f0 > 2000.0f) {
        var_f2 = 0.0f;
    } else {
        switch (temp_v0->soundBits & 0x30000) { /* irregular */
            case 0x10000:
                var_v0 = 0x1F4;
                break;
            case 0x20000:
                var_v0 = 0x29A;
                break;
            case 0x30000:
                var_v0 = 0x3E8;
                break;
            default:
                var_v0 = 0x190;
                break;
        }
        if (temp_f0 < var_v0) {
            var_f2 = (((var_v0 - temp_f0) / var_v0) * 0.5) + 0.5;
        } else {
            var_f2 = (1.0 - ((temp_f0 - var_v0) / (2000.0f - var_v0))) * 0.5;
        }
        var_f2 *= var_f2;
    }
    return var_f2;
}

s8 func_800C15D0(u8 bank, u8 soundId, u8 channel) {
    s32 var_a0;
    s8 var_v0;
    s8 var_v1;

    var_v0 = 0;
    var_v1 = 0;
    if (!(sSoundBanks[bank][soundId].soundBits & 0x200000)) {
        if (sSoundBanks[bank][soundId].distance < 500.0f) {
            var_v0 = (sSoundBanks[bank][soundId].distance / 500.0f) * 10.0f;
        } else {
            var_v0 = 0x0A;
        }
    }
    if (IS_SEQUENCE_CHANNEL_VALID(gSequencePlayers[2].channels[channel])) {
        var_v1 = gSequencePlayers[2].channels[channel]->soundScriptIO[6];
    }
    if (var_v1 == -1) {
        var_v1 = 0;
    }
    var_a0 = *sSoundBanks[bank][soundId].unk18 + var_v0 + var_v1 + D_8018EF10;
    if (var_a0 >= 0x80) {
        var_a0 = 0x7F;
    }
    return var_a0;
}

// This is likely functionally equivallent.
#ifdef NON_MATCHING
// https://decomp.me/scratch/99CQl
s8 func_800C16E8(f32 arg0, f32 arg1, u8 cameraId) {
    f32 var_f0;
    f32 var_f14;
    f32 var_f16;
    f32 var_f18;
    f32 var_f20;
    f32 var_f2;

    if (D_800EA1C0 == 0) {
        if (D_800EA0F4 != 0) {
            var_f2 = 10.0f;
            var_f14 = 20.0f;
            var_f16 = 10.0f;
            var_f18 = 2.5f;
        } else {
            var_f2 = 100.0f;
            var_f14 = 200.0f;
            if (1) {}
            var_f16 = 5.0f;
            var_f18 = 3.3333333f;
        }
        var_f20 = (arg0 < 0.0f) ? -arg0 : arg0;

        if (var_f2 < var_f20) {
            var_f20 = var_f2;
        }
        var_f0 = (arg1 < 0.0f) ? -arg1 : arg1;

        if (var_f2 < var_f0) {
            var_f0 = var_f2;
        }
        if ((arg0 == 0.0f) && (arg1 == 0.0f)) {
            var_f2 = 0.5f;
        } else if ((arg0 >= 0.0f) && (var_f0 <= var_f20)) {
            var_f2 = 1.0f - ((var_f14 - var_f20) / (var_f16 * (var_f14 - var_f0)));
        } else if ((arg0 < 0.0f) && (var_f0 <= var_f20)) {
            var_f2 = (var_f14 - var_f20) / (var_f16 * (var_f14 - var_f0));
        } else {
            var_f2 = (arg0 / (var_f18 * var_f0)) + 0.5f;
        }
        if (var_f2 > 1.0f) {
            var_f2 = 1.0f;
        }
        if (var_f2 < 0.0f) {
            var_f2 = 0.0f;
        }
        return (s8) (s32) ((var_f2 * 127.0f) + 0.5f);
    }
    return (cameraId & 1) * 0x7F;
}
#else
GLOBAL_ASM("asm/non_matchings/audio/external/func_800C16E8.s")
#endif

f32 func_800C1934(u8 bank, u8 soundId) {
    f32 phi_f2;

    phi_f2 = 1.0f;
    if (sSoundBanks[bank][soundId].soundBits & 0x800000) {
        phi_f2 -= ((gAudioRandom & 0xF) / 192.0f);
    }
    return phi_f2;
}

void func_800C19D0(u8 arg0, u8 arg1, u8 arg2) {
    f32 sp3C;
    s8 sp3B;
    f32 sp34;
    s8 sp33;
    UnkStruct8018EF18* temp_s0_2;
    struct SoundCharacteristics* temp_s0;

    sp3B = 0;
    sp33 = 0x40;
    sp3C = 1.0f;
    sp34 = 1.0f;
    switch (arg0) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
            temp_s0 = &sSoundBanks[arg0][arg1];
            temp_s0->distance = sqrtf(temp_s0->distance);
            sp3C = (func_800C1480(arg0, arg1) * *temp_s0->unk14) * D_80192C48[arg0].current;
            sp3B = func_800C15D0(arg0, arg1, arg2);
            sp34 = func_800C1934(arg0, arg1) * *temp_s0->unk10;
            sp33 = func_800C16E8(*temp_s0->unk00[0], *temp_s0->unk08, temp_s0->cameraId);
            break;
    }
    temp_s0_2 = &D_8018EF18[arg2];
    if (sp3C != temp_s0_2->unk0) {
        func_800CBBE8(((arg2 & 0xFF) << 8) | 0x06020000 | 3, (u8) (u32) (sp3C * 127.0f));
        temp_s0_2->unk0 = sp3C;
    }
    if (sp3B != (s8) temp_s0_2->unk8) {
        func_800CBBE8(((arg2 & 0xFF) << 8) | 0x05020000, sp3B);
        temp_s0_2->unk8 = (u8) sp3B;
    }
    if (sp34 != temp_s0_2->unk4) {
        func_800CBB88(((arg2 & 0xFF) << 8) | 0x04020000, sp34);
        temp_s0_2->unk4 = sp34;
    }
    if (sp33 != (s8) temp_s0_2->unk9) {
        func_800CBBE8(((arg2 & 0xFF) << 8) | 0x03020000, sp33);
        temp_s0_2->unk9 = (u8) sp33;
    }
}

struct Unk_8018EFD8* func_800C1C88(u8 arg0, Vec3f position, f32* velocity, f32* arg3, u8 arg4, u32 soundBits) {
    struct Unk_8018EFD8* temp_a1;
    struct Unk_8018EFD8* temp_v1;
    u8 temp_t7;
    u8 why1;

    if (D_8018EFD8[D_8018FB90].next != 0xFF) {
        why1 = D_8018FB90;
        D_8018EFD8[why1].prev = D_8018FB91;
        temp_a1 = &D_8018EFD8[D_8018FB90];
        D_8018EFD8[D_8018FB91].next = why1;
        temp_t7 = temp_a1->next;
        D_8018FB91 = why1;
        D_8018FB90 = D_8018EFD8[D_8018FB90].next;
        D_8018EFD8[D_8018FB90].prev = 0xFF;
        D_8018EFD8[why1].posX = &position[0];
        D_8018EFD8[why1].posY = &position[1];
        D_8018EFD8[why1].posZ = &position[2];
        D_8018EFD8[why1].next = 0xFF;
        D_8018EFD8[why1].velX = &velocity[0];
        D_8018EFD8[why1].velY = &velocity[1];
        D_8018EFD8[why1].velZ = &velocity[2];
        D_8018EFD8[why1].unk18[1] = 0.0f;
        D_8018EFD8[why1].unk24 = arg3;
        D_8018EFD8[why1].cameraId = arg4;
        D_8018EFD8[why1].unk30 = arg0;
        D_8018EFD8[why1].soundBits = soundBits;
        return &D_8018EFD8[why1];
    }
    return NULL;
}

void func_800C1DA4(Camera* arg0, Vec3s rot, struct Unk_8018EFD8* arg2) {
    f32 x;
    f32 y;

    x = arg0->pos[0] - *arg2->posX;
    y = arg0->pos[2] - *arg2->posZ;
    arg2->unk18[0] = func_800416D8(x, y, rot[1]);
    arg2->unk18[2] = func_80041724(x, y, rot[1]);
}

void func_800C1E2C(Camera* camera, Vec3f velocity, struct Unk_8018EFD8* arg2) {
    f32 sp44;
    f32 temp_f6;
    f32 x;
    f32 y;
    f32 dist0;
    f32 dist1;
    f32 thing0;
    f32 thing1;
    f32 temp_f2;

    x = (*arg2->posX) - camera->pos[0];
    y = (*arg2->posZ) - camera->pos[2];

    sp44 = (*arg2->velX) - velocity[0];
    temp_f6 = (*arg2->velZ) - velocity[2];

    thing0 = x + sp44;
    thing1 = y + temp_f6;

    dist0 = sqrtf((x * x) + (y * y));
    dist1 = sqrtf((thing0 * thing0) + (thing1 * thing1));

    temp_f2 = 1.0f / (1.0f - ((dist0 - dist1) / arg2->unk34));

    if (temp_f2 > 0.1f) {
        arg2->unk2C = temp_f2;
    } else {
        arg2->unk2C = 0.1f;
    }

    if ((*arg2->unk24) != 0.0f) {
        arg2->unk2C *= (((*arg2->unk24) / D_800EA06C[arg2->unk30].unk00[1]) + D_800EA06C[arg2->unk30].unk00[0]) +
                       D_800E9F34[arg2->unk30];
    }
}

void func_800C1F8C(void) {
    u8 var_s1;
    u8 var_a1;
    u8 cameraId;
    Camera** camera;

    var_a1 = D_800EA1C0 + 1;
    for (var_s1 = 0; var_s1 < var_a1; var_s1++) {
        gVelocityCamera[var_s1][0] = gCopyCamera[var_s1]->pos[0] - gCameraLastPos[var_s1][0];
        gVelocityCamera[var_s1][2] = gCopyCamera[var_s1]->pos[2] - gCameraLastPos[var_s1][2];
        gCameraLastPos[var_s1][0] = gCopyCamera[var_s1]->pos[0];
        gCameraLastPos[var_s1][2] = gCopyCamera[var_s1]->pos[2];
    }

    var_a1 = 0;
    var_s1 = D_8018EFD8[0].next;
    while (var_s1 != 0xFF) {
        if (D_8018EFD8[var_s1].unk18[1] == 100000.0f) {
            if (D_8018FB91 == var_s1) {
                D_8018FB91 = D_8018EFD8[var_s1].prev;
            } else {
                D_8018EFD8[D_8018EFD8[var_s1].next].prev = D_8018EFD8[var_s1].prev;
            }
            D_8018EFD8[D_8018EFD8[var_s1].prev].next = D_8018EFD8[var_s1].next;
            D_8018EFD8[var_s1].prev = 0xFF;
            D_8018EFD8[var_s1].next = D_8018FB90;
            D_8018EFD8[D_8018FB90].prev = var_s1;
            D_8018FB90 = var_s1;
        } else {
            cameraId = D_8018EFD8[var_s1].cameraId;
            // Why? Why would you do it this way? For what possible reason?
            camera = &gCopyCamera[cameraId];
            func_800C1DA4(*camera, (*camera)->rot, &D_8018EFD8[var_s1]);
            if (D_800EA1C8 != D_8018EFD8[var_s1].velX) {
                func_800C1E2C(*camera, gVelocityCamera[0], &D_8018EFD8[var_s1]);
            }
            var_a1 = var_s1;
        }
        var_s1 = D_8018EFD8[var_a1].next;
        if ((var_s1 != 0xFF) && (D_800EA244 < var_s1)) {
            D_800EA244 = var_s1;
        }
    }
}

Vec3f* func_800C21E8(Vec3f pos, u32 soundBits) {
    u8 it; // iterator
    Vec3f* ret;
    // Only here to force a match
    UNUSED f32* thing = pos;
    struct Unk_8018EFD8* temp_a1;

    ret = 0;
    it = D_8018EFD8[0].next;
    while (it != 0xFF) {
        temp_a1 = &D_8018EFD8[it];
        // It doesn't matter what you set var_v0 to here actually
        it = D_8018EFD8[0].next;
        if ((pos == temp_a1->posX) && (soundBits == temp_a1->soundBits)) {
            // I suspect all this weirdness here is a result of someone not knowing the 'break' keyword
            it = 0xFF;
            if (temp_a1->unk18[1] != 100000.0f) {
                ret = &temp_a1->unk18;
            } else {
                goto test;
            }
        } else {
        test:
            it = temp_a1->next;
        }
    }
    return ret;
}

void func_800C2274(u8 player) {
    s16 sp46;
    s16 var_s1;
    s16 sp42;
    u8 var_s0;
    u8 temp_s0;
    u8 why = 0xFF;
    s32 var_a2;

    var_a2 = 0xF;
    if (gSequencePlayers[player].enabled != 0) {
        switch (gScreenModeSelection) { /* irregular */
            case 0:
                break;
            case 1:
                var_a2 = 0xE;
                break;
            case 3:
                if (gPlayerCountSelection1 == 3) {
                    var_a2 = 0xD;
                } else {
                    var_a2 = 0xC;
                }
                break;
        }
        temp_s0 = gSequencePlayers[player].channels[var_a2]->soundScriptIO[0];
        if (temp_s0 != why) {
            sp46 = gSequencePlayers[player].channels[var_a2]->soundScriptIO[1] % 4u;
            var_s1 = gSequencePlayers[player].channels[var_a2]->soundScriptIO[2] % 16u;
            if (var_s1 >= 0xA) {
                var_s1 = 9;
            }
            sp42 = gSequencePlayers[player].channels[var_a2]->soundScriptIO[3] % 8u;
            func_800CBBE8(((player & 0xFF) << 0x10) | 0x06000000 | ((var_a2 & 0xFF) << 8), -1);
        }
        switch (temp_s0) { /* switch 1; irregular */
            case 1:        /* switch 1 */
                D_8018FC10[sp46][0] = var_s1;
                D_8018FC10[sp46][1] = sp42;
                for (var_s0 = 0; var_s0 < 4; var_s0++) {
                    if (D_8018FC10[var_s0][0] != why) {
                        func_8001AAAC(var_s0, D_8018FC10[var_s0][0], D_8018FC10[var_s0][1]);
                        D_8018FC10[var_s0][0] = why;
                    }
                }
                break;
            case 2: /* switch 1 */
                D_8018FC10[sp46][0] = var_s1;
                D_8018FC10[sp46][1] = sp42;
                break;
        }
    }
}

void func_800C2474(void) {
    u8 var_v0;

    D_8018EF10 = 0;
    gCopyCamera[0] = camera1;
    gCopyCamera[1] = camera2;
    gCopyCamera[2] = camera3;
    gCopyCamera[3] = camera4;
    D_8018FB91 = 0;
    D_8018FB90 = 1;
    D_800EA0F4 = 0;
    D_8018FC08 = 0;
    D_800EA104 = 0;
    D_800EA108 = 0;
    D_800EA0F0 = 0;
    D_800EA16C = 0;
    func_800CBBB8(0xF2000000U, 0);
    D_800EA16C = 0;
    gCurrentMusicSeq = 0;
    D_800EA160 = 0;
    D_800EA164 = 0;
    D_800EA178 = 1.0f;
    D_800EA17C = 0.85f;
    D_800EA180 = 0;
    D_800EA184 = 0;
    for (var_v0 = 0; var_v0 < 4; var_v0++) {
        D_800E9DB4[var_v0] = 0;
        D_800E9DF4[var_v0] = 0; // This is an f32 FYI, but 0.0f doesn't match right :)
        gVelocityCamera[var_v0][0] = 0.0f;
        gVelocityCamera[var_v0][1] = 0.0f;
        gVelocityCamera[var_v0][2] = 0.0f;
        gCameraLastPos[var_v0][0] = 0.0f;
        gCameraLastPos[var_v0][1] = 0.0f;
        gCameraLastPos[var_v0][2] = 0.0f;
        D_800EA0EC[var_v0] = 0;
        D_800E9EA4[var_v0] = 0;
        D_800E9F7C[var_v0].unk_14 = 0;
        D_800E9E74[var_v0] = 0;
        D_800E9E84[var_v0] = 0;
        D_800E9E94[var_v0] = 0;
        gPlayers[var_v0].tyres[AUDIO_LEFT_TYRE].surfaceType = 0;
        gPlayers[var_v0].tyres[AUDIO_RIGHT_TYRE].surfaceType = 0;
        gPlayers[var_v0].effects = 0;
        gPlayers[var_v0].unk_20C = 0.0f;
        gPlayers[var_v0].unk_0C0 = 0;
        gPlayers[var_v0].unk_098 = 0.0f;
        gPlayers[var_v0].waterInteractionFlags = WATER_NO_INTERACTION;
        D_8018FC10[var_v0][0] = 0x00FF;
        D_8018FC10[var_v0][1] = 0;
        D_800EA10C[var_v0] = 0;
        D_800E9F74[var_v0] = 0;
        D_800E9F78[var_v0] = 0;
    }
    for (var_v0 = 0; var_v0 < NUM_PLAYERS; var_v0++) {
        D_800E9F24[var_v0] = 0;
        D_800E9F2C[var_v0] = 0;
        D_800E9F34[var_v0] = 0.0f;
        D_800E9F54[var_v0] = 0.0f;
        D_800EA130[var_v0] = 0.0f;
        D_800EA06C[var_v0].unk0C = 0;
        gPlayers[var_v0].effects = 0;
    }
    for (var_v0 = 0; var_v0 < 16; var_v0++) {
        D_8018EF18[var_v0].unk0 = 1.0f;
        D_8018EF18[var_v0].unk4 = 1.0f;
        D_8018EF18[var_v0].unk8 = 0;
        D_8018EF18[var_v0].unk9 = 0x40;
    }
    D_8018EFD8[0].prev = 0xFF;
    D_8018EFD8[0].next = 0xFF;
    for (var_v0 = 1; var_v0 < 49; var_v0++) {
        D_8018EFD8[var_v0].prev = var_v0 - 1;
        D_8018EFD8[var_v0].next = var_v0 + 1;
    }
    D_8018EFD8[var_v0].prev = var_v0 - 1;
    D_8018EFD8[var_v0].next = 0xFF;
}

void func_800C284C(u8 arg0, u8 arg1, u8 arg2, u16 arg3) {
    u8 var_v1;
    UNUSED s32 pad;

    if ((D_800EA1EC != 0) && (arg0 != 2)) {
        return;
    }

    func_800CBBB8(0x82000000 | (((u32) arg0 & 0xFF) << 0x10) | (((u32) arg1 & 0xFF) << 8), arg3);
    D_801930D0[arg0].unk_248 = arg1 | (arg2 << 8);
    if (D_801930D0[arg0].unk_000 != 1.0f) {
        func_800CBB88(0x41000000 | (((u32) arg0 & 0xFF) << 0x10), D_801930D0[arg0].unk_000);
    }
    D_801930D0[arg0].unk_028 = 0;
    D_801930D0[arg0].unk_018 = 0;
    D_801930D0[arg0].unk_014 = 0;

    for (var_v1 = 0; var_v1 < 16; var_v1++) {
        D_801930D0[arg0].unk_044[var_v1].unk_00 = 1.0f;
        D_801930D0[arg0].unk_044[var_v1].unk_0C = 0;
        D_801930D0[arg0].unk_044[var_v1].unk_10 = 1.0f;
        D_801930D0[arg0].unk_044[var_v1].unk_1C = 0;
    }

    D_801930D0[arg0].unk_244 = 0;
    D_801930D0[arg0].unk_246 = 0;
}

void func_800C29B4(u8 arg0, u16 arg1) {
    func_800CBBB8(((arg0 & 0xFF) << 0x10) | 0x83000000, arg1);
    D_801930D0[arg0].unk_248 = 0xFFFF;
}

void func_800C2A2C(u32 cmd) {
    f32 freqScaleTarget;
    u16 channelMaskDisable;
    u16 val;
    u16 fadeTimer;
    u8 priority;
    u8 ioPort;
    u8 i;
    u8 duration;
    u8 channelIndex;
    u8 found;
    u8 seqId;
    u8 subArgs;
    u8 op;
    u8 seqPlayerIndex;

    op = cmd >> 28;
    seqPlayerIndex = (cmd & 0xF000000) >> 24;

    switch (op) {
        case 0:
            seqId = cmd & 0xFF;
            subArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            func_800C284C(seqPlayerIndex, seqId, subArgs, fadeTimer);
            break;

        case 1:
            fadeTimer = (cmd & 0xFF0000) >> 13;
            func_800C29B4(seqPlayerIndex, fadeTimer);
            break;

        case 2:
            seqId = cmd & 0xFF;
            subArgs = (cmd & 0xFF00) >> 8;
            fadeTimer = (cmd & 0xFF0000) >> 13;
            priority = subArgs;

            for (i = 0; i < D_80192CC6[seqPlayerIndex]; i++) {
                if (D_80192CA8[seqPlayerIndex][i].thing0 == seqId) {
                    if (i == 0) {
                        func_800C284C(seqPlayerIndex, seqId, subArgs, fadeTimer);
                    }
                    return;
                }
            }

            found = D_80192CC6[seqPlayerIndex];
            for (i = 0; i < D_80192CC6[seqPlayerIndex]; i++) {
                if (priority >= D_80192CA8[seqPlayerIndex][i].thing1) {
                    found = i;
                    i = D_80192CC6[seqPlayerIndex];
                }
            }

            if ((found != D_80192CC6[seqPlayerIndex]) || (found == 0)) {
                if (D_80192CC6[seqPlayerIndex] < 5) {
                    D_80192CC6[seqPlayerIndex]++;
                }
                for (i = D_80192CC6[seqPlayerIndex] - 1; i != found; i--) {
                    D_80192CA8[seqPlayerIndex][i].thing1 = D_80192CA8[seqPlayerIndex][i - 1].thing1;
                    D_80192CA8[seqPlayerIndex][i].thing0 = D_80192CA8[seqPlayerIndex][i - 1].thing0;
                }

                D_80192CA8[seqPlayerIndex][found].thing1 = subArgs;
                D_80192CA8[seqPlayerIndex][found].thing0 = seqId;
            }
            if (found == 0) {
                func_800C284C(seqPlayerIndex, seqId, subArgs, fadeTimer);
            }
            break;

        case 3:
            fadeTimer = (cmd & 0xFF0000) >> 13;

            found = D_80192CC6[seqPlayerIndex];
            for (i = 0; i < D_80192CC6[seqPlayerIndex]; i++) {
                seqId = cmd & 0xFF;
                if (D_80192CA8[seqPlayerIndex][i].thing0 == seqId) {
                    found = i;
                    i = D_80192CC6[seqPlayerIndex];
                }
            }

            if (found != D_80192CC6[seqPlayerIndex]) {
                for (i = found; i < (D_80192CC6[seqPlayerIndex] - 1); i++) {
                    D_80192CA8[seqPlayerIndex][i].thing1 = D_80192CA8[seqPlayerIndex][i + 1].thing1;
                    D_80192CA8[seqPlayerIndex][i].thing0 = D_80192CA8[seqPlayerIndex][i + 1].thing0;
                }

                D_80192CC6[seqPlayerIndex]--;
            }
            if (found == 0) {
                func_800C29B4(seqPlayerIndex, fadeTimer);
                if (D_80192CC6[seqPlayerIndex] != 0) {
                    func_800C284C(seqPlayerIndex, D_80192CA8[seqPlayerIndex][0].thing0,
                                  D_80192CA8[seqPlayerIndex][0].thing1, fadeTimer);
                }
            }
            break;

        case 4:
            duration = (cmd & 0xFF0000) >> 15;
            val = cmd & 0xFF;

            if (duration == 0) {
                duration++;
            }

            D_801930D0[seqPlayerIndex].unk_004 = val / 127.0f;
            if (D_801930D0[seqPlayerIndex].unk_000 != D_801930D0[seqPlayerIndex].unk_004) {
                D_801930D0[seqPlayerIndex].unk_008 =
                    (D_801930D0[seqPlayerIndex].unk_000 - D_801930D0[seqPlayerIndex].unk_004) / duration;
                D_801930D0[seqPlayerIndex].unk_00C = duration;
            }
            break;

        case 5:
            duration = (cmd & 0xFF0000) >> 15;
            val = cmd & 0xFFFF;

            if (duration == 0) {
                duration++;
            }
            freqScaleTarget = (f32) val / 1000.0f;
            for (i = 0; i < 0x10; i++) {
                D_801930D0[seqPlayerIndex].unk_044[i].unk_14 = freqScaleTarget;
                D_801930D0[seqPlayerIndex].unk_044[i].unk_1C = duration;
                D_801930D0[seqPlayerIndex].unk_044[i].unk_18 =
                    (D_801930D0[seqPlayerIndex].unk_044[i].unk_10 - freqScaleTarget) / duration;
            }

            D_801930D0[seqPlayerIndex].unk_244 = 0xFFFF;
            break;

        case 6:
            duration = (cmd & 0xFF0000) >> 15;
            channelIndex = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;

            if (duration == 0) {
                duration++;
            }

            D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_04 = val / 127.0f;
            if (D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_00 !=
                D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_04) {
                D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_08 =
                    (D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_00 -
                     D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_04) /
                    duration;
                D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_0C = duration;
                D_801930D0[seqPlayerIndex].unk_244 |= 1 << channelIndex;
            }
            break;

        case 7:
            ioPort = (cmd & 0xFF0000) >> 16;
            val = cmd & 0xFF;
            func_800CBBE8((0x46000000 | ((seqPlayerIndex & 0xFF) << 0x10)) | ((ioPort & 0xFF) << 8), val);
            break;

        case 8:
            channelIndex = (cmd & 0xF00) >> 8;
            ioPort = (cmd & 0xFF0000) >> 16;
            val = cmd & 0xFF;
            if (!(D_801930D0[seqPlayerIndex].unk_24A & (1 << channelIndex))) {
                func_800CBBE8(((0x06000000 | ((seqPlayerIndex & 0xFF) << 0x10)) | (((u32) channelIndex & 0xFF) << 8)) |
                                  (ioPort & 0xFF),
                              val);
            }
            break;

        case 9:
            D_801930D0[seqPlayerIndex].unk_24A = cmd & 0xFFFF;
            break;

        case 10:
            val = 1;
            channelMaskDisable = cmd & 0xFFFF;
            for (i = 0; i < 0x10; i++) {
                func_800CBBE8(0x08000000 | ((seqPlayerIndex & 0xFF) << 0x10) | (((u32) i & 0xFF) << 8),
                              (channelMaskDisable & val) ? 1 : 0);
                val <<= 1;
            }

            break;

        case 11:
            D_801930D0[seqPlayerIndex].unk_014 = cmd;
            break;

        case 12:
            subArgs = (cmd & 0xF00000) >> 20;
            if (subArgs != 0xF) {
                found = D_801930D0[seqPlayerIndex].unk_041++;
                if (found < 5) {
                    D_801930D0[seqPlayerIndex].unk_02C[found] = cmd;
                    D_801930D0[seqPlayerIndex].unk_040 = 2;
                }
            } else {
                D_801930D0[seqPlayerIndex].unk_041 = 0;
            }
            break;

        case 14:
            subArgs = (cmd & 0xF00) >> 8;
            val = cmd & 0xFF;
            switch (subArgs) {
                case 0:
                    func_800CBBB8(0xF0000000, D_800EA1F0[val]);
                    break;

                case 1:
                    D_800EA1EC = val & 1;
                    break;
            }

            break;

        case 15:
            seqId = cmd & 0xFF;
            subArgs = (cmd & 0xFF00) >> 8;
            D_800EA1C0 = subArgs;
            //audio_reset_session_eu((void*) seqId);
            D_800EA1F4[0] = seqId;
            func_800CBBE8(0x46020000, subArgs);
            func_800C5C40();
            break;

        default:
            break;
    }
}

void func_800C3448(u32 arg0) {
    D_80192CD0[D_800EA1E4] = arg0;
    D_800EA1E4 += 1;
}

void func_800C3478(void) {
    for (D_800EA1E8; D_800EA1E4 != D_800EA1E8;) {
        func_800C2A2C(D_80192CD0[D_800EA1E8++]);
    }
}

u16 func_800C3508(u8 player) {
    if (!gSequencePlayers[player].enabled) {
        return -1;
    }
    return D_801930D0[player].unk_248;
}

u8 func_800C357C(s32 arg0) {
    bool var_v1;
    u8 i;

    i = D_800EA1E8;
    var_v1 = true;
    for (i = D_800EA1E8; i < (s32) D_800EA1E4; i++) {
        if ((u32) arg0 == D_80192CD0[i]) {
            var_v1 = false;
            i = D_800EA1E4;
        }
    }
    return var_v1;
}

#ifdef VERSION_EU
u8 func_800C357C_eu(s32 arg0, s32 arg1) {
    u8 var_v1;
    u8 i;

    i = D_800EA1E8;
    var_v1 = 1;
    for (i = D_800EA1E8; i < (s32) D_800EA1E4; i++) {
        if (arg0 == (D_80192CD0[i] & arg1)) {
            var_v1 = 0;
            i = D_800EA1E4;
        }
    }
    return var_v1;
}
#endif

void func_800C35E8(u8 arg0) {
    D_80192CC6[arg0] = 0;
}

void func_800C3608(u8 arg0, u8 arg1) {
    u8 var_v0;
    u8 thing;

    for (var_v0 = 0; var_v0 < D_801930D0[arg0].unk_041; var_v0++) {
        thing = (D_801930D0[arg0].unk_02C[var_v0] & 0xF00000) >> 0x14;
        if (thing == arg1) {
            D_801930D0[arg0].unk_02C[var_v0] = 0xFF000000;
        }
    }
}

void func_800C36C4(u8 arg0, u8 arg1, u8 arg2, u8 arg3) {
    D_801930D0[arg0].unk_00E[arg1] = arg2;
    D_801930D0[arg0].unk_011 = arg3;
    D_801930D0[arg0].unk_012 = 1;
}

void func_800C3724(void) {
    u8 seqPlayerIndex;
    f32 volume;
    u8 tempoOp;
    u16 tempoTarget;
    u8 channelIndex;
    u8 j;
    u32 tempoCmd;
    u16 tempoPrev;
    u8 tempoTimer;
    u8 setupOp;
    u8 targetSeqPlayerIndex;
    u8 setupVal1;
    u8 setupVal2;
    u16 seqId;

    for (seqPlayerIndex = 0; seqPlayerIndex < 3; seqPlayerIndex++) {
        if (D_801930D0[seqPlayerIndex].unk_012) {
            volume = 1.0f;
            for (j = 0; j < 3; j++) {
                volume *= D_801930D0[seqPlayerIndex].unk_00E[j] / 127.0f;
            }

            func_800C3448(0x40000000 | (((u8) seqPlayerIndex) << 0x18) |
                          (((u8) D_801930D0[seqPlayerIndex].unk_011) << 0x10) | ((u16) (u8) (volume * 127.0f)));

            D_801930D0[seqPlayerIndex].unk_012 = 0;
        }
        if (D_801930D0[seqPlayerIndex].unk_00C != 0) {
            D_801930D0[seqPlayerIndex].unk_00C--;
            if (D_801930D0[seqPlayerIndex].unk_00C) {
                D_801930D0[seqPlayerIndex].unk_000 -= D_801930D0[seqPlayerIndex].unk_008;
            } else {
                D_801930D0[seqPlayerIndex].unk_000 = D_801930D0[seqPlayerIndex].unk_004;
            }
            func_800CBB88(0x41000000 | (((u32) seqPlayerIndex & 0xFF) << 0x10), D_801930D0[seqPlayerIndex].unk_000);
        }
        if (D_801930D0[seqPlayerIndex].unk_014 != 0) {
            tempoCmd = D_801930D0[seqPlayerIndex].unk_014;
            tempoTimer = (tempoCmd & 0xFF0000) >> 0xF;
            tempoTarget = tempoCmd & 0xFFF;
            if (tempoTimer == 0) {
                tempoTimer++;
            }
            if (gSequencePlayers[seqPlayerIndex].enabled != 0) {
                tempoPrev = gSequencePlayers[seqPlayerIndex].tempo / 48;
                tempoOp = (tempoCmd & 0xF000) >> 0xC;
                switch (tempoOp) {
                    case 1:
                        tempoTarget += tempoPrev;
                        break;

                    case 2:
                        if (tempoTarget < tempoPrev) {
                            tempoTarget = tempoPrev - tempoTarget;
                        }
                        break;

                    case 3:
                        tempoTarget = tempoPrev * (tempoTarget / 100.0f);
                        break;

                    case 4:
                        tempoTarget =
                            (D_801930D0[seqPlayerIndex].unk_018 != 0) ? D_801930D0[seqPlayerIndex].unk_018 : tempoPrev;
                        break;
                    default:
                        break;
                }

                if (tempoTarget > 300) {
                    tempoTarget = 300;
                }
                if (D_801930D0[seqPlayerIndex].unk_018 == 0) {
                    D_801930D0[seqPlayerIndex].unk_018 = tempoPrev;
                }
                D_801930D0[seqPlayerIndex].unk_020 = tempoTarget;
                D_801930D0[seqPlayerIndex].unk_01C = gSequencePlayers[seqPlayerIndex].tempo / 48;
                D_801930D0[seqPlayerIndex].unk_024 =
                    (D_801930D0[seqPlayerIndex].unk_01C - D_801930D0[seqPlayerIndex].unk_020) / tempoTimer;
                D_801930D0[seqPlayerIndex].unk_028 = tempoTimer;
            }
            D_801930D0[seqPlayerIndex].unk_014 = 0;
        }
        if (D_801930D0[seqPlayerIndex].unk_028 != 0) {
            D_801930D0[seqPlayerIndex].unk_028--;
            if (D_801930D0[seqPlayerIndex].unk_028) {
                D_801930D0[seqPlayerIndex].unk_01C -= D_801930D0[seqPlayerIndex].unk_024;
            } else {
                D_801930D0[seqPlayerIndex].unk_01C = D_801930D0[seqPlayerIndex].unk_020;
            }
            func_800CBBB8(0x47000000 | (((u32) seqPlayerIndex & 0xFF) << 0x10),
                          (s32) D_801930D0[seqPlayerIndex].unk_01C);
        }

        if (D_801930D0[seqPlayerIndex].unk_246 != 0) {
            for (channelIndex = 0; channelIndex < 0x10; channelIndex++) {
                if (D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_0C != 0) {
                    D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_0C--;
                    if (D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_0C) {
                        D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_00 -=
                            D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_08;
                    } else {
                        D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_00 =
                            D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_04;
                        D_801930D0[seqPlayerIndex].unk_246 ^= 1 << channelIndex;
                    }
                    func_800CBB88(0x01000000 | ((seqPlayerIndex & 0xFF) << 0x10) | (((u32) channelIndex & 0xFF) << 8),
                                  D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_00);
                }
            }
        }
        if (D_801930D0[seqPlayerIndex].unk_244 != 0) {
            for (channelIndex = 0; channelIndex < 0x10; channelIndex++) {
                if (D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_1C != 0) {
                    D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_1C--;
                    if (D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_1C) {
                        D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_10 -=
                            D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_18;
                    } else {
                        D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_10 =
                            D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_14;
                        D_801930D0[seqPlayerIndex].unk_244 ^= 1 << channelIndex;
                    }
                    func_800CBB88(0x04000000 | ((seqPlayerIndex & 0xFF) << 0x10) | (((u32) channelIndex & 0xFF) << 8),
                                  D_801930D0[seqPlayerIndex].unk_044[channelIndex].unk_10);
                }
            }
        }
        if (D_801930D0[seqPlayerIndex].unk_041) {
#ifdef VERSION_EU
            if (func_800C357C_eu(-0x10000000, -0x10000000) == 0) {
                D_801930D0[seqPlayerIndex].unk_041 = 0;
                return;
            }
#endif
            if (D_801930D0[seqPlayerIndex].unk_040 != 0) {
                D_801930D0[seqPlayerIndex].unk_040--;
                continue;
            }

            if (gSequencePlayers[seqPlayerIndex].enabled != 0) {
                continue;
            }

            for (j = 0; j < D_801930D0[seqPlayerIndex].unk_041; j++) {
                setupOp = (D_801930D0[seqPlayerIndex].unk_02C[j] & 0xF00000) >> 0x14;
                targetSeqPlayerIndex = (D_801930D0[seqPlayerIndex].unk_02C[j] & 0xF0000) >> 0x10;
                setupVal2 = (D_801930D0[seqPlayerIndex].unk_02C[j] & 0xFF00) >> 8;
                setupVal1 = D_801930D0[seqPlayerIndex].unk_02C[j] & 0xFF;
                switch (setupOp) {
                    case 0:
                        D_801930D0[targetSeqPlayerIndex].unk_012 = 1;
                        D_801930D0[targetSeqPlayerIndex].unk_00E[1] = 0x7F;
                        break;

                    case 1:
                        func_800C3448(0x30000000 | ((u8) seqPlayerIndex) << 0x18 |
                                      (D_801930D0[seqPlayerIndex].unk_248));
                        break;

                    case 2:
                        func_800C3448((((u8) targetSeqPlayerIndex) << 0x18) | 0x10000 |
                                      (u16) (D_801930D0[targetSeqPlayerIndex].unk_248));
                        D_801930D0[targetSeqPlayerIndex].unk_012 = 1;
                        D_801930D0[targetSeqPlayerIndex].unk_00E[1] = 0x7F;
                        break;

                    case 3:
                        func_800C3448(0xB0003000 | (((u8) targetSeqPlayerIndex) << 0x18) | (((u8) setupVal2) << 0x10) |
                                      (u16) setupVal1);
                        break;

                    case 4:
                        func_800C3448(0xB0004000 | (((u8) targetSeqPlayerIndex) << 0x18) | (((u8) setupVal1) << 0x10));
                        break;

                    case 5:
                        seqId = D_801930D0[seqPlayerIndex].unk_02C[j] & 0xFFFF;
                        func_800C3448((((u8) targetSeqPlayerIndex) << 0x18) |
                                      (((u8) D_801930D0[targetSeqPlayerIndex].unk_042) << 0x10) | ((u16) seqId));

                        func_800C36C4(targetSeqPlayerIndex, 1, 0x7F, 0);
                        D_801930D0[targetSeqPlayerIndex].unk_042 = 0;
                        break;

                    case 6:
                        D_801930D0[seqPlayerIndex].unk_042 = setupVal2;
                        break;
                }
            }

            D_801930D0[seqPlayerIndex].unk_041 = 0;
        }
    }
}

void func_800C3F70(void) {
    u8 var_v0;

    for (var_v0 = 0; var_v0 < 3; var_v0++) {
        D_80192CC6[var_v0] = 0;
        D_801930D0[var_v0].unk_248 = 0xFFFF;
        D_801930D0[var_v0].unk_000 = 1.0f;
        D_801930D0[var_v0].unk_00C = 0;
        D_801930D0[var_v0].unk_028 = 0;
        D_801930D0[var_v0].unk_018 = 0;
        D_801930D0[var_v0].unk_014 = 0;
        D_801930D0[var_v0].unk_24A = 0;
        D_801930D0[var_v0].unk_041 = 0;
        D_801930D0[var_v0].unk_042 = 0;
        D_801930D0[var_v0].unk_012 = 0;
        D_801930D0[var_v0].unk_00E[0] = 0x7F;
        D_801930D0[var_v0].unk_00E[1] = 0x7F;
        D_801930D0[var_v0].unk_00E[2] = 0x7F;
        D_801930D0[var_v0].unk_244 = 0;
        D_801930D0[var_v0].unk_246 = 0;
    }
}

void func_800C400C(void) {
    u8 var_v0;

    for (var_v0 = 0; var_v0 < 3; var_v0++) {
        D_80192CC6[var_v0] = 0;
        D_801930D0[var_v0].unk_248 = 0xFFFF;
        D_801930D0[var_v0].unk_028 = 0;
        D_801930D0[var_v0].unk_018 = 0;
        D_801930D0[var_v0].unk_014 = 0;
        D_801930D0[var_v0].unk_24A = 0;
        D_801930D0[var_v0].unk_041 = 0;
        D_801930D0[var_v0].unk_042 = 0;
        D_801930D0[var_v0].unk_244 = 0;
        D_801930D0[var_v0].unk_246 = 0;
    }
}

// Appears to be an unused combo of sound_banks_enable and sound_banks_disable
void func_800C4084(u16 bankMask) {
    u8 bank;

    for (bank = 0; bank < SOUND_BANK_COUNT; bank++) {
        if (bankMask & 1) {
            sSoundBankDisabled[bank] = true;
        } else {
            sSoundBankDisabled[bank] = false;
        }
        bankMask = bankMask >> 1;
    }
}

void func_800C40F0(u8 arg0) {
    D_800EA1C4 &= ((1 << (arg0)) ^ (u16) -1);
    if (!D_800EA1C4) {
        D_801930D0[0].unk_012 = 1;
        D_801930D0[0].unk_00E[2] = 0x7F;
    }
}

void play_sound(u32 soundBits, Vec3f* position, u8 cameraId, f32* arg3, f32* arg4, s8* arg5) {
    u8 bank;
    struct Sound* temp_v0;

    bank = soundBits >> 0x1C;
    if (sSoundBankDisabled[bank] == false) {
        temp_v0 = &sSoundRequests[sSoundRequestCount];
        temp_v0->soundBits = soundBits;
        temp_v0->position = position;
        temp_v0->cameraId = cameraId;
        temp_v0->unk0C = arg3;
        temp_v0->unk10 = arg4;
        temp_v0->unk14 = arg5;
        sSoundRequestCount += 1;
    }
}

void func_800C41CC(u8 arg0, struct SoundCharacteristics* arg1) {
    s32 found;
    u8 soundId;
    struct Sound* sound;

    for (soundId = sNumProcessedSoundRequests; soundId != sSoundRequestCount; soundId++) {
        found = false;
        sound = &sSoundRequests[soundId];
        switch (arg0) {
            case 0:
                if ((sound->soundBits & 0xF0000000) == (arg1->soundBits & 0xF0000000)) {
                    found = true;
                }
                break;
            case 1:
                if (((sound->soundBits & 0xF0000000) == (arg1->soundBits & 0xF0000000)) &&
                    (sound->position == arg1->unk00)) {
                    found = true;
                }
                break;
            case 2:
                if (sound->position == arg1->unk00) {
                    found = true;
                }
                break;
            case 3:
                if ((sound->position == arg1->unk00) && (sound->soundBits == arg1->soundBits)) {
                    found = true;
                }
                break;
            case 4:
                if ((sound->cameraId == arg1->cameraId) && (sound->soundBits == arg1->soundBits)) {
                    found = true;
                }
                break;
            case 5:
                if (sound->soundBits == arg1->soundBits) {
                    found = true;
                }
                break;
        }
        if (found) {
            sound->soundBits = 0;
            if (*sound->position != D_800EA1C8) {
                (*sound->position)[1] = 100000.0f;
            }
        }
    }
}

// Probably a variant of `process_sound_request` from SM64
#ifdef NON_MATCHING
// https://decomp.me/scratch/fmmyb
// Stack issue concerning `var_t2` and `var_t3`, no idea how to fix it
void func_800C4398(void) {
    u8 bank;
    u8 soundIndex;
    u8 var_a3;
    struct Sound* var_a2;
    u8 var_t2 = 0;
    u32 var_t3;

    var_a2 = &sSoundRequests[sNumProcessedSoundRequests];
    if (var_a2->soundBits == 0)
        return;

    bank = ((u32) (var_a2->soundBits & 0xF0000000) >> 0x1C);
    soundIndex = sSoundBanks[bank][0].next;
    var_a3 = 0;
    while ((soundIndex != 0xFF) && (soundIndex != 0)) {
        if (var_a2->position == sSoundBanks[bank][soundIndex].unk00) {
            if (var_a2->soundBits == sSoundBanks[bank][soundIndex].soundBits) {
                var_a3 = D_800EA1A0[D_800EA1C0][bank];
            } else {
                if (var_a3 == 0) {
                    var_t2 = soundIndex;
                    var_t3 = sSoundBanks[bank][soundIndex].soundBits;
                } else if ((u32) (sSoundBanks[bank][soundIndex].soundBits & 0xFF00) < (u32) (var_t3 & 0xFF00)) {
                    var_t2 = soundIndex;
                    var_t3 = sSoundBanks[bank][soundIndex].soundBits;
                }
                var_a3++;
                if (var_a3 == D_800EA1A0[D_800EA1C0][bank]) {
                    if ((u32) (var_a2->soundBits & 0xFF00) >= (u32) (var_t3 & 0xFF00)) {
                        soundIndex = var_t2;
                    } else {
                        soundIndex = 0;
                    }
                }
            }
            if (var_a3 == D_800EA1A0[D_800EA1C0][bank]) {
                if ((var_a2->soundBits & 0x08000000) || (var_a2->soundBits & 0x40000) || (soundIndex == var_t2)) {
                    if ((sSoundBanks[bank][soundIndex].soundBits & 0x80000) &&
                        (sSoundBanks[bank][soundIndex].soundStatus != 1)) {
                        func_800C40F0(sSoundBanks[bank][soundIndex].unk2C);
                    }
                    sSoundBanks[bank][soundIndex].cameraId = var_a2->cameraId;
                    sSoundBanks[bank][soundIndex].soundBits = var_a2->soundBits;
                    sSoundBanks[bank][soundIndex].soundStatus = ((var_a2->soundBits & 0x01000000) >> 0x18);
                    sSoundBanks[bank][soundIndex].freshness = 2;
                    sSoundBanks[bank][soundIndex].unk10 = var_a2->unk0C;
                    sSoundBanks[bank][soundIndex].unk14 = var_a2->unk10;
                    sSoundBanks[bank][soundIndex].unk18 = var_a2->unk14;
                }
                soundIndex = 0;
            }
        }
        if (soundIndex != 0) {
            soundIndex = sSoundBanks[bank][soundIndex].next;
        }
    }
    if ((sSoundBanks[bank][sSoundBankFreeListFront[bank]].next != 0xFF) && (soundIndex != 0)) {
        soundIndex = sSoundBankFreeListFront[bank];
        sSoundBanks[bank][soundIndex].unk00 = &(*var_a2->position)[0];
        sSoundBanks[bank][soundIndex].unk04 = &(*var_a2->position)[1];
        sSoundBanks[bank][soundIndex].unk08 = &(*var_a2->position)[2];
        sSoundBanks[bank][soundIndex].cameraId = var_a2->cameraId;
        sSoundBanks[bank][soundIndex].unk10 = var_a2->unk0C;
        sSoundBanks[bank][soundIndex].unk14 = var_a2->unk10;
        sSoundBanks[bank][0, soundIndex].unk18 = var_a2->unk14;
        sSoundBanks[bank][soundIndex].soundBits = var_a2->soundBits;
        sSoundBanks[bank][soundIndex].soundStatus = (u8) ((u32) (var_a2->soundBits & 0x01000000) >> 0x18);
        sSoundBanks[bank][soundIndex].freshness = 2;
        sSoundBanks[bank][soundIndex].prev = sSoundBankUsedListBack[bank];
        sSoundBanks[bank][sSoundBankUsedListBack[bank]].next = sSoundBankFreeListFront[bank];
        sSoundBankUsedListBack[bank] = sSoundBankFreeListFront[bank];
        sSoundBankFreeListFront[bank] = sSoundBanks[bank][sSoundBankFreeListFront[bank]].next;
        sSoundBanks[bank][sSoundBankFreeListFront[bank]].prev = 0xFF;
        sSoundBanks[bank][soundIndex].next = 0xFF;
    } else if (sSoundBanks[bank][sSoundBankFreeListFront[bank]].next == 0xFF) {
        if (D_800EA1C8 != *var_a2->position) {
            (*var_a2->position)[1] = 100000.0f;
        }
    }
}
#else
GLOBAL_ASM("asm/non_matchings/audio/external/func_800C4398.s")
#endif

void delete_sound_from_bank(u8 bankId, u8 soundId) {
    UNUSED s32 stackPadding;
    struct SoundCharacteristics* temp = &sSoundBanks[bankId][soundId];
    if (*temp->unk00 != D_800EA1C8) {
        *temp->unk04 = 100000.0f;
    }
    if (temp->soundBits & 0x80000) {
        func_800C40F0(temp->unk2C);
    }
    if (soundId == sSoundBankUsedListBack[bankId]) {
        sSoundBankUsedListBack[bankId] = temp->prev;
    } else {
        sSoundBanks[bankId][temp->next].prev = temp->prev;
    }
    sSoundBanks[bankId][temp->prev].next = temp->next;
    temp->next = sSoundBankFreeListFront[bankId];
    temp->prev = 0xFF;
    sSoundBanks[bankId][sSoundBankFreeListFront[bankId]].prev = soundId;
    sSoundBankFreeListFront[bankId] = soundId;
    temp->soundStatus = 0;
}

// Guessing this is `select_current_sounds`?
#ifdef NON_MATCHING
// https://decomp.me/scratch/cqBii
struct why {
    u32 priority;
    u8 soundIndex;
};
void func_800C4888(u8 bankId) {
    u8 i;
    u8 j;
    u8 k;
    u8 latestSoundIndex;
    u8 var_s2;
    u8 var_v1;
    u8 soundIndex;
    u8 var_t1;
    u8 someTemp;
    f32 var_f0;
    s32 requestedPriority;
    struct why sp7C[8];
    s32 temp_t8;
    struct SoundCharacteristics* temp_t2;

    var_s2 = 0;
    for (i = 0; i < 8; i++) {
        sp7C[i].priority = 0x7FFFFFFF;
        sp7C[i].soundIndex = 0xFF;
    }
    latestSoundIndex = 0;
    soundIndex = sSoundBanks[bankId][0].next;
    while (soundIndex != 0xFF) {
        temp_t2 = &sSoundBanks[bankId][soundIndex];
        if ((temp_t2->soundStatus == 1) && ((temp_t2->soundBits & 0x08000000) == 0x08000000)) {
            temp_t2->freshness -= 1;
        }
        if (temp_t2->freshness == 0) {
            delete_sound_from_bank(bankId, soundIndex);
        } else if (temp_t2->soundStatus != 0) {
            if (D_800EA1C8 == *temp_t2->unk00) {
                temp_t2->distance = 0.0f;
            } else {
                temp_t2->distance = ((*temp_t2->unk00)[0] * (*temp_t2->unk00)[0]) + (*temp_t2->unk08 * *temp_t2->unk08);
            }
            requestedPriority = ((u32) (temp_t2->soundBits & 0xFF00) >> 8) & 0xFF;
            if (temp_t2->soundBits & 0x100000) {
                temp_t2->priority =
                    (0xFF - requestedPriority) * (0xFF - requestedPriority) * 0x5A4; // 0x5a4 = 1444 = 38 * 38
            } else {
                temp_t2->priority = (u32) temp_t2->distance + ((0xFF - requestedPriority) * (0xFF - requestedPriority) *
                                                               0x5A4); // 0x5a4 = 1444 = 38 * 38
                if (*temp_t2->unk08 > 0.0f) {
                    temp_t2->priority += (s32) (*temp_t2->unk08 * 6.0f);
                }
            }
            temp_t8 = ((u32) (temp_t2->soundBits & 0x30000) >> 0x10) & 0xFF;
            if (temp_t8 != 0) {
                var_f0 = 4000000.0f / (f32) (temp_t8 * temp_t8);
            } else {
                var_f0 = 1e10f;
            }
            if (var_f0 < temp_t2->distance) {
                if (temp_t2->soundStatus == 4) {
                    func_800CBBE8((temp_t2->unk2C << 8) | 0x06020000, 0);
                    if (temp_t2->soundBits & 0x08000000) {
                        delete_sound_from_bank(bankId, soundIndex);
                        soundIndex = latestSoundIndex;
                    }
                }
            } else {
                var_t1 = D_800EA188[D_800EA1C0][bankId];
                for (i = 0; i < var_t1; i++) {
                    if (sp7C[i].priority >= temp_t2->priority) {
                        if (var_s2 < var_t1) {
                            var_s2++;
                        }
                        for (j = var_t1 - 1; j > i; j--) {
                            sp7C[j].priority = sp7C[j - 1].priority;
                            sp7C[j].soundIndex = sp7C[j - 1].soundIndex;
                        }
                        sp7C[i].priority = temp_t2->priority;
                        sp7C[i].soundIndex = soundIndex;
                        i = var_t1;
                    }
                }
            }
            latestSoundIndex = soundIndex;
        }
        soundIndex = sSoundBanks[bankId][latestSoundIndex].next;
    }
    var_t1 = D_800EA188[D_800EA1C0][bankId];
    for (i = 0; i < var_s2; i++) {
        someTemp = sSoundBanks[bankId][sp7C[i].soundIndex].soundStatus;
        if (someTemp == 1) {
            sSoundBanks[bankId][sp7C[i].soundIndex].soundStatus = 2;
        } else if (someTemp == 4) {
            sSoundBanks[bankId][sp7C[i].soundIndex].soundStatus = 3;
        }
    }
    for (i = 0; i < var_t1; i++) {
        var_v1 = 0;
        soundIndex = D_80192AB8[bankId][i][4];
        if (soundIndex == 0xFF) {
            var_v1 = 1;
        } else {
            temp_t2 = &sSoundBanks[bankId][soundIndex];
            if (temp_t2->soundStatus == 4) {
                if (temp_t2->soundBits & 0x08000000) {
                    delete_sound_from_bank(bankId, soundIndex);
                    var_v1 = 1;
                } else {
                    temp_t2->soundStatus = 1;
                    var_v1 = 1;
                }
            } else if (temp_t2->soundStatus == 0) {
                D_80192AB8[bankId][i][4] = 0xFF;
                var_v1 = 1;
            } else {
                for (j = 0; j < var_t1; j++) {
                    if (soundIndex == sp7C[j].soundIndex) {
                        sp7C[j].soundIndex = 0xFF;
                        j = var_t1;
                    }
                }
            }
        }
        var_s2--;
        if (var_v1 == 1) {
            for (j = 0; j < var_t1; j++) {
                soundIndex = sp7C[j].soundIndex;
                if ((soundIndex != 0xFF) && (sSoundBanks[bankId][soundIndex].soundStatus != 3)) {
                    for (k = 0; k < var_t1; k++) {
                        if (soundIndex == D_80192AB8[bankId][k][4]) {
                            var_v1 = 0;
                            k = var_t1;
                        }
                    }
                    if (var_v1 == 1) {
                        D_80192AB8[bankId][i][4] = soundIndex;
                        sp7C[j].soundIndex = 0xFF;
                        j = var_t1 + 1;
                        var_s2--;
                    }
                }
            }
            if (j == var_t1) {
                D_80192AB8[bankId][i][4] = 0xFF;
            }
        }
    }
}
#else
GLOBAL_ASM("asm/non_matchings/audio/external/func_800C4888.s")
#endif

#ifdef NON_MATCHING
// https://decomp.me/scratch/Lm9EX
/* ~*~*~* UwU Resgister allocation OwO *~*~*~ */
void func_800C4FE4(u8 bankId) {
    u8 soundId;
    u8 var_s4;
    UNUSED u32 cmd;
    struct SoundCharacteristics* temp_s0;
    struct SequenceChannel* thing;

    for (var_s4 = 0; var_s4 < D_800EA188[D_800EA1C0][bankId]; var_s4++) {
        soundId = D_80192AB8[bankId][var_s4][4];
        if (soundId != 0xFF) {
            temp_s0 = &sSoundBanks[bankId][soundId];
            thing = gSequencePlayers[2].channels[D_80192C38];
            if (temp_s0->soundStatus == 2) {
                temp_s0->unk2C = D_80192C38;
                if (temp_s0->soundBits & 0x80000) {
                    D_800EA1C4 |= 1 << D_80192C38;
                    D_801930D0->unk_012 = 1;
                    D_801930D0->unk_00E[2] = 0x28;
                }
                func_800C19D0(bankId, soundId, D_80192C38);
                func_800CBBE8((D_80192C38 << 8) | 0x06020000, 1);
                func_800CBBE8((D_80192C38 << 8) | 0x06020000 | 4, temp_s0->soundBits & 0xFF);
                temp_s0->soundStatus = 4U;
            } else if (((u8) thing->soundScriptIO[7]) == 0x80) {
                func_800CBBE8(((D_80192C38 & 0xFF) << 8) | 0x06020000 | 7, 0);
                delete_sound_from_bank(bankId, soundId);
            } else if (temp_s0->soundStatus == 3) {
                func_800C19D0(bankId, soundId, D_80192C38);
                temp_s0->soundStatus = 4U;
            }
        }
        D_80192C38 += 1;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/audio/external/func_800C4FE4.s")
#endif

// Seems somewhat similar to certain parts of `select_current_sounds` from SM64
void func_800C5278(u8 bankId) {
    UNUSED s32 stackPadding0;
    u8 soundId;
    struct SoundCharacteristics sp60;

    soundId = sSoundBanks[bankId][0].next;
    while (soundId != 0xFF) {
        if (sSoundBanks[bankId][soundId].soundStatus >= 3) {
            func_800CBBE8(((sSoundBanks[bankId][soundId].unk2C & 0xff) << 8) | 0x06020000, 0);
        }
        if (sSoundBanks[bankId][soundId].soundStatus != 0) {
            delete_sound_from_bank(bankId, soundId);
        }
        // This feels wrong, I think it only works because `delete_sound_from_bank`
        // modifies `sSoundBanks[bankId][0].next`, which allows us to eventually(?)
        // break out of the loop
        soundId = sSoundBanks[bankId][0].next;
    }
    sp60.soundBits = bankId << 0x1C;
    func_800C41CC(0, &sp60);
}

void func_800C5384(u8 arg0, Vec3f* arg1) {
    u8 curr;
    u8 next;

    curr = 0;
    next = sSoundBanks[arg0][0].next;

    while (next != 0xff) {
        if (*arg1 == *sSoundBanks[arg0][next].unk00) {
            if (sSoundBanks[arg0][next].soundStatus >= 3) {
                func_800CBBE8((0x06020000 | ((sSoundBanks[arg0][next].unk2C & 0xff) << 8)), 0);
            }

            if (sSoundBanks[arg0][next].soundStatus != 0) {
                delete_sound_from_bank(arg0, next);
            }
        } else {
            curr = next;
        }

        next = sSoundBanks[arg0][curr].next;
    }
}

void func_800C54B8(u8 arg0, Vec3f* arg1) {
    struct SoundCharacteristics unk;
    func_800C5384(arg0, arg1);
    unk.soundBits = arg0 << 0x1C;
    unk.unk00 = arg1;
    func_800C41CC(1, &unk);
}

void func_800C550C(Vec3f* arg0) {
    u8 i;
    struct SoundCharacteristics sp3C;

    for (i = 0; i < 6; i++) {
        func_800C5384(i, arg0);
    }

    sp3C.unk00 = arg0;
    func_800C41CC(2, &sp3C);
}

void func_800C5578(Vec3f* arg0, u32 soundBits) {
    UNUSED s32 stackPadding0;
    u8 bankId;
    u8 next;
    u8 curr;
    struct SoundCharacteristics sp60;

    bankId = (soundBits & 0xF0000000) >> 0x1C;
    next = sSoundBanks[bankId][0].next;
    curr = 0;
    while (next != 0xFF) {
        if ((arg0 == sSoundBanks[bankId][next].unk00) && (soundBits == sSoundBanks[bankId][next].soundBits)) {
            if (sSoundBanks[bankId][next].soundStatus >= 3) {
                func_800CBBE8(((sSoundBanks[bankId][next].unk2C & 0xff) << 8) | 0x06020000, 0);
            }
            if (sSoundBanks[bankId][next].soundStatus != 0) {
                delete_sound_from_bank(bankId, next);
            }
            next = 0xFF;
        } else {
            curr = next;
        }
        if (next != 0xFF) {
            next = sSoundBanks[bankId][curr].next;
        }
    }
    sp60.unk00 = arg0;
    sp60.soundBits = soundBits;
    func_800C41CC(3, &sp60);
}

void func_800C56F0(u32 soundBits) {
    UNUSED s32 stackPadding0;
    u8 bankId;
    u8 next;
    u8 curr;
    struct SoundCharacteristics sp68;

    bankId = (soundBits & 0xF0000000) >> 0x1C;
    next = sSoundBanks[bankId][0].next;
    curr = 0;
    while (next != 0xFF) {
        if (soundBits == sSoundBanks[bankId][next].soundBits) {
            if (sSoundBanks[bankId][next].soundStatus >= 3) {
                func_800CBBE8(((sSoundBanks[bankId][next].unk2C & 0xff) << 8) | 0x06020000, 0);
            }
            if (sSoundBanks[bankId][next].soundStatus != 0) {
                delete_sound_from_bank(bankId, next);
            }
        } else {
            curr = next;
        }
        next = sSoundBanks[bankId][curr].next;
    }
    sp68.soundBits = soundBits;
    func_800C41CC(5, &sp68);
}

// Might be process_all_sound_requests
void func_800C5848(void) {
    while (sSoundRequestCount != sNumProcessedSoundRequests) {
        func_800C4398();
        sNumProcessedSoundRequests++;
    }
}

// EXTREMELY similar to `fade_channel_volume_scale` from SM64
void fade_channel_volume_scale(u8 player, u8 targetScale, u16 fadeDuration) {
    struct ChannelVolumeScaleFade* temp_v0;

    if (fadeDuration == 0) {
        fadeDuration++;
    }
    temp_v0 = &D_80192C48[player];
    temp_v0->target = targetScale / US_FLOAT(127.0);
    temp_v0->remainingFrames = fadeDuration;
    temp_v0->velocity = (temp_v0->current - temp_v0->target) / fadeDuration;
}

void func_800C5968(u8 arg0) {
    struct ChannelVolumeScaleFade* temp_v0;

    temp_v0 = &D_80192C48[arg0];
    if (temp_v0->remainingFrames != 0) {
        temp_v0->remainingFrames--;
        if (temp_v0->remainingFrames != 0) {
            temp_v0->current -= temp_v0->velocity;
        } else {
            temp_v0->current = temp_v0->target;
        }
    }
}

void func_800C59C4(void) {
    u8 i;

    if (gSequencePlayers[2].channels[0] != &gSequenceChannelNone) {
        D_80192C38 = 0;
        for (i = 0; i < 6; i++) {
            func_800C4888(i);
            func_800C4FE4(i);
            func_800C5968(i);
        }
    }
}

// Looks EXTEREMELY similar to sound_init from SM64
void sound_init(void) {
    u8 var_v0;
    u8 var_v1;

    sSoundRequestCount = 0;
    sNumProcessedSoundRequests = 0;
    D_800EA1C4 = 0;
    for (var_v0 = 0; var_v0 < SOUND_BANK_COUNT; var_v0++) {
        sSoundBankUsedListBack[var_v0] = 0;
        sSoundBankFreeListFront[var_v0] = 1;
        sNumSoundsInBank[var_v0] = 0;
        sSoundBankDisabled[var_v0] = false;
        D_80192C48[var_v0].current = 1.0f;
        D_80192C48[var_v0].remainingFrames = 0;
    }
    for (var_v0 = 0; var_v0 < 6; var_v0++) {
        for (var_v1 = 0; var_v1 < 8; var_v1++) {
            D_80192AB8[var_v0][var_v1][4] = 0xFF;
        }
    }
    for (var_v0 = 0; var_v0 < 6; var_v0++) {
        sSoundBanks[var_v0][0].prev = 0xFF;
        sSoundBanks[var_v0][0].next = 0xFF;
        for (var_v1 = 1; var_v1 < 19; var_v1++) {
            sSoundBanks[var_v0][var_v1].prev = var_v1 - 1;
            sSoundBanks[var_v0][var_v1].next = var_v1 + 1;
        }
        sSoundBanks[var_v0][var_v1].prev = var_v1 - 1;
        sSoundBanks[var_v0][var_v1].next = 0xFF;
    }
}

void func_800C5BD0(void) {
    if (D_800EA1C0 == 0) {
        func_800CBBE8(((D_800EA154[gPlayers[0].characterId] & 0xFFFF) << 8) | 0xF3000000, 0);
    } else {
        func_800CBBE8(0xF3004D00, 0);
    }
}

void func_800C5C40(void) {
    func_800C2474();
    if (D_800E9DA0 != 0) {
        func_800C400C();
    } else {
        D_800E9DA0++;
        func_800C3F70();
    }
    sound_init();
    func_800C284C(2, 0, 0xFF, 1);
    func_800C5BD0();
}

// game_audio_init?
void func_800C5CB8(void) {
    func_800C2474();
    func_800C3F70();
    sound_init();
    func_800C284C(2, 0, 0xFF, 1);
}

void func_800C5D04(u8 playerId) {
    if ((gPlayers[playerId].unk_044 & 0x20) == 0x20) {
        D_800E9E34[playerId] = 0;
        if (D_800E9E24[playerId] < 0x4E20) {
            if ((u8) D_800EA16C == 0) {
                D_800E9E24[playerId]++;
            }
        }
        if (D_800E9E24[playerId] == 1) {
            switch (D_800EA0EC[playerId]) {
                case 2:
                    D_800EA0EC[playerId] = 1;
                case 0:
                    func_800C97C4(playerId);
                    func_800C94A4(playerId);
                    break;
                default:
                    break;
            }
        }
    } else {
        D_800E9E24[playerId] = 0;
        if ((D_800E9E34[playerId] < 0x4E20) && ((u8) D_800EA16C == 0)) {
            D_800E9E34[playerId]++;
        }
    }
}

void func_800C5E38(u8 playerId) {
    if (D_800EA108 == 0) {
        if (((gPlayers[playerId].unk_044 & 0x20) != 0x20) && (gPlayers[playerId].unk_098 > 400.0f)) {
            D_800E9E14[playerId] = 1;
            if (D_800EA0EC[playerId] == 0) {
                D_800E9F7C[playerId].unk_10 = 0.6f - D_800E9F54[playerId];
            }
            D_800E9DC4[playerId] = D_800E9F7C[playerId].unk_30;
            D_800E9DD4[playerId] = D_800E9F7C[playerId].unk_24;
            if ((D_800E9E34[playerId] == 1) && ((u8) D_800EA16C == 0)) {
                switch (D_800EA0EC[playerId]) {
                    case 2:
                        D_800EA0EC[playerId] = 1;
                    case 0:
                        if ((D_800E9F74[playerId] == 0) && (gPlayers[playerId].characterId != 3)) {
                            if ((s32) D_800EA1C0 < 2) {
                                play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF9, 0x26), &D_800E9F7C[playerId].pos, playerId,
                                           &D_800E9F7C[playerId].unk_38, &D_800E9F04[playerId],
                                           (u8*) &D_800E9F7C[playerId].unk_14);
                            } else {
                                play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26), &D_800E9F7C[playerId].pos, playerId,
                                           &D_800E9F7C[playerId].unk_38, &D_800E9F04[playerId],
                                           (u8*) &D_800E9F7C[playerId].unk_14);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        } else {
            if (D_800E9E24[playerId] == 0x0000000A) {
                if ((s32) D_800EA1C0 < 2) {
                    func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xF9, 0x26));
                } else {
                    func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26));
                }
            }
            D_800E9E14[playerId] = 0;
            D_800E9F7C[playerId].unk_10 = (1.0f - D_800E9F54[playerId]) - D_800EA130[playerId];
            if ((1.0f - D_800E9F54[playerId]) < D_800EA130[playerId]) {
                D_800E9F7C[playerId].unk_10 = 0.0f;
            }
            D_800E9DC4[playerId] = D_800E9F7C[playerId].unk_2C;
            D_800E9DD4[playerId] = D_800E9F7C[playerId].unk_20;
        }
    }
}

void func_800C6108(u8 playerId) {
    Player* player;

    player = &gPlayers[playerId];
    D_800E9E64[playerId] = (player->unk_098 / D_800E9DC4[playerId]) + D_800E9DD4[playerId];
    if ((player->unk_098 < 1800.0f) && ((player->unk_044 & 0x20) != 0x20)) {
        D_800E9E64[playerId] = (player->unk_098 / D_800E9F7C[playerId].unk_34) + D_800E9F7C[playerId].unk_28;
        if (D_800E9EC4) {} // ?
    }
    if (player->speed > 4.75f) {
        if (D_800E9EB4[playerId] < (D_800E9F7C[playerId].unk_18 + 0.4f)) {
            D_800E9DE4[playerId] += 0.005f;
        }
    } else {
        D_800E9DE4[playerId] = 0.0f;
    }
    if (gPlayers[playerId].unk_0C0 > 0) {
        D_800E9E54[playerId] = (f32) player->unk_0C0;
    } else {
        D_800E9E54[playerId] = (f32) -player->unk_0C0;
    }
    if ((player->effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) {
        D_800E9EB4[playerId] = D_800E9E64[playerId] + D_800E9DE4[playerId];
    } else {
        D_800E9EB4[playerId] = D_800E9E64[playerId] + D_800E9DE4[playerId] - (D_800E9E54[playerId] / 12000.0f);
        if (D_800E9EB4[playerId] < 0.01f) {
            D_800E9EB4[playerId] = 0.01f;
        }
    }
    if ((D_800E9F74[playerId]) || (D_800E9F78[playerId])) {
        if (D_800E9F74[playerId]) {
            if (D_800E9F78[playerId] < 0xF) {
                D_800E9F78[playerId]++;
                D_800E9DF4[playerId] += 0.03f;
            }
        } else if (D_800E9F78[playerId]) {
            D_800E9F78[playerId]--;
            D_800E9DF4[playerId] -= 0.03f;
        }
        D_800E9EB4[playerId] -= D_800E9DF4[playerId];
    }
    D_800E9EE4[playerId] = D_800E9EB4[playerId] - D_800E9EC4[playerId];
#ifdef VERSION_EU
    if ((D_800E9EE4[playerId] > 0.5f) || (D_800E9EE4[playerId] < -0.5f))
#else
    if ((D_800E9EE4[playerId] > 0.5f) || (D_800E9EE4[playerId] < 0.5f)) {
#endif
        D_800E9ED4[playerId] = D_800E9EE4[playerId] * 0.25f;
    D_800E9F7C[playerId].unk_0C = D_800E9EC4[playerId] + D_800E9ED4[playerId] + D_800E9F34[playerId];
}
else {
    D_800E9F7C[playerId].unk_0C = D_800E9EB4[playerId] + D_800E9F34[playerId];
}
#ifdef VERSION_EU
if (D_800E9F7C[playerId].unk_0C < 0.0f) {
    D_800E9F7C[playerId].unk_0C = 0.0f;
}
#endif
if (D_800E9F7C[playerId].unk_0C > 4.0f) {
    D_800E9F7C[playerId].unk_0C = 4.0f;
}
D_800E9EC4[playerId] = D_800E9F7C[playerId].unk_0C;
D_800E9F7C[playerId].unk_38 = (D_800E9F7C[playerId].unk_0C / 1.5f) + 0.4f;
}

void func_800C64A0(u8 playerId) {
    switch (D_800E9E74[playerId]) {
        case 3:
            D_800E9EF4[playerId] = (gPlayers[playerId].speed / 5.0f) + 0.2f;
            break;
        case 1:
        case 13:
        case 14:
        case 17:
            D_800E9EF4[playerId] = ((D_800E9E54[playerId] - 3500.0f) / 3000.0f) + 0.4f;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 22:
        case 23:
        case 24:
        case 29:
        case 30:
        case 31:
            D_800E9EF4[playerId] = (gPlayers[playerId].speed / 5.0f) + 0.2f;
            break;
        default:
            D_800E9EF4[playerId] = 1.0f;
            break;
    }
    if (D_800E9EF4[playerId] > 1.0f) {
        D_800E9EF4[playerId] = 1.0f;
    }
    if (D_800E9EF4[playerId] < 0.0f) {
        D_800E9EF4[playerId] = 0.0f;
    }
    if ((gPlayers[playerId].unk_044 & 0x20) == 0x20) {
        D_800E9F04[playerId] = 0.56f - (D_800E9E24[playerId] * 0.06f);
    } else {
        D_800E9F04[playerId] = (D_800E9E34[playerId] / 50.0f) + 0.25f;
    }
    if (D_800E9F24[playerId] != 0) {
        D_800E9F04[playerId] = 0.0f;
    }
    if (D_800E9F04[playerId] > 0.9f) {
        D_800E9F04[playerId] = 0.9f;
    }
}

void func_800C6758(u8 playerId) {
    switch (D_800E9E74[playerId]) { /* irregular */
        case 3:
            D_800E9F14[playerId] = (gPlayers[playerId].speed / 9.0f) + 0.6f;
            break;
        case 2:
        case 13:
        case 17:
            D_800E9F14[playerId] = (D_800E9E54[playerId] / 13000.0f) + 0.95f;
            break;
        default:
            D_800E9F14[playerId] = 1.0f;
            break;
    }
}

void func_800C683C(u8 cameraId) {
    if ((D_800EA108 == 0) && (D_800EA0EC[cameraId] == 0)) {
        if (D_800E9E74[cameraId] != D_800E9E84[cameraId]) {
            func_800C5578(&D_800E9F7C[cameraId].pos, D_800E9E94[cameraId]);
            switch (D_800E9E74[cameraId]) {
                case 3:
                    play_sound(SOUND_ACTION_TYRE_SQUEAL, &D_800E9F7C[cameraId].pos, cameraId, &D_800E9F14[cameraId],
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ACTION_TYRE_SQUEAL;
                    break;
                case 18:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF8, 0x1D), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800EA1D4, (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF8, 0x1D);
                    break;
                case 19:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF8, 0x22), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800EA1D4, (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF8, 0x22);
                    break;
                case 1:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x09), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x09);
                    break;
                case 2:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF4, 0x0A), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF4, 0x0A);
                    break;
                case 17:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x1E), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x1E);
                    break;
                case 15:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x1F), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x1F);
                    break;
                case 16:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x21), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x21);
                    break;
                case 20:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x27), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x27);
                    break;
                case 25:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x20), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x20);
                    break;
                case 26:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x23), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x23);
                    break;
                case 27:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x46), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x46);
                    break;
                case 28:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x25), &D_800E9F7C[cameraId].pos, cameraId,
                               &D_800E9F14[cameraId], &D_800EA1D4, (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x25);
                    break;
                case 4:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0B), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0B);
                    break;
                case 5:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0C), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0C);
                    break;
                case 6:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0D), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0D);
                    break;
                case 7:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0E), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0E);
                    break;
                case 8:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0F), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x0F);
                    break;
                case 9:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x10), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x10);
                    break;
                case 10:
                case 14:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x11), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x11);
                    break;
                case 11:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x12), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x12);
                    break;
                case 12:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x13), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x13);
                    break;
                case 29:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x48), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x48);
                    break;
                case 30:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x49), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x49);
                    break;
                case 31:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x4A), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x4A);
                    break;
                case 13:
                case 22:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x29), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x29);
                    break;
                case 23:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x2A), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x2A);
                    break;
                case 24:
                    play_sound(SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x2B), &D_800E9F7C[cameraId].pos, cameraId, &D_800EA1D4,
                               &D_800E9EF4[cameraId], (u8*) &D_800E9F7C[cameraId].unk_14);
                    D_800E9E94[cameraId] = SOUND_ARG_LOAD(0x01, 0x00, 0xF0, 0x2B);
                    break;
                default:
                    func_800C5578(&D_800E9F7C[cameraId].pos, D_800E9E94[cameraId]);
                    break;
            }
        }
        D_800E9E84[cameraId] = D_800E9E74[cameraId];
    }
}

void func_800C70A8(u8 playerId) {
    if (D_800EA0EC[playerId] == 0) {
        D_800E9E74[playerId] = 0;
        if ((D_800E9E54[playerId] > 3500.0f) || ((gPlayers[playerId].effects & DRIFTING_EFFECT) == DRIFTING_EFFECT)) {
            D_800E9E74[playerId] = 1;
            switch (gPlayers[playerId].tyres[AUDIO_LEFT_TYRE].surfaceType) {
                case DIRT: /* switch 1 */
                    D_800E9E74[playerId] = 0x0000000D;
                    break;
                case SAND: /* switch 1 */
                    D_800E9E74[playerId] = 0x0000000E;
                    break;
                case BRIDGE: /* switch 1 */
                    D_800E9E74[playerId] = 0x00000011;
                    break;
                case STONE: /* switch 1 */
                    D_800E9E74[playerId] = 0x0000000F;
                    break;
                case SNOW: /* switch 1 */
                    D_800E9E74[playerId] = 0x00000010;
                    break;
                case ICE: /* switch 1 */
                    D_800E9E74[playerId] = 0x00000014;
                    break;
                case ROPE_BRIDGE: /* switch 1 */
                    D_800E9E74[playerId] = 0x00000019;
                    break;
                case WOOD_BRIDGE: /* switch 1 */
                    D_800E9E74[playerId] = 0x0000001A;
                    break;
                case TRAIN_TRACK: /* switch 1 */
                    D_800E9E74[playerId] = 0x0000001B;
                    break;
            }
        }
        if ((gPlayers[playerId].effects & DRIFTING_EFFECT) == DRIFTING_EFFECT) {
            D_800E9E74[playerId] = 2;
            switch (gPlayers[playerId].tyres[AUDIO_LEFT_TYRE].surfaceType) { /* switch 2 */
                case DIRT:                                                   /* switch 2 */
                    D_800E9E74[playerId] = 0x0000000D;
                    break;
                case SAND: /* switch 2 */
                    D_800E9E74[playerId] = 0x0000000E;
                    break;
                case BRIDGE: /* switch 2 */
                    D_800E9E74[playerId] = 0x00000011;
                    break;
                case STONE: /* switch 2 */
                    D_800E9E74[playerId] = 0x0000000F;
                    break;
                case SNOW: /* switch 2 */
                    D_800E9E74[playerId] = 0x00000010;
                    break;
                case ICE: /* switch 2 */
                    D_800E9E74[playerId] = 0x00000014;
                    break;
                case ROPE_BRIDGE: /* switch 2 */
                    D_800E9E74[playerId] = 0x00000019;
                    break;
                case WOOD_BRIDGE: /* switch 2 */
                    D_800E9E74[playerId] = 0x0000001A;
                    break;
                case TRAIN_TRACK: /* switch 2 */
                    D_800E9E74[playerId] = 0x0000001B;
                    break;
            }
        }
        switch (gPlayers[playerId].tyres[AUDIO_LEFT_TYRE].surfaceType) { /* switch 3 */
            case GRASS:                                                  /* switch 3 */
                if (D_800E9E74[playerId] == 6) {
                    D_800E9E74[playerId] = 4;
                } else if (D_800EA1C0 == 0) {
                    D_800E9E74[playerId] = 5;
                } else {
                    D_800E9E74[playerId] = 4;
                }
                break;
            case SAND_OFFROAD: /* switch 3 */
                if (D_800E9E74[playerId] == 0x0000000C) {
                    D_800E9E74[playerId] = 0x0000000A;
                } else if (D_800EA1C0 == 0) {
                    D_800E9E74[playerId] = 0x0000000B;
                } else {
                    D_800E9E74[playerId] = 0x0000000A;
                }
                break;
            case DIRT_OFFROAD: /* switch 3 */
                if (D_800E9E74[playerId] == 0x00000018) {
                    D_800E9E74[playerId] = 0x00000016;
                } else if (D_800EA1C0 == 0) {
                    D_800E9E74[playerId] = 0x00000017;
                } else {
                    D_800E9E74[playerId] = 0x00000016;
                }
                break;
            case SNOW_OFFROAD: /* switch 3 */
                if (D_800E9E74[playerId] == 0x0000001F) {
                    D_800E9E74[playerId] = 0x0000001D;
                } else if (D_800EA1C0 == 0) {
                    D_800E9E74[playerId] = 0x0000001E;
                } else {
                    D_800E9E74[playerId] = 0x0000001D;
                }
                break;
            case WET_SAND: /* switch 3 */
                if (D_800E9F74[playerId] == 0) {
                    if (D_800E9E74[playerId] == 9) {
                        D_800E9E74[playerId] = 7;
                    } else if (D_800EA1C0 == 0) {
                        D_800E9E74[playerId] = 8;
                    } else {
                        D_800E9E74[playerId] = 7;
                    }
                } else {
                    D_800E9E74[playerId] = 0x0000001C;
                }
                break;
            case ROPE_BRIDGE: /* switch 3 */
                D_800E9E74[playerId] = 0x00000019;
                break;
            case WOOD_BRIDGE: /* switch 3 */
                D_800E9E74[playerId] = 0x0000001A;
                break;
            case TRAIN_TRACK: /* switch 3 */
                D_800E9E74[playerId] = 0x0000001B;
                break;
        }
        switch (gPlayers[playerId].tyres[AUDIO_RIGHT_TYRE].surfaceType) { /* switch 4 */
            case GRASS:                                                   /* switch 4 */
                if (D_800E9E74[playerId] == 5) {
                    D_800E9E74[playerId] = 4;
                } else if (D_800EA1C0 == 0) {
                    D_800E9E74[playerId] = 6;
                } else {
                    D_800E9E74[playerId] = 4;
                }
                break;
            case SAND_OFFROAD: /* switch 4 */
                if (D_800E9E74[playerId] == 0x0000000B) {
                    D_800E9E74[playerId] = 0x0000000A;
                } else if (D_800EA1C0 == 0) {
                    D_800E9E74[playerId] = 0x0000000C;
                } else {
                    D_800E9E74[playerId] = 0x0000000A;
                }
                break;
            case DIRT_OFFROAD: /* switch 4 */
                if (D_800E9E74[playerId] == 0x00000017) {
                    D_800E9E74[playerId] = 0x00000016;
                } else if (D_800EA1C0 == 0) {
                    D_800E9E74[playerId] = 0x00000018;
                } else {
                    D_800E9E74[playerId] = 0x00000016;
                }
                break;
            case SNOW_OFFROAD: /* switch 4 */
                if (D_800E9E74[playerId] == 0x0000001E) {
                    D_800E9E74[playerId] = 0x0000001D;
                } else if (D_800EA1C0 == 0) {
                    D_800E9E74[playerId] = 0x0000001F;
                } else {
                    D_800E9E74[playerId] = 0x0000001D;
                }
                break;
            case WET_SAND: /* switch 4 */
                if (D_800E9F74[playerId] == 0) {
                    if (D_800E9E74[playerId] == 8) {
                        D_800E9E74[playerId] = 7;
                    } else if (D_800EA1C0 == 0) {
                        D_800E9E74[playerId] = 9;
                    } else {
                        D_800E9E74[playerId] = 7;
                    }
                } else {
                    D_800E9E74[playerId] = 0x0000001C;
                }
                break;
            case ROPE_BRIDGE: /* switch 4 */
                D_800E9E74[playerId] = 0x00000019;
                break;
            case WOOD_BRIDGE: /* switch 4 */
                D_800E9E74[playerId] = 0x0000001A;
                break;
            case TRAIN_TRACK: /* switch 4 */
                D_800E9E74[playerId] = 0x0000001B;
                break;
        }
        if (((gPlayers[playerId].speed < 0.5f) || ((gPlayers[playerId].effects & 8) == 8)) &&
            (D_800E9E74[playerId] != 0x0000001C)) {
            D_800E9E74[playerId] = 0;
        }
        if ((((gPlayers[playerId].effects & 0x4000) == 0x4000) && ((gPlayers[playerId].type & 0x2000) != 0x2000)) ||
            ((gPlayers[playerId].effects & 0x800) == 0x800) || ((gPlayers[playerId].effects & 0x80) == 0x80) ||
            ((gPlayers[playerId].effects & 0x40) == 0x40) || ((gPlayers[playerId].unk_044 & 0x4000) == 0x4000)) {
            D_800E9E74[playerId] = 0x00000012;
        }
        if ((((gPlayers[playerId].effects & 0x20) == 0x20) && ((gPlayers[playerId].type & 0x2000) != 0x2000)) ||
            ((gPlayers[playerId].effects & 0x800) == 0x800)) {
            D_800E9E74[playerId] = 0x00000013;
        }
        if (gPlayers[playerId].unk_20C != 0.0f) {
            D_800E9E74[playerId] = 3;
        }
    }
}

void func_800C76C0(u8 playerId) {
    if (D_800E9EA4[playerId] != 0) {
        if (D_800E9EA4[playerId] < 0x2BC) {
            D_800E9EA4[playerId]++;
        }
        if (D_800E9EA4[playerId] == 2) {
            func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x28));
            func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFF, 0x2C));
            func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x4C));
            func_800C5578(&D_800E9F7C[playerId].pos, D_800E9E94[playerId]);
            D_800E9E74[playerId] = 0;
            switch (gModeSelection) { /* irregular */
                case GRAND_PRIX:
                    D_800EA0EC[playerId] = 2;
                    func_800C9060(playerId, SOUND_ARG_LOAD(0x19, 0x00, 0xF1, 0x03));
                    if (D_800EA1C0 == 0) {
                        func_800C3448(0x100100FF);
                        func_800C3448(0x110100FF);
                        func_800C5278(5U);
                        if (gPlayers[playerId].currentRank == 0) {
                            func_800C97C4(playerId);
                            D_800EA0F0 = 2;
                            func_800C9A88(playerId);
                            play_sequences(MUSIC_SEQ_FINISH_1ST_PLACE, MUSIC_SEQ_WINNING_RESULTS);
                        } else if (gPlayers[playerId].currentRank < 4) {
                            func_800C97C4(playerId);
                            D_800EA0F0 = 2;
                            func_800C9A88(playerId);
                            play_sequences(MUSIC_SEQ_FINISH_2ND_4TH_PLACE, MUSIC_SEQ_WINNING_RESULTS);
                        } else {
                            func_800C3448(-0x3E9F9C00);
                            play_sequences(MUSIC_SEQ_FINISH_5TH_8TH_PLACE, MUSIC_SEQ_LOSING_RESULTS);
                        }
                    } else {
                        D_800EA0EC[playerId] = 2;
                        func_800C9060(playerId, 0x1900F103U);
                        if (gPlayers[playerId].currentRank == 0) {
                            func_800C3448(0x100100FF);
                            func_800C3448(0x110100FF);
                            func_800C97C4(playerId);
                            D_800EA0F0 = 2;
                            func_800C9A88(playerId);
                            play_sequences(MUSIC_SEQ_FINISH_1ST_PLACE, MUSIC_SEQ_WINNING_RESULTS);
                        } else if (gPlayers[playerId].currentRank < 4) {
                            if (D_800EA104 == 0) {
                                func_800C3448(0x100100FF);
                                func_800C3448(0x110100FF);
                            }
                            func_800C97C4(playerId);
                            D_800EA0F0 = 2;
                            func_800C9A88(playerId);
                            play_sequences(MUSIC_SEQ_FINISH_2ND_4TH_PLACE, MUSIC_SEQ_WINNING_RESULTS);
                        } else if (D_800EA104 == 0) {
                            func_800C3448(0x100100FF);
                            func_800C3448(0x110100FF);
                            func_800C3448(-0x3E9F9C00);
                            play_sequences(MUSIC_SEQ_FINISH_5TH_8TH_PLACE, MUSIC_SEQ_LOSING_RESULTS);
                        }
                        if ((D_800EA104 != 0) || (D_800EA0EC[playerId] != 1)) {
                            func_800C5278(5U);
                        }
                    }
                    break;
                case TIME_TRIALS:
                    func_800C3448(0x100100FF);
                    func_800C3448(0x110100FF);
                    func_800C97C4(playerId);
                    D_800EA0F0 = 2;
                    func_800C9A88(0U);
                    D_800EA0EC[playerId] = 2;
                    func_800C9060(playerId, 0x1900F103U);
                    if (D_801657E5 == 1) {
                        play_sequences(MUSIC_SEQ_FINISH_1ST_PLACE, MUSIC_SEQ_WINNING_RESULTS);
                    } else if (D_8018ED90 == 1) {
                        play_sequences(MUSIC_SEQ_FINISH_2ND_4TH_PLACE, MUSIC_SEQ_WINNING_RESULTS);
                    } else {
                        func_800C3448(0x01640010);
                    }
                    break;
                case VERSUS:
                    D_800EA0EC[playerId] = 2;
                    func_800C9060(playerId, 0x1900F103U);
                    switch (D_800EA1C0) { /* switch 1; irregular */
                        case 1:           /* switch 1 */
                            func_800C3448(0x100100FF);
                            func_800C3448(0x110100FF);
                            func_800C97C4(playerId);
                            D_800EA0F0 = 2;
                            func_800C9A88(playerId);
                            play_sequences(MUSIC_SEQ_FINISH_1ST_PLACE, MUSIC_SEQ_VS_BATTLE_RESULTS);
                            break;
                        case 2: /* switch 1 */
                            if ((D_800EA104 == 0) && (D_800EA0EC[playerId] == 1)) {
                                func_800C3448(0x100100FF);
                                func_800C3448(0x110100FF);
#ifdef VERSION_EU
                                play_sequence2(MUSIC_SEQ_FINISH_1ST_PLACE);
#else
                                play_sequences(MUSIC_SEQ_FINISH_1ST_PLACE, MUSIC_SEQ_VS_BATTLE_RESULTS);
#endif
                                D_800EA104 = 1;
                            } else if ((D_800EA104 == 1) && (D_800EA0EC[playerId] == 1)) {
                                func_800C5278(5U);
#ifndef VERSION_EU
                                if (func_800C3508(1) != 0x000D) {
#endif
                                    D_800EA104 = 0;
                                    play_sequences(MUSIC_SEQ_FINISH_2ND_4TH_PLACE, MUSIC_SEQ_VS_BATTLE_RESULTS);
                                }
                                D_800EA104 = 2;
                            }
                            break;
                        case 3: /* switch 1 */
                            if ((D_800EA104 == 0) && (D_800EA0EC[playerId] == 1)) {
                                func_800C3448(0x100100FF);
                                func_800C3448(0x110100FF);
                                play_sequence2(MUSIC_SEQ_FINISH_1ST_PLACE);
                                D_800EA104 = 1;
                            } else if ((D_800EA104 == 1) && (D_800EA0EC[playerId] == 1)) {
                                if (func_800C3508(1) != 0x000D) {
                                    D_800EA104 = 0;
                                    play_sequence2(MUSIC_SEQ_FINISH_2ND_4TH_PLACE);
                                }
                                D_800EA104 = 2;
                            } else if ((D_800EA104 == 2) && (D_800EA0EC[playerId] == 1)) {
                                func_800C5278(5U);
#ifndef VERSION_EU
                                if (func_800C3508(1) != 0x000E) {
#endif
                                    D_800EA104 = 0;
                                    play_sequences(MUSIC_SEQ_FINISH_2ND_4TH_PLACE, MUSIC_SEQ_VS_BATTLE_RESULTS);
                                }
                                D_800EA104 = 3;
                            }
                            break;
                    }
                    break;
                case BATTLE:
                    switch (D_800EA1C0) { /* switch 2; irregular */
                        case 1:           /* switch 2 */
                            func_800C3448(0x100100FF);
                            func_800C3448(0x110100FF);
                            func_800C5278(5U);
                            func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xF9, 0x26));
                            play_sequence2(MUSIC_SEQ_VS_BATTLE_RESULTS);
                            D_800EA0EC[playerId] = 2;
                            func_800C90F4(playerId, (gPlayers[gPlayerWinningIndex].characterId * 0x10) +
                                                        SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                            break;
                        case 2: /* switch 2 */
                            if ((D_800EA0EC[0] == 1) && (D_800EA0EC[1] == 1) && (D_800EA0EC[2] == 1)) {
                                func_800C5278(5U);
                                func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26));
                                play_sequence2(MUSIC_SEQ_VS_BATTLE_RESULTS);
                                D_800EA0EC[playerId] = 2;
                                func_800C90F4(playerId, (gPlayers[gPlayerWinningIndex].characterId * 0x10) +
                                                            SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                            }
                            break;
                        case 3: /* switch 2 */
                            if ((D_800EA0EC[0] == 1) && (D_800EA0EC[1] == 1) && (D_800EA0EC[2] == 1) &&
                                (D_800EA0EC[3] == 1)) {
                                func_800C5278(5U);
                                func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26));
                                play_sequence2(MUSIC_SEQ_VS_BATTLE_RESULTS);
                                D_800EA0EC[playerId] = 2;
                                func_800C90F4(playerId, (gPlayers[gPlayerWinningIndex].characterId * 0x10) +
                                                            SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                            }
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
        if (D_800E9EA4[playerId] == 0x0000001E) {
            switch (gModeSelection) { /* switch 3; irregular */
                case BATTLE:          /* switch 3 */
                    break;
                case GRAND_PRIX: /* switch 3 */
                    if (gPlayers[playerId].currentRank == 0) {
                        D_800EA0EC[playerId] = 2;
                        func_800C90F4(playerId,
                                      (gPlayers[playerId].characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x07));
                    } else if (gPlayers[playerId].currentRank < 4) {
                        D_800EA0EC[playerId] = 2;
                        func_800C90F4(playerId,
                                      (gPlayers[playerId].characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                    } else {
                        D_800EA0EC[playerId] = 2;
                        func_800C90F4(playerId,
                                      (gPlayers[playerId].characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x03));
                    }
                    break;
                case VERSUS: /* switch 3 */
                    if (gPlayers[playerId].currentRank == 0) {
                        D_800EA0EC[playerId] = 2;
                        func_800C90F4(playerId,
                                      (gPlayers[playerId].characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                    }
                    break;
                case TIME_TRIALS: /* switch 3 */
                    if (D_801657E5 == 1) {
                        D_800EA0EC[playerId] = 2;
                        func_800C90F4(playerId,
                                      (gPlayers[playerId].characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x07));
                    } else if (D_8018ED90 == (u8) 1) {
                        D_800EA0EC[playerId] = 2;
                        func_800C90F4(playerId,
                                      (gPlayers[playerId].characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x0D));
                    }
                    break;
            }
        }
        switch (gModeSelection) { /* switch 4; irregular */
            case GRAND_PRIX:      /* switch 4 */
                if (gPlayers[playerId].currentRank == 0) {
                    if (D_800E9EA4[playerId] >= 0x15F) {
                        if (D_800E9EA4[playerId] == 0x0000015F) {
                            func_800C9D0C(playerId);
                        }
                    } else {
                        D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 400.0f;
                    }
                } else if (gPlayers[playerId].currentRank < 4) {
                    if (D_800E9EA4[playerId] >= 0x15F) {
                        if (D_800E9EA4[playerId] == 0x0000015F) {
                            func_800C9D0C(playerId);
                        }
                    } else {
                        D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 400.0f;
                    }
                } else if (D_800E9EA4[playerId] >= 0x12D) {
                    if (D_800E9EA4[playerId] == 0x0000012D) {
                        func_800C97C4(playerId);
                    }
                } else {
                    D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 330.0f;
                }
                break;
            case VERSUS: /* switch 4 */
                if (D_800EA1C0 == (u8) 1) {
                    if (D_800E9EA4[playerId] >= 0x65) {
                        if (D_800E9EA4[playerId] == 0x00000065) {
                            func_800C9D0C(playerId);
                        }
                    } else {
                        D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 120.0f;
                    }
                } else if (D_800E9EA4[playerId] >= 0x65) {
                    if (D_800E9EA4[playerId] == 0x00000065) {
                        func_800C97C4(playerId);
                    }
                } else {
                    D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 120.0f;
                }
                /* fallthrough */
            case TIME_TRIALS: /* switch 4 */
                if (D_800E9EA4[playerId] >= 0x12D) {
                    if (D_800E9EA4[playerId] == 0x0000012D) {
                        func_800C9D0C(0U);
                    }
                } else {
                    D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 350.0f;
                }
                break;
            case BATTLE: /* switch 4 */
                if (D_800E9EA4[playerId] >= 0x65) {
                    if (D_800E9EA4[playerId] == 0x00000065) {
                        func_800C97C4(playerId);
                    }
                } else {
                    D_800EA130[playerId] = (f32) D_800E9EA4[playerId] / 120.0f;
                }
                break;
        }
    }
}

void func_800C847C(u8 playerId) {
    if ((gPlayers[playerId].waterInteractionFlags & WATER_IS_FULLY_SUBMERGED) == WATER_IS_FULLY_SUBMERGED) {
        if (D_800E9F74[playerId] == 0) {
            if ((s32) D_800EA1C0 < 2) {
                func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0xF9, 0x26));
            } else {
                func_800C9018(playerId, SOUND_ARG_LOAD(0x01, 0x00, 0x80, 0x26));
            }
            func_800C97C4(playerId);
            D_800E9F74[playerId] = 1;
            func_800C94A4(playerId);
            if (((IsChocoMountain()) || (IsBowsersCastle()) ||
                 (IsBansheeBoardwalk()) || (IsYoshiValley()) ||
                 (IsFrappeSnowland()) || (IsKoopaTroopaBeach()) ||
                 (IsRoyalRaceway()) || (IsSherbetLand()) ||
                 (IsDkJungle()) || (IsBigDonut())) &&
                (D_800EA0EC[playerId] == 0)) {
                play_sound((gPlayers[playerId].characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x05),
                           &D_800E9F7C[playerId].pos, playerId, &D_800EA1D4, &D_800EA1D4,
                           (u8*) &D_800E9F7C[playerId].unk_14);
            }
        }
    } else {
        if (D_800E9F74[playerId] == 1) {
            func_800C97C4(playerId);
            D_800E9F74[playerId] = 2;
            func_800C94A4(playerId);
            D_800E9F74[playerId] = 0;
            if ((IsKoopaTroopaBeach()) && (D_800EA0EC[playerId] == 0)) {
                play_sound((gPlayers[playerId].characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x08),
                           &D_800E9F7C[playerId].pos, playerId, &D_800EA1D4, &D_800EA1D4,
                           (u8*) &D_800E9F7C[playerId].unk_14);
            }
        }
    }
}

void func_800C86D8(u8 playerId) {
    if (((gPlayers[playerId].effects & 0x40000000) != 0x40000000) && (D_800E9F24[playerId] == 1)) {
        func_800C90F4(playerId, (gPlayers[playerId].characterId * 0x10) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x08));
    }
}

void func_800C8770(u8 playerId) {
    if ((gPlayers[playerId].effects & 0x40000000) == 0x40000000) {
        D_800E9F24[playerId] = 1;
        if (D_800E9F2C[playerId] < 0xFA) {
            D_800E9F2C[playerId]++;
        }
    } else {
        D_800E9F2C[playerId] = 0;
        if (D_800E9F24[playerId] == 1) {
            D_800E9F24[playerId] = 2;
        }
    }
    switch (D_800E9F24[playerId]) { /* irregular */
        case 1:
            if (D_800E9F34[playerId] < 0.7f) {
                D_800E9F34[playerId] += 0.1f;
                D_800E9F54[playerId] += 0.03f;
            }
            break;
        case 2:
            if (D_800E9F34[playerId] > 0.16f) {
                D_800E9F34[playerId] -= 0.15f;
                D_800E9F54[playerId] -= 0.03f;
            } else {
                D_800E9F34[playerId] = 0.0f;
                D_800E9F54[playerId] = 0.0f;
                D_800E9F24[playerId] = 0;
            }
            break;
    }
}

// Checks the same field for all players before doing something?
void func_800C8920(void) {
    if (((u8) D_800EA168 != 0) && ((gPlayers[0].effects & 0x40000000) != 0x40000000) &&
        ((gPlayers[1].effects & 0x40000000) != 0x40000000) && ((gPlayers[2].effects & 0x40000000) != 0x40000000) &&
        ((gPlayers[3].effects & 0x40000000) != 0x40000000) && ((gPlayers[4].effects & 0x40000000) != 0x40000000) &&
        ((gPlayers[5].effects & 0x40000000) != 0x40000000) && ((gPlayers[6].effects & 0x40000000) != 0x40000000) &&
        ((gPlayers[7].effects & 0x40000000) != 0x40000000)) {
        func_800CAC08();
    }
}

void func_800C89E4(void) {
    if (D_800EA180 != 0) {
        D_800EA180++;
        if (D_800EA180 == 2) {
            D_800EA17C = 1.0f;
        }
        if (D_800EA180 < 0xF) {
            D_800EA178 = 1.0f - (D_800EA180 * 0.012f);
        }
        if ((D_800EA180 >= 0x33) && (D_800EA180 < 0x96)) {
            D_800EA17C = 1.0f - ((D_800EA180 - 0x32) / 110.0f);
        }
        if (D_800EA180 == 0x12D) {
            D_800EA17C = 0.0f;
        }
        if (D_800EA180 == 0x321) {
            D_800EA178 = 1.0f;
            D_800EA17C = 0.85f;
            D_800EA180 = 0;
        }
    }
}

void func_800C8AE4(void) {
    if (IsLuigiRaceway()) {
        if (D_800EA184 != 0) {
            if ((u8) D_800EA16C == 0) {
                // Has to be this way, can't be D_800EA184++
                D_800EA184 += 1;
            }
            if (D_800EA184 == 0x012C) {
                D_800EA17C = 0.85f;
                D_800EA184 = 0;
            }
        } else {
            switch (D_800EA1C0) { /* irregular */
                case 0:
                    if (D_800E9F7C[0].unk_14 != 0) {
                        D_800EA17C = 0.0f;
                        D_800EA184 = 1;
                    }
                    break;
                case 1:
                    if ((D_800E9F7C[0].unk_14 != 0) || (D_800E9F7C[1].unk_14 != 0)) {
                        D_800EA17C = 0.0f;
                        D_800EA184 = 1;
                    }
                    break;
                case 2:
                    if ((D_800E9F7C[0].unk_14 != 0) || (D_800E9F7C[1].unk_14 != 0) || (D_800E9F7C[2].unk_14 != 0)) {
                        D_800EA17C = 0.0f;
                        D_800EA184 = 1;
                    }
                    break;
                case 3:
                    if ((D_800E9F7C[0].unk_14 != 0) || (D_800E9F7C[1].unk_14 != 0) || (D_800E9F7C[2].unk_14 != 0) ||
                        (D_800E9F7C[3].unk_14 != 0)) {
                        D_800EA17C = 0.0f;
                        D_800EA184 = 1;
                    }
                    break;
            }
        }
    }
}

void func_800C8C7C(u8 arg0) {
    D_800EA06C[arg0].unk00[2] = (1.0f - D_800E9F54[arg0]) - D_800EA130[arg0];
}

void func_800C8CCC() {
    u8 var_s0;

    for (var_s0 = 0; var_s0 < D_800EA1C0 + 1; var_s0++) {
        func_800C5D04(var_s0);
        func_800C5E38(var_s0);
        func_800C6108(var_s0);
        func_800C64A0(var_s0);
        func_800C6758(var_s0);
        func_800C683C(var_s0);
        func_800C70A8(var_s0);
        func_800C76C0(var_s0);
        func_800C847C(var_s0);
        func_800C86D8(var_s0);
    }
    if (gModeSelection == GRAND_PRIX) {
        for (var_s0 = 0; var_s0 < 8; var_s0++) {
            func_800C8770(var_s0);
            func_800C8C7C(var_s0);
        }
    } else {
        for (var_s0 = 0; var_s0 < D_800EA1C0 + 1; var_s0++) {
            func_800C8770(var_s0);
            func_800C8C7C(var_s0);
        }
    }
    func_800C8920();
    func_800C89E4();
    func_800C8AE4();
}

void play_sound2(s32 soundBits) {
    if ((soundBits == SOUND_ACTION_REV_ENGINE) && (IsDkJungle())) {
        soundBits = SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x27);
    }

    if ((soundBits == SOUND_ACTION_REV_ENGINE_2) && (IsDkJungle())) {
        soundBits = SOUND_ARG_LOAD(0x49, 0x00, 0x80, 0x28);
    }
    play_sound(soundBits, &D_800EA1C8, 4, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
}

void play_sequence(u16 arg0) {

    HMAS_Stop(HMAS_MUSIC); // Stop the current music sequence

    if(HMAS_IsIDRegistered(arg0)) {
        HMAS_Play(HMAS_MUSIC, arg0, true);
    } else {
        func_800C3448(arg0 | 0x10000);
        gCurrentMusicSeq = arg0;
    }
}

void play_sequence2(u16 arg0) {
    func_800C3448(arg0 | 0x01010000);
    D_800EA160 = arg0;
}

void func_800C8F44(u8 arg0) {
    func_800C36C4(0, 0, arg0, 1);
}

void func_800C8F80(u8 arg0, u32 soundBits) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[arg0]) {
            case 2:
                D_800EA0EC[arg0] = 1;
            case 0:
                play_sound(soundBits, &D_800E9F7C[arg0].pos, arg0, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
                break;
        }
    }
}

void func_800C9018(u8 arg0, u32 soundBits) {
    func_800C5578(&D_800E9F7C[arg0].pos, soundBits);
}

void func_800C9060(u8 playerId, u32 soundBits) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[playerId]) {
            case 2:
                D_800EA0EC[playerId] = 1;
            case 0:
                play_sound(soundBits, &D_800E9F7C[playerId].pos, playerId, &D_800EA1D4, &D_800EA1D4,
                           (u8*) &D_800E9F7C[playerId].unk_14);
                break;
        }
    }
}

void func_800C90F4(u8 playerId, u32 soundBits) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[playerId]) {
            case 2:
                D_800EA0EC[playerId] = 1;
            case 0:
                if (((soundBits & ~0xF0) == SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x03)) ||
                    ((soundBits & ~0xF0) == SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x04)) ||
                    ((soundBits & ~0xF0) == SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x05))) {
                    D_800EA180 = 1;
                }
                if (((gPlayers[playerId].effects & 0x40000000) == 0x40000000) && ((s32) D_800E9F2C[playerId] >= 0x1F)) {
                    play_sound(soundBits, &D_800E9F7C[playerId].pos, playerId, &D_800EA150, &D_800EA1D4,
                               (u8*) &D_800E9F7C[playerId].unk_14);
                } else {
                    play_sound(soundBits, &D_800E9F7C[playerId].pos, playerId, &D_800EA1D4, &D_800EA1D4,
                               (u8*) &D_800E9F7C[playerId].unk_14);
                }
                break;
            default:
                break;
        }
    }
}

void func_800C9250(u8 arg0) {
    func_800C90F4(arg0,
                  (gPlayers[arg0].characterId * 0x10) + (gAudioRandom & 1) + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x01));
}

void func_800C92CC(u8 playerId, u32 soundBits) {
    u8 var_s0;
    struct Unk_8018EFD8* temp_v0;

    if ((D_800EA108 == 0) && (D_800EA0F0 == 0) && ((s32) D_800EA1C0 < 2)) {
        for (var_s0 = 0; var_s0 < D_800EA1C0 + 1; var_s0++) {
            temp_v0 = func_800C1C88(playerId, gPlayers[playerId].pos, gPlayers[playerId].velocity, D_800EA1C8,
                                    (u8) var_s0, soundBits);
            if (temp_v0 != NULL) {
                temp_v0->unk34 = 170.0f;
                if (((gPlayers[playerId].effects & 0x40000000) == 0x40000000) && ((s32) D_800E9F2C[playerId] >= 0x1F)) {
                    play_sound((gPlayers[playerId].characterId * 0x10) + soundBits, &temp_v0->unk18, var_s0,
                               &D_800EA150, &D_800EA1D4, &D_800EA06C[playerId].unk0C);
                } else {
                    play_sound((gPlayers[playerId].characterId * 0x10) + soundBits, &temp_v0->unk18, var_s0,
                               &temp_v0->unk2C, &D_800EA1D4, &D_800EA06C[playerId].unk0C);
                }
            }
        }
    }
}

void func_800C94A4(u8 playerId) {
    u32 var_a0;

    if (D_800EA108 == 0) {
        switch (D_800EA0EC[playerId]) {
            case 2:
                D_800EA0EC[playerId] = 1;
            case 0:
                D_800E9F7C[playerId].unk_0C = 1.0f;
                D_800E9F7C[playerId].unk_10 = 0.0f;
                switch (gPlayers[playerId].characterId) {
                    case 0:
                    case 1:
                        D_800E9F7C[playerId].unk_18 = 2.8f;
                        D_800E9F7C[playerId].unk_1C = 3844.0f;
                        D_800E9F7C[playerId].unk_20 = 0.35f;
                        D_800E9F7C[playerId].unk_24 = -0.8f;
                        D_800E9F7C[playerId].unk_28 = 0.35f;
                        D_800E9F7C[playerId].unk_2C = 1568.9796f;
                        D_800E9F7C[playerId].unk_30 = 1067.7778f;
                        D_800E9F7C[playerId].unk_34 = 2766.065f;
                        break;
                    case 2:
                    case 6:
                        D_800E9F7C[playerId].unk_18 = 3.2f;
                        D_800E9F7C[playerId].unk_1C = 3844.0f;
                        D_800E9F7C[playerId].unk_20 = 0.6f;
                        D_800E9F7C[playerId].unk_24 = -1.7f;
                        D_800E9F7C[playerId].unk_28 = 0.6f;
                        D_800E9F7C[playerId].unk_2C = 1478.4615f;
                        D_800E9F7C[playerId].unk_30 = 784.4898f;
                        D_800E9F7C[playerId].unk_34 = 12813.335f;
                        break;
                    case 3:
                        D_800E9F7C[playerId].unk_18 = 2.8f;
                        D_800E9F7C[playerId].unk_1C = 3844.0f;
                        D_800E9F7C[playerId].unk_20 = 0.6f;
                        D_800E9F7C[playerId].unk_24 = -0.6f;
                        D_800E9F7C[playerId].unk_28 = 0.6f;
                        D_800E9F7C[playerId].unk_2C = 1747.2728f;
                        D_800E9F7C[playerId].unk_30 = 1130.5883f;
                        D_800E9F7C[playerId].unk_34 = 3844.001f;
                        break;
                    case 4:
                        D_800E9F7C[playerId].unk_18 = 2.0f;
                        D_800E9F7C[playerId].unk_1C = 3844.0f;
                        D_800E9F7C[playerId].unk_20 = 0.2f;
                        D_800E9F7C[playerId].unk_24 = -0.4f;
                        D_800E9F7C[playerId].unk_28 = 0.2f;
                        D_800E9F7C[playerId].unk_2C = 2135.5557f;
                        D_800E9F7C[playerId].unk_30 = 1601.6666f;
                        D_800E9F7C[playerId].unk_34 = 3203.333f;
                        break;
                    case 5:
                    case 7:
                        D_800E9F7C[playerId].unk_18 = 2.4f;
                        D_800E9F7C[playerId].unk_1C = 3844.0f;
                        D_800E9F7C[playerId].unk_20 = 0.4f;
                        D_800E9F7C[playerId].unk_24 = -0.8f;
                        D_800E9F7C[playerId].unk_28 = 0.4f;
                        D_800E9F7C[playerId].unk_2C = 1922.0f;
                        D_800E9F7C[playerId].unk_30 = 1201.25f;
                        D_800E9F7C[playerId].unk_34 = 4805.0f;
                        break;
                }
                var_a0 = gPlayers[playerId].characterId + 0x0104FF00;
                switch (D_800E9F74[playerId]) { /* switch 1; irregular */
                    case 0:                     /* switch 1 */
                        if (D_800EA1C0 != 0) {
                            var_a0 += 0x14;
                        }
                        break;
                    case 1: /* switch 1 */
                        var_a0 += 0x2E;
                        break;
                    case 2: /* switch 1 */
                        if (D_800EA1C0 == 0) {
                            var_a0 += 0x36;
                        } else {
                            var_a0 += 0x3E;
                        }
                        break;
                }
                play_sound(var_a0, &D_800E9F7C[playerId].pos, playerId, &D_800E9F7C[playerId].unk_0C,
                           &D_800E9F7C[playerId].unk_10, (u8*) &D_800E9F7C[playerId].unk_14);
                break;
            default:
                break;
        }
    }
}

void func_800C97C4(u8 arg0) {
    func_800C5578(&D_800E9F7C[arg0].pos, gPlayers[arg0].characterId + SOUND_ARG_LOAD(0x01, 0x04, 0xFF, 0x00));
    func_800C5578(&D_800E9F7C[arg0].pos, gPlayers[arg0].characterId + SOUND_ARG_LOAD(0x01, 0x04, 0xFF, 0x14));
    func_800C5578(&D_800E9F7C[arg0].pos, gPlayers[arg0].characterId + SOUND_ARG_LOAD(0x01, 0x04, 0xFF, 0x2E));
    func_800C5578(&D_800E9F7C[arg0].pos, gPlayers[arg0].characterId + SOUND_ARG_LOAD(0x01, 0x04, 0xFF, 0x36));
    func_800C5578(&D_800E9F7C[arg0].pos, gPlayers[arg0].characterId + SOUND_ARG_LOAD(0x01, 0x04, 0xFF, 0x3E));
}

void func_800C98B8(Vec3f position, Vec3f velocity, u32 soundBits) {
    u8 var_s0;
    struct Unk_8018EFD8* temp_v0;

    if ((D_800EA108 == 0) && (D_800EA0F0 == 0)) {
        for (var_s0 = 0; var_s0 < (D_800EA1C0 + 1); var_s0++) {
            temp_v0 = func_800C1C88(0U, position, velocity, D_800EA1C8, (u8) var_s0, soundBits);
            if (temp_v0 != 0) {
                temp_v0->unk34 = 170.0f;
                play_sound(soundBits, &temp_v0->unk18, var_s0, &temp_v0->unk2C, &D_800EA1D4, &D_800EA1DC);
            }
        }
    }
}

void func_800C99E0(Vec3f arg0, s32 soundBits) {
    Vec3f* temp_v0;
    u8 temp_t9;

    if (D_800EA108 == 0) {
        for (temp_t9 = 0; temp_t9 < D_800EA1C0 + 1; temp_t9++) {
            temp_v0 = func_800C21E8(arg0, soundBits);
            if (temp_v0 != NULL) {
                func_800C5578(temp_v0, soundBits);
            }
        }
    }
}

void func_800C9A88(u8 playerId) {
    u8 var_s0;
    u32 soundBits;
    struct Unk_8018EFD8* temp_v0_6;

    if (D_800EA108 == 0) {
        switch (D_800EA0F0) {
            case 2:
                D_800EA0F0 = 1;
                /* fallthrough */
            case 0:
                switch (gPlayers[playerId].characterId) {
                    case 0:
                    case 1:
                        D_800EA06C[playerId].unk00[0] = 0.35f;
                        D_800EA06C[playerId].unk00[1] = 1568.9796f;
                        break;
                    case 2:
                    case 6:
                        D_800EA06C[playerId].unk00[0] = 0.6f;
                        D_800EA06C[playerId].unk00[1] = 1478.4615f;
                        break;
                    case 3:
                        D_800EA06C[playerId].unk00[0] = 0.6f;
                        D_800EA06C[playerId].unk00[1] = 1747.2728f;
                        break;
                    case 4:
                        D_800EA06C[playerId].unk00[0] = 0.2f;
                        D_800EA06C[playerId].unk00[1] = 2135.5557f;
                        break;
                    case 5:
                    case 7:
                        D_800EA06C[playerId].unk00[0] = 0.4f;
                        D_800EA06C[playerId].unk00[1] = 1922.0f;
                }
                if (D_800EA1C0 < 2) {
                    for (var_s0 = 0; var_s0 < D_800EA1C0 + 1; var_s0++) {
                        soundBits = gPlayers[playerId].characterId + SOUND_ARG_LOAD(0x31, 0x02, 0x80, 0x00);
                        temp_v0_6 = func_800C1C88(playerId, gPlayers[playerId].pos, gPlayers[playerId].velocity,
                                                  &gPlayers[playerId].unk_098, var_s0, soundBits);
                        if (temp_v0_6 != NULL) {
                            temp_v0_6->unk34 = 40.0f;
                            play_sound(soundBits, &temp_v0_6->unk18, var_s0, &temp_v0_6->unk2C,
                                       &D_800EA06C[playerId].unk00[2], &D_800EA06C[playerId].unk0C);
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
}

void func_800C9D0C(u8 playerId) {
    func_800C550C(
        func_800C21E8(gPlayers[playerId].pos, gPlayers[playerId].characterId + SOUND_ARG_LOAD(0x31, 0x02, 0x80, 0x00)));
}

void func_800C9D80(Vec3f position, Vec3f velocity, u32 soundBits) {
    u8 var_s0;
    struct Unk_8018EFD8* temp_v0;

    if ((D_800EA108 == 0) && (D_800EA0F0 == 0) && ((s32) D_800EA1C0 < 4)) {
        for (var_s0 = 0; var_s0 < ((D_800EA1C0 + 1)); var_s0++) {
            temp_v0 = func_800C1C88(0U, position, velocity, D_800EA1C8, (u8) var_s0, soundBits);
            if (temp_v0 != 0) {
                temp_v0->unk34 = 170.0f;
                if (soundBits == SOUND_ARG_LOAD(0x51, 0x03, 0x70, 0x0B)) {
                    play_sound(soundBits, &temp_v0->unk18, var_s0, &D_800EA178, &D_800EA17C, &D_800EA1DC);
                } else {
                    play_sound(soundBits, &temp_v0->unk18, var_s0, &temp_v0->unk2C, &D_800EA1D4, &D_800EA1DC);
                }
            }
        }
    }
}

void func_800C9EF4(Vec3f arg0, u32 soundBits) {
    Vec3f* temp;
    u8 i;

    for (i = 0; i < D_800EA1C0 + 1; i++) {
        temp = func_800C21E8(arg0, soundBits);
        if (temp != NULL) {
            func_800C5578(temp, soundBits);
        }
    }
}

void func_800C9F90(u8 arg0) {
    if(HMAS_IsPlaying(HMAS_MUSIC)) {
        HMAS_SetVolume(HMAS_MUSIC, arg0 == 1 ? 0.15f : 0.9f);
    }

    if ((arg0) != 0) {
        play_sound2(SOUND_ACTION_GO_BACK_2);
        func_800CBBB8(0xF1000000, 0);
        D_800EA16C = 1;
    } else {
        play_sound2(SOUND_ACTION_UNKNOWN_CONFIRMATION);
        func_800CBBB8(0xF2000000, 0);
        D_800EA16C = 0;
    }
}

void func_800CA008(u8 arg0, u8 arg1) {
    func_800C36C4(0, 0, 0x7F, 1);
    func_800C36C4(1, 0, 0x7F, 1);

    if (arg1 >= 4) {
        if ((arg1 == 0xC) || (arg1 == 4)) {
            arg1 = 5;
        } else {
            arg1 = 4;
        }
    }
    func_800C3448((arg0 << 8) | 0xF0000000 | arg1);
}

// With -framepointer active, you CANNOT put void
// in the argument list, causes a minor stack difference
void func_800CA0A0() {
    D_800EA108 = 1;
}

void func_800CA0B8() {
    D_800EA108 = 0;
}

void func_800CA0CC() {
    D_800EA108 = 1;
}

void func_800CA0E4(void) {
    func_800C5278(3);
    func_800C5278(5);
}

void func_800CA118(u8 arg0) {
    D_800EA0EC[arg0] = 1;
    D_800E9EA4[arg0] = 1;

    // Audio: Stop the music when the race is over
    if(HMAS_IsPlaying(HMAS_MUSIC)){
        HMAS_AddEffect(HMAS_MUSIC, HMAS_EFFECT_VOLUME, HMAS_LINEAR, 10, 0);
        HMAS_AddEffect(HMAS_MUSIC, HMAS_EFFECT_STOP,   HMAS_INSTANT, 1, 0);
    }

    switch (D_800EA1C0) { /* irregular */
        case 0:
            D_800EA0F0 = 1;
            func_800CA0E4();
            break;
        case 1:
            if ((D_800EA0EC[0] == (u8) 1) && (D_800EA0EC[1] == (u8) 1)) {
                D_800EA0F0 = 1;
                func_800CA0E4();
            }
            break;
        case 2:
            if ((D_800EA0EC[0] == (u8) 1) && (D_800EA0EC[1] == (u8) 1) && (D_800EA0EC[2] == (u8) 1)) {
                D_800EA0F0 = 1;
                func_800CA0E4();
            }
            break;
        case 3:
            if ((D_800EA0EC[0] == (u8) 1) && (D_800EA0EC[1] == (u8) 1) && (D_800EA0EC[2] == (u8) 1) &&
                (D_800EA0EC[3] == (u8) 1)) {
                D_800EA0F0 = 1;
                func_800CA0E4();
            }
            break;
    }
}

void func_800CA24C(u8 arg0) {
    D_800EA0EC[arg0] = 2;
}

void func_800CA270() {
    D_800EA0F4 = 1;
}

void func_800CA288(u8 arg0, s8 arg1) {
    D_800E9F7C[arg0].unk_14 = arg1;
}

void func_800CA2B8(u8 arg0) {
    D_800E9F7C[arg0].unk_14 = 0;
}

void func_800CA2E4(u8 arg0, s8 arg1) {
    D_800EA06C[arg0].unk0C = arg1;
}

void func_800CA30C(u8 arg0) {
    D_800EA06C[arg0].unk0C = 0;
}

void func_800CA330(u8 arg0) {
    func_800C3448(arg0 << 0x10 | 0x100000FF);
    func_800C3448(arg0 << 0x10 | 0x110000FF);
}

void func_800CA388(u8 arg0) {
    arg0 *= 2;
    fade_channel_volume_scale(0, 0, arg0);
    fade_channel_volume_scale(1, 0, arg0);
    fade_channel_volume_scale(2, 0, arg0);
    fade_channel_volume_scale(3, 0, arg0);
    fade_channel_volume_scale(5, 0, arg0);
}

void play_sequences(u16 arg0, u16 arg1) {
    if (D_800EA104 == 0) {
        func_800C3448(func_800C3508(0) | 0x30000000);
        func_800C35E8(0);
        func_800C3448(arg1 | 0xC1510000);
        func_800C3448(arg0 | 0x1000000);
    }
    D_800EA104 = 1;
}

void func_800CA49C(u8 arg0) {
    if (D_800EA108 == 0) {
        if (D_800EA1C0 >= 2) {
            func_800C9060(arg0, 0x1900FF3A);
        } else if (D_800EA164 != 0) {
            func_800C3448(0x100100FF); // 0x19000000
            func_800C3448(0x110100FF);
            play_sequence2(MUSIC_SEQ_FINAL_LAP_FANFARE);
            func_800C3448(0xC1510011);
        } else {
            func_800C3448(0x100100FF); // 0x19000000
            func_800C3448(0x110100FF);
            play_sequence2(MUSIC_SEQ_FINAL_LAP_FANFARE);
            func_800C3448(gCurrentMusicSeq | 0xC1500000);
            func_800C3448(0xC130017D);
        }
        D_8018FC08 = D_8018FC08 + 1;

        // Audio: Speed up the music on the last lap
        if(HMAS_IsPlaying(HMAS_MUSIC)){
            HMAS_AddEffect(HMAS_MUSIC, HMAS_EFFECT_PITCH, HMAS_INSTANT, 1,   1.2f);
            HMAS_AddEffect(HMAS_MUSIC, HMAS_EFFECT_PAUSE, HMAS_INSTANT, 110, 0.0f);
            HMAS_AddEffect(HMAS_MUSIC, HMAS_EFFECT_PAUSE, HMAS_INSTANT, 1,   1.0f);
        }
    }
}

void func_800CA59C(u8 playerId) {
    if ((D_800EA0EC[playerId] == 0) && (D_800EA108 == 0)) {
        play_sound((gPlayers[playerId].characterId * 0x10) + 0x29008001, &D_800E9F7C[playerId].pos, playerId,
                   &D_800EA1D4, &D_800EA1D4, (u8*) &D_800E9F7C[playerId].unk_14);
        D_800EA164 = 1;
        if ((s32) D_800EA1C0 >= 2) {
            func_800C8F80(playerId, 0x0100FF2C);
        } else {
            func_800C3448(0x100100FF);
            if (D_800EA104 != 0) {
                func_800C8F80(playerId, 0x0100FF2C);
            } else if (D_8018FC08 != 0) {
                if ((func_800C3508(1) == 0x000C) || (func_800C357C(0x0101000C) == 0)) {
                    func_800C3448(0xC1F00000);
                    func_800C3448(0xC1510011);
                } else {
                    play_sequence2(MUSIC_SEQ_STAR_JINGLE);
                }
            } else {
                if (1) {} // ?
                play_sequence2(MUSIC_SEQ_STAR_JINGLE);
            }
        }
        D_800EA10C[playerId] = 1;
    }
}

void func_800CA730(u8 arg0) {
    if (D_800EA0EC[arg0] == 0) {
        if ((D_800EA108 == 0) && (D_800EA10C[arg0] != 0)) {
            play_sound(gPlayers[arg0].characterId * 0x10 + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x08),
                       &D_800E9F7C[arg0].pos, arg0, &D_800EA1D4, &D_800EA1D4, (u8*) &D_800E9F7C[arg0].unk_14);
            if (D_800EA10C[arg0] != 0) {
                if ((s32) D_800EA1C0 >= 2) {
                    func_800C9018(arg0, SOUND_ARG_LOAD(0x01, 0x00, 0xFF, 0x2C));
                } else {
                    D_800EA10C[arg0] = 0;
                    if (D_800EA104 != 0) {
                        func_800C9018(arg0, SOUND_ARG_LOAD(0x01, 0x00, 0xFF, 0x2C));
                    } else if ((D_800EA10C[0] == 0) && (D_800EA10C[1] == 0)) {
                        if (D_8018FC08 != 0) {
                            if (((u32) (gSequencePlayers[1].enabled)) == 0) {
                                func_800C3608(1, 5);
                                play_sequence(gCurrentMusicSeq);
                                func_800C3448(0xB001307DU);
                            } else if ((func_800C3508(1) == 0xC) || (func_800C357C(0x0101000C) == 0)) {
                                func_800C3448(0xC1F00000U);
                                func_800C3448(gCurrentMusicSeq | 0xC1500000);
                                func_800C3448(0xC130017DU);
                            } else {
                                func_800C3448(0x110100FFU);
                                play_sequence(gCurrentMusicSeq);
                                func_800C3448(0xB001307DU);
                            }
                        } else {
                            func_800C3448(0x110100FFU);
                            if(!HMAS_IsPlaying(HMAS_MUSIC)){
                                play_sequence(gCurrentMusicSeq);
                            }
                        }
                    }
                    D_800EA164 = 0;
                }
            }
            D_800EA10C[arg0] = 0;
        }
    }
}

void func_800CA984(u8 arg0) {
    u8 i;
    struct Unk_8018EFD8* temp_v0_2;

    if ((D_800EA108 == 0) && (D_800EA0F0 == 0)) {
        for (i = 0; i < D_800EA1C0 + 1; i++) {
            temp_v0_2 =
                func_800C1C88(arg0, gPlayers[arg0].pos, D_800EA1C8, &gPlayers[arg0].unk_098, (u8) i, SOUND_ITEM_STAR);
            if (temp_v0_2) {
                play_sound(SOUND_ITEM_STAR, &temp_v0_2->unk18, i, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
            }
        }
    }
}

void func_800CAACC(u8 playerId) {
    if ((u8) D_800EA108 == 0) {
        func_800C5578(func_800C21E8(gPlayers[playerId].pos, SOUND_ITEM_STAR), SOUND_ITEM_STAR);
    }
}

void func_800CAB4C(u8 arg0) {
    UNUSED u8* temp_v1;
    UNUSED u8 temp_v0;

    if (D_800EA108 == 0) {
        switch (D_800EA0EC[arg0]) {
            case 2:
                D_800EA0EC[arg0] = 1;
            case 0:
                func_800C36C4(0, 1U, 0x55U, 5);
                func_800C9060(arg0, 0x1900F013U);
                play_sound(SOUND_ITEM_THUNDERBOLT, &D_800EA1C8, 0U, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
                break;
            default:
                break;
        }
        D_800EA168 = 1;
    }
}

void func_800CAC08() {
    if (D_800EA108 == 0) {
        func_800C36C4(0, 1U, 0x7FU, 0x19);
        func_800C56F0(SOUND_ITEM_THUNDERBOLT);
        D_800EA168 = 0;
    }
}

void func_800CAC60(UNUSED s32 arg0) {
    if ((D_800EA108 == 0) && (D_800EA0F0 == 0)) {
        play_sound(SOUND_ACTION_EXPLOSION_2, &D_800EA1C8, 0U, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
        if ((D_800EA10C[0] != 1) && (D_800EA10C[1] != 1)) {
            func_800C36C4(0, 1, 0x37U, 5);
            play_sound(SOUND_ITEM_THUNDERBOLT, &D_800EA1C8, 0U, &D_800EA1D4, &D_800EA1D4, &D_800EA1DC);
            D_800EA168 = 1;
        }
    }
}

void func_800CAD40(UNUSED s32 arg0) {
    if (D_800EA108 == 0) {
        if ((D_800EA170[0] == 0) && (D_800EA170[1] == 0) && (D_800EA170[2] == 0) && (D_800EA170[3] == 0)) {
            func_800C36C4(0, 1, 0x7FU, 0x19);
        }
        func_800C56F0(SOUND_ITEM_THUNDERBOLT);
        D_800EA168 = 0;
    }
}

void func_800CADD0(u8 arg0, f32 arg1) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[arg0]) {
            case 2:
                D_800EA0EC[arg0] = 1;
                // Purposeful fallthrough
            case 0:
                arg1 = (arg1 * 0.55f) + 0.45f;
                if (arg1 > 1.0f) {
                    arg1 = 1.0f;
                }
                if (arg1 < 0.0f) {
                    arg1 = 0.0f;
                }
                D_800EA110[arg0] = arg1;
                play_sound(0x1900A209U, &D_800E9F7C[arg0].pos, arg0, &D_800EA1D4, &D_800EA110[arg0],
                           (u8*) &D_800E9F7C[arg0].unk_14);
                break;
            default:
                break;
        }
    }
}

void func_800CAEC4(u8 playerId, f32 arg1) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[playerId]) {
            case 2:
                D_800EA0EC[playerId] = 1;
                // Purposeful fallthrough
            case 0:
                arg1 = (arg1 * 0.7f) + 0.1f;
                if (arg1 > 1.0f) {
                    arg1 = 1.0f;
                }
                if (arg1 < 0.0f) {
                    arg1 = 0.0f;
                }
                D_800EA120[playerId] = arg1;
                //! @warning this used to be gCurrentCourseId + 0x19007020
                // This may not be equivallent.
                play_sound(GetCourseIndex() + 0x19007020, &D_800E9F7C[playerId].pos, playerId, &D_800EA1D4,
                           &D_800EA120[playerId], (u8*) &D_800E9F7C[playerId].unk_14);
                break;
            default:
                break;
        }
    }
}

void func_800CAFC0(u8 arg0) {
    if (D_800EA108 == 0) {
        switch (D_800EA0EC[arg0]) {
            case 2:
                D_800EA0EC[arg0] = 1;
            case 0:
                func_800C36C4(0, 1, 0x55U, 5);
                func_800C9060(arg0, 0x19009E59U);
                func_800C8F80(arg0, 0x0100FA4C);
                D_800EA170[arg0] = 1;
                break;
        }
    }
}

void func_800CB064(u8 arg0) {
    if (D_800EA108 == 0) {
        if (D_800EA170[arg0] == 1) {
            if ((u8) D_800EA168 == 0) {
                func_800C36C4(0, 1U, 0x7FU, 0x19);
            }
            func_800C90F4(arg0, gPlayers[arg0].characterId * 0x10 + SOUND_ARG_LOAD(0x29, 0x00, 0x80, 0x08));
            func_800C9018(arg0, SOUND_ARG_LOAD(0x01, 0x00, 0xFA, 0x4C));
            D_800EA170[arg0] = 0;
        }
    }
}

void func_800CB134() {
    D_800EA174 = 1;
}

void func_800CB14C() {
    if (D_800EA174 != 0) {
        D_800EA174++;
        if (D_800EA174 == 3) {
            play_sequence(MUSIC_SEQ_AWARD_CEREMONY_BUILDUP);
            func_800C3448(0x4000007F);
        }
        if (D_800EA174 == 0x012C) {
            play_sequence(MUSIC_SEQ_AWARD_CEREMONY_1ST_3RD);
            func_800C3448(0x4000007F);
            play_sequence2(MUSIC_SEQ_AWARD_CEREMONY_4TH_8TH);
            func_800C3448(0x41000000);
        }
        if (D_800EA174 == 0x0230) {
            func_800C3448(0x40640000);
            func_800C3448(0xB0640073);
            func_800C3448(0x4150007F);
            func_800C3448(0xB1640073);
        }
        if (D_800EA174 == 0x02A8) {
            func_800C3448(0x100100FF);
        }
        if (D_800EA174 == 0x041A) {
            func_800C3448(0xB1500001);
            func_800C3448(0x51500001);
        }
        if (D_800EA174 == 0x046A) {
            func_800C3448(0x41320000);
        }
        if (D_800EA174 == 0x04B0) {
            func_800C3448(0x110100FF);
        }
        if (D_800EA174 == 0x04CE) {
            play_sequence(MUSIC_SEQ_GAME_OVER);
            func_800C3448(0x4000007F);
        }
    }
}

void audio_set_player_volume(u8 player, f32 volume) {
    func_800C36C4(player, 0, volume * 127, 1);
}

// run audio?
void func_800CB2C4() {
    func_800C1F8C();
    func_800C3724();
    func_800C3478();
    func_800C5848();
    func_800C59C4();
    func_800C8CCC();
    func_800C2274(0);
    func_800CBC24();
}
