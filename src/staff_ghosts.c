#include <libultraship.h>
#include <macros.h>
#include <mk64.h>
#include <stubs.h>
#include <common_structs.h>
#include <defines.h>
#include <decode.h>
#include "main.h"
#include "code_800029B0.h"
#include "buffers.h"
#include "save.h"
#include "staff_ghosts.h"
#include "code_8006E9C0.h"
#include "menu_items.h"
#include "code_80057C60.h"
#include "kart_dma.h"
#include "port/Game.h"
#include "courses/staff_ghost_data.h"

u8* sReplayGhostBuffer;
size_t sReplayGhostBufferSize;
s16 D_80162D86;
u16 D_80162D88;

u32 D_80162D8C;
s16 D_80162D90;
u32* sReplayGhostDecompressed;

u16 D_80162D98;
u32 D_80162D9C;
s16 D_80162DA0;
u32* D_80162DA4;

u16 D_80162DA8;
s32 D_80162DAC;
s16 D_80162DB0;
u32* D_80162DB4;

s16 D_80162DB8;
u32* D_80162DBC;

u16 D_80162DC0;
StaffGhost* D_80162DC4;
s32 D_80162DC8;
s32 D_80162DCC;
s32 D_80162DD0;
u16 D_80162DD4;
u16 D_80162DD6;
u16 D_80162DD8;
s32 D_80162DDC;
s32 D_80162DE0; // ghost kart id?
s32 D_80162DE4;
s32 D_80162DE8;
s32 D_80162DEC;
s32 D_80162DF0;
s32 D_80162DF4;
s32 D_80162DF8;
s32 D_80162DFC;

s32 D_80162E00;

u32* sReplayGhostEncoded = (u32*) &D_802BFB80.arraySize8[0][2][3];
u32* gReplayGhostCompressed = (u32*) &D_802BFB80.arraySize8[1][1][3];

extern s32 gLapCountByPlayerId[];

void func_80004EF0(void) {
    D_80162DA4 = (u32*) &D_802BFB80.arraySize8[0][2][3];
    u8* dest = (u8*) D_80162DA4;
    osInvalDCache(&D_80162DA4[0], 0x4000);

    u8* ghost = (u8*) D_80162DC4;

    size_t size = 0;
    if (ghost == d_luigi_raceway_staff_ghost) {
        size = 187 * sizeof(StaffGhost);
    } else if (ghost == d_mario_raceway_staff_ghost) {
        size = 208 * sizeof(StaffGhost);
    } else if (ghost == d_royal_raceway_staff_ghost) {
        size = 377 * sizeof(StaffGhost);
    }
    
    // Manual memcpy required for byte swap
    for (int i = 0; i < size; i += 4) {
        dest[i] = ghost[i + 3];
        dest[i + 1] = ghost[i + 2];
        dest[i + 2] = ghost[i + 1];
        dest[i + 3] = ghost[i];
    }

    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    D_80162D9C = (*D_80162DA4 & 0xFF0000);
    D_80162DA0 = 0;
}

void func_80004FB0(void) {
    D_80162DB4 = (u32*) &D_802BFB80.arraySize8[0][D_80162DD0][3];
    D_80162DAC = *D_80162DB4 & 0xFF0000;
    D_80162DB0 = 0;
}

void func_80004FF8(void) {
    sReplayGhostDecompressed = (u32*) &D_802BFB80.arraySize8[0][D_80162DC8][3];
    D_80162D8C = (s32) *sReplayGhostDecompressed & 0xFF0000;
    D_80162D90 = 0;
}
/**
 * Activates staff ghost if time trial lap time is lower enough
 *
 */
#ifdef VERSION_EU
#define BLAH 10700
#define BLAH2 19300
#define BLAH3 13300
#else
#define BLAH 9000
#define BLAH2 16000
#define BLAH3 11200

#endif

void set_staff_ghost(void) {
    CM_SetStaffGhost();
}

