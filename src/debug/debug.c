#include <libultraship.h>
#include <debug.h>
#include "debug.inc.c"
#include <libc/stdio.h>

#if DVDL

u8 sDisplayListState = OK;

static u32 variable_to_u64(variableWatchAttributes*);
static void round_up_float(u32*, u8);
static void u64_to_string(variableWatchAttributes*, u32, u8);
static u32 _strlen(const char*);
static void _memcpy(char*, const char*, u32);

void display_dvdl(void) {
    u32 variable;
    u32 i, vNameLen;
    u32 arraySize = sizeof(gMainVariableWatchList) / sizeof(variableWatchAttributes);
    s32 text_y_possition = TEXT_Y_POSSITION;
    u8 base;
    char *vName, *cBuffer;

    load_debug_font();
    for (i = 0; i < arraySize; i++) {
        variableWatchAttributes* currentAttribute = &gMainVariableWatchList[i];
        currentAttribute->characterBuffer = currentAttribute->buffer;
        cBuffer = currentAttribute->characterBuffer;
        vName = currentAttribute->variableName;

        variable = variable_to_u64(currentAttribute);
        base = 0;

        if (currentAttribute->variableFlag & (DISPLAY_FLOAT_AS_TYPE | DISPLAY_FLOAT_WITH_ROUNDING)) {
            if (currentAttribute->variableFlag & DISPLAY_FLOAT_NUMBER) {
                sDisplayListState = BAD;
            } else if (currentAttribute->variableFlag & DISPLAY_FLOAT_WITH_ROUNDING) {
                round_up_float(&variable, currentAttribute->variableSize);
            }
        }

        switch (currentAttribute->variableFlag & 0x8F) {
            case DISPLAY_FLOAT_NUMBER:
                sprintf(cBuffer, "%.3f", *(f32*) &variable);
                break;
            case DISPLAY_HEXIDECIMAL_NUMBER:
                base = HEXIDECIMAL;
                break;
            case DISPLAY_DECIMAL_NUMBER:
                base = DECIMAL;
                break;
            case DISPLAY_OCTAL_NUMBER:
                base = OCTAL;
                break;
            case DISPLAY_BINARY_NUMBER:
                base = BINARY;
                break;
            default:
                sDisplayListState = BAD;
        }
        if (base) {
            u64_to_string(currentAttribute, variable, base);
        }

        if (sDisplayListState == BAD) {
            cBuffer = "NaN";
        }
        sDisplayListState = OK;

        vNameLen = _strlen(vName);

        debug_print_str2(TEXT_X_POSSITION, text_y_possition, vName);
        debug_print_str2(TEXT_X_POSSITION + (vNameLen * 0x8), text_y_possition, cBuffer);
        text_y_possition += 0x8;
    }
    gSPDisplayList(gDisplayListHead++, D_0D007EB8);
    gSPDisplayList(gDisplayListHead++, D_020076E0);
    func_80093C98(1);
}

static u32 variable_to_u64(variableWatchAttributes* attribute) {
    u32 variable;

    switch (attribute->variableSize) {
        case sizeof(u8):
            variable = *((u8*) attribute->variablePointer);
            break;
        case sizeof(u16):
            variable = *((u16*) attribute->variablePointer);
            break;
        case sizeof(u32):
        case sizeof(u64):
            variable = *((u32*) attribute->variablePointer); // no floating point rounding
            break;
        default:
            sDisplayListState = BAD;
    }

    return variable;
}

static void round_up_float(u32* variable, u8 variableSize) {
    switch (variableSize) {
        case sizeof(u64):
        case sizeof(u32):
            *variable = (u32) (*(f32*) &*variable);
            break;
        default:
            sDisplayListState = BAD;
    }
}

static void u64_to_string(variableWatchAttributes* attribute, u32 variable, u8 base) {
    s32 signedVariable;
    u32 stringDifference, indexesToFillAVariable, stringLengthWithZero;
    u32 indexesToFillAByte, upperIndex, remainder;
    u32 stringLength, lowerIndex, i;
    u8 variableSize;
    char* bufferedString;
    char swapRegister;

    if (sDisplayListState == BAD) {
        return;
    }

    bufferedString = attribute->characterBuffer;
    variableSize = attribute->variableSize;

    // converts a minus number into it's unsigned equivalent for proper string conversion
    if (attribute->variableFlag & DISPLAY_SIGNED_NUMBER) {
        switch (variableSize) {
            case sizeof(u8):
                signedVariable = (s8) variable;
                if (signedVariable < 0) {
                    signedVariable = -signedVariable;
                    variable = (u8) signedVariable;
                    *bufferedString = '-';
                    bufferedString++;
                }
                break;
            case sizeof(u16):
                signedVariable = (s16) variable;
                if (signedVariable < 0) {
                    signedVariable = -signedVariable;
                    variable = (u16) signedVariable;
                    *bufferedString = '-';
                    bufferedString++;
                }
                break;
            case sizeof(u32):
            case sizeof(u64):
                signedVariable = (s32) variable;
                if (signedVariable < 0) {
                    signedVariable = -signedVariable;
                    variable = (u32) signedVariable;
                    *bufferedString = '-';
                    bufferedString++;
                }
                break;
        }
    }

    stringLength = 0;

    // convert u64 into a string but it gets put in reverse
    if (base != HEXIDECIMAL) {
        do {
            stringLength++;
            *bufferedString = variable % base + '0';
            bufferedString++;
            variable /= base;
        } while (variable != 0);
    } else {
        do {
            stringLength++;
            remainder = variable % 16;

            if ((remainder >= 0) && (remainder <= 9)) {
                *bufferedString = '0' + remainder;
            } else {
                *bufferedString = 'A' + (remainder - 10);
            }

            bufferedString++;
            variable /= 16;
        } while (variable != 0);
    }

    bufferedString -= stringLength;
    upperIndex = stringLength - 1;

    // flip string 4321 --> 1234
    for (lowerIndex = 0; lowerIndex < stringLength >> 1; lowerIndex++) {
        swapRegister = bufferedString[lowerIndex];
        bufferedString[lowerIndex] = bufferedString[upperIndex];
        bufferedString[upperIndex] = swapRegister;
        upperIndex--;
    }

    switch (base) {
        case BINARY:
            indexesToFillAByte = 8;
            break;
        case OCTAL:
            indexesToFillAByte = 3;
            break;
        case DECIMAL:
            bufferedString[stringLength] = '\0';
            return;
        case HEXIDECIMAL:
            indexesToFillAByte = 2;
            break;
    }

    stringLengthWithZero = stringLength - 1;
    indexesToFillAVariable = indexesToFillAByte * variableSize;
    stringDifference = indexesToFillAVariable - stringLengthWithZero;

    // makes space and fills in extra space. u32 FE -> 000000FE
    for (i = stringLength; i > 0; i--) {
        bufferedString[stringDifference + i - 2] = bufferedString[i - 1];
    }
    for (i = 0; i < stringDifference - 1; i++) {
        bufferedString[i] = '0';
    }
    bufferedString[indexesToFillAVariable] = '\0';
}

static u32 _strlen(const char* str) {
    u32 len;

    len = 0;
    for (; *str != '\0'; str++) {
        len++;
    }

    return len;
}

// unused but may be useful
static void _memcpy(char* destStr, const char* copyStr, u32 copySize) {
    u32 i;

    for (i = 0; i < copySize; i++) {
        *destStr = *copyStr;
        destStr++;
        copyStr++;
    }
}

#endif
