#ifndef REPLAYS_H
#define REPLAYS_H

#include <libultraship.h>
#include <common_structs.h>

void func_80005B18(void);
void load_course_ghost(void);
void load_post_time_trial_replay(void);
void load_player_ghost(void);
void set_staff_ghost(void);
s32 func_800051C4(void);
void func_8000522C(void);
void func_800052A4(void);
void func_80005310(void);
void process_post_time_trial_replay(void);
void process_course_ghost_replay(void);
void process_player_ghost_replay(void);
void save_player_replay(void);
void func_80005AE8(Player*);
void func_80005E6C(void);
void replays_loop(void);

// mi0decode

extern s32 mio0encode(s32 input, s32, s32);

extern StaffGhost* D_80162DC4;
extern s32 D_80162DC8;
extern s32 D_80162DCC;
extern u16 bPlayerGhostDisabled;
extern u16 bCourseGhostDisabled;
extern u16 D_80162DD8;
extern s32 D_80162E00;
extern s32 D_80162DE0;
extern s32 D_80162DE4;
extern s32 D_80162DE8;
extern s32 gPauseTriggered;
extern s32 D_80162DF4;
extern s32 gPostTimeTrialReplayCannotSave;

extern u8* sReplayGhostBuffer;
extern size_t sReplayGhostBufferSize;
extern u32* sPlayerGhostReplay;

#endif /* REPLAYS_H */