s32 func_800051C4(void) {
    s32 phi_v0;

    if (sReplayGhostBufferSize != 0) {
        // func_80040174 in mio0_decode.s
        func_80040174((void*) sReplayGhostBuffer, (sReplayGhostBufferSize * 4) + 0x20, (s32) sReplayGhostEncoded);
        phi_v0 = mio0encode((s32) sReplayGhostEncoded, (sReplayGhostBufferSize * 4) + 0x20, (s32) gReplayGhostCompressed);
        return phi_v0 + 0x1e;
    }
}

void func_8000522C(void) {
    sReplayGhostDecompressed = (u32*) &D_802BFB80.arraySize8[0][D_80162DC8][3];
    mio0decode((u8*) gReplayGhostCompressed, (u8*) sReplayGhostDecompressed);
    D_80162D8C = (s32) (*sReplayGhostDecompressed & 0xFF0000);
    D_80162D90 = 0;
    D_80162E00 = 1;
}

void func_800052A4(void) {
    s16 temp_v0;

    if (D_80162DC8 == 1) {
        D_80162DC8 = 0;
        D_80162DCC = 1;
    } else {
        D_80162DC8 = 1;
        D_80162DCC = 0;
    }
    temp_v0 = D_80162DB8;
    sReplayGhostBuffer = (void*) &D_802BFB80.arraySize8[0][D_80162DC8][3];
    sReplayGhostBufferSize = temp_v0;
    D_80162D86 = temp_v0;
}

void func_80005310(void) {

    if (gModeSelection == TIME_TRIALS) {

        set_staff_ghost();

        if (D_80162DC0 != gCurrentCourseId) {
            D_80162DD4 = 1;
        }

        D_80162DC0 = (u16) gCurrentCourseId;
        D_80162DF0 = 0;
        D_80162DEC = 0;
        D_80162DF8 = 0;

        if (gModeSelection == TIME_TRIALS && gActiveScreenMode == SCREEN_MODE_1P) {

            if (D_8015F890 == 1) {
                func_80004FB0();
                if (D_80162DD8 == 0) {
                    func_80004FF8();
                }
                if (D_80162DD6 == 0) {
                    func_80004EF0();
                }
            } else {

                D_80162DD8 = 1U;
                D_80162DBC = (u32*) &D_802BFB80.arraySize8[0][D_80162DCC][3];
                D_80162DBC[0] = -1;
                D_80162DB8 = 0;
                D_80162DDC = 0;
                func_80091EE4();
                if (D_80162DD4 == 0) {
                    func_80004FF8();
                }
                if (D_80162DD6 == 0) {
                    func_80004EF0();
                }
            }
        }
    }
}

void func_8000546C(void) {
    u32 temp_a0;
    u32 temp_a1;
    UNUSED u16 unk;
    u16 temp_v1;
    s16 phi_v1;
    s16 phi_v0 = 0;

    if (D_80162DB0 >= 0x1000) {
        gPlayerOne->type = PLAYER_CINEMATIC_MODE | PLAYER_START_SEQUENCE | PLAYER_KART_AI;
        return;
    }

    temp_a0 = D_80162DB4[D_80162DB0];
    temp_a1 = temp_a0 & 0xFF;

    if (temp_a1 < 0x80U) {
        phi_v1 = (s16) (temp_a1 & 0xFF);
    } else {
        phi_v1 = (s16) (temp_a1 | (~0xFF));
    }

    temp_a1 = (u32) (temp_a0 & 0xFF00) >> 8;
    gControllerEight->rawStickX = phi_v1;

    if (temp_a1 < 0x80U) {
        phi_v1 = (s16) (temp_a1 & 0xFF);
    } else {
        phi_v1 = (s16) (temp_a1 | (~0xFF));
    }
    gControllerEight->rawStickY = phi_v1;
    if (temp_a0 & 0x80000000) {
        phi_v0 |= A_BUTTON;
    }
    if (temp_a0 & 0x40000000) {
        phi_v0 |= B_BUTTON;
    }
    if (temp_a0 & 0x20000000) {
        phi_v0 |= Z_TRIG;
    }
    if (temp_a0 & 0x10000000) {
        phi_v0 |= R_TRIG;
    }
    temp_v1 = gControllerEight->buttonPressed & 0x1F0F;
    gControllerEight->buttonPressed = (phi_v0 & (phi_v0 ^ D_80162DA8)) | temp_v1;
    temp_v1 = gControllerEight->buttonDepressed & 0x1F0F;
    gControllerEight->buttonDepressed = (D_80162DA8 & (phi_v0 ^ D_80162DA8)) | temp_v1;
    D_80162DA8 = phi_v0;
    gControllerEight->button = phi_v0;

    if (D_80162DAC == 0) {
        D_80162DB0++;
        D_80162DAC = (s32) (D_80162DB4[D_80162DB0] & 0xFF0000);
    } else {
        D_80162DAC += 0xFFFF0000;
    }
}

