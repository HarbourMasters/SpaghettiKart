#pragma once

#include <libultraship.h>
#include <libultra/gbi.h>
#include "engine/objects/Flagpole.h"
#include "ObjectPicker.h"

class AShip;

class Editor {
public:
    Editor();

    void Tick();
    void Draw();
    void MouseClick();
private:
    bool _draw = false;
    AShip* object;
    Vec3f _ray;
    AActor* _selected;
    AActor* _lastSelected;

	s32 _colourIdFramebuffer = -1;

    s32 Inverse(MtxF* src, MtxF* dest);
    void Copy(MtxF* src, MtxF* dest);
    void Clear(MtxF* mf);

    void DrawObj(float length);

    ObjectPicker eObjectPicker;


Vtx box_Cube_mesh_vtx_cull[8] = {
	{{ {-46, -46, -46}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-46, -46, 46}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-46, 46, 46}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-46, 46, -46}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {46, -46, -46}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {46, -46, 46}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {46, 46, 46}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {46, 46, -46}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx box_Cube_mesh_vtx_0[24] = {
	{{ {-46, -46, 46}, 0, {368, 1008}, {129, 0, 0, 255} }},
	{{ {-46, 46, 46}, 0, {624, 1008}, {129, 0, 0, 255} }},
	{{ {-46, 46, -46}, 0, {624, 752}, {129, 0, 0, 255} }},
	{{ {-46, -46, -46}, 0, {368, 752}, {129, 0, 0, 255} }},
	{{ {-46, -46, -46}, 0, {368, 752}, {0, 0, 129, 255} }},
	{{ {-46, 46, -46}, 0, {624, 752}, {0, 0, 129, 255} }},
	{{ {46, 46, -46}, 0, {624, 496}, {0, 0, 129, 255} }},
	{{ {46, -46, -46}, 0, {368, 496}, {0, 0, 129, 255} }},
	{{ {46, -46, -46}, 0, {368, 496}, {127, 0, 0, 255} }},
	{{ {46, 46, -46}, 0, {624, 496}, {127, 0, 0, 255} }},
	{{ {46, 46, 46}, 0, {624, 240}, {127, 0, 0, 255} }},
	{{ {46, -46, 46}, 0, {368, 240}, {127, 0, 0, 255} }},
	{{ {46, -46, 46}, 0, {368, 240}, {0, 0, 127, 255} }},
	{{ {46, 46, 46}, 0, {624, 240}, {0, 0, 127, 255} }},
	{{ {-46, 46, 46}, 0, {624, -16}, {0, 0, 127, 255} }},
	{{ {-46, -46, 46}, 0, {368, -16}, {0, 0, 127, 255} }},
	{{ {-46, -46, -46}, 0, {112, 496}, {0, 129, 0, 255} }},
	{{ {46, -46, -46}, 0, {368, 496}, {0, 129, 0, 255} }},
	{{ {46, -46, 46}, 0, {368, 240}, {0, 129, 0, 255} }},
	{{ {-46, -46, 46}, 0, {112, 240}, {0, 129, 0, 255} }},
	{{ {46, 46, -46}, 0, {624, 496}, {0, 127, 0, 255} }},
	{{ {-46, 46, -46}, 0, {880, 496}, {0, 127, 0, 255} }},
	{{ {-46, 46, 46}, 0, {880, 240}, {0, 127, 0, 255} }},
	{{ {46, 46, 46}, 0, {624, 240}, {0, 127, 0, 255} }},
};

Gfx box_Cube_mesh_tri_0[8] = {
	gsSPVertex(box_Cube_mesh_vtx_0 + 0, 24, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
	gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
	gsSPEndDisplayList(),
};

Gfx mat_box_f3dlite_material[6] = {
	gsSPClearGeometryMode(G_CLIPPING),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPEndDisplayList(),
};

Gfx mat_revert_box_f3dlite_material[4] = {
	gsSPSetGeometryMode(G_CLIPPING),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx box_Cube_mesh[13] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(box_Cube_mesh_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPDisplayList(mat_box_f3dlite_material),
	gsSPDisplayList(box_Cube_mesh_tri_0),
	gsSPDisplayList(mat_revert_box_f3dlite_material),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};



};
