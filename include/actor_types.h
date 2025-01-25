#ifndef ACTOR_TYPES_H
#define ACTOR_TYPES_H

#include <libultraship.h>
#include <macros.h>
#include <common_structs.h>
#include <assert.h>

// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wmicrosoft-extension"
// #pragma GCC diagnostic ignored "-Wmissing-member-name-in-structure-/-union"

/**
 * gActorList should be understood to be populated by generic Actor structs.
 * However, for human readability, many functions interacting with actor list elements expect one of the many
 * specialized types found in this file.
 *
 * Note that specialized types must be the same size as a plain Actor. Don't be mislead into thinking that
 * because its a separate type that it can modified separately from plain Actor. If you modify/add an actor type
 * and its size is different from plain Actor's, you WILL run into buggy (potentially crash inducing) behaviour.
 *
 * Specialized structs are customizable so long as the following member specifications are met:
 *
 * In general:
 *     0x00 -> s16 type
 *     0x02 -> s16 flags
 *     0x30 -> Collision unk30
 *
 * If player can collide with the actor:
 *     0x0C -> f32 boundingBoxSize
 *
 * If the actor makes sound (necessary for doppler/volume stuff):
 *     0x18 -> Vec3f pos
 *     0x24 -> Vec3f velocity
 *
 * Other members are more flexible, and even the non-general specifications can be ignored IF AND ONLY IF you know
 * exactly what you're doing.
 */
enum ActorType {
    ACTOR_UNKNOWN_0x01 = 0x01,
    ACTOR_TREE_MARIO_RACEWAY,
    ACTOR_TREE_YOSHI_VALLEY,
    ACTOR_TREE_ROYAL_RACEWAY,
    ACTOR_FALLING_ROCK,
    ACTOR_BANANA,
    ACTOR_GREEN_SHELL,
    ACTOR_RED_SHELL,
    ACTOR_YOSHI_EGG,
    ACTOR_PIRANHA_PLANT,
    ACTOR_UNKNOWN_0x0B,
    ACTOR_ITEM_BOX,
    ACTOR_FAKE_ITEM_BOX,
    ACTOR_BANANA_BUNCH,
    ACTOR_TRAIN_ENGINE,
    ACTOR_TRAIN_TENDER,
    ACTOR_TRAIN_PASSENGER_CAR,
    ACTOR_COW,
    ACTOR_TREE_MOO_MOO_FARM,
    ACTOR_UNKNOWN_0x14,
    ACTOR_TRIPLE_GREEN_SHELL,
    ACTOR_TRIPLE_RED_SHELL,
    ACTOR_MARIO_SIGN,
    ACTOR_UNKNOWN_0x18,
    ACTOR_PALM_TREE,
    ACTOR_UNKNOWN_0x1A,
    ACTOR_UNKNOWN_0x1B,
    ACTOR_TREE_BOWSERS_CASTLE,
    ACTOR_TREE_FRAPPE_SNOWLAND,
    ACTOR_CACTUS1_KALAMARI_DESERT,
    ACTOR_CACTUS2_KALAMARI_DESERT,
    ACTOR_CACTUS3_KALAMARI_DESERT,
    ACTOR_BUSH_BOWSERS_CASTLE,
    ACTOR_UNKNOWN_0x21,
    ACTOR_WARIO_SIGN,
    ACTOR_UNKNOWN_0x23,
    ACTOR_BOX_TRUCK,
    ACTOR_PADDLE_BOAT,
    ACTOR_RAILROAD_CROSSING,
    ACTOR_SCHOOL_BUS,
    ACTOR_TANKER_TRUCK,
    ACTOR_BLUE_SPINY_SHELL,
    ACTOR_HOT_AIR_BALLOON_ITEM_BOX,
    ACTOR_CAR,
    ACTOR_KIWANO_FRUIT
};
size_t CM_GetActorSize(void);
#define ACTOR_LIST_SIZE CM_GetActorSize()

struct Actor* CM_GetActor(size_t);
#define GET_ACTOR(index) CM_GetActor(index)

// Actor flags
#define ACTOR_IS_NOT_EXPIRED 0xF // The actor possesses some kind of collision and can be removed

// Actor shell->state (green, red and blue)
enum ShellState {
    HELD_SHELL,                  // Single shell that has not been dropped. (probably holding Z).
    RELEASED_SHELL,              // This is the short window where single shells aren't being held or launched.
    MOVING_SHELL,                // Moving towards its target after being shot.
    RED_SHELL_LOCK_ON,           // Red shell is targeting.
    TRIPLE_GREEN_SHELL,          // Loses triple shell state when shot.
    GREEN_SHELL_HIT_A_RACER,     // A racer has been hit by a green shell.
    TRIPLE_RED_SHELL,            // Loses triple shell state when shot.
    DESTROYED_SHELL,             // Collision with the shell.
    BLUE_SHELL_LOCK_ON,          // A blue shell has found a target and is hastily approaching it.
    BLUE_SHELL_TARGET_ELIMINATED // Mission completed, well done boss.
};

