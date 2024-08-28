#ifndef CODE_80004740_H
#define CODE_80004740_H

#include <common_structs.h>

/*
What I know about animation stuff so far

Its not clear to me exactly what animations even are in MK64, my speculation is that their some form of matrix
manipulation sequence. Presumably the "models" of the objects that are getting animated are a bunch of matrices and the
animations somehow change them. animation appears to be responsible for the handling of animations. Animation seem to be
reserved for objects, namely: seagulls in KTB, flags in YV, chain chomps in RR, and penguins in SL.

Each object has 2 pointers of interest, at offsets 0x70 and 0x74  (unk_070 and unk_074).
These are passed to render_animated_model, along with what appears to be an index and a timer.
render_animated_model(unk70, unk74, index, timer)

unk_070 points to what I will call a "Type 1" array.
I call it an array, but I don't think that's entirely accurate.
There are 4 potential "types" of "type 1" arrays: 0, 1, 2, and 3
Only type 0 has anything below the "size" entry.
The rest only have the type and size.
The contents look like this struct
struct {
    s32 type;
    s32 size;
    s32 always_zero_never_used;
    s32 maybe_a_dl_pointer_maybe_null;
    s32 thing1;
    s32 thing2;
    s32 thing3;
}
Again, note that types 1, 2, and 3 only have the "type" and "size" elements.

They are handled in render_armature.
Each "type" indicates a different action to take while iterating over the array.
Type 1: Only used to modify Type 0's behaviour
Type 2: Pop a matrix
Type 3: End of array, stop processing
Type 0: Always handles some part of the animation. If preceded by a Type 1 entry it will pop a matrix prior to the
animation handling.

The "size" entry is used in a weird way. If you have a set of entries like:

    0x00000001, <- Our unk_070 pointer starts here
    0x00000002,

    0x00000000,
    0x00000007,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,

    0x00000001,
    0x00000002,

Then the size of 2 in the first entry is used to move the pointer 2 words forward

    0x00000001,
    0x00000002,

    0x00000000, <- Goes here
    0x00000007,
    0x00000000,
    0x00000000,
    0x00000000,
    0xfffffffb, (this is really -5)
    0x00000005,

    0x00000001,
    0x00000002,

Then, the 7 does the same thing

    0x00000001,
    0x00000002,

    0x00000000,
    0x00000007,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,

    0x00000001, <- Now we're here
    0x00000002,

This is why I don't think referring to this as an "array" is really correct.
Iterating over this data not like iterating over a proper array at all.

I don't really understand the "thing" entries yet.
They seem to be used in render_limb_or_add_mtx to set some values in Vec3f which is then used to set some values in a
Mat4. This, I assume, is related to the matrix maniplation stuff that's used to actually accomplish the animation.

The unk_074 pointer points to a list of other pointers.
As best as I can tell only the penguins actually have multiple entries in this list, all the other animated objects have
just 1 entry. The pointers in this list have entires that look like this struct:

struct {
    s32 always_set_to_something_but_never_used;
    s32 always_zero_never_used;
    s/u16 animation_length;
    s/u16 has_value_but_never_used;
    s32 angle_array;
    s32 AnimationCycleSpecVector;
}

I will refer to these structs as Struct 2's.

Type 2 arrays appear to be just s/u16 arrays.
Their use will be discussed later

Type 3 arrays appear to be arrays of pairs of s/u16 numbers that look like this struct:

struct {
    s/u16 animation_length;
    s/u16 indexCycle;
};

Type 3 entires are interesting.
They're used in render_armature and render_limb_or_add_mtx, and they're always used in triples
The `timer` argument to render_animated_model is compared to the `animation_length` entry.
If it less than the limiter then its value it used elsewhere, otherwise 0 is used (more details below).
The `indexCycle` value is always used.

Then, the 2 chosen values are used to select a value from the Type 2 array.
So, for example, if you have a Struct 2 like:


    0x00010000,
    0x00000000,
    0x0037, <- animation lenght
    0x000a,
    d_course_koopa_troopa_beach_unk_data2,
    d_course_koopa_troopa_beach_unk_data3,

d_course_koopa_troopa_beach_unk_data2 should be a Type 2 array while d_course_koopa_troopa_beach_unk_data3 is a Type 3
array.

d_course_koopa_troopa_beach_unk_data3 has entries that look like:
    //limiter //offset
    0x0001,     0x0000,
    0x0037,     0x0001,
    0x0001,     0x0000,

In practice the limiter value is always 1 or the animation length, meaning that you either choose 0 or the current
animation timer. There's never a situation where you will choose the animation timer until it hits X and then swap to 0.
Its always one or the other, never swapping.

The first triplet is used in render_armature to access the Type 2 array and the values accessed is placed into
sOriginalPosAnimation. sOriginalPosAnimation is a Vec3s is then used to set the values of some Vec3f in
render_limb_or_add_mtx. All further triplets are used in render_limb_or_add_mtx to collect Type 2 values to another
Vec3s local to the function. Both the local Vec3s and Vec3f are used to create a Mat4, which is then converted to a Mtx,
which is then pushed into the matrix pool.

The chosen values are then used to access the Type 2 array and the value accessed is placed into sOriginalPosAnimation.
sOriginalPosAnimation is a Vec3s that is used to set values in the some Vec3f that goes on to be used to modify the same
Mat4 that the `thing` values are placed into.

*/

