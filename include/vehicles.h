#ifndef VEHICLES_H
#define VEHICLES_H

#include <common_structs.h>

#define NUM_1P_PASSENGER_CARS 4
#define NUM_2P_PASSENGER_CARS 1
#define NUM_PASSENGER_CAR_ENTRIES 5 // Max 16
#define NUM_TENDERS 1
#define NUM_TRAINS 2
#define LOCOMOTIVE_ONLY 0

#define NUM_CROSSINGS 2
// This allows karts to jump the train despite the crossing being activated.
// but still stop for the train once it's close
#define FRAMES_SINCE_CROSSING_ACTIVATED 240

#define NUM_CROSSINGS 2
// This allows karts to jump the train despite the crossing being activated.
// but still stop for the train once it's close
#define FRAMES_SINCE_CROSSING_ACTIVATED 240

// It seems like there's room for 2 Ferries, but only 1 is ever active
#define NUM_ACTIVE_PADDLE_BOATS 1
#define NUM_PADDLE_BOATS 2

#define NUM_RACE_BOX_TRUCKS 7
#define NUM_RACE_SCHOOL_BUSES 7
#define NUM_RACE_TANKER_TRUCKS 7
#define NUM_RACE_CARS 7

#define NUM_TIME_TRIAL_BOX_TRUCKS (NUM_RACE_BOX_TRUCKS + 1)
#define NUM_TIME_TRIAL_SCHOOL_BUSES (NUM_RACE_SCHOOL_BUSES + 1)
#define NUM_TIME_TRIAL_TANKER_TRUCKS (NUM_RACE_TANKER_TRUCKS + 1)
#define NUM_TIME_TRIAL_CARS (NUM_RACE_CARS + 1)

#define TRAIN_SMOKE_RENDER_DISTANCE 2000.0f
#define TRAIN_CROSSING_AI_DISTANCE 1000.0f
#define BOAT_SMOKE_RENDER_DISTANCE 2000.0f

#define RENDER_VEHICLE 1

typedef struct {
    /* 0x00 */ s16 isActive; // Only used for Tender and Passenger Cars, unused for Locomotives
    /* 0x02 */ s16 compilerPadding;
    /* 0x04 */ Vec3f position;
    /* 0x10 */ Vec3f velocity;
    /* 0x1C */ u16 waypointIndex;
    /* 0x1E */ s16 actorIndex;
    /* 0x20 */ s32 unused; // Not read or written. Could be padding?
} TrainCarStuff;           // size = 0x24;

typedef struct {
    /* 0x000 */ TrainCarStuff locomotive;
    /* 0x024 */ TrainCarStuff tender;
    /* 0x048 */ TrainCarStuff passengerCars[NUM_PASSENGER_CAR_ENTRIES];
    /* 0x0FC */ f32 speed;
    /* 0x100 */ s32 someFlags;
                s32 numCarriages;
    /* 0x104 */ s32 numCars; // Non-locomotive car count?
    /* 0x108 */ s32 unused;  // Not read or written. Could be padding?
} TrainStuff;                // size = 0x10C

typedef struct {
    /* 0x00 */ s16 isActive;      // The paddle wheel boat only shows up if the number of players is < 3
    /* 0x02 */                    // s16 compilerPadding;
    /* 0x04 */ Vec3f position;    //
    /* 0x10 */ Vec3f velocity;    //
    /* 0x1C */ u16 waypointIndex; //
    /* 0x1E */ s16 actorIndex;    //
    /* 0x20 */ f32 speed;         //
    /* 0x24 */ s16 rotY;          // Only Y rotation is tracked in this struct
    /* 0x26 */                    // s16 compilerPadding2;
                                  /**
                                   * Bit field that tracks whether a given human player is within a certain distance of the boat
                                   * Probably audio related
                                   **/
    /* 0x28 */ s32 someFlags;
} PaddleBoatStuff; // size = 0x2C

typedef struct {
    /* 0x00 */ s16 unused; // Gets set to 0, but doesn't seem to have any purpose
    /* 0x02 */             // s16 compilerPadding;
    /* 0x04 */ Vec3f position;
    /* 0x10 */ Vec3f velocity;
    /* 0x1C */ u16 waypointIndex;
    /* 0x1E */ s16 actorIndex;
    /* 0x20 */ f32 speed;                   //
    /* 0x24 */ f32 someMultiplierTheSequel; //
    /* 0x28 */ Vec3s rotation;
    /* 0x2E */ s16 someType;          //
                                      /**
                                       * Each set of flags seems to be a bit field tracking whether a given human player
                                       * is within a certain distance of the vehicle. Likely related to audio stuff
                                       * Seems to turn on/off based on player distance from the vehicle
                                       **/
    /* 0x30 */ s8 someFlags;          //
    /* 0x31 */ s8 someFlagsTheSequel; //
    /* 0x32 */                        // s16 compilerPadding2;
} VehicleStuff;                       // size = 0x34

extern TrainStuff gTrainList[];

// This is an array, maybe they intended to have multiple boats at some point in development?
extern PaddleBoatStuff gPaddleBoats[];

// Lists for different vehicles in Toad's Turnpike
extern VehicleStuff gBoxTruckList[];
extern VehicleStuff gSchoolBusList[];
extern VehicleStuff gTankerTruckList[];
extern VehicleStuff gCarList[];

#endif
