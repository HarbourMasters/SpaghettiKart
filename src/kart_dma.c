#include <libultraship.h>
#include <macros.h>
#include "main.h"
#include "buffers.h"
#include <common_structs.h>
#include "kart_dma.h"
#include "defines.h"
#include <assets/textures/karts/mario_kart.h>
#include <assets/textures/karts/luigi_kart.h>
#include <assets/textures/karts/peach_kart.h>
#include <assets/textures/karts/yoshi_kart.h>
#include <assets/textures/karts/toad_kart.h>
#include <assets/textures/karts/bowser_kart.h>
#include <assets/textures/karts/wario_kart.h>
#include <assets/textures/karts/donkeykong_kart.h>
#include "render_player.h"

// Seemingly the largest compressed size for a frame for a given character
// They seem slightly oversized too, either an alignemnt or safety thing
u16 D_800DDEB0[] = {
    0x06c0, 0x06e0, 0x06e0, 0x0680, 0x07c0, 0x0700, 0x0680, 0x0910,
};

#define KART_TEXTURE_MACRO(var) \
    u8* var[4] = { \
        (u8*)var##Wheel0, (u8*)var##Wheel1, (u8*)var##Wheel2, (u8*)var##Wheel3 \
    };

#define GROUP_0_COMMENT // group 0
#define GROUP_1_COMMENT // group 1
#define GROUP_2_COMMENT // group 2
#define GROUP_3_COMMENT // group 3
#define GROUP_4_COMMENT // group 4
#define GROUP_5_COMMENT // group 5
#define GROUP_6_COMMENT // group 6
#define GROUP_7_COMMENT // group 7
#define GROUP_8_COMMENT // group 8
#define GROUP_9_10_COMMENT // group 9 and 10
#define GROUP_11_COMMENT // group 11
#define GROUP_12_13_14_COMMENT // group 12, 13 and 14
#define GROUP_15_COMMENT // group 15
#define GROUP_16_17_COMMENT // group 16 and 17

#define ALSO_INCLUDE_GROUP_0_1 // also includes group 0 and 1
#define ALSO_INCLUDE_GROUP_2 // also includes group 2
#define ALSO_INCLUDE_GROUP_3_4_5 // also includes group 3, 4 and 5
#define ALSO_INCLUDE_GROUP_6 // also includes group 6
#define ALSO_INCLUDE_GROUP_7_8 // also includes group 7 and 8