void func_8000561C(void) {
    u32 temp_a0;
    u32 temp_v0;
    UNUSED u16 unk;
    u16 temp_v1;
    s16 phi_v1;
    s16 phi_a2 = 0;

    if (D_80162DA0 >= 0x1000) {
        func_80005AE8(gPlayerThree);
        return;
    }
    temp_a0 = D_80162DA4[D_80162DA0];
    temp_v0 = temp_a0 & 0xFF;
    if (temp_v0 < 0x80U) {
        phi_v1 = (s16) (temp_v0 & 0xFF);
    } else {
        phi_v1 = (s16) (temp_v0 | (~0xFF));
    }

    temp_v0 = (u32) (temp_a0 & 0xFF00) >> 8;
    gControllerSeven->rawStickX = phi_v1;

    if (temp_v0 < 0x80U) {
        phi_v1 = (s16) (temp_v0 & 0xFF);
    } else {
        phi_v1 = (s16) (temp_v0 | (~0xFF));
    }
    gControllerSeven->rawStickY = phi_v1;

    if (temp_a0 & 0x80000000) {
        phi_a2 = A_BUTTON;
    }
    if (temp_a0 & 0x40000000) {
        phi_a2 |= B_BUTTON;
    }
    if (temp_a0 & 0x20000000) {
        phi_a2 |= Z_TRIG;
    }
    if (temp_a0 & 0x10000000) {
        phi_a2 |= R_TRIG;
    }

    temp_v1 = gControllerSeven->buttonPressed & 0x1F0F;
    gControllerSeven->buttonPressed = (phi_a2 & (phi_a2 ^ D_80162D98)) | temp_v1;
    temp_v1 = gControllerSeven->buttonDepressed & 0x1F0F;
    gControllerSeven->buttonDepressed = (D_80162D98 & (phi_a2 ^ D_80162D98)) | temp_v1;
    D_80162D98 = phi_a2;
    gControllerSeven->button = phi_a2;
    if (D_80162D9C == 0) {
        D_80162DA0++;
        D_80162D9C = (s32) (D_80162DA4[D_80162DA0] & 0xFF0000);
    } else {
        D_80162D9C += (s32) 0xFFFF0000;
    }
}

