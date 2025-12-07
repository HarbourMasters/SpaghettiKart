#include <libultra/types.h>
#include "course_offsets.h"
#include <assets/models/tracks/koopa_troopa_beach/koopa_troopa_beach_data.h>
#include "animation.h"

Gfx* d_course_koopa_troopa_beach_dl_list1[] = {
    d_course_koopa_troopa_beach_dl_90,   d_course_koopa_troopa_beach_dl_278,  d_course_koopa_troopa_beach_dl_188,
    d_course_koopa_troopa_beach_dl_3C0,  d_course_koopa_troopa_beach_dl_4B0,  d_course_koopa_troopa_beach_dl_6E0,
    d_course_koopa_troopa_beach_dl_5A0,  d_course_koopa_troopa_beach_dl_828,  d_course_koopa_troopa_beach_dl_960,
    d_course_koopa_troopa_beach_dl_B98,  d_course_koopa_troopa_beach_dl_A58,  d_course_koopa_troopa_beach_dl_CF0,
    d_course_koopa_troopa_beach_dl_E48,  d_course_koopa_troopa_beach_dl_10D0, d_course_koopa_troopa_beach_dl_F60,
    d_course_koopa_troopa_beach_dl_1218, d_course_koopa_troopa_beach_dl_13B8, d_course_koopa_troopa_beach_dl_15E0,
    d_course_koopa_troopa_beach_dl_1498, d_course_koopa_troopa_beach_dl_1708, d_course_koopa_troopa_beach_dl_1818,
    d_course_koopa_troopa_beach_dl_1988, d_course_koopa_troopa_beach_dl_18E0, d_course_koopa_troopa_beach_dl_1A50,
    d_course_koopa_troopa_beach_dl_1AD8, d_course_koopa_troopa_beach_dl_1C48, d_course_koopa_troopa_beach_dl_1BB8,
    d_course_koopa_troopa_beach_dl_1D20, d_course_koopa_troopa_beach_dl_1DE0, d_course_koopa_troopa_beach_dl_1FE8,
    d_course_koopa_troopa_beach_dl_1EB0, d_course_koopa_troopa_beach_dl_20C8, d_course_koopa_troopa_beach_dl_2200,
    d_course_koopa_troopa_beach_dl_2440, d_course_koopa_troopa_beach_dl_22C8, d_course_koopa_troopa_beach_dl_2540,
    d_course_koopa_troopa_beach_dl_2660, d_course_koopa_troopa_beach_dl_2898, d_course_koopa_troopa_beach_dl_2760,
    d_course_koopa_troopa_beach_dl_2988, d_course_koopa_troopa_beach_dl_2AA0, d_course_koopa_troopa_beach_dl_2CC0,
    d_course_koopa_troopa_beach_dl_2B78, d_course_koopa_troopa_beach_dl_2DC0, d_course_koopa_troopa_beach_dl_2EC0,
    d_course_koopa_troopa_beach_dl_30E0, d_course_koopa_troopa_beach_dl_2FB0, d_course_koopa_troopa_beach_dl_3218,
    d_course_koopa_troopa_beach_dl_3308, d_course_koopa_troopa_beach_dl_3550, d_course_koopa_troopa_beach_dl_33F8,
    d_course_koopa_troopa_beach_dl_36D8, d_course_koopa_troopa_beach_dl_37B8, d_course_koopa_troopa_beach_dl_3A10,
    d_course_koopa_troopa_beach_dl_38A8, d_course_koopa_troopa_beach_dl_3BB8, d_course_koopa_troopa_beach_dl_3CC0,
    d_course_koopa_troopa_beach_dl_3F20, d_course_koopa_troopa_beach_dl_3DC0, d_course_koopa_troopa_beach_dl_40C0,
    d_course_koopa_troopa_beach_dl_41D0, d_course_koopa_troopa_beach_dl_4450, d_course_koopa_troopa_beach_dl_42D8,
    d_course_koopa_troopa_beach_dl_45D0, d_course_koopa_troopa_beach_dl_4700, d_course_koopa_troopa_beach_dl_49F0,
    d_course_koopa_troopa_beach_dl_4840, d_course_koopa_troopa_beach_dl_4BC8, d_course_koopa_troopa_beach_dl_4D08,
    d_course_koopa_troopa_beach_dl_4FF8, d_course_koopa_troopa_beach_dl_4E70, d_course_koopa_troopa_beach_dl_51C8,
    d_course_koopa_troopa_beach_dl_52D8, d_course_koopa_troopa_beach_dl_5500, d_course_koopa_troopa_beach_dl_53B8,
    d_course_koopa_troopa_beach_dl_5628, d_course_koopa_troopa_beach_dl_5778, d_course_koopa_troopa_beach_dl_5A50,
    d_course_koopa_troopa_beach_dl_58E0, d_course_koopa_troopa_beach_dl_5BB8, d_course_koopa_troopa_beach_dl_5D28,
    d_course_koopa_troopa_beach_dl_5F58, d_course_koopa_troopa_beach_dl_5E28, d_course_koopa_troopa_beach_dl_6050,
    d_course_koopa_troopa_beach_dl_61D0, d_course_koopa_troopa_beach_dl_6410, d_course_koopa_troopa_beach_dl_62F8,
    d_course_koopa_troopa_beach_dl_64F0, d_course_koopa_troopa_beach_dl_6660, d_course_koopa_troopa_beach_dl_6898,
    d_course_koopa_troopa_beach_dl_67A0, d_course_koopa_troopa_beach_dl_6968, d_course_koopa_troopa_beach_dl_6AC0,
    d_course_koopa_troopa_beach_dl_6D10, d_course_koopa_troopa_beach_dl_6C18, d_course_koopa_troopa_beach_dl_6E18,
    d_course_koopa_troopa_beach_dl_6F70, d_course_koopa_troopa_beach_dl_71C8, d_course_koopa_troopa_beach_dl_7100,
    d_course_koopa_troopa_beach_dl_7338, d_course_koopa_troopa_beach_dl_7478, d_course_koopa_troopa_beach_dl_7718,
    d_course_koopa_troopa_beach_dl_7640, d_course_koopa_troopa_beach_dl_78E8, d_course_koopa_troopa_beach_dl_79E8,
    d_course_koopa_troopa_beach_dl_7C08, d_course_koopa_troopa_beach_dl_7B50, d_course_koopa_troopa_beach_dl_7D58,
    d_course_koopa_troopa_beach_dl_7E38, d_course_koopa_troopa_beach_dl_8048, d_course_koopa_troopa_beach_dl_7F68,
    d_course_koopa_troopa_beach_dl_8150, d_course_koopa_troopa_beach_dl_8228, d_course_koopa_troopa_beach_dl_8448,
    d_course_koopa_troopa_beach_dl_8368, d_course_koopa_troopa_beach_dl_8550, d_course_koopa_troopa_beach_dl_8650,
    d_course_koopa_troopa_beach_dl_8878, d_course_koopa_troopa_beach_dl_8790, d_course_koopa_troopa_beach_dl_8958,
    d_course_koopa_troopa_beach_dl_8A98, d_course_koopa_troopa_beach_dl_8D48, d_course_koopa_troopa_beach_dl_8C48,
    d_course_koopa_troopa_beach_dl_8E30, d_course_koopa_troopa_beach_dl_9008, d_course_koopa_troopa_beach_dl_9288,
    d_course_koopa_troopa_beach_dl_9188, d_course_koopa_troopa_beach_dl_9378, d_course_koopa_troopa_beach_dl_9548,
    d_course_koopa_troopa_beach_dl_9798, d_course_koopa_troopa_beach_dl_96A0, d_course_koopa_troopa_beach_dl_98A0,
    d_course_koopa_troopa_beach_dl_9A30, d_course_koopa_troopa_beach_dl_9C38, d_course_koopa_troopa_beach_dl_9B58,
    d_course_koopa_troopa_beach_dl_9D98, d_course_koopa_troopa_beach_dl_9ED8, d_course_koopa_troopa_beach_dl_A0D0,
    d_course_koopa_troopa_beach_dl_9FE8, d_course_koopa_troopa_beach_dl_A220, d_course_koopa_troopa_beach_dl_A340,
    d_course_koopa_troopa_beach_dl_A608, d_course_koopa_troopa_beach_dl_A4D8, d_course_koopa_troopa_beach_dl_A780,
    d_course_koopa_troopa_beach_dl_A8D0, d_course_koopa_troopa_beach_dl_AB88, d_course_koopa_troopa_beach_dl_AA38,
    d_course_koopa_troopa_beach_dl_AC68,
};

