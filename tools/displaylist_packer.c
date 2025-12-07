#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/PR/gbi.h"

/* Fallbacks for symbols that may be hidden behind ucode/asm guards in gbi.h */

#define MAX_STRING_LENGTH 256
#define MAX_STRING_OUTPUT 10000

/* Named constants to replace magic comparisons */
/* gSPSetOtherModeL presets (low 16 bits), rewritten using gbi.h flags */
/* 0x2078 = AA_EN | Z_CMP | Z_UPD | IM_RD | ALPHA_CVG_SEL */
/* Explicit name for readability: Anti-Aliasing + Z Compare/Update + Image Read + Alpha Coverage Select */
#define OML_AA_ZCMP_ZUPD_IMRD_ALPHA_CVG_SEL (AA_EN | Z_CMP | Z_UPD | IM_RD | ALPHA_CVG_SEL)
/* 0x3078 = AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_X_ALPHA | ALPHA_CVG_SEL */
/* Explicit name: same as above, with Coverage x Alpha enabled */
#define OML_AA_ZCMP_ZUPD_IMRD_CVG_X_ALPHA_ALPHA_CVG_SEL (AA_EN | Z_CMP | Z_UPD | IM_RD | CVG_X_ALPHA | ALPHA_CVG_SEL)

/* gSPTexture parameters (low 16 bits) */
#define TEX_PARAM_ON  0x0001
#define TEX_PARAM_OFF 0xFFFF

/* gDPSetCombine presets (low 16 bits).
 * Nommage en fonction des macros GBI G_CC_* utilisées côté runtime (memory.c):
 *  - 0xFFFF → G_CC_MODULATERGBA
 *  - 0xF3F9 → G_CC_MODULATERGBDECALA
 *  - 0x793C → G_CC_SHADE
 *  - 0xFFFD → G_CC_DECALRGBA
 */
#define COMB_CC_MODULATERGBA        0xFFFF
#define COMB_CC_MODULATERGBDECALA   0xF3F9
#define COMB_CC_SHADE               0x793C
#define COMB_CC_DECALRGBA           0xFFFD
/* Variante « ALT » vue dans certains exports (reste une heuristique) */
#define COMB_PRESET_ALT1            0xF9FC
/* (aliases retirés pour éviter la confusion et favoriser les noms G_CC_ explicites) */

/* gDPSetTextureImage quirks (kept for context; not used after gbi refactor) */
/* #define SETTIMG_P4_EXPECT 0x70 */
/* #define LOADBLOCK_CMD_OR  0x300000000ULL */

/* Segmented base addresses (common segment bases used in MK64) */
#define SEG_BASE_4 0x04000000ULL
#define SEG_BASE_5 0x05000000ULL

/* G_LOADBLOCK variants are detected by decoding tile/lrs/dxt */

/* Tile/size configurations are detected by decoding SETTILE/SETTILESIZE */

/*
 * Non standard/special input opcode observed (0xD0):
 * Note: There is no official PR/gbi.h opcode for 0xD0. Keep this as a
 * project-specific remap for rare/unknown commands encountered in inputs.
 */
#define G_OP_D0 0xD0

// Compile using gcc -o tools/displaylist_packer tools/displaylist_packer.c
// Run using ./displaylist_packer input.bin output.bin

void pack(FILE *input_file, FILE *output_file);

int main(int argc, char *argv[]) {
    // Check if we have the correct number of arguments
    if (argc != 3) {
        printf("Usage: ./dl_unpack input.bin output.bin\n");
        exit(1);
    }

    // Open the input file
    FILE *input_file = fopen(argv[1], "rb");
    if (input_file == NULL) {
        printf("Failed to open input file: %s\n", argv[1]);
        exit(1);
    }

    // Create the output file
    FILE *output_file = fopen(argv[2], "wb");
    if (output_file == NULL) {
        printf("Failed to create output file: %s\n", argv[2]);
        exit(1);
    }

    pack(input_file, output_file);
    return 0;
}

uint64_t swap_endian(uint64_t value) {
    uint64_t result = 0;
    int i;
    for (i = 0; i < 8; i++) {
        result = (result << 8) | ((value >> (i * 8)) & 0xFF);
    }
    return result;
}

uint32_t compressB1(uint8_t a, uint8_t b, uint8_t c) {
    return (a / 2) | ((b / 2) << 5) | ((c / 2) << 10);
}

