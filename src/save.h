#ifndef SAVE_H
#define SAVE_H

#include <libultraship.h>
#include <common_structs.h>

/* define symbols */

#define CONTROLLER_1 0
#define CONTROLLER_2 1
#define CONTROLLER_3 2
#define CONTROLLER_4 3

#define PFS_NO_ERROR 0        // controller pak no error return
#define PFS_NO_PAK_INSERTED 1 // no pak pressent or wrong device inserted into the controller
#define PFS_PAK_BAD_READ 2    // controller pak error while data transfer
#define PFS_PAK_CORRUPTED 3   // controller pak has courrupted or has weird data
#define PFS_FILE_OVERFLOW 4   // too many files written to in the controller pak
#define PFS_INVALID_DATA -1   // bad arguments, game note does not exist, osPfsInit was not called.
// controller pak test functions
#define PFS_NUM_FILES_ERROR -2
#define PFS_FREE_BLOCKS_ERROR -3
#define PFS_PAK_STATE_OK -4

#define NO_PAK 0
#define PAK 1
#define PAK_NOT_INSERTED -1

/* Function Prototypes */

void func_800B45E0(s32);
void write_save_data_grand_prix_points_and_sound_mode(void);
void func_800B46D0(void);
void func_800B4728(s32);
void reset_save_data_grand_prix_points_and_sound_mode(void);
u8 checksum_time_trial_records(s32);
s32 func_800B6348(s32);
s32 func_800B639C(s32);
s32 func_800B63F0(s32);
u8 compute_save_data_checksum_1(void);
u8 compute_save_data_checksum_2(void);
void load_save_data(void);
void func_800B4A9C(s32);
void validate_save_data(void);
void populate_time_trial_record(u8* timeTrialRecord, u32 time, s32 characterId);
u32 func_800B4DF4(u8*);
s32 func_800B4E24(s32);
u32 func_800B4EB4(s32, s32);
s32 func_800B4F2C(void);
s32 func_800B4FB0(s32);
s32 func_800B5020(u32, s32);
s32 func_800B5218(void);
void func_800B536C(s32);
void func_800B5404(s32, s32);
u8 func_800B54C0(s32, s32);
u8 func_800B54EC(s32, s32);
u8 func_800B5508(s32, s32, s32);
s32 is_cc_mode_complete(s32);
s32 has_unlocked_extra_mode(void);
s32 has_completed_extra_mode(void);
void func_800B559C(s32);
u8 func_800B578C(s32);
s32 func_800B5888(s32);
s32 func_800B58C4(s32);
void update_save_data_backup(void);
u8 compute_save_data_checksum_backup_1(void);
u8 compute_save_data_checksum_backup_2(void);
s32 validate_save_data_checksum_backup(void);
s32 check_for_controller_pak(s32);
s32 func_800B5B2C(s32);
s32 controller_pak_1_status(void);
s32 controller_pak_2_status(void);
s32 func_800B5F30(void);
s32 func_800B6014(void);
s32 func_800B6088(s32);
u8 func_800B60E8(s32);
s32 func_800B6178(s32);
s32 func_800B64EC(s32);
s32 func_800B65F4(s32, s32);
void func_800B6708(void);
void func_800B6798(void);
u8 func_800B6828(s32);
u8 func_800B68F4(s32);
s32 func_800B69BC(s32);
s32 func_800B6A68(void);

/* data */
// these might not be in this file, but for now...

extern u32* gReplayGhostCompressed;
extern struct_8018EE10_entry D_8018EE10[];

extern u16 gCompanyCode;
extern u32 gGameCode;           // osPfs gamecode
extern s8 gControllerPak1State; // Current state of Controller Pak 1

extern s8 sControllerPak2State; // Current state of the Controller Pak 2
extern const u8 D_800F2E60[];
extern const u8 gGameName[];
extern const u8 gExtCode[];
extern u16 D_80162DD6;
extern s32 D_80162DE0;
extern s32 D_80162DFC;
extern OSPfs gControllerPak1FileHandle;
extern OSPfs gControllerPak2FileHandle;
extern OSPfsState pfsState[16];
extern s32 pfsError[16];
extern s32 gControllerPak1NumFilesUsed;
extern s32 gControllerPak1MaxWriteableFiles;
extern s8 D_8018EDE5;
extern s8 D_8018EDE6;
extern s8 D_8018EDE7;

#endif /* CODE_800B045E0_H */