Gfx* koopa_troopa_beach_dls2[] = {
    d_course_koopa_troopa_beach_dl_118,  d_course_koopa_troopa_beach_dl_328,  d_course_koopa_troopa_beach_dl_218,
    d_course_koopa_troopa_beach_dl_458,  d_course_koopa_troopa_beach_dl_530,  d_course_koopa_troopa_beach_dl_790,
    d_course_koopa_troopa_beach_dl_658,  d_course_koopa_troopa_beach_dl_8F0,  d_course_koopa_troopa_beach_dl_9E0,
    d_course_koopa_troopa_beach_dl_C50,  d_course_koopa_troopa_beach_dl_B10,  d_course_koopa_troopa_beach_dl_DB8,
    d_course_koopa_troopa_beach_dl_EE0,  d_course_koopa_troopa_beach_dl_1178, d_course_koopa_troopa_beach_dl_1040,
    d_course_koopa_troopa_beach_dl_1310, d_course_koopa_troopa_beach_dl_1430, d_course_koopa_troopa_beach_dl_1680,
    d_course_koopa_troopa_beach_dl_1558, d_course_koopa_troopa_beach_dl_17B0, d_course_koopa_troopa_beach_dl_1878,
    d_course_koopa_troopa_beach_dl_19E8, d_course_koopa_troopa_beach_dl_1940, d_course_koopa_troopa_beach_dl_1AA8,
    d_course_koopa_troopa_beach_dl_1B50, d_course_koopa_troopa_beach_dl_1CB0, d_course_koopa_troopa_beach_dl_1C00,
    d_course_koopa_troopa_beach_dl_1D88, d_course_koopa_troopa_beach_dl_1E40, d_course_koopa_troopa_beach_dl_2058,
    d_course_koopa_troopa_beach_dl_1F68, d_course_koopa_troopa_beach_dl_2178, d_course_koopa_troopa_beach_dl_2258,
    d_course_koopa_troopa_beach_dl_24C0, d_course_koopa_troopa_beach_dl_2390, d_course_koopa_troopa_beach_dl_25D8,
    d_course_koopa_troopa_beach_dl_26E0, d_course_koopa_troopa_beach_dl_2910, d_course_koopa_troopa_beach_dl_2800,
    d_course_koopa_troopa_beach_dl_2A18, d_course_koopa_troopa_beach_dl_2B08, d_course_koopa_troopa_beach_dl_2D50,
    d_course_koopa_troopa_beach_dl_2C28, d_course_koopa_troopa_beach_dl_2E38, d_course_koopa_troopa_beach_dl_2F30,
    d_course_koopa_troopa_beach_dl_3198, d_course_koopa_troopa_beach_dl_3050, d_course_koopa_troopa_beach_dl_3278,
    d_course_koopa_troopa_beach_dl_3370, d_course_koopa_troopa_beach_dl_3628, d_course_koopa_troopa_beach_dl_34B8,
    d_course_koopa_troopa_beach_dl_3730, d_course_koopa_troopa_beach_dl_3830, d_course_koopa_troopa_beach_dl_3B10,
    d_course_koopa_troopa_beach_dl_3980, d_course_koopa_troopa_beach_dl_3C28, d_course_koopa_troopa_beach_dl_3D40,
    d_course_koopa_troopa_beach_dl_4028, d_course_koopa_troopa_beach_dl_3EA8, d_course_koopa_troopa_beach_dl_4138,
    d_course_koopa_troopa_beach_dl_4260, d_course_koopa_troopa_beach_dl_4548, d_course_koopa_troopa_beach_dl_43C0,
    d_course_koopa_troopa_beach_dl_4670, d_course_koopa_troopa_beach_dl_47C0, d_course_koopa_troopa_beach_dl_4B28,
    d_course_koopa_troopa_beach_dl_4948, d_course_koopa_troopa_beach_dl_4C70, d_course_koopa_troopa_beach_dl_4DC0,
    d_course_koopa_troopa_beach_dl_5120, d_course_koopa_troopa_beach_dl_4F58, d_course_koopa_troopa_beach_dl_5248,
    d_course_koopa_troopa_beach_dl_5358, d_course_koopa_troopa_beach_dl_55D0, d_course_koopa_troopa_beach_dl_5480,
    d_course_koopa_troopa_beach_dl_56D0, d_course_koopa_troopa_beach_dl_5840, d_course_koopa_troopa_beach_dl_5B48,
    d_course_koopa_troopa_beach_dl_59C8, d_course_koopa_troopa_beach_dl_5C88, d_course_koopa_troopa_beach_dl_5DB8,
    d_course_koopa_troopa_beach_dl_6000, d_course_koopa_troopa_beach_dl_5EE0, d_course_koopa_troopa_beach_dl_6128,
    d_course_koopa_troopa_beach_dl_6268, d_course_koopa_troopa_beach_dl_6498, d_course_koopa_troopa_beach_dl_6398,
    d_course_koopa_troopa_beach_dl_65B8, d_course_koopa_troopa_beach_dl_6718, d_course_koopa_troopa_beach_dl_6918,
    d_course_koopa_troopa_beach_dl_6830, d_course_koopa_troopa_beach_dl_6A28, d_course_koopa_troopa_beach_dl_6B88,
    d_course_koopa_troopa_beach_dl_6DB8, d_course_koopa_troopa_beach_dl_6CA0, d_course_koopa_troopa_beach_dl_6ED0,
    d_course_koopa_troopa_beach_dl_7060, d_course_koopa_troopa_beach_dl_72B0, d_course_koopa_troopa_beach_dl_7168,
    d_course_koopa_troopa_beach_dl_73F8, d_course_koopa_troopa_beach_dl_75A0, d_course_koopa_troopa_beach_dl_7840,
    d_course_koopa_troopa_beach_dl_76A8, d_course_koopa_troopa_beach_dl_7978, d_course_koopa_troopa_beach_dl_7AE0,
    d_course_koopa_troopa_beach_dl_7CE0, d_course_koopa_troopa_beach_dl_7BA0, d_course_koopa_troopa_beach_dl_7DD0,
    d_course_koopa_troopa_beach_dl_7EE0, d_course_koopa_troopa_beach_dl_80E0, d_course_koopa_troopa_beach_dl_7FC8,
    d_course_koopa_troopa_beach_dl_81B8, d_course_koopa_troopa_beach_dl_82D8, d_course_koopa_troopa_beach_dl_84D0,
    d_course_koopa_troopa_beach_dl_83D0, d_course_koopa_troopa_beach_dl_85E8, d_course_koopa_troopa_beach_dl_8710,
    d_course_koopa_troopa_beach_dl_88F0, d_course_koopa_troopa_beach_dl_8800, d_course_koopa_troopa_beach_dl_8A20,
    d_course_koopa_troopa_beach_dl_8BB0, d_course_koopa_troopa_beach_dl_8DC0, d_course_koopa_troopa_beach_dl_8CC8,
    d_course_koopa_troopa_beach_dl_8F50, d_course_koopa_troopa_beach_dl_90F0, d_course_koopa_troopa_beach_dl_92E8,
    d_course_koopa_troopa_beach_dl_9210, d_course_koopa_troopa_beach_dl_9498, d_course_koopa_troopa_beach_dl_9618,
    d_course_koopa_troopa_beach_dl_9818, d_course_koopa_troopa_beach_dl_9720, d_course_koopa_troopa_beach_dl_99A0,
    d_course_koopa_troopa_beach_dl_9AE0, d_course_koopa_troopa_beach_dl_9D00, d_course_koopa_troopa_beach_dl_9BC8,
    d_course_koopa_troopa_beach_dl_9E60, d_course_koopa_troopa_beach_dl_9F78, d_course_koopa_troopa_beach_dl_A190,
    d_course_koopa_troopa_beach_dl_A070, d_course_koopa_troopa_beach_dl_A2E8, d_course_koopa_troopa_beach_dl_A428,
    d_course_koopa_troopa_beach_dl_A6F0, d_course_koopa_troopa_beach_dl_A578, d_course_koopa_troopa_beach_dl_A828,
    d_course_koopa_troopa_beach_dl_A9A0, d_course_koopa_troopa_beach_dl_AC10, d_course_koopa_troopa_beach_dl_AAE8,
    d_course_koopa_troopa_beach_dl_AD40,
};