#define ARG1(val) ((val) >> 48)
#define ARG1WORD(val) ((val) >> 32)
#define ARG2(val) ((val) >> 40)
#define ARG2WORD(val) ((val) >> 32)
#define OPCODE(val) (uint8_t)((val) >> 56)

enum packed_op {
    /* Set combine presets (renommés en fonction des macros G_CC_*) */
    PG_SETCOMBINE_CC_MODULATERGBA      = 0x15, /* ex-PG_SETCOMBINE_PRESET_FFFF */
    PG_SETCOMBINE_CC_MODULATERGBDECALA = 0x16, /* ex-PG_SETCOMBINE_PRESET_F3F9 */
    PG_SETCOMBINE_CC_SHADE             = 0x17, /* ex-PG_SETCOMBINE_PRESET_793C */

    /* Render modes via gSPSetOtherModeL shortcuts (parité memory.c) */
    PG_RMODE_OPA     = 0x18, /* ex-PG_SETOTHERMODE_L_2078 */
    PG_RMODE_TEXEDGE = 0x19, /* ex-PG_SETOTHERMODE_L_3078 */

    /* Tile/tile size configurations following RDPTILESYNC+SETTILE+SETTILESIZE */
    PG_TILECFG_A = 0x1A, /* 0xF51010000007C07C */
    PG_TILECFG_B = 0x1B, /* 0xF5102000000FC07C */
    PG_TILECFG_C = 0x1C, /* 0xF51010000007C0FC */
    PG_TILECFG_D = 0x1D, /* 0xF57010000007C07C */
    PG_TILECFG_E = 0x1E, /* 0xF5702000000FC07C */
    PG_TILECFG_F = 0x1F, /* 0xF57010000007C0FC */

    /* Texture loadblock variants after gDPSetTextureImage */
    PG_TIMG_LOADBLOCK_0 = 0x20, /* 0xF3000000073FF100 */
    PG_TIMG_LOADBLOCK_1 = 0x21, /* 0xF3000000077FF080 */
    PG_TIMG_LOADBLOCK_2 = 0x22, /* 0xF3000000077FF100 */
    PG_TIMG_LOADBLOCK_3 = 0x23, /* 0xF3000003073FF100 */
    PG_TIMG_LOADBLOCK_4 = 0x24, /* 0xF3000003077FF080 */
    PG_TIMG_LOADBLOCK_5 = 0x25, /* 0xF3000003077FF100 */

    /* gSPTexture on/off */
    PG_TEXTURE_OFF = 0x26,
    PG_TEXTURE_ON  = 0x27,

    /* VTX compact op (bank/count encoded) */
    PG_VTX1  = 0x28,

    /* Display list ops */
    PG_TRI1  = 0x29,
    PG_ENDDL = 0x2A,
    PG_DL    = 0x2B,
    PG_TILECFG_G = 0x2C, /* 0xF51011000007C07C */
    PG_CULLDL = 0x2D,

    /* Alternate combine preset used in memory.c */
    PG_SETCOMBINE_ALT = 0x2E,

    /* XLU render mode (not currently emitted) */
    PG_RMODE_XLU = 0x2F,

    /* Project-specific primitive (spline), not currently emitted here */
    PG_SPLINE3D = 0x30,

    /* Vertex packet base (computed as base + bank index) */
    PG_VTX_BASE = 0x32,

    /* Extra combine preset used by memory.c */
    PG_SETCOMBINE_CC_DECALRGBA = 0x53, /* ex-PG_SETCOMBINE_DECALRGBA */

    /* Render mode decal variants (not currently emitted) */
    PG_RMODE_OPA_DECAL = 0x54,
    PG_RMODE_XLU_DECAL = 0x55,

    /* Geometry mode */
    PG_SETGEOMETRYMODE   = 0x56,
    PG_CLEARGEOMETRYMODE = 0x57,

    /* Triangle pair */
    PG_TRI2 = 0x58,

    /* Special remap */
    PG_D0_REMAP = 0xDD,

    /* Fallback/terminators */
    PG_UNKNOWN = 0xEE,
    PG_EOF     = 0xFF,
};

