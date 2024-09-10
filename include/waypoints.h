#ifndef WAYPOINTS_H
#define WAYPOINTS_H

#include <common_structs.h>

typedef struct {
    /* 0x00 */ s16 posX;
    /* 0x02 */ s16 posY;
    /* 0x04 */ s16 posZ;
    /* 0x06 */ u16 trackSectionId;
} TrackWaypoint; // size = 0x08

/**
 * These are per-path arrays that contain some information relating to waypoints
 * The arrays in D_80164550 contain X/Y/Z and track segment information
 * The arrays in D_80164560 and D_80164570 track some other X/Y/Z, but the track segment is always 0 (so,
 *untracked/unused) Its unclear how these arrays relate to each other
 **/
extern TrackWaypoint* D_80164550[];
extern TrackWaypoint* D_80164560[];
extern TrackWaypoint* D_80164570[];

/**
 * Don't know what exactly these are, but like D_80164550, D_80164560, and D_80164570
 * they track something about the waypoints on a per-path basis
 **/
// Waypoint types?
extern s16* D_80164580[];
// Based on func_80010DBC this may be angles between waypoints
// D_80164590[i] = atan2(waypoint_i, waypoint_i+1)?
extern s16* D_80164590[];
// No idea. Adjacency list?
extern s16* D_801645A0[];

/**
 * Certain parts of the waypoint logic will copy some path/player specific data to a temporary variable.
 * For example: D_80164490 is always a value from D_80164550. Depending on which path
 * a given player is on, the specific value may change
 **/
// Shadows values from gNearestWaypointByPlayerId
extern s16 sSomeNearestWaypoint;
// Shadows values from gPathIndexByPlayerId
extern s32 D_80163448;
// Shadows values from D_80164560
extern TrackWaypoint* D_801631D0;
// Shadows values from D_80164570
extern TrackWaypoint* D_801631D4;
// Shadows values from D_80164580
extern s16* D_801631D8;
// Shadows values from D_80164590
extern s16* D_801631DC;
// Shadowd values from gWaypointCountByPathIndex
extern u16 D_80164430;
// Shadows values from D_80164550
extern TrackWaypoint* D_80164490;
// Shadows values from D_801645A0
extern s16* D_801645E0;

extern u16 gNearestWaypointByPlayerId[]; // D_80164438
// Total waypoints passed by playerId?
extern s32 D_80164450[];
extern u16 gPathIndexByPlayerId[];      // D_801645B0
extern u16 gWaypointCountByPathIndex[]; // D_801645C8
// These values are only used when the camera is in "cinematic" mode
extern s16 gNearestWaypointByCameraId[]; // D_80164668

/**
 * Stuff that may not be directly related to waypoints, but are only referenced in code_80005FD0.
 * So they are at least waypoint adjacent.
 **/

// Tracks something on a per-player basis, no idea what though
extern f32 D_80163068[];
// Track segment by playerId, although it curiously does NOT track values for human players
// So, in 2 Player Grand Prix, the first 2 entries are always 0
extern u16 D_80163318[];
// Seems to be a per-path overcount of the waypoint count
extern s32 D_80163368[];
// Seemingly the Z position of the 1st waypoint in the 0th path?
extern f32 D_8016344C;
// These seem to track whether a player has entered or exited the "unknown" zone in yoshi's valley
// See func_80009000 and func_800090F0
// Is 1 when a player is in the "unknown" zone in yoshi's valley
extern s16 D_80163490[];
// Is 1 when a player leaves the "unknown" zone in yoshi's valley
extern s16 D_801634A8[];
// Tracks whether a given player is in the "unknown" zone of yoshi's valley
extern s16 D_801644F8[];

#endif
