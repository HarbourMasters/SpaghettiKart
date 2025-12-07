#include "Collision.h"

#include <libultraship/libultraship.h>
#include <libultra/gbi.h>
#include "Matrix.h"

#include "engine/Actor.h"
#include "engine/objects/Object.h"
#include "engine/editor/GameObject.h"

extern "C" {
#include "main.h"
#include "assets/textures/other_textures.h"
}

namespace Editor {
    void GenerateCollisionMesh(std::variant<AActor*, OObject*, GameObject*> object, Gfx* model, float scale) {
        int8_t opcode;
        uintptr_t lo;
        uintptr_t hi;

        Gfx* ptr = model;
        Vtx* vtx = NULL;
        size_t i = 0;
        bool run = true;

        //! @attention Objects will not be clickable if editor is enabled mid-race.
        if (CVarGetInteger("gEditorEnabled", false) == true) {
            return;
        }

        while (run) {
            i++;
            lo = ptr->words.w0;
            hi = ptr->words.w1;

            opcode = (EDITOR_GFX_GET_OPCODE(lo) >> 24);
            switch(opcode) {
                case G_DL:
                    GenerateCollisionMesh(object, (Gfx*)hi, scale);
                    break;
                case G_DL_OTR_HASH:
                    ptr++;
                    GenerateCollisionMesh(object, (Gfx*)ResourceGetDataByCrc(((uint64_t)(ptr->words.w0 << 32)) + ptr->words.w1), scale);
                    break;
                case G_DL_OTR_FILEPATH:
                   // printf("otr filepath: %s\n", (const char*)hi);
                    GenerateCollisionMesh(object, (Gfx*)ResourceGetDataByName((const char*)hi), scale);
                    break;
                case G_VTX:
                    vtx = (Vtx*)ptr->words.w1;
                    break;
                case G_VTX_OTR_HASH: {
                    ptr++;
                    vtx = (Vtx*)ResourceGetDataByCrc(((uint64_t)(ptr->words.w0 << 32)) + ptr->words.w1);
                    break;
                }
                case G_VTX_OTR_FILEPATH: {
                    const char* filePath = (const char*)hi;
                    ptr++;
                    size_t vtxDataOff = ptr->words.w1 & 0xFFFF;
                    vtx = ( (Vtx*)ResourceGetDataByName(filePath) ) + vtxDataOff;
                    break;
                }
                case G_TRI1: {
                    if (vtx == NULL) {
                        ptr++;
                        continue;
                    }
                    uint32_t v1 = ((hi & 0x00FF0000) >> 16) / 2;
                    uint32_t v2 = ((hi & 0x0000FF00) >> 8) / 2;
                    uint32_t v3 = (hi & 0x000000FF) / 2;

                    FVector p1 = FVector(vtx[v1].v.ob[0], vtx[v1].v.ob[1], vtx[v1].v.ob[2]);
                    FVector p2 = FVector(vtx[v2].v.ob[0], vtx[v2].v.ob[1], vtx[v2].v.ob[2]);
                    FVector p3 = FVector(vtx[v3].v.ob[0], vtx[v3].v.ob[1], vtx[v3].v.ob[2]);
                    std::visit([p1, p2, p3](auto* obj) {
                        if (obj) {
                            obj->Triangles.push_back({p1, p2, p3});
                        }
                    }, object);
                    break;
                }
                case G_TRI1_OTR: {
                    if (vtx == NULL) {
                        ptr++;
                        continue;
                    }

                    // The shift values here are different than the above. No idea why. But it has to be this way.
                    uint32_t v1 = (lo & 0x0000FFFF);
                    uint32_t v2 = (hi >> 16);
                    uint32_t v3 = (hi & 0x0000FFFF);

                    FVector p1 = FVector(vtx[v1].v.ob[0], vtx[v1].v.ob[1], vtx[v1].v.ob[2]);
                    FVector p2 = FVector(vtx[v2].v.ob[0], vtx[v2].v.ob[1], vtx[v2].v.ob[2]);
                    FVector p3 = FVector(vtx[v3].v.ob[0], vtx[v3].v.ob[1], vtx[v3].v.ob[2]);
                    std::visit([p1, p2, p3](auto* obj) {
                        if (obj) {
                            obj->Triangles.push_back({p1, p2, p3});
                        }
                    }, object);
                    break;
                }
                case G_TRI2: {
                    if (vtx == NULL) {
                        ptr++;
                        continue;
                    }
                    uint32_t v1 = ((lo & 0x00FF0000) >> 16) / 2;
                    uint32_t v2 = ((lo & 0x0000FF00) >> 8) / 2;
                    uint32_t v3 = (lo & 0x000000FF) / 2;

                    // This is actually triangle 2; vert 1,2,3.
                    uint32_t v4 = ((hi & 0x00FF0000) >> 16) / 2;
                    uint32_t v5 = ((hi & 0x0000FF00) >> 8) / 2;
                    uint32_t v6 = (hi & 0x000000FF) / 2;

                    FVector p1 = FVector(vtx[v1].v.ob[0], vtx[v1].v.ob[1], vtx[v1].v.ob[2]);
                    FVector p2 = FVector(vtx[v2].v.ob[0], vtx[v2].v.ob[1], vtx[v2].v.ob[2]);
                    FVector p3 = FVector(vtx[v3].v.ob[0], vtx[v3].v.ob[1], vtx[v3].v.ob[2]);

                    FVector p4 = FVector(vtx[v4].v.ob[0], vtx[v4].v.ob[1], vtx[v4].v.ob[2]);
                    FVector p5 = FVector(vtx[v5].v.ob[0], vtx[v5].v.ob[1], vtx[v5].v.ob[2]);
                    FVector p6 = FVector(vtx[v6].v.ob[0], vtx[v6].v.ob[1], vtx[v6].v.ob[2]);

                    std::visit([p1, p2, p3, p4, p5, p6](auto* obj) {
                        if (obj) {
                            obj->Triangles.push_back({p1, p2, p3});
                            obj->Triangles.push_back({p4, p5, p6});
                        }
                    }, object);
                    break;
                }
                case G_QUAD: {
                    if (vtx == NULL) {
                        ptr++;
                        continue;
                    }
                    uint32_t v1 = ((hi & 0x00FF0000) >> 16) / 2;
                    uint32_t v2 = ((hi & 0x0000FF00) >> 8) / 2;
                    uint32_t v3 = (hi & 0x000000FF) / 2;
                    uint32_t v4 = ((hi & 0xFF000000) >> 24) / 2;

                    FVector p1 = FVector(vtx[v1].v.ob[0], vtx[v1].v.ob[1], vtx[v1].v.ob[2]);
                    FVector p2 = FVector(vtx[v2].v.ob[0], vtx[v2].v.ob[1], vtx[v2].v.ob[2]);
                    FVector p3 = FVector(vtx[v3].v.ob[0], vtx[v3].v.ob[1], vtx[v3].v.ob[2]);
                    FVector p4 = FVector(vtx[v4].v.ob[0], vtx[v4].v.ob[1], vtx[v4].v.ob[2]);

                    std::visit([p1, p2, p3, p4](auto* obj) {
                        if (obj) {
                            obj->Triangles.push_back({p1, p2, p3});
                            obj->Triangles.push_back({p1, p3, p4});
                        }
                    }, object);
                    break;
                }
                case G_ENDDL:
                    run = false;
                    break;
                case G_MARKER:
                    ptr++;
                    break;
            }

            ptr++;
        }
    }