// 0x06014828
// animations? lights?
uintptr_t d_course_koopa_troopa_beach_unk_data1[] = {
    0x00000000,
    0x00000001,
    0x00000000,
    0x00000001,
};

// 0x06014838
s16 d_course_koopa_troopa_beach_unk_data2[] = {
    0x0000, 0x0000, 0x0000, 0x0001, 0x0002, 0x0003, 0x0005, 0x0006, 0x0007, 0x0007, 0x0007, 0x0006, 0x0005, 0x0003,
    0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
    0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0x0001, 0x0001,
    0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0x0000, 0x0001, 0x0001, 0x0001, 0x0000, 0x0000, 0x0000,
    0x3fff, 0x3fff, 0x3fff, 0x18a7, 0x0e34, 0x03c1, 0xfaa6, 0xf434, 0xf1c2, 0xf434, 0xfaa7, 0x03c3, 0x0e36, 0x18a8,
    0x21c6, 0x2838, 0x2aaa, 0x2845, 0x216b, 0x1728, 0x0b90, 0x00b6, 0xf8af, 0xf58c, 0xf8e3, 0x003c, 0x0795, 0x0aed,
    0x0aaa, 0x09ed, 0x08cc, 0x0759, 0x05ab, 0x03d3, 0x01e8, 0xfffe, 0xfe26, 0xfc78, 0xfb05, 0xf9e4, 0xf927, 0xf8e4,
    0x0000, 0x071c, 0x0652, 0x044a, 0x0183, 0xfe7d, 0xfbb6, 0xf9ae, 0xf8e4, 0xf9f5, 0xfcea, 0x0163, 0x0705, 0x0d71,
    0x1449, 0x1b2f, 0xfac3, 0xf4e1, 0xf2ba, 0xf570, 0xfc94, 0x06ac, 0x1240, 0x1dd3, 0x27ec, 0x2f11, 0x31c6, 0x2e8b,
    0x263e, 0x1afb, 0x0edc, 0x03f9, 0xfcee, 0xfa23, 0xfa72, 0xfcb3, 0xffbd, 0x0268, 0x038e, 0x0387, 0x0373, 0x0355,
    0x032f, 0x0305, 0x02d8, 0x02ab, 0x0280, 0x025b, 0x023d, 0x0229, 0x0222, 0x05b0, 0x0143, 0xf90d, 0xf4a0, 0x071c,
    0x0dfa, 0x1110, 0x11c6, 0x0f68, 0x097b, 0x01c7, 0xfa14, 0xf427, 0xf1c8, 0xfc72, 0x071c, 0x0716, 0x06e6, 0x065d,
    0x054b, 0xfdde, 0xe698, 0xeea7, 0xfa0b, 0x0719, 0x1428, 0x1f8d, 0x279b, 0x2aaa, 0x2816, 0x2131, 0x173d, 0x0b7b,
    0xff2d, 0xf394, 0xe9f3, 0xe38a, 0xe234, 0xe55c, 0xea9a, 0xef85, 0xf1b8, 0xf1ba, 0xf1b7, 0xf1b7, 0xf1c4, 0xf1d8,
    0xf1e7, 0xf1f3, 0xf1f9, 0xf1fc, 0xf1fa, 0xf1f4, 0xf1e9, 0xf1db, 0xf1c8, 0xee3a, 0xf4b0, 0x02ae, 0x103a, 0x1555,
    0x0b47, 0xfa83, 0xf1c8, 0xf05c, 0xee3a, 0xf330, 0xfe33, 0x0939, 0x0e38, 0x0bd6, 0x05bb, 0xfd65, 0xf451, 0xebfd,
    0xe5e6, 0xe741, 0xf1ad, 0xfc24, 0x054b, 0x0bc7, 0x0e3e, 0x0bcc, 0x0559, 0xfc3d, 0xf1ca, 0xe758, 0xde3a, 0xd7c8,
    0xd556, 0xd7c8, 0xde95, 0xe8b9, 0xf44d, 0xff32, 0x0749, 0x0a74, 0x071d, 0xffc4, 0xf86b, 0xf513, 0xf559, 0xf61d,
    0xf74a, 0xf8cb, 0xfa8a, 0xfc74, 0xfe73, 0x0071, 0x025b, 0x041a, 0x059b, 0x06c8, 0x078c, 0x07d2, 0x005b, 0xf8e4,
    0xf9ae, 0xfbb6, 0xfe7d, 0x0183, 0x044a, 0x0652, 0x071c, 0x060a, 0x0312, 0xfe95, 0xf8ef, 0xf281, 0xebaa, 0xe4c8,
    0x051e, 0x0b0f, 0x0d46, 0x0a90, 0x036c, 0xf954, 0xedc0, 0xe22d, 0xd814, 0xd0ef, 0xce3a, 0xd176, 0xd9c4, 0xe508,
    0xf128, 0xfc07, 0x030e, 0x05d8, 0x0589, 0x034a, 0x0041, 0xfd97, 0xfc72, 0xfc79, 0xfc8d, 0xfcab, 0xfcd1, 0xfcfb,
    0xfd28, 0xfd55, 0xfd80, 0xfda5, 0xfdc3, 0xfdd7, 0xfdde, 0xfa50, 0xfe8e, 0x066d, 0x0aaa, 0xf8e4, 0xf08a, 0xee33,
    0xee3a, 0xf098, 0xf685, 0xfe39, 0x05ec, 0x0bd9, 0x0e38, 0x0333, 0xf82e, 0xf848, 0xf8a7, 0xf968, 0xfaa7, 0x0222,
    0x1968, 0x1159, 0x05f5, 0xf8e7, 0xebd8, 0xe073, 0xd865, 0xd556, 0xd865, 0xe073, 0xebd8, 0xf8e7, 0x05f5, 0x1159,
    0x1968, 0x1c76, 0x1afd, 0x1778, 0x1346, 0x0fc2, 0x0e48, 0x0e46, 0x0e42, 0x0e3e, 0x0e3c, 0x0e3c, 0x0e3c, 0x0e3b,
    0x0e3b, 0x0e3a, 0x0e3a, 0x0e39, 0x0e39, 0x0e38, 0x0e38, 0x11c6, 0x0b51, 0xfd4c, 0xefbd, 0xeaab, 0xf4b6, 0x056a,
    0x0e38, 0x0fa4, 0x11c6, 0x0cd6, 0x01d4, 0xf6cd, 0xf1c8, 0xf425, 0xfa3e, 0x0294, 0x0ba9, 0x1400, 0x1a19, 0x3fff,
    0x3fff, 0xc001, 0x3fff, 0x8001,
};

