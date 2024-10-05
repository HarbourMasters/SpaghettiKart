#ifndef collision_H
#define collision_H

#include <common_structs.h>

#define COLLISION 0x1
#define NO_COLLISION 0x0

void nullify_displaylist(uintptr_t);
void func_802AAAAC(Collision*);
f32 func_802AAB4C(Player*);
s32 check_collision_zx(Collision*, f32, f32, f32, f32, u16);
s32 check_collision_yx(Collision*, f32, f32, f32, f32, u16);
s32 check_collision_zy(Collision*, f32, f32, f32, f32, u16);
s8 get_surface_type(u16);
s16 get_track_section_id(u16);
s16 func_802ABD7C(u16);
s16 func_802ABDB8(u16);
s16 func_802ABDF4(u16);
f32 calculate_surface_height(f32, f32, f32, u16);
f32 func_802ABEAC(Collision*, Vec3f);
void shell_collision(Collision*, Vec3f);
void process_shell_collision(Vec3f, f32, Vec3f, f32);
u16 player_terrain_collision(Player*, KartTyre*, f32, f32, f32);
void adjust_pos_orthogonally(Vec3f, f32, Vec3f, f32);
s32 detect_tyre_collision(KartTyre*);
u16 actor_terrain_collision(Collision*, f32, f32, f32, f32, f32, f32, f32);
u16 check_bounding_collision(Collision*, f32, f32, f32, f32);
f32 spawn_actor_on_surface(f32, f32, f32);
void set_vtx_buffer(uintptr_t, u32, u32);
s32 is_line_intersecting_rectangle(s16, s16, s16, s16, s16, s16, s16, s16);
s32 is_triangle_intersecting_bounding_box(s16, s16, s16, s16, u16);
void generate_collision_grid(void);
void generate_collision_mesh_with_defaults(Gfx*);
void generate_collision_mesh_with_default_section_id(Gfx*, s8);
void generate_collision_mesh(Gfx*, s8, u16);
void find_and_set_tile_size(uintptr_t, s32, s32);
void set_vertex_colours(uintptr_t, u32, s32, s8, u8, u8, u8);
void find_vtx_and_set_colours(Gfx*, s8, u8, u8, u8);
void subtract_scaled_vector(Vec3f, f32, Vec3f);

#endif
