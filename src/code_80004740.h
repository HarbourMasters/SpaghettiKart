#ifndef CODE_80004740_H
#define CODE_80004740_H

#include "common_structs.h"

/*
What I know about animation stuff so far

Its not clear to me exactly what animations even are in MK64, my speculation is that their some form of matrix manipulation sequence.
Presumably the "models" of the objects that are getting animated are a bunch of matrices and the animations somehow change them.
code_80004740 appears to be responsible for the handling of animations.
Animation seem to be reserved for objects, namely: seagulls in KTB, flags in YV, chain chomps in RR, and penguins in SL.

Each object has 2 pointers of interest, at offsets 0x70 and 0x74  (unk_070 and unk_074).
These are passed to func_80004DFC, along with what appears to be an index and a timer.
func_80004DFC(unk70, unk74, index, timer)

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

They are handled in func_80004C30.
Each "type" indicates a different action to take while iterating over the array.
Type 1: Only used to modify Type 0's behaviour
Type 2: Pop a matrix
Type 3: End of array, stop processing
Type 0: Always handles some part of the animation. If preceded by a Type 1 entry it will pop a matrix prior to the animation handling.

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
They seem to be used in func_80004A1C to set some values in Vec3f which is then used to set some values in a Mat4.
This, I assume, is related to the matrix maniplation stuff that's used to actually accomplish the animation.

The unk_074 pointer points to a list of other pointers.
As best as I can tell only the penguins actually have multiple entries in this list, all the other animated objects have just 1 entry.
The pointers in this list have entires that look like this struct:

struct {
    s32 always_set_to_something_but_never_used;
    s32 always_zero_never_used;
    s/u16 animation_length;
    s/u16 has_value_but_never_used;
    s32 type_2_array_pointer;
    s32 type_3_array_pointer;
}

I will refer to these structs as Struct 2's.

Type 2 arrays appear to be just s/u16 arrays.
Their use will be discussed later

Type 3 arrays appear to be arrays of pairs of s/u16 numbers that look like this struct:

struct {
    s/u16 some_limiter;
    s/u16 some_offset;
};

Type 3 entires are interesting.
They're used in func_80004C30 and func_80004A1C, and they're always used in triples
The `timer` argument to func_80004DFC is compared to the `some_limiter` entry.
If it less than the limiter then its value it used elsewhere, otherwise 0 is used (more details below).
The `some_offset` value is always used.

Then, the 2 chosen values are used to select a value from the Type 2 array.
So, for example, if you have a Struct 2 like:


    0x00010000,
    0x00000000,
    0x0037, <- animation lenght
    0x000a,
    d_course_koopa_troopa_beach_unk_data2,
    d_course_koopa_troopa_beach_unk_data3,

d_course_koopa_troopa_beach_unk_data2 should be a Type 2 array while d_course_koopa_troopa_beach_unk_data3 is a Type 3 array.

d_course_koopa_troopa_beach_unk_data3 has entries that look like:
    //limiter //offset
    0x0001,     0x0000,
    0x0037,     0x0001,
    0x0001,     0x0000,

In practice the limiter value is always 1 or the animation length, meaning that you either choose 0 or the current animation timer.
There's never a situation where you will choose the animation timer until it hits X and then swap to 0.
Its always one or the other, never swapping.

The first triplet is used in func_80004C30 to access the Type 2 array and the values accessed is placed into D_80162D70.
D_80162D70 is a Vec3s is then used to set the values of some Vec3f in func_80004A1C.
All further triplets are used in func_80004A1C to collect Type 2 values to another Vec3s local to the function.
Both the local Vec3s and Vec3f are used to create a Mat4, which is then converted to a Mtx, which is then pushed into the matrix pool.

The chosen values are then used to access the Type 2 array and the value accessed is placed into D_80162D70.
D_80162D70 is a Vec3s that is used to set values in the some Vec3f that goes on to be used to modify the same Mat4 that the `thing` values are placed into.

*/

typedef struct {
    /* 0x00 */ s64 type;
    /* 0x04 */ s64 size;
    /* 0x08 */ s64 always_zero_never_used;
    /* 0x0C */ Gfx *optional_segmented_dl_address;
    /* 0x10 */ s64 thing[3];
} animation_type_1;

typedef struct {
    /* 0x00 */ u16 some_limiter;
    /* 0x02 */ u16 some_offset;
} animation_type_3;

typedef animation_type_3 animation_type_3_triplet[3];

typedef struct {
    /* 0x00 */ s32 always_set_to_something_but_never_used;
    /* 0x04 */ s32 always_zero_never_used;
    /* 0x08 */ s16 animation_length;
    /* 0x0A */ u16 has_value_but_never_used;
    /* 0x0C */ s16 *type_2_array_pointer;
    /* 0x10 */ animation_type_3_triplet *type_3_array_pointer;
} animation_type_2;

struct stru_80004EAC {
    s32 unk0;
    s32 unk4;
    s16 unk8;
};

/* Function Prototypes */

void func_80004740(Mtx *dest, Mat4 src);
void mtxf_translate_rotate2(Mat4 dest, Vec3f b, Vec3s c);
s16  func_80004EAC(void*, s16);
void func_80004A1C(animation_type_1*, s16*, animation_type_3_triplet, s32);
void func_80004C30(animation_type_1*, animation_type_2*, s16);
s16  func_80004DFC(animation_type_1*, animation_type_2**, s16, s16);

#endif