void func_800057DC(void) {
    u32 temp_a0;
    u32 temp_v0;
    UNUSED u16 unk;
    u16 temp_v1;
    s16 phi_v1;
    s16 phi_a2 = 0;

    if (D_80162D90 >= 0x1000) {
        func_80005AE8(gPlayerTwo);
        return;
    }
    temp_a0 = sReplayGhostDecompressed[D_80162D90];
    temp_v0 = temp_a0 & 0xFF;
    if (temp_v0 < 0x80U) {
        phi_v1 = (s16) (temp_v0 & 0xFF);
    } else {
        phi_v1 = (s16) (temp_v0 | ~0xFF);
    }

    temp_v0 = (u32) (temp_a0 & 0xFF00) >> 8;

    gControllerSix->rawStickX = phi_v1;

    if (temp_v0 < 0x80U) {
        phi_v1 = (s16) (temp_v0 & 0xFF);
    } else {
        phi_v1 = (s16) (temp_v0 | (~0xFF));
    }

    gControllerSix->rawStickY = phi_v1;

    if (temp_a0 & 0x80000000) {
        phi_a2 |= A_BUTTON;
    }
    if (temp_a0 & 0x40000000) {
        phi_a2 |= B_BUTTON;
    }
    if (temp_a0 & 0x20000000) {
        phi_a2 |= Z_TRIG;
    }
    if (temp_a0 & 0x10000000) {
        phi_a2 |= R_TRIG;
    }
    temp_v1 = gControllerSix->buttonPressed & 0x1F0F;
    gControllerSix->buttonPressed = (phi_a2 & (phi_a2 ^ D_80162D88)) | temp_v1;

    temp_v1 = gControllerSix->buttonDepressed & 0x1F0F;
    gControllerSix->buttonDepressed = (D_80162D88 & (phi_a2 ^ D_80162D88)) | temp_v1;
    D_80162D88 = phi_a2;
    gControllerSix->button = phi_a2;

    if (D_80162D8C == 0) {
        D_80162D90++;
        D_80162D8C = (s32) (sReplayGhostDecompressed[D_80162D90] & 0xFF0000);
    } else {
        D_80162D8C += (s32) 0xFFFF0000;
    }
}

void func_8000599C(void) {
    s16 temp_a2;
    u32 phi_a3;
    u32 temp_v1;
    u32 temp_v2;
    u32 temp_v0;
    u32 temp_t0;
    u32 temp_a0_2;

    if (((D_80162DB8 >= 0x1000) || ((gPlayerOne->unk_0CA & 2) != 0)) || ((gPlayerOne->unk_0CA & 8) != 0)) {
        D_80162DF8 = 1;
        return;
    }

    temp_v1 = gControllerOne->rawStickX;
    temp_v1 &= 0xFF;
    temp_v2 = gControllerOne->rawStickY;
    temp_v2 = (temp_v2 & 0xFF) << 8;
    temp_a2 = gControllerOne->button;
    phi_a3 = 0;
    if (temp_a2 & 0x8000) {
        phi_a3 |= 0x80000000;
    }
    if (temp_a2 & 0x4000) {
        phi_a3 |= 0x40000000;
    }
    if (temp_a2 & 0x2000) {
        phi_a3 |= 0x20000000;
    }
    if (temp_a2 & 0x0010) {
        phi_a3 |= 0x10000000;
    }
    phi_a3 |= temp_v1;
    phi_a3 |= temp_v2;
    temp_t0 = D_80162DBC[D_80162DB8];
    temp_a0_2 = temp_t0 & 0xFF00FFFF;

    if ((*D_80162DBC) == 0xFFFFFFFF) {

        D_80162DBC[D_80162DB8] = phi_a3;

    } else if (temp_a0_2 == phi_a3) {

        temp_v0 = temp_t0 & 0xFF0000;

        if (temp_v0 == 0xFF0000) {

            D_80162DB8++;
            D_80162DBC[D_80162DB8] = phi_a3;

        } else {

            temp_t0 += 0x10000;
            D_80162DBC[D_80162DB8] = temp_t0;
        }
    } else {
        D_80162DB8++;
        D_80162DBC[D_80162DB8] = phi_a3;
    }
}

// sets player to AI? (unconfirmed)
void func_80005AE8(Player* ply) {
    if (((ply->type & PLAYER_INVISIBLE_OR_BOMB) != 0) && (ply != gPlayerOne)) {
        ply->type = PLAYER_CINEMATIC_MODE | PLAYER_START_SEQUENCE | PLAYER_KART_AI;
    }
}