#define DECLARE_KART_TEXTURES(var) \
    GROUP_0_COMMENT \
    KART_TEXTURE_MACRO(var##000) \
    KART_TEXTURE_MACRO(var##001) \
    KART_TEXTURE_MACRO(var##002) \
    KART_TEXTURE_MACRO(var##003) \
    KART_TEXTURE_MACRO(var##004) \
    KART_TEXTURE_MACRO(var##005) \
    KART_TEXTURE_MACRO(var##006) \
    KART_TEXTURE_MACRO(var##007) \
    KART_TEXTURE_MACRO(var##008) \
    KART_TEXTURE_MACRO(var##009) \
    KART_TEXTURE_MACRO(var##010) \
    KART_TEXTURE_MACRO(var##011) \
    KART_TEXTURE_MACRO(var##012) \
    KART_TEXTURE_MACRO(var##013) \
    KART_TEXTURE_MACRO(var##014) \
    KART_TEXTURE_MACRO(var##015) \
    KART_TEXTURE_MACRO(var##016) \
    KART_TEXTURE_MACRO(var##017) \
    KART_TEXTURE_MACRO(var##018) \
    KART_TEXTURE_MACRO(var##019) \
    KART_TEXTURE_MACRO(var##020) \
    GROUP_1_COMMENT \
    KART_TEXTURE_MACRO(var##021) \
    KART_TEXTURE_MACRO(var##022) \
    KART_TEXTURE_MACRO(var##023) \
    KART_TEXTURE_MACRO(var##024) \
    KART_TEXTURE_MACRO(var##025) \
    KART_TEXTURE_MACRO(var##026) \
    KART_TEXTURE_MACRO(var##027) \
    KART_TEXTURE_MACRO(var##028) \
    KART_TEXTURE_MACRO(var##029) \
    KART_TEXTURE_MACRO(var##030) \
    KART_TEXTURE_MACRO(var##031) \
    KART_TEXTURE_MACRO(var##032) \
    KART_TEXTURE_MACRO(var##033) \
    KART_TEXTURE_MACRO(var##034) \
    KART_TEXTURE_MACRO(var##035) \
    KART_TEXTURE_MACRO(var##036) \
    KART_TEXTURE_MACRO(var##037) \
    KART_TEXTURE_MACRO(var##038) \
    KART_TEXTURE_MACRO(var##039) \
    KART_TEXTURE_MACRO(var##040) \
    KART_TEXTURE_MACRO(var##041) \
    GROUP_2_COMMENT \
    KART_TEXTURE_MACRO(var##042) \
    KART_TEXTURE_MACRO(var##043) \
    KART_TEXTURE_MACRO(var##044) \
    KART_TEXTURE_MACRO(var##045) \
    KART_TEXTURE_MACRO(var##046) \
    KART_TEXTURE_MACRO(var##047) \
    KART_TEXTURE_MACRO(var##048) \
    KART_TEXTURE_MACRO(var##049) \
    KART_TEXTURE_MACRO(var##050) \
    KART_TEXTURE_MACRO(var##051) \
    KART_TEXTURE_MACRO(var##052) \
    KART_TEXTURE_MACRO(var##053) \
    KART_TEXTURE_MACRO(var##054) \
    KART_TEXTURE_MACRO(var##055) \
    KART_TEXTURE_MACRO(var##056) \
    KART_TEXTURE_MACRO(var##057) \
    KART_TEXTURE_MACRO(var##058) \
    KART_TEXTURE_MACRO(var##059) \
    KART_TEXTURE_MACRO(var##060) \
    KART_TEXTURE_MACRO(var##061) \
    KART_TEXTURE_MACRO(var##062) \
    GROUP_3_COMMENT \
    KART_TEXTURE_MACRO(var##063) \
    KART_TEXTURE_MACRO(var##064) \
    KART_TEXTURE_MACRO(var##065) \
    KART_TEXTURE_MACRO(var##066) \
    KART_TEXTURE_MACRO(var##067) \
    KART_TEXTURE_MACRO(var##068) \
    KART_TEXTURE_MACRO(var##069) \
    KART_TEXTURE_MACRO(var##070) \
    KART_TEXTURE_MACRO(var##071) \
    KART_TEXTURE_MACRO(var##072) \
    KART_TEXTURE_MACRO(var##073) \
    KART_TEXTURE_MACRO(var##074) \
    KART_TEXTURE_MACRO(var##075) \
    KART_TEXTURE_MACRO(var##076) \
    KART_TEXTURE_MACRO(var##077) \
    KART_TEXTURE_MACRO(var##078) \
    KART_TEXTURE_MACRO(var##079) \
    KART_TEXTURE_MACRO(var##080) \
    KART_TEXTURE_MACRO(var##081) \
    KART_TEXTURE_MACRO(var##082) \
    KART_TEXTURE_MACRO(var##083) \
    GROUP_4_COMMENT \
    KART_TEXTURE_MACRO(var##084) \
    KART_TEXTURE_MACRO(var##085) \
    KART_TEXTURE_MACRO(var##086) \
    KART_TEXTURE_MACRO(var##087) \
    KART_TEXTURE_MACRO(var##088) \
    KART_TEXTURE_MACRO(var##089) \
    KART_TEXTURE_MACRO(var##090) \
    KART_TEXTURE_MACRO(var##091) \
    KART_TEXTURE_MACRO(var##092) \
    KART_TEXTURE_MACRO(var##093) \
    KART_TEXTURE_MACRO(var##094) \
    KART_TEXTURE_MACRO(var##095) \
    KART_TEXTURE_MACRO(var##096) \
    KART_TEXTURE_MACRO(var##097) \
    KART_TEXTURE_MACRO(var##098) \
    KART_TEXTURE_MACRO(var##099) \
    KART_TEXTURE_MACRO(var##100) \
    KART_TEXTURE_MACRO(var##101) \
    KART_TEXTURE_MACRO(var##102) \
    KART_TEXTURE_MACRO(var##103) \
    KART_TEXTURE_MACRO(var##104) \
    GROUP_5_COMMENT \
    KART_TEXTURE_MACRO(var##105) \
    KART_TEXTURE_MACRO(var##106) \
    KART_TEXTURE_MACRO(var##107) \
    KART_TEXTURE_MACRO(var##108) \
    KART_TEXTURE_MACRO(var##109) \
    KART_TEXTURE_MACRO(var##110) \
    KART_TEXTURE_MACRO(var##111) \
    KART_TEXTURE_MACRO(var##112) \
    KART_TEXTURE_MACRO(var##113) \
    KART_TEXTURE_MACRO(var##114) \
    KART_TEXTURE_MACRO(var##115) \
    KART_TEXTURE_MACRO(var##116) \
    KART_TEXTURE_MACRO(var##117) \
    KART_TEXTURE_MACRO(var##118) \
    KART_TEXTURE_MACRO(var##119) \
    KART_TEXTURE_MACRO(var##120) \
    KART_TEXTURE_MACRO(var##121) \
    KART_TEXTURE_MACRO(var##122) \
    KART_TEXTURE_MACRO(var##123) \
    KART_TEXTURE_MACRO(var##124) \
    KART_TEXTURE_MACRO(var##125) \
    GROUP_6_COMMENT \
    KART_TEXTURE_MACRO(var##126) \
    KART_TEXTURE_MACRO(var##127) \
    KART_TEXTURE_MACRO(var##128) \
    KART_TEXTURE_MACRO(var##129) \
    KART_TEXTURE_MACRO(var##130) \
    KART_TEXTURE_MACRO(var##131) \
    KART_TEXTURE_MACRO(var##132) \
    KART_TEXTURE_MACRO(var##133) \
    KART_TEXTURE_MACRO(var##134) \
    KART_TEXTURE_MACRO(var##135) \
    KART_TEXTURE_MACRO(var##136) \
    KART_TEXTURE_MACRO(var##137) \
    KART_TEXTURE_MACRO(var##138) \
    KART_TEXTURE_MACRO(var##139) \
    KART_TEXTURE_MACRO(var##140) \
    KART_TEXTURE_MACRO(var##141) \
    KART_TEXTURE_MACRO(var##142) \
    KART_TEXTURE_MACRO(var##143) \
    KART_TEXTURE_MACRO(var##144) \
    KART_TEXTURE_MACRO(var##145) \
    KART_TEXTURE_MACRO(var##146) \
    GROUP_7_COMMENT \
    KART_TEXTURE_MACRO(var##147) \
    KART_TEXTURE_MACRO(var##148) \
    KART_TEXTURE_MACRO(var##149) \
    KART_TEXTURE_MACRO(var##150) \
    KART_TEXTURE_MACRO(var##151) \
    KART_TEXTURE_MACRO(var##152) \
    KART_TEXTURE_MACRO(var##153) \
    KART_TEXTURE_MACRO(var##154) \
    KART_TEXTURE_MACRO(var##155) \
    KART_TEXTURE_MACRO(var##156) \
    KART_TEXTURE_MACRO(var##157) \
    KART_TEXTURE_MACRO(var##158) \
    KART_TEXTURE_MACRO(var##159) \
    KART_TEXTURE_MACRO(var##160) \
    KART_TEXTURE_MACRO(var##161) \
    KART_TEXTURE_MACRO(var##162) \
    KART_TEXTURE_MACRO(var##163) \
    KART_TEXTURE_MACRO(var##164) \
    KART_TEXTURE_MACRO(var##165) \
    KART_TEXTURE_MACRO(var##166) \
    KART_TEXTURE_MACRO(var##167) \
    GROUP_8_COMMENT \
    KART_TEXTURE_MACRO(var##168) \
    KART_TEXTURE_MACRO(var##169) \
    KART_TEXTURE_MACRO(var##170) \
    KART_TEXTURE_MACRO(var##171) \
    KART_TEXTURE_MACRO(var##172) \
    KART_TEXTURE_MACRO(var##173) \
    KART_TEXTURE_MACRO(var##174) \
    KART_TEXTURE_MACRO(var##175) \
    KART_TEXTURE_MACRO(var##176) \
    KART_TEXTURE_MACRO(var##177) \
    KART_TEXTURE_MACRO(var##178) \
    KART_TEXTURE_MACRO(var##179) \
    KART_TEXTURE_MACRO(var##180) \
    KART_TEXTURE_MACRO(var##181) \
    KART_TEXTURE_MACRO(var##182) \
    KART_TEXTURE_MACRO(var##183) \
    KART_TEXTURE_MACRO(var##184) \
    KART_TEXTURE_MACRO(var##185) \
    KART_TEXTURE_MACRO(var##186) \
    KART_TEXTURE_MACRO(var##187) \
    KART_TEXTURE_MACRO(var##188) \
    GROUP_9_10_COMMENT \
    KART_TEXTURE_MACRO(var##189) \
    KART_TEXTURE_MACRO(var##190) \
    KART_TEXTURE_MACRO(var##191) \
    KART_TEXTURE_MACRO(var##192) \
    KART_TEXTURE_MACRO(var##193) \
    KART_TEXTURE_MACRO(var##194) \
    ALSO_INCLUDE_GROUP_0_1 \
    KART_TEXTURE_MACRO(var##195) \
    KART_TEXTURE_MACRO(var##196) \
    KART_TEXTURE_MACRO(var##197) \
    KART_TEXTURE_MACRO(var##198) \
    KART_TEXTURE_MACRO(var##199) \
    KART_TEXTURE_MACRO(var##200) \
    KART_TEXTURE_MACRO(var##201) \
    KART_TEXTURE_MACRO(var##202) \
    KART_TEXTURE_MACRO(var##203) \
    KART_TEXTURE_MACRO(var##204) \
    KART_TEXTURE_MACRO(var##205) \
    KART_TEXTURE_MACRO(var##206) \
    KART_TEXTURE_MACRO(var##207) \
    KART_TEXTURE_MACRO(var##208) \
    GROUP_11_COMMENT \
    KART_TEXTURE_MACRO(var##209) \
    KART_TEXTURE_MACRO(var##210) \
    KART_TEXTURE_MACRO(var##211) \
    KART_TEXTURE_MACRO(var##212) \
    KART_TEXTURE_MACRO(var##213) \
    KART_TEXTURE_MACRO(var##214) \
    ALSO_INCLUDE_GROUP_2 \
    KART_TEXTURE_MACRO(var##215) \
    KART_TEXTURE_MACRO(var##216) \
    KART_TEXTURE_MACRO(var##217) \
    KART_TEXTURE_MACRO(var##218) \
    KART_TEXTURE_MACRO(var##219) \
    KART_TEXTURE_MACRO(var##220) \
    KART_TEXTURE_MACRO(var##221) \
    KART_TEXTURE_MACRO(var##222) \
    KART_TEXTURE_MACRO(var##223) \
    KART_TEXTURE_MACRO(var##224) \
    KART_TEXTURE_MACRO(var##225) \
    KART_TEXTURE_MACRO(var##226) \
    KART_TEXTURE_MACRO(var##227) \
    KART_TEXTURE_MACRO(var##228) \
    GROUP_12_13_14_COMMENT \
    KART_TEXTURE_MACRO(var##229) \
    KART_TEXTURE_MACRO(var##230) \
    KART_TEXTURE_MACRO(var##231) \
    KART_TEXTURE_MACRO(var##232) \
    KART_TEXTURE_MACRO(var##233) \
    KART_TEXTURE_MACRO(var##234) \
    ALSO_INCLUDE_GROUP_3_4_5 \
    KART_TEXTURE_MACRO(var##235) \
    KART_TEXTURE_MACRO(var##236) \
    KART_TEXTURE_MACRO(var##237) \
    KART_TEXTURE_MACRO(var##238) \
    KART_TEXTURE_MACRO(var##239) \
    KART_TEXTURE_MACRO(var##240) \
    KART_TEXTURE_MACRO(var##241) \
    KART_TEXTURE_MACRO(var##242) \
    KART_TEXTURE_MACRO(var##243) \
    KART_TEXTURE_MACRO(var##244) \
    KART_TEXTURE_MACRO(var##245) \
    KART_TEXTURE_MACRO(var##246) \
    KART_TEXTURE_MACRO(var##247) \
    KART_TEXTURE_MACRO(var##248) \
    GROUP_15_COMMENT \
    KART_TEXTURE_MACRO(var##249) \
    KART_TEXTURE_MACRO(var##250) \
    KART_TEXTURE_MACRO(var##251) \
    KART_TEXTURE_MACRO(var##252) \
    KART_TEXTURE_MACRO(var##253) \
    KART_TEXTURE_MACRO(var##254) \
    ALSO_INCLUDE_GROUP_6 \
    KART_TEXTURE_MACRO(var##255) \
    KART_TEXTURE_MACRO(var##256) \
    KART_TEXTURE_MACRO(var##257) \
    KART_TEXTURE_MACRO(var##258) \
    KART_TEXTURE_MACRO(var##259) \
    KART_TEXTURE_MACRO(var##260) \
    KART_TEXTURE_MACRO(var##261) \
    KART_TEXTURE_MACRO(var##262) \
    KART_TEXTURE_MACRO(var##263) \
    KART_TEXTURE_MACRO(var##264) \
    KART_TEXTURE_MACRO(var##265) \
    KART_TEXTURE_MACRO(var##266) \
    KART_TEXTURE_MACRO(var##267) \
    KART_TEXTURE_MACRO(var##268) \
    GROUP_16_17_COMMENT \
    KART_TEXTURE_MACRO(var##269) \
    KART_TEXTURE_MACRO(var##270) \
    KART_TEXTURE_MACRO(var##271) \
    KART_TEXTURE_MACRO(var##272) \
    KART_TEXTURE_MACRO(var##273) \
    KART_TEXTURE_MACRO(var##274) \
    ALSO_INCLUDE_GROUP_7_8 \
    KART_TEXTURE_MACRO(var##275) \
    KART_TEXTURE_MACRO(var##276) \
    KART_TEXTURE_MACRO(var##277) \
    KART_TEXTURE_MACRO(var##278) \
    KART_TEXTURE_MACRO(var##279) \
    KART_TEXTURE_MACRO(var##280) \
    KART_TEXTURE_MACRO(var##281) \
    KART_TEXTURE_MACRO(var##282) \
    KART_TEXTURE_MACRO(var##283) \
    KART_TEXTURE_MACRO(var##284) \
    KART_TEXTURE_MACRO(var##285) \
    KART_TEXTURE_MACRO(var##286) \
    KART_TEXTURE_MACRO(var##287) \
    KART_TEXTURE_MACRO(var##288)

#define DECLARE_KART_GROUP(var) \
    kart_texture_t var##Group0[] = { \
        var##000, var##001, var##002, var##003, var##004, var##005, var##006, \
        var##007, var##008, var##009, var##010, var##011, var##012, var##013, \
        var##014, var##015, var##016, var##017, var##018, var##019, var##020, \
        var##195, var##196, var##197, var##198, var##199, var##200, var##201, \
        var##202, var##203, var##204, var##205, var##206, var##207, var##208, \
    }; \
    kart_texture_t var##Group1[] = { \
        var##021, var##022, var##023, var##024, var##025, var##026, var##027, \
        var##028, var##029, var##030, var##031, var##032, var##033, var##034, \
        var##035, var##036, var##037, var##038, var##039, var##040, var##041, \
        var##195, var##196, var##197, var##198, var##199, var##200, var##201, \
        var##202, var##203, var##204, var##205, var##206, var##207, var##208, \
    }; \
    kart_texture_t var##Group2[] = { \
        var##042, var##043, var##044, var##045, var##046, var##047, var##048, \
        var##049, var##050, var##051, var##052, var##053, var##054, var##055, \
        var##056, var##057, var##058, var##059, var##060, var##061, var##062, \
        var##215, var##216, var##217, var##218, var##219, var##220, var##221, \
        var##222, var##223, var##224, var##225, var##226, var##227, var##228, \
    }; \
    kart_texture_t var##Group3[] = { \
        var##063, var##064, var##065, var##066, var##067, var##068, var##069, \
        var##070, var##071, var##072, var##073, var##074, var##075, var##076, \
        var##077, var##078, var##079, var##080, var##081, var##082, var##083, \
        var##235, var##236, var##237, var##238, var##239, var##240, var##241, \
        var##242, var##243, var##244, var##245, var##246, var##247, var##248, \
    }; \
    kart_texture_t var##Group4[] = { \
        var##084, var##085, var##086, var##087, var##088, var##089, var##090, \
        var##091, var##092, var##093, var##094, var##095, var##096, var##097, \
        var##098, var##099, var##100, var##101, var##102, var##103, var##104, \
        var##235, var##236, var##237, var##238, var##239, var##240, var##241, \
        var##242, var##243, var##244, var##245, var##246, var##247, var##248, \
    }; \
    kart_texture_t var##Group5[] = { \
        var##105, var##106, var##107, var##108, var##109, var##110, var##111, \
        var##112, var##113, var##114, var##115, var##116, var##117, var##118, \
        var##119, var##120, var##121, var##122, var##123, var##124, var##125, \
        var##235, var##236, var##237, var##238, var##239, var##240, var##241, \
        var##242, var##243, var##244, var##245, var##246, var##247, var##248, \
    }; \
    kart_texture_t var##Group6[] = { \
        var##126, var##127, var##128, var##129, var##130, var##131, var##132, \
        var##133, var##134, var##135, var##136, var##137, var##138, var##139, \
        var##140, var##141, var##142, var##143, var##144, var##145, var##146, \
        var##255, var##256, var##257, var##258, var##259, var##260, var##261, \
        var##262, var##263, var##264, var##265, var##266, var##267, var##268, \
    }; \
    kart_texture_t var##Group7[] = { \
        var##147, var##148, var##149, var##150, var##151, var##152, var##153, \
        var##154, var##155, var##156, var##157, var##158, var##159, var##160, \
        var##161, var##162, var##163, var##164, var##165, var##166, var##167, \
        var##275, var##276, var##277, var##278, var##279, var##280, var##281, \
        var##282, var##283, var##284, var##285, var##286, var##287, var##288, \
    }; \
    kart_texture_t var##Group8[] = { \
        var##168, var##169, var##170, var##171, var##172, var##173, var##174, \
        var##175, var##176, var##177, var##178, var##179, var##180, var##181, \
        var##182, var##183, var##184, var##185, var##186, var##187, var##188, \
        var##275, var##276, var##277, var##278, var##279, var##280, var##281, \
        var##282, var##283, var##284, var##285, var##286, var##287, var##288, \
    }; \
    kart_texture_t var##Group9[] = { \
        var##189, var##190, var##191, var##192, var##193, var##194, var##195, \
        var##196, var##197, var##198, var##199, var##200, var##201, var##202, \
        var##203, var##204, var##205, var##206, var##207, var##208, \
    }; \
    kart_texture_t var##Group10[] = { \
        var##189, var##190, var##191, var##192, var##193, var##194, var##195, \
        var##196, var##197, var##198, var##199, var##200, var##201, var##202, \
        var##203, var##204, var##205, var##206, var##207, var##208, \
    }; \
    kart_texture_t var##Group11[] = { \
        var##209, var##210, var##211, var##212, var##213, var##214, var##215, \
        var##216, var##217, var##218, var##219, var##220, var##221, var##222, \
        var##223, var##224, var##225, var##226, var##227, var##228, \
    }; \
    kart_texture_t var##Group12[] = { \
        var##229, var##230, var##231, var##232, var##233, var##234, var##235, \
        var##236, var##237, var##238, var##239, var##240, var##241, var##242, \
        var##243, var##244, var##245, var##246, var##247, var##248, \
    }; \
    kart_texture_t var##Group13[] = { \
        var##229, var##230, var##231, var##232, var##233, var##234, var##235, \
        var##236, var##237, var##238, var##239, var##240, var##241, var##242, \
        var##243, var##244, var##245, var##246, var##247, var##248, \
    }; \
    kart_texture_t var##Group14[] = { \
        var##229, var##230, var##231, var##232, var##233, var##234, var##235, \
        var##236, var##237, var##238, var##239, var##240, var##241, var##242, \
        var##243, var##244, var##245, var##246, var##247, var##248, \
    }; \
    kart_texture_t var##Group15[] = { \
        var##249, var##250, var##251, var##252, var##253, var##254, var##255, \
        var##256, var##257, var##258, var##259, var##260, var##261, var##262, \
        var##263, var##264, var##265, var##266, var##267, var##268, \
    }; \
    kart_texture_t var##Group16[] = { \
        var##269, var##270, var##271, var##272, var##273, var##274, var##275, \
        var##276, var##277, var##278, var##279, var##280, var##281, var##282, \
        var##283, var##284, var##285, var##286, var##287, var##288, \
    }; \
    kart_texture_t var##Group17[] = { \
        var##269, var##270, var##271, var##272, var##273, var##274, var##275, \
        var##276, var##277, var##278, var##279, var##280, var##281, var##282, \
        var##283, var##284, var##285, var##286, var##287, var##288, \
    };

#define DECLARE_KART_TABLE0(var) \
    kart_texture_t* var##Table0[] = { \
        var##Group8, var##Group7, var##Group6, var##Group5, var##Group4, \
        var##Group3, var##Group2, var##Group1, var##Group0, \
    };

#define DECLARE_KART_TABLE1(var) \
    kart_texture_t* var##Table1[] = { \
        var##Group17, var##Group16, var##Group15, var##Group14, var##Group13, \
        var##Group12, var##Group11, var##Group10, var##Group9, \
    };

#define DECLARE_TUMBLE_KART_TEXTURES(var) \
    KART_TEXTURE_MACRO(var##289) \
    KART_TEXTURE_MACRO(var##290) \
    KART_TEXTURE_MACRO(var##291) \
    KART_TEXTURE_MACRO(var##292) \
    KART_TEXTURE_MACRO(var##293) \
    KART_TEXTURE_MACRO(var##294) \
    KART_TEXTURE_MACRO(var##295) \
    KART_TEXTURE_MACRO(var##296) \
    KART_TEXTURE_MACRO(var##297) \
    KART_TEXTURE_MACRO(var##298) \
    KART_TEXTURE_MACRO(var##299) \
    KART_TEXTURE_MACRO(var##300) \
    KART_TEXTURE_MACRO(var##301) \
    KART_TEXTURE_MACRO(var##302) \
    KART_TEXTURE_MACRO(var##303) \
    KART_TEXTURE_MACRO(var##304) \
    KART_TEXTURE_MACRO(var##305) \
    KART_TEXTURE_MACRO(var##306) \
    KART_TEXTURE_MACRO(var##307) \
    KART_TEXTURE_MACRO(var##308) \
    KART_TEXTURE_MACRO(var##309) \
    KART_TEXTURE_MACRO(var##310) \
    KART_TEXTURE_MACRO(var##311) \
    KART_TEXTURE_MACRO(var##312) \
    KART_TEXTURE_MACRO(var##313) \
    KART_TEXTURE_MACRO(var##314) \
    KART_TEXTURE_MACRO(var##315) \
    KART_TEXTURE_MACRO(var##316) \
    KART_TEXTURE_MACRO(var##317) \
    KART_TEXTURE_MACRO(var##318) \
    KART_TEXTURE_MACRO(var##319) \
    KART_TEXTURE_MACRO(var##320)

#define DECLARE_KART_TUMBLE_TABLE(var) \
    kart_texture_t var##Tumble[] = { \
        var##289, var##290, var##291, var##292, var##293, var##294, var##295, \
        var##296, var##297, var##298, var##299, var##300, var##301, var##302, \
        var##303, var##304, var##305, var##306, var##307, var##308, var##309, \
        var##310, var##311, var##312, var##313, var##314, var##315, var##316, \
        var##317, var##318, var##319, var##320 \
    };

typedef u8** kart_texture_t;

DECLARE_KART_TEXTURES(gKartMario)
DECLARE_KART_GROUP(gKartMario)
DECLARE_KART_TEXTURES(gKartToad)
DECLARE_KART_GROUP(gKartToad)
DECLARE_KART_TEXTURES(gKartLuigi)
DECLARE_KART_GROUP(gKartLuigi)
DECLARE_KART_TEXTURES(gKartYoshi)
DECLARE_KART_GROUP(gKartYoshi)
DECLARE_KART_TEXTURES(gKartDK)
DECLARE_KART_GROUP(gKartDK)
DECLARE_KART_TEXTURES(gKartBowser)
DECLARE_KART_GROUP(gKartBowser)
DECLARE_KART_TEXTURES(gKartPeach)
DECLARE_KART_GROUP(gKartPeach)
DECLARE_KART_TEXTURES(gKartWario)
DECLARE_KART_GROUP(gKartWario)

DECLARE_KART_TABLE0(gKartMario)
DECLARE_KART_TABLE0(gKartLuigi)
DECLARE_KART_TABLE0(gKartYoshi)
DECLARE_KART_TABLE0(gKartToad)
DECLARE_KART_TABLE0(gKartDK)
DECLARE_KART_TABLE0(gKartWario)
DECLARE_KART_TABLE0(gKartPeach)
DECLARE_KART_TABLE0(gKartBowser)

DECLARE_KART_TABLE1(gKartMario)
DECLARE_KART_TABLE1(gKartLuigi)
DECLARE_KART_TABLE1(gKartYoshi)
DECLARE_KART_TABLE1(gKartToad)
DECLARE_KART_TABLE1(gKartDK)
DECLARE_KART_TABLE1(gKartWario)
DECLARE_KART_TABLE1(gKartPeach)
DECLARE_KART_TABLE1(gKartBowser)

kart_texture_t** gKartTextureTable0[] = {
    gKartMarioTable0, gKartLuigiTable0, gKartYoshiTable0, gKartToadTable0,
    gKartDKTable0,    gKartWarioTable0, gKartPeachTable0, gKartBowserTable0,
};

kart_texture_t** gKartTextureTable1[] = {
    gKartMarioTable1, gKartLuigiTable1, gKartYoshiTable1, gKartToadTable1,
    gKartDKTable1,    gKartWarioTable1, gKartPeachTable1, gKartBowserTable1,
};

DECLARE_KART_TUMBLE_TABLE(gKartMario)
DECLARE_KART_TUMBLE_TABLE(gKartLuigi)
DECLARE_KART_TUMBLE_TABLE(gKartBowser)
DECLARE_KART_TUMBLE_TABLE(gKartToad)
DECLARE_KART_TUMBLE_TABLE(gKartYoshi)
DECLARE_KART_TUMBLE_TABLE(gKartDK)
DECLARE_KART_TUMBLE_TABLE(gKartPeach)
DECLARE_KART_TUMBLE_TABLE(gKartWario)

u8** gKartTextureTumbles[] = {
    gKartMarioTumble, gKartLuigiTumble, gKartYoshiTumble, gKartToadTumble,
    gKartDKTumble,    gKartWarioTumble, gKartPeachTumble, gKartBowserTumble,
};

u8* gKartPalettes[] = {
    gKartMarioPalette, gKartLuigiPalette, gKartYoshiPalette, gKartToadPalette,
    gKartDKPalette,    gKartWarioPalette, gKartPeachPalette, gKartBowserPalette,
};

/**
 * @brief Place DMA mio0 compressed character textures in a buffer.
 * Later, this data becomes decompressed.
 *
 * The player struct tracks the texture indices to load.
 *
 * @param player
 * @param playerId Player ID.
 * @param screenId Rom buffer index appears to always be 0-3. Sometimes subtracted by 2.
 * @param screenId2 Second buffer index appears to always be 0-3.
 * @param index First buffer index always zero.
 **/
void load_kart_texture(Player* player, s8 playerId, s8 screenId, s8 screenId2, s8 index) {
    s32 temp = player->effects;
    s16 tyreSpeed = player->tyreSpeed;
    if (((temp & 0x80) == 0x80) || ((temp & 0x40) == 0x40) || ((temp & 0x80000) == 0x80000) ||
        ((temp & 0x800000) == 0x800000) || ((temp & 0x20000) == 0x20000) || ((player->unk_044 & 0x800) != 0)) {
        if (player->animFrameSelector[screenId] != 0) {
            osInvalDCache(&gEncodedKartTexture[index][screenId2][playerId], D_800DDEB0[player->characterId]);
            gEncodedKartTexture[index][screenId2][playerId].unk_00 =
                gKartTextureTable1[player->characterId][player->animGroupSelector[screenId]]
                                  [player->animFrameSelector[screenId]][tyreSpeed >> 8];
        } else {
            osInvalDCache(&gEncodedKartTexture[index][screenId2][playerId], D_800DDEB0[player->characterId]);
            gEncodedKartTexture[index][screenId2][playerId].unk_00 =
                gKartTextureTable0[player->characterId][player->animGroupSelector[screenId]]
                                  [player->animFrameSelector[screenId]][0];
        }
    } else if (((temp & 0x400) == 0x400) || ((temp & 0x01000000) == 0x01000000) ||
               ((temp & 0x02000000) == 0x02000000) || ((temp & 0x10000) == 0x10000)) {
        osInvalDCache(&gEncodedKartTexture[index][screenId2][playerId], 0x780U);
// player->unk_0A8 >> 8 converts an 8.8 fixed-point animation frame to a whole number.
        gEncodedKartTexture[index][screenId2][playerId].unk_00 =
            gKartTextureTumbles[player->characterId][player->unk_0A8 >> 8];
    } else {
        osInvalDCache(&gEncodedKartTexture[index][screenId2][playerId], D_800DDEB0[player->characterId]);
        gEncodedKartTexture[index][screenId2][playerId].unk_00 =
            gKartTextureTable0[player->characterId][player->animGroupSelector[screenId]]
                              [player->animFrameSelector[screenId]][tyreSpeed >> 8];
    }
}

void load_kart_texture_non_blocking(Player* player, s8 arg1, s8 arg2, s8 arg3, s8 arg4) {
    s32 temp = player->effects;
    s16 tyreSpeed = player->tyreSpeed;

    if (((temp & 0x80) == 0x80) || ((temp & 0x40) == 0x40) || ((temp & 0x80000) == 0x80000) ||
        ((temp & 0x800000) == 0x800000) || ((temp & 0x20000) == 0x20000) || ((player->unk_044 & 0x800) != 0)) {
        if (player->animFrameSelector[arg2] != 0) {
            osInvalDCache(&gEncodedKartTexture[arg4][arg3][arg1], D_800DDEB0[player->characterId]);

#ifdef TARGET_N64
            osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ,
                         (uintptr_t) &_kart_texturesSegmentRomStart[SEGMENT_OFFSET(
                             gKartTextureTable1[player->characterId][player->animGroupSelector[arg2]]
                                               [player->animFrameSelector[arg2]])],
                         &gEncodedKartTexture[arg4][arg3][arg1], D_800DDEB0[player->characterId], &gDmaMesgQueue);
#else
            gEncodedKartTexture[arg4][arg3][arg1].unk_00 =
                gKartTextureTable1[player->characterId][player->animGroupSelector[arg2]]
                                  [player->animFrameSelector[arg2]][tyreSpeed >> 8];
#endif
        } else {
            osInvalDCache(&gEncodedKartTexture[arg4][arg3][arg1], D_800DDEB0[player->characterId]);

#ifdef TARGET_N64
            osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ,
                         (uintptr_t) &_kart_texturesSegmentRomStart[SEGMENT_OFFSET(
                             gKartTextureTable0[player->characterId][player->animGroupSelector[arg2]]
                                               [player->animFrameSelector[arg2]])],
                         &gEncodedKartTexture[arg4][arg3][arg1], D_800DDEB0[player->characterId], &gDmaMesgQueue);
#else
            gEncodedKartTexture[arg4][arg3][arg1].unk_00 =
                gKartTextureTable0[player->characterId][player->animGroupSelector[arg2]]
                                  [player->animFrameSelector[arg2]][tyreSpeed >> 8];
#endif
        }
    } else if (((temp & 0x400) == 0x400) || ((temp & 0x01000000) == 0x01000000) ||
               ((temp & 0x02000000) == 0x02000000) || ((temp & 0x10000) == 0x10000)) {
        osInvalDCache(&gEncodedKartTexture[arg4][arg3][arg1], 0x780);
// player->unk_0A8 >> 8 converts an 8.8 fixed-point animation frame to a whole number.
#ifdef TARGET_N64
        osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ,
                     (uintptr_t) &_kart_texturesSegmentRomStart[SEGMENT_OFFSET(
                         gKartTextureTumbles[player->characterId][player->unk_0A8 >> 8])],
                     &gEncodedKartTexture[arg4][arg3][arg1], 0x900, &gDmaMesgQueue);
#else
        gEncodedKartTexture[arg4][arg3][arg1].unk_00 = gKartTextureTumbles[player->characterId][player->unk_0A8 >> 8];
#endif
    } else {
        osInvalDCache(&gEncodedKartTexture[arg4][arg3][arg1], D_800DDEB0[player->characterId]);

#ifdef TARGET_N64
        osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ,
                     (uintptr_t) &_kart_texturesSegmentRomStart[SEGMENT_OFFSET(
                         gKartTextureTable0[player->characterId][player->animGroupSelector[arg2]]
                                           [player->animFrameSelector[arg2]])],
                     &gEncodedKartTexture[arg4][arg3][arg1], D_800DDEB0[player->characterId], &gDmaMesgQueue);
#else
        gEncodedKartTexture[arg4][arg3][arg1].unk_00 =
            gKartTextureTable0[player->characterId][player->animGroupSelector[arg2]][player->animFrameSelector[arg2]][tyreSpeed >> 8];
#endif
    }
}

void load_kart_palette(Player* player, s8 playerId, s8 screenId, s8 buffer) {
    u8* asset;
    size_t size;
    struct_D_802F1F80* temp_s0 = &gPlayerPalettesList[buffer][screenId][playerId];
    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            osInvalDCache(temp_s0, sizeof(struct_D_802F1F80));

            size = ResourceGetTexSizeByName(gKartPalettes[player->characterId]);
            asset = (u8*) LOAD_ASSET(gKartPalettes[player->characterId]);
            memcpy(&temp_s0->kart_palette[0], asset, size);
            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN: // Code identical to above
            osInvalDCache(temp_s0, sizeof(struct_D_802F1F80));

            size = ResourceGetTexSizeByName(gKartPalettes[player->characterId]);
            asset = (u8*) LOAD_ASSET(gKartPalettes[player->characterId]);
            memcpy(&temp_s0->kart_palette[0], asset, size);
            break;
    }
}

void load_player_data(UNUSED Player* player, s32 arg1, void* vAddr, u16 size) {
    osInvalDCache(vAddr, size);

#ifdef TARGET_N64
    osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ,
                 (uintptr_t) &_kart_texturesSegmentRomStart[SEGMENT_OFFSET(arg1)], vAddr, size, &gDmaMesgQueue);

    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
#endif
}

/**
 * @brief read data from ROM and write it to the given virtual address
 *
 * @param player Player struct
 * @param arg1 ROM offset
 * @param vAddr Virtual address
 * @param size Size of data to read
 */
void load_wheel_palette_non_blocking(UNUSED Player* player, const char* texture, void* vAddr, u16 size) {
    osInvalDCache(vAddr, size);

#ifdef TARGET_N64
    osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ,
                 (uintptr_t) &_kart_texturesSegmentRomStart[SEGMENT_OFFSET(arg1)], vAddr, size, &gDmaMesgQueue);
#else
    u16* tex = (u16*) LOAD_ASSET(texture);
    size_t textureSize = ResourceGetTexSizeByName(texture);
    memcpy(vAddr, tex, size);
#endif
}