// Actor banana->state
enum BananaState {
    HELD_BANANA,               // Single banana that has not been dropped.
    DROPPED_BANANA,            // A banana in the state of being dropped on the ground (it only last for a few frames).
    FIRST_BANANA_BUNCH_BANANA, // The first banana of the banana bunch
    BANANA_BUNCH_BANANA,       // Every banana of the banana bunch except the first one.
    BANANA_ON_GROUND,          // A banana sitting on the ground.
    DESTROYED_BANANA           // Collision with the banana.
};

// Actor fakeItemBox->state
#define HELD_FAKE_ITEM_BOX 0      // Item box is being held be Z.
#define FAKE_ITEM_BOX_ON_GROUND 1 // Item box is on the ground.
#define DESTROYED_FAKE_ITEM_BOX 2 // Collision with fake item box.

struct Actor {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 state;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ f32 boundingBoxSize;
    /* 0x10 */ Vec3s rot;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ Vec3f pos;
    /* 0x24 */ Vec3f velocity;
    /* 0x30 */ Collision unk30;
}; // size = 0x70

// Duplicate declare for simplicity when externing actors & packed files.
extern struct Actor gActorList[100]; // D_8015F9B8

/*
Specialized actor types
*/

/*
Used by the locomotive, tender, and passenger car
*/
struct TrainCar {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 wheelRot;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ Vec3s rot;
    /* 0x10 */ s16 unk_16;
    /* 0x18 */ Vec3f pos;
    /* 0x24 */ Vec3f velocity;
    /* 0x30 */ Collision unk30;
}; // size = 0x70

struct RailroadCrossing {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 someTimer;
    /* 0x06 */ s16 crossingId;        // unused now
    /* 0x08 */ void* crossingTrigger; // Crossing Trigger Class
    /* 0x10 */ Vec3s rot;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ Vec3f pos;
    /* 0x24 */ Vec3f velocity;
    /* 0x30 */ Collision unk30;
}; // size = 0x70

// crossingTrigger might ruin struct size when compiled on 32 bit
static_assert(sizeof(struct RailroadCrossing) == sizeof(struct Actor),
              "RailroadCrossing struct size does not match base struct size");

struct FallingRock {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 respawnTimer;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ f32 boundingBoxSize;
    /* 0x10 */ Vec3s rot;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ Vec3f pos;
    /* 0x24 */ Vec3f velocity;
    /* 0x30 */ Collision unk30;
}; // size = 0x70

struct ActorSpawnData {
    /* 0x00 */ Vec3s pos;
    union {
        /* 0x06 */ u16 someId; // Usually populated, but not necessarily used by all actors types
        /* 0x06 */ s16 signedSomeId;
    };
};

// Required for evaluate_collision_player_palm_trees due to diff size.
// members unverified. data located at d_course_dks_jungle_parkway_tree_spawn
/**
 * There are nearly 100 trees in DK Jungle Parkway. If they were put into the actor list proper
 * they would fill it up, leaving no space for stuff like item boxes, shells, bananas, kiwano fruits,
 * etc.
 * So, this struct type acts as both spawn data AND a stripped down Actor for those trees.
 * Give the tree a position, a byte for flags stuffed into an s16 used to indicate tree sub-type,
 * and an s16 containing as the tree's original Y position.
 **/
struct UnkActorSpawnData {
    /* 0x00 */ Vec3s pos;
    // Techinically only the bottom byte of someId is the "id". The top byte is used for flags.
    /* 0x06 */ s16 someId;
    // Stores the tree's original Y position.
    /* 0x08 */ s16 unk8;
};

struct YoshiValleyEgg {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ f32 pathRadius;
    /* 0x0C */ f32 boundingBoxSize;
    /* 0x10 */ s16 pathRot;
    /* 0x12 */ s16 eggRot;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ Vec3f pos;
    // Note, pathCenter[1] should be understood to be the Y velocity of the egg
    // pathCenter[0] and pathCenter[2] are the X,Z coordinates of the center of the path
    /* 0x24 */ Vec3f pathCenter;
    /* 0x30 */ Collision unk30;
}; // size = 0x70

struct KiwanoFruit {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16
        targetPlayer; // Id of the player this actor tracks. Each player has their own kiwano actor just for them
    /* 0x06 */ s16 state;
    /* 0x08 */ f32 bonkTimer; // bonkState? Not sure what this is tracking, but its some form of count down that starts
                              // after the fruit hits you
    /* 0x0C */ f32 boundingBoxSize;
    /* 0x10 */ s16 animState;
    /* 0x12 */ s16 animTimer;
    /* 0x14 */ s16 unk_14;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ Vec3f pos;
    /* 0x24 */ Vec3f velocity;
    /* 0x30 */ Collision unk30;
}; // size = 0x70

struct PaddleWheelBoat {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 wheelRot;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ Vec3s boatRot;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ Vec3f pos;
    /* 0x24 */ Vec3f velocity;
    /* 0x30 */ Collision unk30;
}; // size = 0x70