void pack(FILE *input_file, FILE *output_file) {

    // Initialize the string to an empty string

    uint8_t p1;
    uint8_t p2;
    uint8_t p3;
    /* temporary vars */
    uint16_t p7;
    

    // Read every u32 in the input file and concatenate a string based on the value
    uint64_t cmd;
    uint8_t opCode;
    uint32_t offset = 0;
    uint32_t count = 0;
    // Warning: Static variable size may result in overflow if input file is too large.
    // Solution: Increase array size.
    uint8_t data[50000];
    int debug = getenv("DLPACKER_DEBUG") != NULL;
    int lb_bias = 0; /* 0 for RGBA (A/B/C/G), 3 for IA (D/E/F) */
    while (fread(&cmd, sizeof(uint64_t), 1, input_file) == 1) {
        cmd = swap_endian(cmd);
        opCode = OPCODE(cmd);
        //printf("%X \n", opCode);
        switch (opCode) {
            case (uint8_t)G_SETOTHERMODE_L:
                p7 = (uint16_t) cmd;
                if (p7 == OML_AA_ZCMP_ZUPD_IMRD_ALPHA_CVG_SEL) {
                    data[count++] = PG_RMODE_OPA;
                    if (debug) fprintf(stderr, "@%u PG_RMODE_OPA\n", count-1);
                } else if (p7 == OML_AA_ZCMP_ZUPD_IMRD_CVG_X_ALPHA_ALPHA_CVG_SEL) {
                    data[count++] = PG_RMODE_TEXEDGE;
                    if (debug) fprintf(stderr, "@%u PG_RMODE_TEXEDGE\n", count-1);
                } else if ((p7 & ZMODE_XLU) == ZMODE_XLU) {
                    /* Any render mode with ZMODE_XLU set maps to translucent; refine as DECAL if ALPHA_CVG_SEL is set */
                    if ((p7 & ALPHA_CVG_SEL) == ALPHA_CVG_SEL) {
                        data[count++] = PG_RMODE_XLU_DECAL;
                        if (debug) fprintf(stderr, "@%u PG_RMODE_XLU_DECAL (ZMODE_XLU|ALPHA_CVG_SEL)\n", count-1);
                    } else {
                        data[count++] = PG_RMODE_XLU;
                        if (debug) fprintf(stderr, "@%u PG_RMODE_XLU (ZMODE_XLU)\n", count-1);
                    }
                } else if ((p7 & ALPHA_CVG_SEL) == ALPHA_CVG_SEL) {
                    /* Opaque decal variant (no ZMODE_XLU, but with ALPHA_CVG_SEL) */
                    data[count++] = PG_RMODE_OPA_DECAL;
                    if (debug) fprintf(stderr, "@%u PG_RMODE_OPA_DECAL (ALPHA_CVG_SEL)\n", count-1);
                }
                break;
            case (uint8_t)G_TRI1:
                data[count++] = PG_TRI1;
                if (debug) fprintf(stderr, "@%u PG_TRI1\n", count-1);
                p1 = (uint8_t) (cmd >> 16) / 2;
                p2 = (uint8_t) (cmd >> 8) / 2;
                p3 = (uint8_t) (cmd) / 2;

                *(uint16_t*) (data + count) = (uint16_t) (p1 | (p2 << 5) | (p3 << 10));
                count++; count++;
                break;
            case G_DL:
                data[count++] = PG_DL;
                if (debug) fprintf(stderr, "@%u PG_DL\n", count-1);
                *(uint16_t*) (data + count) = (uint16_t)(((uint32_t)cmd) / 8);
                count++; count++;
                break;
            case (uint8_t)G_QUAD: {
                /* Pack MK64 quad into compact SPLINE3D triplet, mirroring unpack_spline_3D expectations */
                data[count++] = PG_SPLINE3D;
                if (debug) fprintf(stderr, "@%u PG_SPLINE3D\n", count-1);
                uint32_t w1 = (uint32_t)cmd;
                uint8_t a0 = (uint8_t)(w1 >> 24) / 2;
                uint8_t t0 = (uint8_t)(w1 >> 16) / 2;
                uint8_t a3 = (uint8_t)(w1 >> 8)  / 2;
                uint8_t a2 = (uint8_t)(w1)       / 2;
                /* Build three packed bytes (non-mirror path in memory.c):
                 * P0: [a3 bits2:0]<<5 | (t0 & 0x1F)
                 * P1: [a0 bit0]<<7 | (a2 & 0x1F)<<2 | [a3 bits4:3]
                 * P2: (a0 >> 1) & 0x0F
                 */
                uint8_t P0 = ((a3 & 0x7) << 5) | (t0 & 0x1F);
                uint8_t P1 = ((a0 & 0x1) << 7) | ((a2 & 0x1F) << 2) | ((a3 >> 3) & 0x3);
                uint8_t P2 = (a0 >> 1) & 0x0F;
                data[count++] = P0;
                data[count++] = P1;
                data[count++] = P2;
                if (debug) fprintf(stderr, "    quad a0=%u t0=%u a3=%u a2=%u -> P0=%02X P1=%02X P2=%02X\n", a0, t0, a3, a2, P0, P1, P2);
                break;
            }
            case (uint8_t)G_TRI2:
                data[count++] = PG_TRI2;
                if (debug) fprintf(stderr, "@%u PG_TRI2\n", count-1);
                *(uint16_t*) (data + count) = compressB1(ARG1(cmd), ARG2(cmd), cmd >> 32);
                count++; count++;
                *(uint16_t*) (data + count) = compressB1(cmd >> 16, cmd >> 8, cmd);
                count++; count++;
                break;
            case G_VTX:
                // Skip the opcode and read bytes 2/3 from the u64 (Byte 1 is the opcode 0x04).
                data[count++] = (uint8_t)(((((uint16_t)ARG1WORD(cmd)) + 1) / 0x410) + PG_VTX_BASE);
                if (debug) fprintf(stderr, "@%u PG_VTX_%u\n", count-1, data[count-1]-PG_VTX_BASE);
                // Read the right side of the u64 (as a u32).
                *(uint16_t*) (data + count) = (uint16_t)(((uint32_t)cmd - (uint32_t)SEG_BASE_4) / 16);
                count++; count++;
                break;
            case G_SETTIMG: {
                /* Texture image parameters (usually segment 0x05) */
                p1 = (uint32_t)(cmd - SEG_BASE_5) >> 11; /* address >> 11, compacted */
                p2 = 0x00; /* reserved in the existing packed format */
                p3 = 0x70; /* expected format/siz on the packed side */

                /* Look ahead for the corresponding G_LOADBLOCK, consuming SETTILE/LOADSYNC */
                {
                    uint64_t next;
                    int found = 0;
                    for (int i = 0; i < 12; i++) {
                        if (fread(&next, sizeof(uint64_t), 1, input_file) != 1) {
                            printf("Error: Unexpected EOF scanning after G_SETTIMG\n");
                            break;
                        }
                        next = swap_endian(next);
                        uint8_t nop = OPCODE(next);
                        if (nop == (uint8_t)G_LOADBLOCK) {
                            cmd = next;
                            found = 1;
                            break;
                        }
                        if (nop == (uint8_t)G_SETTILE || nop == (uint8_t)G_RDPLOADSYNC || nop == (uint8_t)G_RDPPIPESYNC) {
                            continue; /* consumed, keep scanning */
                        }
                        /* Unexpected: keep scanning a few opcodes anyway */
                    }
                    if (!found) {
                        printf("Error: Did not find G_LOADBLOCK after G_SETTIMG\n");
                        /* Emit a default value and exit this case */
                        data[count++] = PG_TIMG_LOADBLOCK_0;
                        data[count++] = p1;
                        data[count++] = p2;
                        data[count++] = p3;
                        break;
                    }
                }

                /* Classify by (lrs,dxt) then apply a bias from the last TILECFG (RGBA=+0, IA=+3) */
                {
                    uint32_t lb_w0 = (uint32_t)(cmd >> 32);
                    uint32_t lb_w1 = (uint32_t)(cmd);
                    uint16_t uls = (lb_w0 >> 12) & 0xFFF;
                    uint16_t ult = (lb_w0 >> 0) & 0xFFF;
                    uint8_t  tile = (lb_w1 >> 24) & 0x7;
                    uint16_t lrs = (lb_w1 >> 12) & 0xFFF;
                    uint16_t dxt = (lb_w1 >> 0) & 0xFFF;
                    int base = -1;
                    if (tile == G_TX_LOADTILE && uls == 0 && (ult == 0 || ult == 3)) {
                        if (lrs == 0x3FF && dxt == 0x0100) base = 0;
                        else if (lrs == 0x7FF && dxt == 0x0080) base = 1;
                        else if (lrs == 0x7FF && dxt == 0x0100) base = 2;
                    }
                    if (base < 0) {
                        printf("Error: Unrecognized/Unexpected G_LOADBLOCK (tile=%u uls=%u ult=%u lrs=%03X dxt=%03X)\n", tile, uls, ult, lrs, dxt);
                        base = 0;
                    }
                    uint8_t variant = (uint8_t)(PG_TIMG_LOADBLOCK_0 + base + lb_bias);
                    data[count++] = variant;
                    if (debug) fprintf(stderr, "@%u PG_TIMG_LOADBLOCK_%d (base=%d,bias=%d) cmd=%016llX\n", count-1, (int)(variant-PG_TIMG_LOADBLOCK_0), base, lb_bias, (unsigned long long)cmd);
                }

                data[count++] = p1;
                data[count++] = p2;
                data[count++] = p3;
                break;
            }
            case G_RDPTILESYNC: {
                /* Read G_SETTILE (0xF5) */
                if (fread(&cmd, sizeof(uint64_t), 1, input_file) != 1) {
                    printf("Error: Unexpected EOF after G_RDPTILESYNC (SETTILE)\n");
                    break;
                }
                cmd = swap_endian(cmd);
                if ((uint8_t)OPCODE(cmd) != (uint8_t)G_SETTILE) {
                    printf("Error: Expected G_SETTILE after G_RDPTILESYNC, got 0x%02X\n", OPCODE(cmd));
                    break;
                }

                uint32_t st_w0 = (uint32_t)(cmd >> 32);
                uint32_t st_w1 = (uint32_t)cmd;
                uint8_t fmt   = (st_w0 >> 21) & 0x7;
                uint8_t siz   = (st_w0 >> 19) & 0x3;
                uint16_t line = (st_w0 >> 9) & 0x1FF;
                uint16_t tmem = (st_w0 >> 0) & 0x1FF;
                uint8_t tile  = (st_w1 >> 24) & 0x7;
                uint8_t pal   = (st_w1 >> 20) & 0xF;
                uint8_t cmt   = (st_w1 >> 18) & 0x3;
                uint8_t maskt = (st_w1 >> 14) & 0xF;
                uint8_t shiftt= (st_w1 >> 10) & 0xF;
                uint8_t cms   = (st_w1 >> 8)  & 0x3;
                uint8_t masks = (st_w1 >> 4)  & 0xF;
                uint8_t shifts= (st_w1 >> 0)  & 0xF;

        /* p1/p2 stay as before (re-encodes 2 nibbles of clamp/mirror flags) */
                p1 = (((cmd >> 14) & 0xF) << 4) | ((cmd >> 18) & 0xF); /* shiftt|cmt */
                p2 = (((cmd >> 4) & 0xF) << 4) | ((cmd >> 8) & 0xF);   /* masks|cms */

        /* Read G_SETTILESIZE (0xF2), skipping syncs if present */
                {
                    int found = 0;
                    for (int i = 0; i < 6; i++) {
                        if (fread(&cmd, sizeof(uint64_t), 1, input_file) != 1) {
                            printf("Error: Unexpected EOF after G_SETTILE (SETTILESIZE)\n");
                            break;
                        }
                        cmd = swap_endian(cmd);
                        uint8_t op2 = OPCODE(cmd);
                        if (op2 == (uint8_t)G_SETTILESIZE) { found = 1; break; }
                        if (op2 == (uint8_t)G_RDPLOADSYNC || op2 == (uint8_t)G_RDPPIPESYNC) {
                            continue;
                        }
            /* something else: stop */
                        break;
                    }
                    if (!found) {
                        printf("Error: Expected G_SETTILESIZE after G_SETTILE, got 0x%02X\n", OPCODE(cmd));
                        break;
                    }
                }

                uint32_t ss_w0 = (uint32_t)(cmd >> 32);
                uint32_t ss_w1 = (uint32_t)cmd;
                uint16_t uls = (ss_w0 >> 12) & 0xFFF;
                uint16_t ult = (ss_w0 >> 0)  & 0xFFF;
                /* ss_tile not used in the current packed format */
                uint16_t lrs = (ss_w1 >> 12) & 0xFFF;
                uint16_t lrt = (ss_w1 >> 0)  & 0xFFF;

                    /* Classification A..G based on (fmt,siz) and (lrs,lrt), without constraining cmt/cms/masks/line/etc. */
                if (fmt == G_IM_FMT_RGBA && lrs == 0x07C && lrt == 0x07C) {
                                        /* G-variant: legacy hint only (w0 byte[1] == 0x11). Do not rely on p1/p2 mask/shift patterns. */
                    uint8_t w0_b1 = (uint8_t)((st_w0 >> 8) & 0xFF);
                    if (debug) fprintf(stderr, "#TILE RGBA7C7C st_w0=%08X ss_w1=%08X p2=%02X p1=%02X w0_b1=%02X\n", st_w0, ss_w1, p2, p1, w0_b1);
                    if (w0_b1 == 0x11) {
                        data[count++] = PG_TILECFG_G; /* legacy G variant */
                        if (debug) fprintf(stderr, "@%u PG_TILECFG_G p2=%02X p1=%02X\n", count-1, p2, p1);
                    } else {
                        data[count++] = PG_TILECFG_A; /* default RGBA16 tile */
                        if (debug) fprintf(stderr, "@%u PG_TILECFG_A p2=%02X p1=%02X\n", count-1, p2, p1);
                    }
                    lb_bias = 0;
                } else if (fmt == G_IM_FMT_RGBA && lrs == 0x0FC && lrt == 0x07C) {
                    data[count++] = PG_TILECFG_B; /* F5102000000FC07C */
                    if (debug) fprintf(stderr, "@%u PG_TILECFG_B p2=%02X p1=%02X\n", count-1, p2, p1);
                    lb_bias = 0;
                } else if (fmt == G_IM_FMT_RGBA && lrs == 0x07C && lrt == 0x0FC) {
                    data[count++] = PG_TILECFG_C; /* F51010000007C0FC */
                    if (debug) fprintf(stderr, "@%u PG_TILECFG_C p2=%02X p1=%02X\n", count-1, p2, p1);
                    lb_bias = 0;
                } else if (fmt == G_IM_FMT_IA && lrs == 0x07C && lrt == 0x07C) {
                    data[count++] = PG_TILECFG_D; /* F57010000007C07C */
                    if (debug) fprintf(stderr, "@%u PG_TILECFG_D p2=%02X p1=%02X\n", count-1, p2, p1);
                    lb_bias = 3;
                } else if (fmt == G_IM_FMT_IA && lrs == 0x0FC && lrt == 0x07C) {
                    data[count++] = PG_TILECFG_E; /* F5702000000FC07C */
                    if (debug) fprintf(stderr, "@%u PG_TILECFG_E p2=%02X p1=%02X\n", count-1, p2, p1);
                    lb_bias = 3;
                } else if (fmt == G_IM_FMT_IA && lrs == 0x07C && lrt == 0x0FC) {
                    data[count++] = PG_TILECFG_F; /* F57010000007C0FC */
                    if (debug) fprintf(stderr, "@%u PG_TILECFG_F p2=%02X p1=%02X\n", count-1, p2, p1);
                    lb_bias = 3;
                } else {
                            /* Unrecognized: choose A by default (rare) and trace in debug */
                    if (debug) fprintf(stderr, "@%u PG_TILECFG_A (default) fmt=%u siz=%u line=%u tmem=%u tile=%u pal=%u cmt=%u cms=%u uls=%03X ult=%03X lrs=%03X lrt=%03X maskt=%u masks=%u shiftt=%u shifts=%u\n",
                        count, fmt, siz, line, tmem, tile, pal, cmt, cms, uls, ult, lrs, lrt, maskt, masks, shiftt, shifts);
                    data[count++] = PG_TILECFG_A; lb_bias = 0;
                }

                                /* Keep p2/p1 as historically encoded */
                data[count++] = p2;
                data[count++] = p1;
                break;
            }
            case (uint8_t)G_TEXTURE: {
                                /* Parity with the legacy packer: primary test on t (low16 of w1) */
                uint16_t t = (uint16_t)cmd;
                if (t == 0x0001) {
                    data[count++] = PG_TEXTURE_ON;
                    if (debug) fprintf(stderr, "@%u PG_TEXTURE_ON (t==0001)\n", count-1);
                    break;
                }
                if (t == 0xFFFF) {
                    data[count++] = PG_TEXTURE_OFF;
                    if (debug) fprintf(stderr, "@%u PG_TEXTURE_OFF (t==FFFF)\n", count-1);
                    break;
                }
                                /* Fallback: decode the 'on' flag via gbi.h for robustness */
                uint32_t w0 = (uint32_t)(cmd >> 32);
                uint8_t on8 = (uint8_t)(w0 & 0xFF);
                uint8_t on7 = (uint8_t)((w0 >> 1) & 0x7F);
                data[count++] = ((on8 != 0) || (on7 != 0)) ? PG_TEXTURE_ON : PG_TEXTURE_OFF;
                if (debug) fprintf(stderr, "@%u PG_TEXTURE_%s (decoded) on8=%u on7=%u\n", count-1, (((on8!=0)||(on7!=0))?"ON":"OFF"), on8, on7);
                break;
            }
            case (uint8_t)G_ENDDL:
                data[count++] = PG_ENDDL;
                if (debug) fprintf(stderr, "@%u PG_ENDDL\n", count-1);
                break;
            case (uint8_t)G_RDPLOADSYNC: /* 0xE6 */
            case (uint8_t)G_RDPPIPESYNC: /* 0xE7 */
                /* ignored in the packed stream */
                break;
            case (uint8_t)G_LOADBLOCK: /* Fallback: if encountered outside SETTIMG flow, ignore */
                break;
            case (uint8_t)G_CULLDL:
                data[count++] = PG_CULLDL;
                if (debug) fprintf(stderr, "@%u PG_CULLDL\n", count-1);
                break;
            case G_OP_D0:
                data[count++] = PG_D0_REMAP;
                if (debug) fprintf(stderr, "@%u PG_D0_REMAP\n", count-1);
                break;
            case G_SETCOMBINE:
                p7 = (uint16_t)cmd;
                if (p7 == COMB_CC_MODULATERGBDECALA) {
                    data[count++] = PG_SETCOMBINE_CC_MODULATERGBDECALA; /* was: PRESET_F3F9 */
                    if (debug) fprintf(stderr, "@%u PG_SETCOMBINE_PRESET_F3F9\n", count-1);
                } else if (p7 == COMB_CC_MODULATERGBA) {
                    data[count++] = PG_SETCOMBINE_CC_MODULATERGBA; /* was: PRESET_FFFF */
                    if (debug) fprintf(stderr, "@%u PG_SETCOMBINE_PRESET_FFFF\n", count-1);
                } else if (p7 == COMB_CC_SHADE) {
                    data[count++] = PG_SETCOMBINE_CC_SHADE; /* was: PRESET_793C */
                    if (debug) fprintf(stderr, "@%u PG_SETCOMBINE_PRESET_793C\n", count-1);
                } else if (p7 == COMB_CC_DECALRGBA) {
                    data[count++] = PG_SETCOMBINE_CC_DECALRGBA;
                    if (debug) fprintf(stderr, "@%u PG_SETCOMBINE_DECALRGBA\n", count-1);
                } else if (p7 == COMB_PRESET_ALT1) {
                    data[count++] = PG_SETCOMBINE_ALT;
                    if (debug) fprintf(stderr, "@%u PG_SETCOMBINE_ALT\n", count-1);
                }
                //data[count++] = 0x53;
                break;
            case (uint8_t)G_SETGEOMETRYMODE:
                data[count++] = PG_SETGEOMETRYMODE;
                if (debug) fprintf(stderr, "@%u PG_SETGEOMETRYMODE\n", count-1);
                break;
            case (uint8_t)G_CLEARGEOMETRYMODE:
                data[count++] = PG_CLEARGEOMETRYMODE;
                if (debug) fprintf(stderr, "@%u PG_CLEARGEOMETRYMODE\n", count-1);
                break;
            //case 0xFF:
            //    data[count++] = 0xFF;
            //    goto eos; // end of switch
            //    break;
            default:
                printf("Error: Unknown Opcode: 0x%X\n", opCode);
                printf("Opcode written to file as 0xEE\n");
                data[count++] = PG_UNKNOWN;
                break;
        }

        offset += 4;
    }
    //eos: ;
    data[count++] = PG_EOF;
    size_t num_elements_written = fwrite(data, sizeof(uint8_t), count, output_file);
    if (num_elements_written != count) {
        printf("Failed to write data to file.\n");
        exit(1);
    }

    // Close the files and free the memory
    fclose(input_file);
    fclose(output_file);
}
