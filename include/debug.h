#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <libultraship.h>
#include <defines.h>

/**
 * Debug Variable Display List (DVDL), display variables and address at runtime.
 * to enable it change DVDL to 1 or 0 to disable it
 * and compile with DEBUG=1 "make DEBUG=1".
 * To edit the variables displayed edit the structure array in src/debug/debug.inc.c.
 *
 * Additional features
 * Holding L + R + Z + A will start the creditis sequence.
 * Holding L + R + Z + B will start the ending sequence.
 */
#define DVDL 0
#if DVDL

#define CHARACTER_BUFFER_SIZE 200 // size of the string buffer

/**
 * This is the position of the displayed text.
 * It's calculated from the top left.
 */
#define TEXT_Y_POSSITION -0x8
#define TEXT_X_POSSITION -0x5

// flags
#define DISPLAY_DECIMAL_NUMBER 0x1
#define DISPLAY_HEXIDECIMAL_NUMBER 0x2
#define DISPLAY_OCTAL_NUMBER 0x4
#define DISPLAY_BINARY_NUMBER 0x8
#define DISPLAY_SIGNED_NUMBER 0x10
#define DISPLAY_FLOAT_AS_TYPE 0x20
#define DISPLAY_FLOAT_WITH_ROUNDING 0x40
#define DISPLAY_FLOAT_NUMBER 0x80

#define HEXIDECIMAL 16
#define DECIMAL 10
#define OCTAL 8
#define BINARY 2

/**
 * This structure is the heart of the DVDL.
 * only the first 4 attributes should be set by the user, the other 2 are used by the program.
 */
typedef struct {
    char* variableName;                 // name of the variable to be displayed
    void* variablePointer;              // pointer to the variable so it can display it
    u8 variableSize;                    // size of the variable to make up for templates from C++
    u8 variableFlag;                    // flag to change how the variable is displayed
    char buffer[CHARACTER_BUFFER_SIZE]; // when converting a variable to string this will be filled
    char* characterBuffer;              // silly pointer so i don't need to do stuff
} variableWatchAttributes;

/**
 * This structure array is what you edit to display in the list.
 * First index is the variable name, second is a pointer to the variable and
 * third is the size of the variable.
 *
 * initilized in debug/debug.inc.c
 */
extern variableWatchAttributes gMainVariableWatchList[];

/**
 * This is what calls the debug watch list. Because of how mk64 was programed,
 * It's called at 4 different parts of the code. One for menus, race,
 * ending sequence and creditis. It does not display during loading time because
 * the RSP is not initialized during that portion of the code.
 */
void display_dvdl(void);

#endif

#endif