#define SIMPLE_ANIMATION_INSTRUCTION(x) x, 0x00000002

#define ANIMATION_DISABLE_AUTOMATIC_POP SIMPLE_ANIMATION_INSTRUCTION(DISABLE_AUTOMATIC_POP_MATRIX)
#define ANIMATION_POP_MATRIX SIMPLE_ANIMATION_INSTRUCTION(POP_MATRIX)
#define ANIMATION_STOP SIMPLE_ANIMATION_INSTRUCTION(STOP_ANIMATION)
//! the pos is relative to the previous RENDER_MODEL_OR_ADD_POS if ANIMATION_DISABLE_AUTOMATIC_POP is used
#define ANIMATION_RENDER_MODEL_AT(model, x, y, z) RENDER_MODEL_OR_ADD_POS, 0x00000007, 0x00000000, (s64) model, x, y, z
//! use RENDER_MODEL_OR_ADD_POS
#define ANIMATION_RENDER_MODEL(model) ANIMATION_RENDER_MODEL_AT(model, 0x00000000, 0x00000000, 0x00000000)
//! use RENDER_MODEL_OR_ADD_POS
#define ANIMATION_ADD_POS(x, y, z) ANIMATION_RENDER_MODEL_AT((s64) NULL, x, y, z)

enum animation_type { RENDER_MODEL_OR_ADD_POS, DISABLE_AUTOMATIC_POP_MATRIX, POP_MATRIX, STOP_ANIMATION };

/**
 * @brief The armature is associated with an AnimationLimbVector which is equal to the number of RENDER_MODEL_OR_ADD_POS
 * in the armature
 */
typedef struct {
    /* 0x00 */ s64 type;
    /* 0x04 */ s64 size;
    /* 0x08 */ s64 always_zero_never_used;
    /* 0x0C */ Gfx* model;
    /* 0x10 */ s64 pos[3];
} Armature;

typedef struct {
    /* 0x00 */ u16 animation_length;
    /* 0x02 */ u16 indexCycle;
} AnimationCycleSpec;

//! the first are the original position and other are animation specification for each "limb" who correspond to each
//! RENDER_MODEL_OR_ADD_POS so ANIMATION_RENDER_MODEL and ANIMATION_ADD_POS
typedef AnimationCycleSpec AnimationLimbVector[3];

typedef struct {
    /* 0x00 */ s32 always_set_to_something_but_never_used;
    /* 0x04 */ s32 always_zero_never_used;
    /* 0x08 */ s16 animation_length;
    /* 0x0A */ u16 has_value_but_never_used;
    /* 0x0C */ s16* angle_array;
    /* 0x10 */ AnimationLimbVector* animation_cycle_spec_vector;
} Animation;

/* Function Prototypes */

void convert_to_fixed_point_matrix_animation(Mtx* dest, Mat4 src);
void mtxf_translate_rotate2(Mat4 dest, Vec3f pos, Vec3s angle);
s16 get_animation_length(Animation**, s16);
void render_limb_or_add_mtx(Armature*, s16*, AnimationLimbVector, s32);
void render_armature(Armature*, Animation*, s16);
s16 render_animated_model(Armature*, Animation**, s16, s16);

#endif