    std::unordered_map<GameObject*, std::vector<Vtx>> gDebugObjVtxCache;

    // Render a collision model
    void DebugCollision(GameObject* obj, FVector pos, IRotator rot, FVector scale, const std::vector<Triangle>& triangles) {
        if (obj == NULL || triangles.empty()) {
            return;
        }
    
        gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF);
    
        auto& vtxBuffer = gDebugObjVtxCache[obj];
        if (vtxBuffer.empty()) {
            for (const auto& tri : triangles) {
                vtxBuffer.push_back({(s16)tri.v0.x, (s16)tri.v0.y, (s16)tri.v0.z, 0, {0, 0}, {255, 255, 255, 255}});
                vtxBuffer.push_back({(s16)tri.v1.x, (s16)tri.v1.y, (s16)tri.v1.z, 0, {0, 0}, {255, 255, 255, 255}});
                vtxBuffer.push_back({(s16)tri.v2.x, (s16)tri.v2.y, (s16)tri.v2.z, 0, {0, 0}, {255, 255, 255, 255}});
            }
        }
    
        // Setup matrix and render state
        Mat4 mtx;
        ApplyMatrixTransformations(mtx, pos, rot, scale);
        Editor_AddMatrix(mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    
        gSPSetGeometryMode(gDisplayListHead++, G_FOG);
        gDPPipeSync(gDisplayListHead++);
        gDPSetCombineMode(gDisplayListHead++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
        gSPTexture(gDisplayListHead++, 0, 0, 0, G_TX_RENDERTILE, G_OFF);
        gDPSetRenderMode(gDisplayListHead++, G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2);
        uint32_t hash = (uint32_t)((uintptr_t)obj ^ ((uintptr_t)obj >> 16));
        u8 r = (hash >> 16) & 0xFF;
        u8 g = (hash >> 8) & 0xFF;
        u8 b = hash & 0xFF;

        gDPSetPrimColor(gDisplayListHead++, 0, 0, r, g, b, 255);

        // Render triangles in batches of 3
        for (size_t i = 0; i + 2 < vtxBuffer.size(); i += 3) {
            gSPVertex(gDisplayListHead++, (uintptr_t)&vtxBuffer[i], 3, 0);
            gSP1Triangle(gDisplayListHead++, 0, 1, 2, 0);
        }

        gSPSetGeometryMode(gDisplayListHead++, G_CULL_BACK | G_ZBUFFER);
    }
}