// 0x06014B50
AnimationLimbVector d_course_koopa_troopa_beach_unk_data3[] = {
    {
        { 0x0001, 0x0000 },
        { 0x0037, 0x0001 },
        { 0x0001, 0x0000 },
    },
    {
        { 0x0001, 0x0038 },
        { 0x0001, 0x0039 },
        { 0x0001, 0x003a },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0000 },
        { 0x0001, 0x018b },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0189 },
        { 0x0001, 0x018a },
    },
    {
        { 0x0001, 0x00a9 },
        { 0x0037, 0x00aa },
        { 0x0001, 0x0000 },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0037, 0x0072 },
        { 0x0001, 0x0000 },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0037, 0x003b },
        { 0x0001, 0x0000 },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0001, 0x0187 },
        { 0x0001, 0x0188 },
    },
    {
        { 0x0001, 0x014f },
        { 0x0037, 0x0150 },
        { 0x0001, 0x0000 },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0037, 0x0118 },
        { 0x0001, 0x0000 },
    },
    {
        { 0x0001, 0x0000 },
        { 0x0037, 0x00e1 },
        { 0x0001, 0x0000 },
    },
};

// 0x06014BD4
Animation d_course_koopa_troopa_beach_unk_data4 = {
    0x00010000,
    0x00000000,
    0x0037,
    0x000a,
    d_course_koopa_troopa_beach_unk_data2,
    d_course_koopa_troopa_beach_unk_data3,
};

