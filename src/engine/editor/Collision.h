#pragma once

#include <libultraship.h>
#include <libultra/gbi.h>

#include "EditorMath.h"

/**
 * @file Editor Collision
 * 
 * Most actors use cylinder collision. 
 * Proper vtx intersection tests are necessary for object picking
 * 
 * Therefore, generate a full collision mesh for actors
 */

#define EDITOR_GFX_GET_OPCODE(var) ((uint32_t) ((var) & 0xFF000000))

void GenerateCollisionMesh(GameObject& object, Gfx* model, float scale);
