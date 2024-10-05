#include <libultraship.h>
#include <libultra/gbi.h>

Lights1 mario_f3dlite_material_lights = gdSPDefLights1(
	0x7F, 0x28, 0x35,
	0xFF, 0x58, 0x71, 0x49, 0x49, 0x49);

Vtx mario_Plane_mesh_vtx_cull[8] = {
	{{ {-4022, 0, -4022}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-4022, 0, 4022}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-4022, 0, 4022}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-4022, 0, -4022}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {4022, 0, -4022}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {4022, 0, 4022}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {4022, 0, 4022}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {4022, 0, -4022}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx mario_Plane_mesh_vtx_0[4] = {
	{{ {4022, 0, -4022}, 0, {-16, 1008}, {10, 10, 5, 255} }},
	{{ {-4022, 0, -4022}, 0, {1008, 1008}, {10, 10, 5, 255} }},
	{{ {-4022, 0, 4022}, 0, {1008, -16}, {10, 10, 5, 255} }},
	{{ {4022, 0, 4022}, 0, {-16, -16}, {10, 10, 5, 255} }},
};

Gfx mario_Plane_mesh_tri_0[] = {
	gsSPVertex(mario_Plane_mesh_vtx_0 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Gfx mat_mario_f3dlite_material[] = {
	gsSPClearGeometryMode(G_CLIPPING),
	gsSPSetLights1(mario_f3dlite_material_lights),
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsDPSetAlphaDither(G_AD_NOISE),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPEndDisplayList(),
};

Gfx mat_revert_mario_f3dlite_material[] = {
	gsSPSetGeometryMode(G_CLIPPING),
	gsDPPipeSync(),
	gsDPSetAlphaDither(G_AD_DISABLE),
	gsSPEndDisplayList(),
};

Gfx mario_Plane_mesh[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(mario_Plane_mesh_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPDisplayList(mat_mario_f3dlite_material),
	gsSPDisplayList(mario_Plane_mesh_tri_0),
	gsSPDisplayList(mat_revert_mario_f3dlite_material),
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsSPEndDisplayList(),
};