//! 0x60169b
uintptr_t d_course_koopa_troopa_beach_unk4[] = {
    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_ADD_POS(0x00000000, 0x00000000, 0x00000000),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_RENDER_MODEL(d_course_koopa_troopa_beach_dl_16630),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_ADD_POS(0x00000000, -0x00000005, 0x00000005),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_RENDER_MODEL(d_course_koopa_troopa_beach_dl_16850),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_RENDER_MODEL_AT(d_course_koopa_troopa_beach_dl_168F0, 0x00000024, 0x00000000, 0x00000000),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_RENDER_MODEL_AT(d_course_koopa_troopa_beach_dl_16990, 0x00000019, 0x00000000, 0x00000000),

    ANIMATION_POP_MATRIX,

    ANIMATION_POP_MATRIX,

    ANIMATION_POP_MATRIX,

    ANIMATION_ADD_POS(0x00000000, -0x00000005, -0x00000005),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_RENDER_MODEL(d_course_koopa_troopa_beach_dl_16850),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_RENDER_MODEL_AT(d_course_koopa_troopa_beach_dl_168F0, 0x00000024, 0x00000000, 0x00000000),

    ANIMATION_DISABLE_AUTOMATIC_POP,

    ANIMATION_RENDER_MODEL_AT(d_course_koopa_troopa_beach_dl_16990, 0x00000019, 0x00000000, 0x00000000),

    ANIMATION_POP_MATRIX,

    ANIMATION_POP_MATRIX,

    ANIMATION_POP_MATRIX,

    ANIMATION_POP_MATRIX,

    ANIMATION_POP_MATRIX,

    ANIMATION_POP_MATRIX,

    ANIMATION_STOP,
};

// 0x06016B60
Animation* d_course_koopa_troopa_beach_unk_data5[] = {
    &d_course_koopa_troopa_beach_unk_data4,
};

// No idea what this stuff is
uintptr_t d_course_koopa_troopa_beach_unk_data6[] = {
    0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x00000001,
};