struct PiranhaPlant {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ Vec4s visibilityStates; // A per-camera visibilty state tracker
    /* 0x0C */ f32 boundingBoxSize;
    /* 0x10 */ Vec4s unk10;
    /* 0x18 */ Vec3f pos;
    /* 0x24 */ Vec4s timers; // A per-camera timer. Might be more appropriate to call this state
    /* 0x2C */ f32 unk_02C;
    /* 0x30 */ Collision unk30;
}; // size = 0x70

struct PalmTree {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 variant; // four different types of palm trees
    /* 0x06 */ s16 state;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ f32 boundingBoxSize;
    /* 0x10 */ Vec3s rot;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ Vec3f pos;
    /* 0x24 */ Vec3f velocity;
    /* 0x30 */ Collision unk30;
}; // size = 0x70

typedef struct {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 shellsAvailable;
    /* 0x06 */ s16 state;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ s16 rotVelocity;
    /* 0x12 */ s16 rotAngle;
    /* 0x14 */ s16 playerId; // Id of the player that "owns" the shells
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ Vec3f unk_18;
    /* 0x24 */ Vec3f shellIndices; // Indices in gActorList for the shells "owned" by this parent
    /* 0x30 */ Collision unk30;
} TripleShellParent; // size = 0x70

struct ShellActor {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    // Index in gActorList for the parent actor of this shell
    // Seems to pull double duty as a timer
    union {
        /* 0x04 */ s16 parentIndex;
        /* 0x04 */ s16 someTimer;
        // Red Shells only (maybe blue shells?)
        /* 0x04 */ s16 targetPlayer; // Player the shell is after
    };
    /* 0x06 */ s16 state;
    /* 0x08 */ f32 shellId; // 0, 1, or 2. Indicates which shell in the triplet this one is
    /* 0x0C */ f32 boundingBoxSize;
    /* 0x10 */ s16 rotVelocity; // Change in rotAngle on a per-update basis
    union {
        /* 0x12 */ s16 rotAngle;  // Angle of rotation around player (or parent?), not the rotation of the shell itself
        /* 0x12 */ u16 pathIndex; // Index in the set of points that make up the "path" the red/blue shell follows (may
                                  // be GP mode exclusive)
    };
    /* 0x14 */ s16 playerId; // Id of the player that "owns" the shell
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ Vec3f pos;
    /* 0x24 */ Vec3f velocity; // All 0 until the shell is fired
    /* 0x30 */ Collision unk30;
}; // size = 0x70

struct ItemBox {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 someTimer;
    /* 0x06 */ s16 state;
    /* 0x08 */ f32 resetDistance; // Value added to the Y position when box is touched. Expected to be negative
                                  // Distance at which a player can activate the item box
                                  // Named "bounding box" to match the name used for the "size" of a kart
    /* 0x0C */ f32 boundingBoxSize;
    /* 0x10 */ Vec3s rot;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ Vec3f pos;
    /* 0x24 */ f32 origY; // Original Y position. Basically the Y position the box will reset to after being touched
    /* 0x28 */ f32 unk_028;
    /* 0x2C */ f32 unk_02C;
    /* 0x30 */ Collision unk30;
}; // size = 0x70

struct FakeItemBox {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 someTimer;
    /* 0x06 */ s16 state;
    /* 0x08 */ f32 sizeScaling; // Controls the size of the box
    /* 0x0C */ f32 boundingBoxSize;
    /* 0x10 */ Vec3s rot;
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ Vec3f pos;
    /* 0x24 */ f32 playerId;
    /* 0x28 */ f32 targetY;
    /* 0x2C */ f32 unk_02C;
    /* 0x30 */ Collision unk30;
}; // size = 0x70

struct BananaBunchParent {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 state;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ s16 playerId;         // Player that own the bananas
    /* 0x12 */ s16 bananaIndices[5]; // Indices in gActorList for the bananas owned by this parent
    /* 0x1C */ s16 bananasAvailable;
    /* 0x1E */ s16 unk_1E;
    /* 0x20 */ f32 unk_20[4];
    /* 0x30 */ Collision unk30;
}; // size = 0x70

struct BananaActor {
    /* 0x00 */ s16 type;
    /* 0x02 */ s16 flags;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 state;
    /* 0x08 */ s16 parentIndex;
    /* 0x0A */ s16 bananaId; // ? Appears to indiciate which banana of the bunch this one is
    /* 0x0C */ f32 boundingBoxSize;
    union {
        /* 0x10 */ Vec3s rot;
        struct {
            /* 0x10 */ s16 playerId;     // Id of the player that owns this banana
            /* 0x12 */ s16 elderIndex;   // Index in gActorList of the next-oldest banana in the bunch
            /* 0x14 */ s16 youngerIndex; // Index in gActorList of the next-youngest banana in the bunch
        };
    };
    /* 0x16 */ s16 unk_16;
    /* 0x18 */ Vec3f pos;
    /* 0x24 */ Vec3f velocity;
    /* 0x30 */ Collision unk30;
}; // size = 0x70

// #pragma GCC diagnostic pop

#endif // ACTOR_TYPES_H
