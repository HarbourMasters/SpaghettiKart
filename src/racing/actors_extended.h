#ifndef ACTORS_EXTENDED_H
#define ACTORS_EXTENDED_H

#include <common_structs.h>
#include "actor_types.h"

/** @cond */

void copy_collision(Collision*, Collision*);
void triple_shell_actor_collide_with_player(struct ShellActor*, s32);
void func_802B039C(struct BananaActor*);
void func_802B0464(s16);
void func_802B04E8(struct BananaActor*, s16);
void destroy_banana_in_banana_bunch(struct BananaActor*);
void func_802B0648(struct BananaBunchParent*);
void func_802B0788(s16, struct BananaBunchParent*, Player*);
s32 func_802B09C0(s16);
void update_actor_banana_bunch(struct BananaBunchParent*);
bool is_shell_exist(s16);
void update_actor_triple_shell(TripleShellParent*, s16);
s32 use_banana_bunch_item(Player*);
s32 use_triple_shell_item(Player*, s16);
s32 init_triple_shell(TripleShellParent*, Player*, s16, u16);
s32 use_green_shell_item(Player*);
s32 use_red_shell_item(Player*);
void use_blue_shell_item(Player*);
void update_actor_banana(struct BananaActor*);
void func_802B2914(struct BananaBunchParent*, Player*, s16);
s32 use_fake_itembox_item(Player*);
s32 use_banana_item(Player*);
void use_thunder_item(Player*);
void player_use_item(Player*);
void check_player_use_item(void);
void update_actor_green_shell(struct ShellActor*);
void func_802B3B44(struct ShellActor*);
void func_802B3E7C(struct ShellActor*, Player*);
s16 func_802B3FD0(Player*, struct ShellActor*);
void func_802B4104(struct ShellActor*);
void update_actor_red_blue_shell(struct ShellActor*);
void func_802B4E30(struct Actor*);

// audio/external.c
extern void func_800CAB4C(u8);

extern f32 D_802B9F68;

extern s16 gPlayerBalloonCount[];

/** @endcond */

#endif // ACTORS_EXTENDED_H
