#ifndef podium_ceremony_actors_H
#define podium_ceremony_actors_H

#include <libultraship.h>
#include <common_structs.h>

enum CeremonyActorType { Initial, Balloon, FireworkCone, FireworkBurst };

// unk
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 actorTimer;
    /* 0x0C */ u8 unk[16]; // 10 bytes
    /* 0x1C */ s8 unk1C;
    /* 0x1D */ s8 unk1D;
    /* 0x1E */ s8 unk1E; // gCharacterSelections
} struct_D_802874D8;

typedef struct {
    s16 type;
    Vec3s unk2;
    s8 unk8;
    s8 unk9;
    s8 unkA;
    s8 unkB;
} ActorInitParams;

// A generic actor, usually a balloon.
typedef struct {
    /* 0x00 */ s32 isActive;
    /* 0x04 */ s16 type;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s8 unk8;
    /* 0x09 */ s8 unk9;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ ActorInitParams* initParams;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s32 timer; // Used to delete the actor after some time.
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
} CeremonyActor;

typedef struct {
    /* 0x00 */ s32 isActive;
    /* 0x04 */ s16 type;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s8 unk8;
    /* 0x09 */ s8 unk9;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ ActorInitParams* initParams;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30; // s32 instead of s16
    /* 0x34 */ f32 unk34; // f32 instead of s16
    /* 0x38 */ f32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
} Firework;

void update_actors_loop(void);
void func_80280650(void);
void set_initial_position(CeremonyActor*);
CeremonyActor* find_available_entry(void);
CeremonyActor* new_actor(ActorInitParams*);
u16 random_u16_credits(void);
f32 random_float_between_0_and_1(void);
f32 random_who_knows(f32);
void func_80280884(void);
void firework_cone_update_and_spawn_burst(Firework*);
void func_80280A28(Vec3f, Vec3s, f32);
void render_fireworks(Vec3f, f32, s32, s16);
void balloons_and_fireworks_init(void);
void func_80280FFC(void);
void func_8028100C(s32, s32, s32);
void func_8028150C(void);
void func_80281520(void);
void func_80281528(void);
void func_80281530(void);
void func_80281538(void);
void func_80281540(void);
void podium_ceremony_loop(void);

extern struct_D_802874D8 D_802874D8;
extern CeremonyActor* sPodiumActorList;
extern s32 D_802874FC;

extern ActorInitParams initBalloon;
extern ActorInitParams initCone;
extern ActorInitParams initInitUnk;
extern ActorInitParams initBurst;

extern Gfx* D_802874D4;
extern s32 D_802874FC;
extern Mat4 D_80287500;

#endif