void func_80005B18(void) {
    if (gModeSelection == TIME_TRIALS) {
        if ((gLapCountByPlayerId[0] == 3) && (D_80162DDC == 0) && (D_80162DF8 != 1)) {
            if (D_80162DD4 == 1) {
                D_80162DD0 = D_80162DCC;
                func_800052A4();
                D_80162DD4 = 0;
                D_80162DDC = 1;
                D_80162DE0 = gPlayerOne->characterId;
                D_80162DE8 = gPlayerOne->characterId;
                D_80162E00 = 0;
                D_80162DFC = playerHUD[PLAYER_ONE].someTimer;
                func_80005AE8(gPlayerTwo);
                func_80005AE8(gPlayerThree);
            } else if (gLapCountByPlayerId[1] != 3) {
                D_80162DD0 = D_80162DCC;
                func_800052A4();
                D_80162DDC = 1;
                D_80162DE0 = gPlayerOne->characterId;
                D_80162DFC = playerHUD[PLAYER_ONE].someTimer;
                D_80162E00 = 0;
                D_80162DE8 = gPlayerOne->characterId;
                func_80005AE8(gPlayerTwo);
                func_80005AE8(gPlayerThree);
            } else {
                sReplayGhostBuffer = D_802BFB80.arraySize8[0][D_80162DC8][3].pixel_index_array;
                sReplayGhostBufferSize = D_80162D86;
                D_80162DD0 = D_80162DCC;
                D_80162DE8 = gPlayerOne->characterId;
                D_80162DD8 = 0;
                D_80162DD4 = 0;
                D_80162DDC = 1;
                func_80005AE8(gPlayerTwo);
                func_80005AE8(gPlayerThree);
            }
        } else {
            if ((gLapCountByPlayerId[0] == 3) && (D_80162DDC == 0) && (D_80162DF8 == 1)) {
                sReplayGhostBuffer = D_802BFB80.arraySize8[0][D_80162DC8][3].pixel_index_array;
                sReplayGhostBufferSize = D_80162D86;
                D_80162DDC = 1;
            }
            if ((gPlayerOne->type & 0x800) == 0x800) {
                func_80005AE8(gPlayerTwo);
                func_80005AE8(gPlayerThree);
            } else {
                D_80162DEC += 1;
                if (D_80162DEC >= 0x65) {
                    D_80162DEC = 0x00000064;
                }
                if ((gModeSelection == TIME_TRIALS) && (gActiveScreenMode == SCREEN_MODE_1P)) {
                    if ((D_80162DD4 == 0) && (gLapCountByPlayerId[1] != 3)) {
                        func_800057DC();
                    }
                    if ((D_80162DD6 == 0) && (gLapCountByPlayerId[2] != 3)) {
                        func_8000561C();
                    }
                    if (!(gPlayerOne->type & 0x800)) {
                        func_8000599C();
                    }
                }
            }
        }
    }
}

void func_80005E6C(void) {
    if ((gModeSelection == TIME_TRIALS) && (gModeSelection == TIME_TRIALS) && (gActiveScreenMode == SCREEN_MODE_1P)) {
        if ((D_80162DD8 == 0) && (gLapCountByPlayerId[1] != 3)) {
            func_800057DC(); // 3
        }
        if ((D_80162DD6 == 0) && (gLapCountByPlayerId[2] != 3)) {
            func_8000561C(); // 2
        }
        if ((gPlayerOne->type & PLAYER_CINEMATIC_MODE) != PLAYER_CINEMATIC_MODE) {
            func_8000546C(); // 1
            return;
        }
        func_80005AE8(gPlayerTwo);
        func_80005AE8(gPlayerThree);
    }
}

void staff_ghosts_loop(void) {
    if (D_8015F890 == 1) {
        func_80005E6C();
        return;
    }
    if (!D_80162DF0) {
        func_80005B18();
        return;
    }
    D_80162DF8 = 1;
}
