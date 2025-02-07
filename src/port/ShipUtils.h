#ifndef SHIP_UTILS_H
#define SHIP_UTILS_H

#include <libultraship/libultraship.h>

#ifdef __cplusplus

void LoadGuiTextures();

extern "C" {
#endif

bool Ship_IsCStringEmpty(const char* str);

#ifdef __cplusplus
}
#endif

#endif // SHIP_UTILS_H
