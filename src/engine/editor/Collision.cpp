#include "Collision.h"


void GenerateCollisionMesh(GameObject& object, Gfx* model, float scale) {
    int8_t opcode;
    uintptr_t lo;
    uintptr_t hi;

    Gfx* ptr = model;
    Vtx* vtx = NULL;
    size_t i = 0;
    bool run = true;
    while (run) {
        i++;
        lo = ptr->words.w0;
        hi = ptr->words.w1;
        opcode = (EDITOR_GFX_GET_OPCODE(lo) >> 24);

        switch(opcode) {
            case G_DL:
                GenerateCollisionMesh(object, (Gfx*)hi, scale);
                break;
            case G_VTX:
                vtx = (Vtx*)ptr->words.w1;
                break;
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

                object.Triangles.push_back({p1, p2, p3});
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

                object.Triangles.push_back({p1, p2, p3});
                object.Triangles.push_back({p4, p5, p6});
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

                object.Triangles.push_back({p1, p2, p3});
                object.Triangles.push_back({p1, p3, p4});
                break;
            }
            case G_ENDDL:
                run = false;
                break;
        }

        ptr++;
    }
}
