#include <libultraship.h>
#include <macros.h>
#include "main.h"
#include "buffers.h"
#include <common_structs.h>
#include "kart_dma.h"
#include "defines.h"
#include <assets/mario_kart.h>
#include <assets/luigi_kart.h>
#include <assets/peach_kart.h>
#include <assets/yoshi_kart.h>
#include <assets/toad_kart.h>
#include <assets/bowser_kart.h>
#include <assets/wario_kart.h>
#include <assets/donkeykong_kart.h>
#include "render_player.h"

// Seemingly the largest compressed size for a frame for a given character
// They seem slightly oversized too, either an alignemnt or safety thing
u16 D_800DDEB0[] = {
    0x06c0, 0x06e0, 0x06e0, 0x0680, 0x07c0, 0x0700, 0x0680, 0x0910,
};

u8* gKartMarioGroup0[] = {
    gKartMario000, gKartMario001, gKartMario002, gKartMario003, gKartMario004, gKartMario005, gKartMario006,
    gKartMario007, gKartMario008, gKartMario009, gKartMario010, gKartMario011, gKartMario012, gKartMario013,
    gKartMario014, gKartMario015, gKartMario016, gKartMario017, gKartMario018, gKartMario019, gKartMario020,
    gKartMario195, gKartMario196, gKartMario197, gKartMario198, gKartMario199, gKartMario200, gKartMario201,
    gKartMario202, gKartMario203, gKartMario204, gKartMario205, gKartMario206, gKartMario207, gKartMario208,
};

u8* gKartMarioGroup1[] = {
    gKartMario021, gKartMario022, gKartMario023, gKartMario024, gKartMario025, gKartMario026, gKartMario027,
    gKartMario028, gKartMario029, gKartMario030, gKartMario031, gKartMario032, gKartMario033, gKartMario034,
    gKartMario035, gKartMario036, gKartMario037, gKartMario038, gKartMario039, gKartMario040, gKartMario041,
    gKartMario195, gKartMario196, gKartMario197, gKartMario198, gKartMario199, gKartMario200, gKartMario201,
    gKartMario202, gKartMario203, gKartMario204, gKartMario205, gKartMario206, gKartMario207, gKartMario208,
};

u8* gKartMarioGroup2[] = {
    gKartMario042, gKartMario043, gKartMario044, gKartMario045, gKartMario046, gKartMario047, gKartMario048,
    gKartMario049, gKartMario050, gKartMario051, gKartMario052, gKartMario053, gKartMario054, gKartMario055,
    gKartMario056, gKartMario057, gKartMario058, gKartMario059, gKartMario060, gKartMario061, gKartMario062,
    gKartMario215, gKartMario216, gKartMario217, gKartMario218, gKartMario219, gKartMario220, gKartMario221,
    gKartMario222, gKartMario223, gKartMario224, gKartMario225, gKartMario226, gKartMario227, gKartMario228,
};

u8* gKartMarioGroup3[] = {
    gKartMario063, gKartMario064, gKartMario065, gKartMario066, gKartMario067, gKartMario068, gKartMario069,
    gKartMario070, gKartMario071, gKartMario072, gKartMario073, gKartMario074, gKartMario075, gKartMario076,
    gKartMario077, gKartMario078, gKartMario079, gKartMario080, gKartMario081, gKartMario082, gKartMario083,
    gKartMario235, gKartMario236, gKartMario237, gKartMario238, gKartMario239, gKartMario240, gKartMario241,
    gKartMario242, gKartMario243, gKartMario244, gKartMario245, gKartMario246, gKartMario247, gKartMario248,
};

u8* gKartMarioGroup4[] = {
    gKartMario084, gKartMario085, gKartMario086, gKartMario087, gKartMario088, gKartMario089, gKartMario090,
    gKartMario091, gKartMario092, gKartMario093, gKartMario094, gKartMario095, gKartMario096, gKartMario097,
    gKartMario098, gKartMario099, gKartMario100, gKartMario101, gKartMario102, gKartMario103, gKartMario104,
    gKartMario235, gKartMario236, gKartMario237, gKartMario238, gKartMario239, gKartMario240, gKartMario241,
    gKartMario242, gKartMario243, gKartMario244, gKartMario245, gKartMario246, gKartMario247, gKartMario248,
};

u8* gKartMarioGroup5[] = {
    gKartMario105, gKartMario106, gKartMario107, gKartMario108, gKartMario109, gKartMario110, gKartMario111,
    gKartMario112, gKartMario113, gKartMario114, gKartMario115, gKartMario116, gKartMario117, gKartMario118,
    gKartMario119, gKartMario120, gKartMario121, gKartMario122, gKartMario123, gKartMario124, gKartMario125,
    gKartMario235, gKartMario236, gKartMario237, gKartMario238, gKartMario239, gKartMario240, gKartMario241,
    gKartMario242, gKartMario243, gKartMario244, gKartMario245, gKartMario246, gKartMario247, gKartMario248,
};

u8* gKartMarioGroup6[] = {
    gKartMario126, gKartMario127, gKartMario128, gKartMario129, gKartMario130, gKartMario131, gKartMario132,
    gKartMario133, gKartMario134, gKartMario135, gKartMario136, gKartMario137, gKartMario138, gKartMario139,
    gKartMario140, gKartMario141, gKartMario142, gKartMario143, gKartMario144, gKartMario145, gKartMario146,
    gKartMario255, gKartMario256, gKartMario257, gKartMario258, gKartMario259, gKartMario260, gKartMario261,
    gKartMario262, gKartMario263, gKartMario264, gKartMario265, gKartMario266, gKartMario267, gKartMario268,
};

u8* gKartMarioGroup7[] = {
    gKartMario147, gKartMario148, gKartMario149, gKartMario150, gKartMario151, gKartMario152, gKartMario153,
    gKartMario154, gKartMario155, gKartMario156, gKartMario157, gKartMario158, gKartMario159, gKartMario160,
    gKartMario161, gKartMario162, gKartMario163, gKartMario164, gKartMario165, gKartMario166, gKartMario167,
    gKartMario275, gKartMario276, gKartMario277, gKartMario278, gKartMario279, gKartMario280, gKartMario281,
    gKartMario282, gKartMario283, gKartMario284, gKartMario285, gKartMario286, gKartMario287, gKartMario288,
};

u8* gKartMarioGroup8[] = {
    gKartMario168, gKartMario169, gKartMario170, gKartMario171, gKartMario172, gKartMario173, gKartMario174,
    gKartMario175, gKartMario176, gKartMario177, gKartMario178, gKartMario179, gKartMario180, gKartMario181,
    gKartMario182, gKartMario183, gKartMario184, gKartMario185, gKartMario186, gKartMario187, gKartMario188,
    gKartMario275, gKartMario276, gKartMario277, gKartMario278, gKartMario279, gKartMario280, gKartMario281,
    gKartMario282, gKartMario283, gKartMario284, gKartMario285, gKartMario286, gKartMario287, gKartMario288,
};

u8* gKartMarioGroup9[] = {
    gKartMario189, gKartMario190, gKartMario191, gKartMario192, gKartMario193, gKartMario194, gKartMario195,
    gKartMario196, gKartMario197, gKartMario198, gKartMario199, gKartMario200, gKartMario201, gKartMario202,
    gKartMario203, gKartMario204, gKartMario205, gKartMario206, gKartMario207, gKartMario208,
};

u8* gKartMarioGroup10[] = {
    gKartMario189, gKartMario190, gKartMario191, gKartMario192, gKartMario193, gKartMario194, gKartMario195,
    gKartMario196, gKartMario197, gKartMario198, gKartMario199, gKartMario200, gKartMario201, gKartMario202,
    gKartMario203, gKartMario204, gKartMario205, gKartMario206, gKartMario207, gKartMario208,
};

u8* gKartMarioGroup11[] = {
    gKartMario209, gKartMario210, gKartMario211, gKartMario212, gKartMario213, gKartMario214, gKartMario215,
    gKartMario216, gKartMario217, gKartMario218, gKartMario219, gKartMario220, gKartMario221, gKartMario222,
    gKartMario223, gKartMario224, gKartMario225, gKartMario226, gKartMario227, gKartMario228,
};

u8* gKartMarioGroup12[] = {
    gKartMario229, gKartMario230, gKartMario231, gKartMario232, gKartMario233, gKartMario234, gKartMario235,
    gKartMario236, gKartMario237, gKartMario238, gKartMario239, gKartMario240, gKartMario241, gKartMario242,
    gKartMario243, gKartMario244, gKartMario245, gKartMario246, gKartMario247, gKartMario248,
};

u8* gKartMarioGroup13[] = {
    gKartMario229, gKartMario230, gKartMario231, gKartMario232, gKartMario233, gKartMario234, gKartMario235,
    gKartMario236, gKartMario237, gKartMario238, gKartMario239, gKartMario240, gKartMario241, gKartMario242,
    gKartMario243, gKartMario244, gKartMario245, gKartMario246, gKartMario247, gKartMario248,
};

u8* gKartMarioGroup14[] = {
    gKartMario229, gKartMario230, gKartMario231, gKartMario232, gKartMario233, gKartMario234, gKartMario235,
    gKartMario236, gKartMario237, gKartMario238, gKartMario239, gKartMario240, gKartMario241, gKartMario242,
    gKartMario243, gKartMario244, gKartMario245, gKartMario246, gKartMario247, gKartMario248,
};

u8* gKartMarioGroup15[] = {
    gKartMario249, gKartMario250, gKartMario251, gKartMario252, gKartMario253, gKartMario254, gKartMario255,
    gKartMario256, gKartMario257, gKartMario258, gKartMario259, gKartMario260, gKartMario261, gKartMario262,
    gKartMario263, gKartMario264, gKartMario265, gKartMario266, gKartMario267, gKartMario268,
};

u8* gKartMarioGroup16[] = {
    gKartMario269, gKartMario270, gKartMario271, gKartMario272, gKartMario273, gKartMario274, gKartMario275,
    gKartMario276, gKartMario277, gKartMario278, gKartMario279, gKartMario280, gKartMario281, gKartMario282,
    gKartMario283, gKartMario284, gKartMario285, gKartMario286, gKartMario287, gKartMario288,
};

u8* gKartMarioGroup17[] = {
    gKartMario269, gKartMario270, gKartMario271, gKartMario272, gKartMario273, gKartMario274, gKartMario275,
    gKartMario276, gKartMario277, gKartMario278, gKartMario279, gKartMario280, gKartMario281, gKartMario282,
    gKartMario283, gKartMario284, gKartMario285, gKartMario286, gKartMario287, gKartMario288,
};

u8* gKartToadGroup0[] = {
    gKartToad000, gKartToad001, gKartToad002, gKartToad003, gKartToad004, gKartToad005, gKartToad006,
    gKartToad007, gKartToad008, gKartToad009, gKartToad010, gKartToad011, gKartToad012, gKartToad013,
    gKartToad014, gKartToad015, gKartToad016, gKartToad017, gKartToad018, gKartToad019, gKartToad020,
    gKartToad195, gKartToad196, gKartToad197, gKartToad198, gKartToad199, gKartToad200, gKartToad201,
    gKartToad202, gKartToad203, gKartToad204, gKartToad205, gKartToad206, gKartToad207, gKartToad208,
};

u8* gKartToadGroup1[] = {
    gKartToad021, gKartToad022, gKartToad023, gKartToad024, gKartToad025, gKartToad026, gKartToad027,
    gKartToad028, gKartToad029, gKartToad030, gKartToad031, gKartToad032, gKartToad033, gKartToad034,
    gKartToad035, gKartToad036, gKartToad037, gKartToad038, gKartToad039, gKartToad040, gKartToad041,
    gKartToad195, gKartToad196, gKartToad197, gKartToad198, gKartToad199, gKartToad200, gKartToad201,
    gKartToad202, gKartToad203, gKartToad204, gKartToad205, gKartToad206, gKartToad207, gKartToad208,
};

u8* gKartToadGroup2[] = {
    gKartToad042, gKartToad043, gKartToad044, gKartToad045, gKartToad046, gKartToad047, gKartToad048,
    gKartToad049, gKartToad050, gKartToad051, gKartToad052, gKartToad053, gKartToad054, gKartToad055,
    gKartToad056, gKartToad057, gKartToad058, gKartToad059, gKartToad060, gKartToad061, gKartToad062,
    gKartToad215, gKartToad216, gKartToad217, gKartToad218, gKartToad219, gKartToad220, gKartToad221,
    gKartToad222, gKartToad223, gKartToad224, gKartToad225, gKartToad226, gKartToad227, gKartToad228,
};

u8* gKartToadGroup3[] = {
    gKartToad063, gKartToad064, gKartToad065, gKartToad066, gKartToad067, gKartToad068, gKartToad069,
    gKartToad070, gKartToad071, gKartToad072, gKartToad073, gKartToad074, gKartToad075, gKartToad076,
    gKartToad077, gKartToad078, gKartToad079, gKartToad080, gKartToad081, gKartToad082, gKartToad083,
    gKartToad235, gKartToad236, gKartToad237, gKartToad238, gKartToad239, gKartToad240, gKartToad241,
    gKartToad242, gKartToad243, gKartToad244, gKartToad245, gKartToad246, gKartToad247, gKartToad248,
};

u8* gKartToadGroup4[] = {
    gKartToad084, gKartToad085, gKartToad086, gKartToad087, gKartToad088, gKartToad089, gKartToad090,
    gKartToad091, gKartToad092, gKartToad093, gKartToad094, gKartToad095, gKartToad096, gKartToad097,
    gKartToad098, gKartToad099, gKartToad100, gKartToad101, gKartToad102, gKartToad103, gKartToad104,
    gKartToad235, gKartToad236, gKartToad237, gKartToad238, gKartToad239, gKartToad240, gKartToad241,
    gKartToad242, gKartToad243, gKartToad244, gKartToad245, gKartToad246, gKartToad247, gKartToad248,
};

u8* gKartToadGroup5[] = {
    gKartToad105, gKartToad106, gKartToad107, gKartToad108, gKartToad109, gKartToad110, gKartToad111,
    gKartToad112, gKartToad113, gKartToad114, gKartToad115, gKartToad116, gKartToad117, gKartToad118,
    gKartToad119, gKartToad120, gKartToad121, gKartToad122, gKartToad123, gKartToad124, gKartToad125,
    gKartToad235, gKartToad236, gKartToad237, gKartToad238, gKartToad239, gKartToad240, gKartToad241,
    gKartToad242, gKartToad243, gKartToad244, gKartToad245, gKartToad246, gKartToad247, gKartToad248,
};

u8* gKartToadGroup6[] = {
    gKartToad126, gKartToad127, gKartToad128, gKartToad129, gKartToad130, gKartToad131, gKartToad132,
    gKartToad133, gKartToad134, gKartToad135, gKartToad136, gKartToad137, gKartToad138, gKartToad139,
    gKartToad140, gKartToad141, gKartToad142, gKartToad143, gKartToad144, gKartToad145, gKartToad146,
    gKartToad255, gKartToad256, gKartToad257, gKartToad258, gKartToad259, gKartToad260, gKartToad261,
    gKartToad262, gKartToad263, gKartToad264, gKartToad265, gKartToad266, gKartToad267, gKartToad268,
};

u8* gKartToadGroup7[] = {
    gKartToad147, gKartToad148, gKartToad149, gKartToad150, gKartToad151, gKartToad152, gKartToad153,
    gKartToad154, gKartToad155, gKartToad156, gKartToad157, gKartToad158, gKartToad159, gKartToad160,
    gKartToad161, gKartToad162, gKartToad163, gKartToad164, gKartToad165, gKartToad166, gKartToad167,
    gKartToad275, gKartToad276, gKartToad277, gKartToad278, gKartToad279, gKartToad280, gKartToad281,
    gKartToad282, gKartToad283, gKartToad284, gKartToad285, gKartToad286, gKartToad287, gKartToad288,
};

u8* gKartToadGroup8[] = {
    gKartToad168, gKartToad169, gKartToad170, gKartToad171, gKartToad172, gKartToad173, gKartToad174,
    gKartToad175, gKartToad176, gKartToad177, gKartToad178, gKartToad179, gKartToad180, gKartToad181,
    gKartToad182, gKartToad183, gKartToad184, gKartToad185, gKartToad186, gKartToad187, gKartToad188,
    gKartToad275, gKartToad276, gKartToad277, gKartToad278, gKartToad279, gKartToad280, gKartToad281,
    gKartToad282, gKartToad283, gKartToad284, gKartToad285, gKartToad286, gKartToad287, gKartToad288,
};

u8* gKartToadGroup9[] = {
    gKartToad189, gKartToad190, gKartToad191, gKartToad192, gKartToad193, gKartToad194, gKartToad195,
    gKartToad196, gKartToad197, gKartToad198, gKartToad199, gKartToad200, gKartToad201, gKartToad202,
    gKartToad203, gKartToad204, gKartToad205, gKartToad206, gKartToad207, gKartToad208,
};

u8* gKartToadGroup10[] = {
    gKartToad189, gKartToad190, gKartToad191, gKartToad192, gKartToad193, gKartToad194, gKartToad195,
    gKartToad196, gKartToad197, gKartToad198, gKartToad199, gKartToad200, gKartToad201, gKartToad202,
    gKartToad203, gKartToad204, gKartToad205, gKartToad206, gKartToad207, gKartToad208,
};

u8* gKartToadGroup11[] = {
    gKartToad209, gKartToad210, gKartToad211, gKartToad212, gKartToad213, gKartToad214, gKartToad215,
    gKartToad216, gKartToad217, gKartToad218, gKartToad219, gKartToad220, gKartToad221, gKartToad222,
    gKartToad223, gKartToad224, gKartToad225, gKartToad226, gKartToad227, gKartToad228,
};

u8* gKartToadGroup12[] = {
    gKartToad229, gKartToad230, gKartToad231, gKartToad232, gKartToad233, gKartToad234, gKartToad235,
    gKartToad236, gKartToad237, gKartToad238, gKartToad239, gKartToad240, gKartToad241, gKartToad242,
    gKartToad243, gKartToad244, gKartToad245, gKartToad246, gKartToad247, gKartToad248,
};

u8* gKartToadGroup13[] = {
    gKartToad229, gKartToad230, gKartToad231, gKartToad232, gKartToad233, gKartToad234, gKartToad235,
    gKartToad236, gKartToad237, gKartToad238, gKartToad239, gKartToad240, gKartToad241, gKartToad242,
    gKartToad243, gKartToad244, gKartToad245, gKartToad246, gKartToad247, gKartToad248,
};

u8* gKartToadGroup14[] = {
    gKartToad229, gKartToad230, gKartToad231, gKartToad232, gKartToad233, gKartToad234, gKartToad235,
    gKartToad236, gKartToad237, gKartToad238, gKartToad239, gKartToad240, gKartToad241, gKartToad242,
    gKartToad243, gKartToad244, gKartToad245, gKartToad246, gKartToad247, gKartToad248,
};

u8* gKartToadGroup15[] = {
    gKartToad249, gKartToad250, gKartToad251, gKartToad252, gKartToad253, gKartToad254, gKartToad255,
    gKartToad256, gKartToad257, gKartToad258, gKartToad259, gKartToad260, gKartToad261, gKartToad262,
    gKartToad263, gKartToad264, gKartToad265, gKartToad266, gKartToad267, gKartToad268,
};

u8* gKartToadGroup16[] = {
    gKartToad269, gKartToad270, gKartToad271, gKartToad272, gKartToad273, gKartToad274, gKartToad275,
    gKartToad276, gKartToad277, gKartToad278, gKartToad279, gKartToad280, gKartToad281, gKartToad282,
    gKartToad283, gKartToad284, gKartToad285, gKartToad286, gKartToad287, gKartToad288,
};

u8* gKartToadGroup17[] = {
    gKartToad269, gKartToad270, gKartToad271, gKartToad272, gKartToad273, gKartToad274, gKartToad275,
    gKartToad276, gKartToad277, gKartToad278, gKartToad279, gKartToad280, gKartToad281, gKartToad282,
    gKartToad283, gKartToad284, gKartToad285, gKartToad286, gKartToad287, gKartToad288,
};

u8* gKartLuigiGroup0[] = {
    gKartLuigi000, gKartLuigi001, gKartLuigi002, gKartLuigi003, gKartLuigi004, gKartLuigi005, gKartLuigi006,
    gKartLuigi007, gKartLuigi008, gKartLuigi009, gKartLuigi010, gKartLuigi011, gKartLuigi012, gKartLuigi013,
    gKartLuigi014, gKartLuigi015, gKartLuigi016, gKartLuigi017, gKartLuigi018, gKartLuigi019, gKartLuigi020,
    gKartLuigi195, gKartLuigi196, gKartLuigi197, gKartLuigi198, gKartLuigi199, gKartLuigi200, gKartLuigi201,
    gKartLuigi202, gKartLuigi203, gKartLuigi204, gKartLuigi205, gKartLuigi206, gKartLuigi207, gKartLuigi208,
};

u8* gKartLuigiGroup1[] = {
    gKartLuigi021, gKartLuigi022, gKartLuigi023, gKartLuigi024, gKartLuigi025, gKartLuigi026, gKartLuigi027,
    gKartLuigi028, gKartLuigi029, gKartLuigi030, gKartLuigi031, gKartLuigi032, gKartLuigi033, gKartLuigi034,
    gKartLuigi035, gKartLuigi036, gKartLuigi037, gKartLuigi038, gKartLuigi039, gKartLuigi040, gKartLuigi041,
    gKartLuigi195, gKartLuigi196, gKartLuigi197, gKartLuigi198, gKartLuigi199, gKartLuigi200, gKartLuigi201,
    gKartLuigi202, gKartLuigi203, gKartLuigi204, gKartLuigi205, gKartLuigi206, gKartLuigi207, gKartLuigi208,
};

u8* gKartLuigiGroup2[] = {
    gKartLuigi042, gKartLuigi043, gKartLuigi044, gKartLuigi045, gKartLuigi046, gKartLuigi047, gKartLuigi048,
    gKartLuigi049, gKartLuigi050, gKartLuigi051, gKartLuigi052, gKartLuigi053, gKartLuigi054, gKartLuigi055,
    gKartLuigi056, gKartLuigi057, gKartLuigi058, gKartLuigi059, gKartLuigi060, gKartLuigi061, gKartLuigi062,
    gKartLuigi215, gKartLuigi216, gKartLuigi217, gKartLuigi218, gKartLuigi219, gKartLuigi220, gKartLuigi221,
    gKartLuigi222, gKartLuigi223, gKartLuigi224, gKartLuigi225, gKartLuigi226, gKartLuigi227, gKartLuigi228,
};

u8* gKartLuigiGroup3[] = {
    gKartLuigi063, gKartLuigi064, gKartLuigi065, gKartLuigi066, gKartLuigi067, gKartLuigi068, gKartLuigi069,
    gKartLuigi070, gKartLuigi071, gKartLuigi072, gKartLuigi073, gKartLuigi074, gKartLuigi075, gKartLuigi076,
    gKartLuigi077, gKartLuigi078, gKartLuigi079, gKartLuigi080, gKartLuigi081, gKartLuigi082, gKartLuigi083,
    gKartLuigi235, gKartLuigi236, gKartLuigi237, gKartLuigi238, gKartLuigi239, gKartLuigi240, gKartLuigi241,
    gKartLuigi242, gKartLuigi243, gKartLuigi244, gKartLuigi245, gKartLuigi246, gKartLuigi247, gKartLuigi248,
};

u8* gKartLuigiGroup4[] = {
    gKartLuigi084, gKartLuigi085, gKartLuigi086, gKartLuigi087, gKartLuigi088, gKartLuigi089, gKartLuigi090,
    gKartLuigi091, gKartLuigi092, gKartLuigi093, gKartLuigi094, gKartLuigi095, gKartLuigi096, gKartLuigi097,
    gKartLuigi098, gKartLuigi099, gKartLuigi100, gKartLuigi101, gKartLuigi102, gKartLuigi103, gKartLuigi104,
    gKartLuigi235, gKartLuigi236, gKartLuigi237, gKartLuigi238, gKartLuigi239, gKartLuigi240, gKartLuigi241,
    gKartLuigi242, gKartLuigi243, gKartLuigi244, gKartLuigi245, gKartLuigi246, gKartLuigi247, gKartLuigi248,
};

u8* gKartLuigiGroup5[] = {
    gKartLuigi105, gKartLuigi106, gKartLuigi107, gKartLuigi108, gKartLuigi109, gKartLuigi110, gKartLuigi111,
    gKartLuigi112, gKartLuigi113, gKartLuigi114, gKartLuigi115, gKartLuigi116, gKartLuigi117, gKartLuigi118,
    gKartLuigi119, gKartLuigi120, gKartLuigi121, gKartLuigi122, gKartLuigi123, gKartLuigi124, gKartLuigi125,
    gKartLuigi235, gKartLuigi236, gKartLuigi237, gKartLuigi238, gKartLuigi239, gKartLuigi240, gKartLuigi241,
    gKartLuigi242, gKartLuigi243, gKartLuigi244, gKartLuigi245, gKartLuigi246, gKartLuigi247, gKartLuigi248,
};

u8* gKartLuigiGroup6[] = {
    gKartLuigi126, gKartLuigi127, gKartLuigi128, gKartLuigi129, gKartLuigi130, gKartLuigi131, gKartLuigi132,
    gKartLuigi133, gKartLuigi134, gKartLuigi135, gKartLuigi136, gKartLuigi137, gKartLuigi138, gKartLuigi139,
    gKartLuigi140, gKartLuigi141, gKartLuigi142, gKartLuigi143, gKartLuigi144, gKartLuigi145, gKartLuigi146,
    gKartLuigi255, gKartLuigi256, gKartLuigi257, gKartLuigi258, gKartLuigi259, gKartLuigi260, gKartLuigi261,
    gKartLuigi262, gKartLuigi263, gKartLuigi264, gKartLuigi265, gKartLuigi266, gKartLuigi267, gKartLuigi268,
};

u8* gKartLuigiGroup7[] = {
    gKartLuigi147, gKartLuigi148, gKartLuigi149, gKartLuigi150, gKartLuigi151, gKartLuigi152, gKartLuigi153,
    gKartLuigi154, gKartLuigi155, gKartLuigi156, gKartLuigi157, gKartLuigi158, gKartLuigi159, gKartLuigi160,
    gKartLuigi161, gKartLuigi162, gKartLuigi163, gKartLuigi164, gKartLuigi165, gKartLuigi166, gKartLuigi167,
    gKartLuigi275, gKartLuigi276, gKartLuigi277, gKartLuigi278, gKartLuigi279, gKartLuigi280, gKartLuigi281,
    gKartLuigi282, gKartLuigi283, gKartLuigi284, gKartLuigi285, gKartLuigi286, gKartLuigi287, gKartLuigi288,
};

u8* gKartLuigiGroup8[] = {
    gKartLuigi168, gKartLuigi169, gKartLuigi170, gKartLuigi171, gKartLuigi172, gKartLuigi173, gKartLuigi174,
    gKartLuigi175, gKartLuigi176, gKartLuigi177, gKartLuigi178, gKartLuigi179, gKartLuigi180, gKartLuigi181,
    gKartLuigi182, gKartLuigi183, gKartLuigi184, gKartLuigi185, gKartLuigi186, gKartLuigi187, gKartLuigi188,
    gKartLuigi275, gKartLuigi276, gKartLuigi277, gKartLuigi278, gKartLuigi279, gKartLuigi280, gKartLuigi281,
    gKartLuigi282, gKartLuigi283, gKartLuigi284, gKartLuigi285, gKartLuigi286, gKartLuigi287, gKartLuigi288,
};

u8* gKartLuigiGroup9[] = {
    gKartLuigi189, gKartLuigi190, gKartLuigi191, gKartLuigi192, gKartLuigi193, gKartLuigi194, gKartLuigi195,
    gKartLuigi196, gKartLuigi197, gKartLuigi198, gKartLuigi199, gKartLuigi200, gKartLuigi201, gKartLuigi202,
    gKartLuigi203, gKartLuigi204, gKartLuigi205, gKartLuigi206, gKartLuigi207, gKartLuigi208,
};

u8* gKartLuigiGroup10[] = {
    gKartLuigi189, gKartLuigi190, gKartLuigi191, gKartLuigi192, gKartLuigi193, gKartLuigi194, gKartLuigi195,
    gKartLuigi196, gKartLuigi197, gKartLuigi198, gKartLuigi199, gKartLuigi200, gKartLuigi201, gKartLuigi202,
    gKartLuigi203, gKartLuigi204, gKartLuigi205, gKartLuigi206, gKartLuigi207, gKartLuigi208,
};

u8* gKartLuigiGroup11[] = {
    gKartLuigi209, gKartLuigi210, gKartLuigi211, gKartLuigi212, gKartLuigi213, gKartLuigi214, gKartLuigi215,
    gKartLuigi216, gKartLuigi217, gKartLuigi218, gKartLuigi219, gKartLuigi220, gKartLuigi221, gKartLuigi222,
    gKartLuigi223, gKartLuigi224, gKartLuigi225, gKartLuigi226, gKartLuigi227, gKartLuigi228,
};

u8* gKartLuigiGroup12[] = {
    gKartLuigi229, gKartLuigi230, gKartLuigi231, gKartLuigi232, gKartLuigi233, gKartLuigi234, gKartLuigi235,
    gKartLuigi236, gKartLuigi237, gKartLuigi238, gKartLuigi239, gKartLuigi240, gKartLuigi241, gKartLuigi242,
    gKartLuigi243, gKartLuigi244, gKartLuigi245, gKartLuigi246, gKartLuigi247, gKartLuigi248,
};

u8* gKartLuigiGroup13[] = {
    gKartLuigi229, gKartLuigi230, gKartLuigi231, gKartLuigi232, gKartLuigi233, gKartLuigi234, gKartLuigi235,
    gKartLuigi236, gKartLuigi237, gKartLuigi238, gKartLuigi239, gKartLuigi240, gKartLuigi241, gKartLuigi242,
    gKartLuigi243, gKartLuigi244, gKartLuigi245, gKartLuigi246, gKartLuigi247, gKartLuigi248,
};

u8* gKartLuigiGroup14[] = {
    gKartLuigi229, gKartLuigi230, gKartLuigi231, gKartLuigi232, gKartLuigi233, gKartLuigi234, gKartLuigi235,
    gKartLuigi236, gKartLuigi237, gKartLuigi238, gKartLuigi239, gKartLuigi240, gKartLuigi241, gKartLuigi242,
    gKartLuigi243, gKartLuigi244, gKartLuigi245, gKartLuigi246, gKartLuigi247, gKartLuigi248,
};

u8* gKartLuigiGroup15[] = {
    gKartLuigi249, gKartLuigi250, gKartLuigi251, gKartLuigi252, gKartLuigi253, gKartLuigi254, gKartLuigi255,
    gKartLuigi256, gKartLuigi257, gKartLuigi258, gKartLuigi259, gKartLuigi260, gKartLuigi261, gKartLuigi262,
    gKartLuigi263, gKartLuigi264, gKartLuigi265, gKartLuigi266, gKartLuigi267, gKartLuigi268,
};

u8* gKartLuigiGroup16[] = {
    gKartLuigi269, gKartLuigi270, gKartLuigi271, gKartLuigi272, gKartLuigi273, gKartLuigi274, gKartLuigi275,
    gKartLuigi276, gKartLuigi277, gKartLuigi278, gKartLuigi279, gKartLuigi280, gKartLuigi281, gKartLuigi282,
    gKartLuigi283, gKartLuigi284, gKartLuigi285, gKartLuigi286, gKartLuigi287, gKartLuigi288,
};

u8* gKartLuigiGroup17[] = {
    gKartLuigi269, gKartLuigi270, gKartLuigi271, gKartLuigi272, gKartLuigi273, gKartLuigi274, gKartLuigi275,
    gKartLuigi276, gKartLuigi277, gKartLuigi278, gKartLuigi279, gKartLuigi280, gKartLuigi281, gKartLuigi282,
    gKartLuigi283, gKartLuigi284, gKartLuigi285, gKartLuigi286, gKartLuigi287, gKartLuigi288,
};

u8* gKartYoshiGroup0[] = {
    gKartYoshi000, gKartYoshi001, gKartYoshi002, gKartYoshi003, gKartYoshi004, gKartYoshi005, gKartYoshi006,
    gKartYoshi007, gKartYoshi008, gKartYoshi009, gKartYoshi010, gKartYoshi011, gKartYoshi012, gKartYoshi013,
    gKartYoshi014, gKartYoshi015, gKartYoshi016, gKartYoshi017, gKartYoshi018, gKartYoshi019, gKartYoshi020,
    gKartYoshi195, gKartYoshi196, gKartYoshi197, gKartYoshi198, gKartYoshi199, gKartYoshi200, gKartYoshi201,
    gKartYoshi202, gKartYoshi203, gKartYoshi204, gKartYoshi205, gKartYoshi206, gKartYoshi207, gKartYoshi208,
};

u8* gKartYoshiGroup1[] = {
    gKartYoshi021, gKartYoshi022, gKartYoshi023, gKartYoshi024, gKartYoshi025, gKartYoshi026, gKartYoshi027,
    gKartYoshi028, gKartYoshi029, gKartYoshi030, gKartYoshi031, gKartYoshi032, gKartYoshi033, gKartYoshi034,
    gKartYoshi035, gKartYoshi036, gKartYoshi037, gKartYoshi038, gKartYoshi039, gKartYoshi040, gKartYoshi041,
    gKartYoshi195, gKartYoshi196, gKartYoshi197, gKartYoshi198, gKartYoshi199, gKartYoshi200, gKartYoshi201,
    gKartYoshi202, gKartYoshi203, gKartYoshi204, gKartYoshi205, gKartYoshi206, gKartYoshi207, gKartYoshi208,
};

u8* gKartYoshiGroup2[] = {
    gKartYoshi042, gKartYoshi043, gKartYoshi044, gKartYoshi045, gKartYoshi046, gKartYoshi047, gKartYoshi048,
    gKartYoshi049, gKartYoshi050, gKartYoshi051, gKartYoshi052, gKartYoshi053, gKartYoshi054, gKartYoshi055,
    gKartYoshi056, gKartYoshi057, gKartYoshi058, gKartYoshi059, gKartYoshi060, gKartYoshi061, gKartYoshi062,
    gKartYoshi215, gKartYoshi216, gKartYoshi217, gKartYoshi218, gKartYoshi219, gKartYoshi220, gKartYoshi221,
    gKartYoshi222, gKartYoshi223, gKartYoshi224, gKartYoshi225, gKartYoshi226, gKartYoshi227, gKartYoshi228,
};

u8* gKartYoshiGroup3[] = {
    gKartYoshi063, gKartYoshi064, gKartYoshi065, gKartYoshi066, gKartYoshi067, gKartYoshi068, gKartYoshi069,
    gKartYoshi070, gKartYoshi071, gKartYoshi072, gKartYoshi073, gKartYoshi074, gKartYoshi075, gKartYoshi076,
    gKartYoshi077, gKartYoshi078, gKartYoshi079, gKartYoshi080, gKartYoshi081, gKartYoshi082, gKartYoshi083,
    gKartYoshi235, gKartYoshi236, gKartYoshi237, gKartYoshi238, gKartYoshi239, gKartYoshi240, gKartYoshi241,
    gKartYoshi242, gKartYoshi243, gKartYoshi244, gKartYoshi245, gKartYoshi246, gKartYoshi247, gKartYoshi248,
};

u8* gKartYoshiGroup4[] = {
    gKartYoshi084, gKartYoshi085, gKartYoshi086, gKartYoshi087, gKartYoshi088, gKartYoshi089, gKartYoshi090,
    gKartYoshi091, gKartYoshi092, gKartYoshi093, gKartYoshi094, gKartYoshi095, gKartYoshi096, gKartYoshi097,
    gKartYoshi098, gKartYoshi099, gKartYoshi100, gKartYoshi101, gKartYoshi102, gKartYoshi103, gKartYoshi104,
    gKartYoshi235, gKartYoshi236, gKartYoshi237, gKartYoshi238, gKartYoshi239, gKartYoshi240, gKartYoshi241,
    gKartYoshi242, gKartYoshi243, gKartYoshi244, gKartYoshi245, gKartYoshi246, gKartYoshi247, gKartYoshi248,
};

u8* gKartYoshiGroup5[] = {
    gKartYoshi105, gKartYoshi106, gKartYoshi107, gKartYoshi108, gKartYoshi109, gKartYoshi110, gKartYoshi111,
    gKartYoshi112, gKartYoshi113, gKartYoshi114, gKartYoshi115, gKartYoshi116, gKartYoshi117, gKartYoshi118,
    gKartYoshi119, gKartYoshi120, gKartYoshi121, gKartYoshi122, gKartYoshi123, gKartYoshi124, gKartYoshi125,
    gKartYoshi235, gKartYoshi236, gKartYoshi237, gKartYoshi238, gKartYoshi239, gKartYoshi240, gKartYoshi241,
    gKartYoshi242, gKartYoshi243, gKartYoshi244, gKartYoshi245, gKartYoshi246, gKartYoshi247, gKartYoshi248,
};

u8* gKartYoshiGroup6[] = {
    gKartYoshi126, gKartYoshi127, gKartYoshi128, gKartYoshi129, gKartYoshi130, gKartYoshi131, gKartYoshi132,
    gKartYoshi133, gKartYoshi134, gKartYoshi135, gKartYoshi136, gKartYoshi137, gKartYoshi138, gKartYoshi139,
    gKartYoshi140, gKartYoshi141, gKartYoshi142, gKartYoshi143, gKartYoshi144, gKartYoshi145, gKartYoshi146,
    gKartYoshi255, gKartYoshi256, gKartYoshi257, gKartYoshi258, gKartYoshi259, gKartYoshi260, gKartYoshi261,
    gKartYoshi262, gKartYoshi263, gKartYoshi264, gKartYoshi265, gKartYoshi266, gKartYoshi267, gKartYoshi268,
};

u8* gKartYoshiGroup7[] = {
    gKartYoshi147, gKartYoshi148, gKartYoshi149, gKartYoshi150, gKartYoshi151, gKartYoshi152, gKartYoshi153,
    gKartYoshi154, gKartYoshi155, gKartYoshi156, gKartYoshi157, gKartYoshi158, gKartYoshi159, gKartYoshi160,
    gKartYoshi161, gKartYoshi162, gKartYoshi163, gKartYoshi164, gKartYoshi165, gKartYoshi166, gKartYoshi167,
    gKartYoshi275, gKartYoshi276, gKartYoshi277, gKartYoshi278, gKartYoshi279, gKartYoshi280, gKartYoshi281,
    gKartYoshi282, gKartYoshi283, gKartYoshi284, gKartYoshi285, gKartYoshi286, gKartYoshi287, gKartYoshi288,
};

u8* gKartYoshiGroup8[] = {
    gKartYoshi168, gKartYoshi169, gKartYoshi170, gKartYoshi171, gKartYoshi172, gKartYoshi173, gKartYoshi174,
    gKartYoshi175, gKartYoshi176, gKartYoshi177, gKartYoshi178, gKartYoshi179, gKartYoshi180, gKartYoshi181,
    gKartYoshi182, gKartYoshi183, gKartYoshi184, gKartYoshi185, gKartYoshi186, gKartYoshi187, gKartYoshi188,
    gKartYoshi275, gKartYoshi276, gKartYoshi277, gKartYoshi278, gKartYoshi279, gKartYoshi280, gKartYoshi281,
    gKartYoshi282, gKartYoshi283, gKartYoshi284, gKartYoshi285, gKartYoshi286, gKartYoshi287, gKartYoshi288,
};

u8* gKartYoshiGroup9[] = {
    gKartYoshi189, gKartYoshi190, gKartYoshi191, gKartYoshi192, gKartYoshi193, gKartYoshi194, gKartYoshi195,
    gKartYoshi196, gKartYoshi197, gKartYoshi198, gKartYoshi199, gKartYoshi200, gKartYoshi201, gKartYoshi202,
    gKartYoshi203, gKartYoshi204, gKartYoshi205, gKartYoshi206, gKartYoshi207, gKartYoshi208,
};

u8* gKartYoshiGroup10[] = {
    gKartYoshi189, gKartYoshi190, gKartYoshi191, gKartYoshi192, gKartYoshi193, gKartYoshi194, gKartYoshi195,
    gKartYoshi196, gKartYoshi197, gKartYoshi198, gKartYoshi199, gKartYoshi200, gKartYoshi201, gKartYoshi202,
    gKartYoshi203, gKartYoshi204, gKartYoshi205, gKartYoshi206, gKartYoshi207, gKartYoshi208,
};

u8* gKartYoshiGroup11[] = {
    gKartYoshi209, gKartYoshi210, gKartYoshi211, gKartYoshi212, gKartYoshi213, gKartYoshi214, gKartYoshi215,
    gKartYoshi216, gKartYoshi217, gKartYoshi218, gKartYoshi219, gKartYoshi220, gKartYoshi221, gKartYoshi222,
    gKartYoshi223, gKartYoshi224, gKartYoshi225, gKartYoshi226, gKartYoshi227, gKartYoshi228,
};

u8* gKartYoshiGroup12[] = {
    gKartYoshi229, gKartYoshi230, gKartYoshi231, gKartYoshi232, gKartYoshi233, gKartYoshi234, gKartYoshi235,
    gKartYoshi236, gKartYoshi237, gKartYoshi238, gKartYoshi239, gKartYoshi240, gKartYoshi241, gKartYoshi242,
    gKartYoshi243, gKartYoshi244, gKartYoshi245, gKartYoshi246, gKartYoshi247, gKartYoshi248,
};

u8* gKartYoshiGroup13[] = {
    gKartYoshi229, gKartYoshi230, gKartYoshi231, gKartYoshi232, gKartYoshi233, gKartYoshi234, gKartYoshi235,
    gKartYoshi236, gKartYoshi237, gKartYoshi238, gKartYoshi239, gKartYoshi240, gKartYoshi241, gKartYoshi242,
    gKartYoshi243, gKartYoshi244, gKartYoshi245, gKartYoshi246, gKartYoshi247, gKartYoshi248,
};

u8* gKartYoshiGroup14[] = {
    gKartYoshi229, gKartYoshi230, gKartYoshi231, gKartYoshi232, gKartYoshi233, gKartYoshi234, gKartYoshi235,
    gKartYoshi236, gKartYoshi237, gKartYoshi238, gKartYoshi239, gKartYoshi240, gKartYoshi241, gKartYoshi242,
    gKartYoshi243, gKartYoshi244, gKartYoshi245, gKartYoshi246, gKartYoshi247, gKartYoshi248,
};

u8* gKartYoshiGroup15[] = {
    gKartYoshi249, gKartYoshi250, gKartYoshi251, gKartYoshi252, gKartYoshi253, gKartYoshi254, gKartYoshi255,
    gKartYoshi256, gKartYoshi257, gKartYoshi258, gKartYoshi259, gKartYoshi260, gKartYoshi261, gKartYoshi262,
    gKartYoshi263, gKartYoshi264, gKartYoshi265, gKartYoshi266, gKartYoshi267, gKartYoshi268,
};

u8* gKartYoshiGroup16[] = {
    gKartYoshi269, gKartYoshi270, gKartYoshi271, gKartYoshi272, gKartYoshi273, gKartYoshi274, gKartYoshi275,
    gKartYoshi276, gKartYoshi277, gKartYoshi278, gKartYoshi279, gKartYoshi280, gKartYoshi281, gKartYoshi282,
    gKartYoshi283, gKartYoshi284, gKartYoshi285, gKartYoshi286, gKartYoshi287, gKartYoshi288,
};

u8* gKartYoshiGroup17[] = {
    gKartYoshi269, gKartYoshi270, gKartYoshi271, gKartYoshi272, gKartYoshi273, gKartYoshi274, gKartYoshi275,
    gKartYoshi276, gKartYoshi277, gKartYoshi278, gKartYoshi279, gKartYoshi280, gKartYoshi281, gKartYoshi282,
    gKartYoshi283, gKartYoshi284, gKartYoshi285, gKartYoshi286, gKartYoshi287, gKartYoshi288,
};

u8* gKartDKGroup0[] = {
    gKartDK000, gKartDK001, gKartDK002, gKartDK003, gKartDK004, gKartDK005, gKartDK006, gKartDK007, gKartDK008,
    gKartDK009, gKartDK010, gKartDK011, gKartDK012, gKartDK013, gKartDK014, gKartDK015, gKartDK016, gKartDK017,
    gKartDK018, gKartDK019, gKartDK020, gKartDK195, gKartDK196, gKartDK197, gKartDK198, gKartDK199, gKartDK200,
    gKartDK201, gKartDK202, gKartDK203, gKartDK204, gKartDK205, gKartDK206, gKartDK207, gKartDK208,
};

u8* gKartDKGroup1[] = {
    gKartDK021, gKartDK022, gKartDK023, gKartDK024, gKartDK025, gKartDK026, gKartDK027, gKartDK028, gKartDK029,
    gKartDK030, gKartDK031, gKartDK032, gKartDK033, gKartDK034, gKartDK035, gKartDK036, gKartDK037, gKartDK038,
    gKartDK039, gKartDK040, gKartDK041, gKartDK195, gKartDK196, gKartDK197, gKartDK198, gKartDK199, gKartDK200,
    gKartDK201, gKartDK202, gKartDK203, gKartDK204, gKartDK205, gKartDK206, gKartDK207, gKartDK208,
};

u8* gKartDKGroup2[] = {
    gKartDK042, gKartDK043, gKartDK044, gKartDK045, gKartDK046, gKartDK047, gKartDK048, gKartDK049, gKartDK050,
    gKartDK051, gKartDK052, gKartDK053, gKartDK054, gKartDK055, gKartDK056, gKartDK057, gKartDK058, gKartDK059,
    gKartDK060, gKartDK061, gKartDK062, gKartDK215, gKartDK216, gKartDK217, gKartDK218, gKartDK219, gKartDK220,
    gKartDK221, gKartDK222, gKartDK223, gKartDK224, gKartDK225, gKartDK226, gKartDK227, gKartDK228,
};

u8* gKartDKGroup3[] = {
    gKartDK063, gKartDK064, gKartDK065, gKartDK066, gKartDK067, gKartDK068, gKartDK069, gKartDK070, gKartDK071,
    gKartDK072, gKartDK073, gKartDK074, gKartDK075, gKartDK076, gKartDK077, gKartDK078, gKartDK079, gKartDK080,
    gKartDK081, gKartDK082, gKartDK083, gKartDK235, gKartDK236, gKartDK237, gKartDK238, gKartDK239, gKartDK240,
    gKartDK241, gKartDK242, gKartDK243, gKartDK244, gKartDK245, gKartDK246, gKartDK247, gKartDK248,
};

u8* gKartDKGroup4[] = {
    gKartDK084, gKartDK085, gKartDK086, gKartDK087, gKartDK088, gKartDK089, gKartDK090, gKartDK091, gKartDK092,
    gKartDK093, gKartDK094, gKartDK095, gKartDK096, gKartDK097, gKartDK098, gKartDK099, gKartDK100, gKartDK101,
    gKartDK102, gKartDK103, gKartDK104, gKartDK235, gKartDK236, gKartDK237, gKartDK238, gKartDK239, gKartDK240,
    gKartDK241, gKartDK242, gKartDK243, gKartDK244, gKartDK245, gKartDK246, gKartDK247, gKartDK248,
};

u8* gKartDKGroup5[] = {
    gKartDK105, gKartDK106, gKartDK107, gKartDK108, gKartDK109, gKartDK110, gKartDK111, gKartDK112, gKartDK113,
    gKartDK114, gKartDK115, gKartDK116, gKartDK117, gKartDK118, gKartDK119, gKartDK120, gKartDK121, gKartDK122,
    gKartDK123, gKartDK124, gKartDK125, gKartDK235, gKartDK236, gKartDK237, gKartDK238, gKartDK239, gKartDK240,
    gKartDK241, gKartDK242, gKartDK243, gKartDK244, gKartDK245, gKartDK246, gKartDK247, gKartDK248,
};

u8* gKartDKGroup6[] = {
    gKartDK126, gKartDK127, gKartDK128, gKartDK129, gKartDK130, gKartDK131, gKartDK132, gKartDK133, gKartDK134,
    gKartDK135, gKartDK136, gKartDK137, gKartDK138, gKartDK139, gKartDK140, gKartDK141, gKartDK142, gKartDK143,
    gKartDK144, gKartDK145, gKartDK146, gKartDK255, gKartDK256, gKartDK257, gKartDK258, gKartDK259, gKartDK260,
    gKartDK261, gKartDK262, gKartDK263, gKartDK264, gKartDK265, gKartDK266, gKartDK267, gKartDK268,
};

u8* gKartDKGroup7[] = {
    gKartDK147, gKartDK148, gKartDK149, gKartDK150, gKartDK151, gKartDK152, gKartDK153, gKartDK154, gKartDK155,
    gKartDK156, gKartDK157, gKartDK158, gKartDK159, gKartDK160, gKartDK161, gKartDK162, gKartDK163, gKartDK164,
    gKartDK165, gKartDK166, gKartDK167, gKartDK275, gKartDK276, gKartDK277, gKartDK278, gKartDK279, gKartDK280,
    gKartDK281, gKartDK282, gKartDK283, gKartDK284, gKartDK285, gKartDK286, gKartDK287, gKartDK288,
};

u8* gKartDKGroup8[] = {
    gKartDK168, gKartDK169, gKartDK170, gKartDK171, gKartDK172, gKartDK173, gKartDK174, gKartDK175, gKartDK176,
    gKartDK177, gKartDK178, gKartDK179, gKartDK180, gKartDK181, gKartDK182, gKartDK183, gKartDK184, gKartDK185,
    gKartDK186, gKartDK187, gKartDK188, gKartDK275, gKartDK276, gKartDK277, gKartDK278, gKartDK279, gKartDK280,
    gKartDK281, gKartDK282, gKartDK283, gKartDK284, gKartDK285, gKartDK286, gKartDK287, gKartDK288,
};

u8* gKartDKGroup9[] = {
    gKartDK189, gKartDK190, gKartDK191, gKartDK192, gKartDK193, gKartDK194, gKartDK195,
    gKartDK196, gKartDK197, gKartDK198, gKartDK199, gKartDK200, gKartDK201, gKartDK202,
    gKartDK203, gKartDK204, gKartDK205, gKartDK206, gKartDK207, gKartDK208,
};

u8* gKartDKGroup10[] = {
    gKartDK189, gKartDK190, gKartDK191, gKartDK192, gKartDK193, gKartDK194, gKartDK195,
    gKartDK196, gKartDK197, gKartDK198, gKartDK199, gKartDK200, gKartDK201, gKartDK202,
    gKartDK203, gKartDK204, gKartDK205, gKartDK206, gKartDK207, gKartDK208,
};

u8* gKartDKGroup11[] = {
    gKartDK209, gKartDK210, gKartDK211, gKartDK212, gKartDK213, gKartDK214, gKartDK215,
    gKartDK216, gKartDK217, gKartDK218, gKartDK219, gKartDK220, gKartDK221, gKartDK222,
    gKartDK223, gKartDK224, gKartDK225, gKartDK226, gKartDK227, gKartDK228,
};

u8* gKartDKGroup12[] = {
    gKartDK229, gKartDK230, gKartDK231, gKartDK232, gKartDK233, gKartDK234, gKartDK235,
    gKartDK236, gKartDK237, gKartDK238, gKartDK239, gKartDK240, gKartDK241, gKartDK242,
    gKartDK243, gKartDK244, gKartDK245, gKartDK246, gKartDK247, gKartDK248,
};

u8* gKartDKGroup13[] = {
    gKartDK229, gKartDK230, gKartDK231, gKartDK232, gKartDK233, gKartDK234, gKartDK235,
    gKartDK236, gKartDK237, gKartDK238, gKartDK239, gKartDK240, gKartDK241, gKartDK242,
    gKartDK243, gKartDK244, gKartDK245, gKartDK246, gKartDK247, gKartDK248,
};

u8* gKartDKGroup14[] = {
    gKartDK229, gKartDK230, gKartDK231, gKartDK232, gKartDK233, gKartDK234, gKartDK235,
    gKartDK236, gKartDK237, gKartDK238, gKartDK239, gKartDK240, gKartDK241, gKartDK242,
    gKartDK243, gKartDK244, gKartDK245, gKartDK246, gKartDK247, gKartDK248,
};

u8* gKartDKGroup15[] = {
    gKartDK249, gKartDK250, gKartDK251, gKartDK252, gKartDK253, gKartDK254, gKartDK255,
    gKartDK256, gKartDK257, gKartDK258, gKartDK259, gKartDK260, gKartDK261, gKartDK262,
    gKartDK263, gKartDK264, gKartDK265, gKartDK266, gKartDK267, gKartDK268,
};

u8* gKartDKGroup16[] = {
    gKartDK269, gKartDK270, gKartDK271, gKartDK272, gKartDK273, gKartDK274, gKartDK275,
    gKartDK276, gKartDK277, gKartDK278, gKartDK279, gKartDK280, gKartDK281, gKartDK282,
    gKartDK283, gKartDK284, gKartDK285, gKartDK286, gKartDK287, gKartDK288,
};

u8* gKartDKGroup17[] = {
    gKartDK269, gKartDK270, gKartDK271, gKartDK272, gKartDK273, gKartDK274, gKartDK275,
    gKartDK276, gKartDK277, gKartDK278, gKartDK279, gKartDK280, gKartDK281, gKartDK282,
    gKartDK283, gKartDK284, gKartDK285, gKartDK286, gKartDK287, gKartDK288,
};

u8* gKartBowserGroup0[] = {
    gKartBowser000, gKartBowser001, gKartBowser002, gKartBowser003, gKartBowser004, gKartBowser005, gKartBowser006,
    gKartBowser007, gKartBowser008, gKartBowser009, gKartBowser010, gKartBowser011, gKartBowser012, gKartBowser013,
    gKartBowser014, gKartBowser015, gKartBowser016, gKartBowser017, gKartBowser018, gKartBowser019, gKartBowser020,
    gKartBowser195, gKartBowser196, gKartBowser197, gKartBowser198, gKartBowser199, gKartBowser200, gKartBowser201,
    gKartBowser202, gKartBowser203, gKartBowser204, gKartBowser205, gKartBowser206, gKartBowser207, gKartBowser208,
};

u8* gKartBowserGroup1[] = {
    gKartBowser021, gKartBowser022, gKartBowser023, gKartBowser024, gKartBowser025, gKartBowser026, gKartBowser027,
    gKartBowser028, gKartBowser029, gKartBowser030, gKartBowser031, gKartBowser032, gKartBowser033, gKartBowser034,
    gKartBowser035, gKartBowser036, gKartBowser037, gKartBowser038, gKartBowser039, gKartBowser040, gKartBowser041,
    gKartBowser195, gKartBowser196, gKartBowser197, gKartBowser198, gKartBowser199, gKartBowser200, gKartBowser201,
    gKartBowser202, gKartBowser203, gKartBowser204, gKartBowser205, gKartBowser206, gKartBowser207, gKartBowser208,
};

u8* gKartBowserGroup2[] = {
    gKartBowser042, gKartBowser043, gKartBowser044, gKartBowser045, gKartBowser046, gKartBowser047, gKartBowser048,
    gKartBowser049, gKartBowser050, gKartBowser051, gKartBowser052, gKartBowser053, gKartBowser054, gKartBowser055,
    gKartBowser056, gKartBowser057, gKartBowser058, gKartBowser059, gKartBowser060, gKartBowser061, gKartBowser062,
    gKartBowser215, gKartBowser216, gKartBowser217, gKartBowser218, gKartBowser219, gKartBowser220, gKartBowser221,
    gKartBowser222, gKartBowser223, gKartBowser224, gKartBowser225, gKartBowser226, gKartBowser227, gKartBowser228,
};

u8* gKartBowserGroup3[] = {
    gKartBowser063, gKartBowser064, gKartBowser065, gKartBowser066, gKartBowser067, gKartBowser068, gKartBowser069,
    gKartBowser070, gKartBowser071, gKartBowser072, gKartBowser073, gKartBowser074, gKartBowser075, gKartBowser076,
    gKartBowser077, gKartBowser078, gKartBowser079, gKartBowser080, gKartBowser081, gKartBowser082, gKartBowser083,
    gKartBowser235, gKartBowser236, gKartBowser237, gKartBowser238, gKartBowser239, gKartBowser240, gKartBowser241,
    gKartBowser242, gKartBowser243, gKartBowser244, gKartBowser245, gKartBowser246, gKartBowser247, gKartBowser248,
};

u8* gKartBowserGroup4[] = {
    gKartBowser084, gKartBowser085, gKartBowser086, gKartBowser087, gKartBowser088, gKartBowser089, gKartBowser090,
    gKartBowser091, gKartBowser092, gKartBowser093, gKartBowser094, gKartBowser095, gKartBowser096, gKartBowser097,
    gKartBowser098, gKartBowser099, gKartBowser100, gKartBowser101, gKartBowser102, gKartBowser103, gKartBowser104,
    gKartBowser235, gKartBowser236, gKartBowser237, gKartBowser238, gKartBowser239, gKartBowser240, gKartBowser241,
    gKartBowser242, gKartBowser243, gKartBowser244, gKartBowser245, gKartBowser246, gKartBowser247, gKartBowser248,
};

u8* gKartBowserGroup5[] = {
    gKartBowser105, gKartBowser106, gKartBowser107, gKartBowser108, gKartBowser109, gKartBowser110, gKartBowser111,
    gKartBowser112, gKartBowser113, gKartBowser114, gKartBowser115, gKartBowser116, gKartBowser117, gKartBowser118,
    gKartBowser119, gKartBowser120, gKartBowser121, gKartBowser122, gKartBowser123, gKartBowser124, gKartBowser125,
    gKartBowser235, gKartBowser236, gKartBowser237, gKartBowser238, gKartBowser239, gKartBowser240, gKartBowser241,
    gKartBowser242, gKartBowser243, gKartBowser244, gKartBowser245, gKartBowser246, gKartBowser247, gKartBowser248,
};

u8* gKartBowserGroup6[] = {
    gKartBowser126, gKartBowser127, gKartBowser128, gKartBowser129, gKartBowser130, gKartBowser131, gKartBowser132,
    gKartBowser133, gKartBowser134, gKartBowser135, gKartBowser136, gKartBowser137, gKartBowser138, gKartBowser139,
    gKartBowser140, gKartBowser141, gKartBowser142, gKartBowser143, gKartBowser144, gKartBowser145, gKartBowser146,
    gKartBowser255, gKartBowser256, gKartBowser257, gKartBowser258, gKartBowser259, gKartBowser260, gKartBowser261,
    gKartBowser262, gKartBowser263, gKartBowser264, gKartBowser265, gKartBowser266, gKartBowser267, gKartBowser268,
};

u8* gKartBowserGroup7[] = {
    gKartBowser147, gKartBowser148, gKartBowser149, gKartBowser150, gKartBowser151, gKartBowser152, gKartBowser153,
    gKartBowser154, gKartBowser155, gKartBowser156, gKartBowser157, gKartBowser158, gKartBowser159, gKartBowser160,
    gKartBowser161, gKartBowser162, gKartBowser163, gKartBowser164, gKartBowser165, gKartBowser166, gKartBowser167,
    gKartBowser275, gKartBowser276, gKartBowser277, gKartBowser278, gKartBowser279, gKartBowser280, gKartBowser281,
    gKartBowser282, gKartBowser283, gKartBowser284, gKartBowser285, gKartBowser286, gKartBowser287, gKartBowser288,
};

u8* gKartBowserGroup8[] = {
    gKartBowser168, gKartBowser169, gKartBowser170, gKartBowser171, gKartBowser172, gKartBowser173, gKartBowser174,
    gKartBowser175, gKartBowser176, gKartBowser177, gKartBowser178, gKartBowser179, gKartBowser180, gKartBowser181,
    gKartBowser182, gKartBowser183, gKartBowser184, gKartBowser185, gKartBowser186, gKartBowser187, gKartBowser188,
    gKartBowser275, gKartBowser276, gKartBowser277, gKartBowser278, gKartBowser279, gKartBowser280, gKartBowser281,
    gKartBowser282, gKartBowser283, gKartBowser284, gKartBowser285, gKartBowser286, gKartBowser287, gKartBowser288,
};

u8* gKartBowserGroup9[] = {
    gKartBowser189, gKartBowser190, gKartBowser191, gKartBowser192, gKartBowser193, gKartBowser194, gKartBowser195,
    gKartBowser196, gKartBowser197, gKartBowser198, gKartBowser199, gKartBowser200, gKartBowser201, gKartBowser202,
    gKartBowser203, gKartBowser204, gKartBowser205, gKartBowser206, gKartBowser207, gKartBowser208,
};

u8* gKartBowserGroup10[] = {
    gKartBowser189, gKartBowser190, gKartBowser191, gKartBowser192, gKartBowser193, gKartBowser194, gKartBowser195,
    gKartBowser196, gKartBowser197, gKartBowser198, gKartBowser199, gKartBowser200, gKartBowser201, gKartBowser202,
    gKartBowser203, gKartBowser204, gKartBowser205, gKartBowser206, gKartBowser207, gKartBowser208,
};

u8* gKartBowserGroup11[] = {
    gKartBowser209, gKartBowser210, gKartBowser211, gKartBowser212, gKartBowser213, gKartBowser214, gKartBowser215,
    gKartBowser216, gKartBowser217, gKartBowser218, gKartBowser219, gKartBowser220, gKartBowser221, gKartBowser222,
    gKartBowser223, gKartBowser224, gKartBowser225, gKartBowser226, gKartBowser227, gKartBowser228,
};

u8* gKartBowserGroup12[] = {
    gKartBowser229, gKartBowser230, gKartBowser231, gKartBowser232, gKartBowser233, gKartBowser234, gKartBowser235,
    gKartBowser236, gKartBowser237, gKartBowser238, gKartBowser239, gKartBowser240, gKartBowser241, gKartBowser242,
    gKartBowser243, gKartBowser244, gKartBowser245, gKartBowser246, gKartBowser247, gKartBowser248,
};

u8* gKartBowserGroup13[] = {
    gKartBowser229, gKartBowser230, gKartBowser231, gKartBowser232, gKartBowser233, gKartBowser234, gKartBowser235,
    gKartBowser236, gKartBowser237, gKartBowser238, gKartBowser239, gKartBowser240, gKartBowser241, gKartBowser242,
    gKartBowser243, gKartBowser244, gKartBowser245, gKartBowser246, gKartBowser247, gKartBowser248,
};

u8* gKartBowserGroup14[] = {
    gKartBowser229, gKartBowser230, gKartBowser231, gKartBowser232, gKartBowser233, gKartBowser234, gKartBowser235,
    gKartBowser236, gKartBowser237, gKartBowser238, gKartBowser239, gKartBowser240, gKartBowser241, gKartBowser242,
    gKartBowser243, gKartBowser244, gKartBowser245, gKartBowser246, gKartBowser247, gKartBowser248,
};

u8* gKartBowserGroup15[] = {
    gKartBowser249, gKartBowser250, gKartBowser251, gKartBowser252, gKartBowser253, gKartBowser254, gKartBowser255,
    gKartBowser256, gKartBowser257, gKartBowser258, gKartBowser259, gKartBowser260, gKartBowser261, gKartBowser262,
    gKartBowser263, gKartBowser264, gKartBowser265, gKartBowser266, gKartBowser267, gKartBowser268,
};

u8* gKartBowserGroup16[] = {
    gKartBowser269, gKartBowser270, gKartBowser271, gKartBowser272, gKartBowser273, gKartBowser274, gKartBowser275,
    gKartBowser276, gKartBowser277, gKartBowser278, gKartBowser279, gKartBowser280, gKartBowser281, gKartBowser282,
    gKartBowser283, gKartBowser284, gKartBowser285, gKartBowser286, gKartBowser287, gKartBowser288,
};

u8* gKartBowserGroup17[] = {
    gKartBowser269, gKartBowser270, gKartBowser271, gKartBowser272, gKartBowser273, gKartBowser274, gKartBowser275,
    gKartBowser276, gKartBowser277, gKartBowser278, gKartBowser279, gKartBowser280, gKartBowser281, gKartBowser282,
    gKartBowser283, gKartBowser284, gKartBowser285, gKartBowser286, gKartBowser287, gKartBowser288,
};

u8* gKartPeachGroup0[] = {
    gKartPeach000, gKartPeach001, gKartPeach002, gKartPeach003, gKartPeach004, gKartPeach005, gKartPeach006,
    gKartPeach007, gKartPeach008, gKartPeach009, gKartPeach010, gKartPeach011, gKartPeach012, gKartPeach013,
    gKartPeach014, gKartPeach015, gKartPeach016, gKartPeach017, gKartPeach018, gKartPeach019, gKartPeach020,
    gKartPeach195, gKartPeach196, gKartPeach197, gKartPeach198, gKartPeach199, gKartPeach200, gKartPeach201,
    gKartPeach202, gKartPeach203, gKartPeach204, gKartPeach205, gKartPeach206, gKartPeach207, gKartPeach208,
};

u8* gKartPeachGroup1[] = {
    gKartPeach021, gKartPeach022, gKartPeach023, gKartPeach024, gKartPeach025, gKartPeach026, gKartPeach027,
    gKartPeach028, gKartPeach029, gKartPeach030, gKartPeach031, gKartPeach032, gKartPeach033, gKartPeach034,
    gKartPeach035, gKartPeach036, gKartPeach037, gKartPeach038, gKartPeach039, gKartPeach040, gKartPeach041,
    gKartPeach195, gKartPeach196, gKartPeach197, gKartPeach198, gKartPeach199, gKartPeach200, gKartPeach201,
    gKartPeach202, gKartPeach203, gKartPeach204, gKartPeach205, gKartPeach206, gKartPeach207, gKartPeach208,
};

u8* gKartPeachGroup2[] = {
    gKartPeach042, gKartPeach043, gKartPeach044, gKartPeach045, gKartPeach046, gKartPeach047, gKartPeach048,
    gKartPeach049, gKartPeach050, gKartPeach051, gKartPeach052, gKartPeach053, gKartPeach054, gKartPeach055,
    gKartPeach056, gKartPeach057, gKartPeach058, gKartPeach059, gKartPeach060, gKartPeach061, gKartPeach062,
    gKartPeach215, gKartPeach216, gKartPeach217, gKartPeach218, gKartPeach219, gKartPeach220, gKartPeach221,
    gKartPeach222, gKartPeach223, gKartPeach224, gKartPeach225, gKartPeach226, gKartPeach227, gKartPeach228,
};

u8* gKartPeachGroup3[] = {
    gKartPeach063, gKartPeach064, gKartPeach065, gKartPeach066, gKartPeach067, gKartPeach068, gKartPeach069,
    gKartPeach070, gKartPeach071, gKartPeach072, gKartPeach073, gKartPeach074, gKartPeach075, gKartPeach076,
    gKartPeach077, gKartPeach078, gKartPeach079, gKartPeach080, gKartPeach081, gKartPeach082, gKartPeach083,
    gKartPeach235, gKartPeach236, gKartPeach237, gKartPeach238, gKartPeach239, gKartPeach240, gKartPeach241,
    gKartPeach242, gKartPeach243, gKartPeach244, gKartPeach245, gKartPeach246, gKartPeach247, gKartPeach248,
};

u8* gKartPeachGroup4[] = {
    gKartPeach084, gKartPeach085, gKartPeach086, gKartPeach087, gKartPeach088, gKartPeach089, gKartPeach090,
    gKartPeach091, gKartPeach092, gKartPeach093, gKartPeach094, gKartPeach095, gKartPeach096, gKartPeach097,
    gKartPeach098, gKartPeach099, gKartPeach100, gKartPeach101, gKartPeach102, gKartPeach103, gKartPeach104,
    gKartPeach235, gKartPeach236, gKartPeach237, gKartPeach238, gKartPeach239, gKartPeach240, gKartPeach241,
    gKartPeach242, gKartPeach243, gKartPeach244, gKartPeach245, gKartPeach246, gKartPeach247, gKartPeach248,
};

u8* gKartPeachGroup5[] = {
    gKartPeach105, gKartPeach106, gKartPeach107, gKartPeach108, gKartPeach109, gKartPeach110, gKartPeach111,
    gKartPeach112, gKartPeach113, gKartPeach114, gKartPeach115, gKartPeach116, gKartPeach117, gKartPeach118,
    gKartPeach119, gKartPeach120, gKartPeach121, gKartPeach122, gKartPeach123, gKartPeach124, gKartPeach125,
    gKartPeach235, gKartPeach236, gKartPeach237, gKartPeach238, gKartPeach239, gKartPeach240, gKartPeach241,
    gKartPeach242, gKartPeach243, gKartPeach244, gKartPeach245, gKartPeach246, gKartPeach247, gKartPeach248,
};

u8* gKartPeachGroup6[] = {
    gKartPeach126, gKartPeach127, gKartPeach128, gKartPeach129, gKartPeach130, gKartPeach131, gKartPeach132,
    gKartPeach133, gKartPeach134, gKartPeach135, gKartPeach136, gKartPeach137, gKartPeach138, gKartPeach139,
    gKartPeach140, gKartPeach141, gKartPeach142, gKartPeach143, gKartPeach144, gKartPeach145, gKartPeach146,
    gKartPeach255, gKartPeach256, gKartPeach257, gKartPeach258, gKartPeach259, gKartPeach260, gKartPeach261,
    gKartPeach262, gKartPeach263, gKartPeach264, gKartPeach265, gKartPeach266, gKartPeach267, gKartPeach268,
};

u8* gKartPeachGroup7[] = {
    gKartPeach147, gKartPeach148, gKartPeach149, gKartPeach150, gKartPeach151, gKartPeach152, gKartPeach153,
    gKartPeach154, gKartPeach155, gKartPeach156, gKartPeach157, gKartPeach158, gKartPeach159, gKartPeach160,
    gKartPeach161, gKartPeach162, gKartPeach163, gKartPeach164, gKartPeach165, gKartPeach166, gKartPeach167,
    gKartPeach275, gKartPeach276, gKartPeach277, gKartPeach278, gKartPeach279, gKartPeach280, gKartPeach281,
    gKartPeach282, gKartPeach283, gKartPeach284, gKartPeach285, gKartPeach286, gKartPeach287, gKartPeach288,
};

u8* gKartPeachGroup8[] = {
    gKartPeach168, gKartPeach169, gKartPeach170, gKartPeach171, gKartPeach172, gKartPeach173, gKartPeach174,
    gKartPeach175, gKartPeach176, gKartPeach177, gKartPeach178, gKartPeach179, gKartPeach180, gKartPeach181,
    gKartPeach182, gKartPeach183, gKartPeach184, gKartPeach185, gKartPeach186, gKartPeach187, gKartPeach188,
    gKartPeach275, gKartPeach276, gKartPeach277, gKartPeach278, gKartPeach279, gKartPeach280, gKartPeach281,
    gKartPeach282, gKartPeach283, gKartPeach284, gKartPeach285, gKartPeach286, gKartPeach287, gKartPeach288,
};

u8* gKartPeachGroup9[] = {
    gKartPeach189, gKartPeach190, gKartPeach191, gKartPeach192, gKartPeach193, gKartPeach194, gKartPeach195,
    gKartPeach196, gKartPeach197, gKartPeach198, gKartPeach199, gKartPeach200, gKartPeach201, gKartPeach202,
    gKartPeach203, gKartPeach204, gKartPeach205, gKartPeach206, gKartPeach207, gKartPeach208,
};

u8* gKartPeachGroup10[] = {
    gKartPeach189, gKartPeach190, gKartPeach191, gKartPeach192, gKartPeach193, gKartPeach194, gKartPeach195,
    gKartPeach196, gKartPeach197, gKartPeach198, gKartPeach199, gKartPeach200, gKartPeach201, gKartPeach202,
    gKartPeach203, gKartPeach204, gKartPeach205, gKartPeach206, gKartPeach207, gKartPeach208,
};

u8* gKartPeachGroup11[] = {
    gKartPeach209, gKartPeach210, gKartPeach211, gKartPeach212, gKartPeach213, gKartPeach214, gKartPeach215,
    gKartPeach216, gKartPeach217, gKartPeach218, gKartPeach219, gKartPeach220, gKartPeach221, gKartPeach222,
    gKartPeach223, gKartPeach224, gKartPeach225, gKartPeach226, gKartPeach227, gKartPeach228,
};

u8* gKartPeachGroup12[] = {
    gKartPeach229, gKartPeach230, gKartPeach231, gKartPeach232, gKartPeach233, gKartPeach234, gKartPeach235,
    gKartPeach236, gKartPeach237, gKartPeach238, gKartPeach239, gKartPeach240, gKartPeach241, gKartPeach242,
    gKartPeach243, gKartPeach244, gKartPeach245, gKartPeach246, gKartPeach247, gKartPeach248,
};

u8* gKartPeachGroup13[] = {
    gKartPeach229, gKartPeach230, gKartPeach231, gKartPeach232, gKartPeach233, gKartPeach234, gKartPeach235,
    gKartPeach236, gKartPeach237, gKartPeach238, gKartPeach239, gKartPeach240, gKartPeach241, gKartPeach242,
    gKartPeach243, gKartPeach244, gKartPeach245, gKartPeach246, gKartPeach247, gKartPeach248,
};

u8* gKartPeachGroup14[] = {
    gKartPeach229, gKartPeach230, gKartPeach231, gKartPeach232, gKartPeach233, gKartPeach234, gKartPeach235,
    gKartPeach236, gKartPeach237, gKartPeach238, gKartPeach239, gKartPeach240, gKartPeach241, gKartPeach242,
    gKartPeach243, gKartPeach244, gKartPeach245, gKartPeach246, gKartPeach247, gKartPeach248,
};

u8* gKartPeachGroup15[] = {
    gKartPeach249, gKartPeach250, gKartPeach251, gKartPeach252, gKartPeach253, gKartPeach254, gKartPeach255,
    gKartPeach256, gKartPeach257, gKartPeach258, gKartPeach259, gKartPeach260, gKartPeach261, gKartPeach262,
    gKartPeach263, gKartPeach264, gKartPeach265, gKartPeach266, gKartPeach267, gKartPeach268,
};

u8* gKartPeachGroup16[] = {
    gKartPeach269, gKartPeach270, gKartPeach271, gKartPeach272, gKartPeach273, gKartPeach274, gKartPeach275,
    gKartPeach276, gKartPeach277, gKartPeach278, gKartPeach279, gKartPeach280, gKartPeach281, gKartPeach282,
    gKartPeach283, gKartPeach284, gKartPeach285, gKartPeach286, gKartPeach287, gKartPeach288,
};

u8* gKartPeachGroup17[] = {
    gKartPeach269, gKartPeach270, gKartPeach271, gKartPeach272, gKartPeach273, gKartPeach274, gKartPeach275,
    gKartPeach276, gKartPeach277, gKartPeach278, gKartPeach279, gKartPeach280, gKartPeach281, gKartPeach282,
    gKartPeach283, gKartPeach284, gKartPeach285, gKartPeach286, gKartPeach287, gKartPeach288,
};

u8* gKartWarioGroup0[] = {
    gKartWario000, gKartWario001, gKartWario002, gKartWario003, gKartWario004, gKartWario005, gKartWario006,
    gKartWario007, gKartWario008, gKartWario009, gKartWario010, gKartWario011, gKartWario012, gKartWario013,
    gKartWario014, gKartWario015, gKartWario016, gKartWario017, gKartWario018, gKartWario019, gKartWario020,
    gKartWario195, gKartWario196, gKartWario197, gKartWario198, gKartWario199, gKartWario200, gKartWario201,
    gKartWario202, gKartWario203, gKartWario204, gKartWario205, gKartWario206, gKartWario207, gKartWario208,
};

u8* gKartWarioGroup1[] = {
    gKartWario021, gKartWario022, gKartWario023, gKartWario024, gKartWario025, gKartWario026, gKartWario027,
    gKartWario028, gKartWario029, gKartWario030, gKartWario031, gKartWario032, gKartWario033, gKartWario034,
    gKartWario035, gKartWario036, gKartWario037, gKartWario038, gKartWario039, gKartWario040, gKartWario041,
    gKartWario195, gKartWario196, gKartWario197, gKartWario198, gKartWario199, gKartWario200, gKartWario201,
    gKartWario202, gKartWario203, gKartWario204, gKartWario205, gKartWario206, gKartWario207, gKartWario208,
};

u8* gKartWarioGroup2[] = {
    gKartWario042, gKartWario043, gKartWario044, gKartWario045, gKartWario046, gKartWario047, gKartWario048,
    gKartWario049, gKartWario050, gKartWario051, gKartWario052, gKartWario053, gKartWario054, gKartWario055,
    gKartWario056, gKartWario057, gKartWario058, gKartWario059, gKartWario060, gKartWario061, gKartWario062,
    gKartWario215, gKartWario216, gKartWario217, gKartWario218, gKartWario219, gKartWario220, gKartWario221,
    gKartWario222, gKartWario223, gKartWario224, gKartWario225, gKartWario226, gKartWario227, gKartWario228,
};

u8* gKartWarioGroup3[] = {
    gKartWario063, gKartWario064, gKartWario065, gKartWario066, gKartWario067, gKartWario068, gKartWario069,
    gKartWario070, gKartWario071, gKartWario072, gKartWario073, gKartWario074, gKartWario075, gKartWario076,
    gKartWario077, gKartWario078, gKartWario079, gKartWario080, gKartWario081, gKartWario082, gKartWario083,
    gKartWario235, gKartWario236, gKartWario237, gKartWario238, gKartWario239, gKartWario240, gKartWario241,
    gKartWario242, gKartWario243, gKartWario244, gKartWario245, gKartWario246, gKartWario247, gKartWario248,
};

u8* gKartWarioGroup4[] = {
    gKartWario084, gKartWario085, gKartWario086, gKartWario087, gKartWario088, gKartWario089, gKartWario090,
    gKartWario091, gKartWario092, gKartWario093, gKartWario094, gKartWario095, gKartWario096, gKartWario097,
    gKartWario098, gKartWario099, gKartWario100, gKartWario101, gKartWario102, gKartWario103, gKartWario104,
    gKartWario235, gKartWario236, gKartWario237, gKartWario238, gKartWario239, gKartWario240, gKartWario241,
    gKartWario242, gKartWario243, gKartWario244, gKartWario245, gKartWario246, gKartWario247, gKartWario248,
};

u8* gKartWarioGroup5[] = {
    gKartWario105, gKartWario106, gKartWario107, gKartWario108, gKartWario109, gKartWario110, gKartWario111,
    gKartWario112, gKartWario113, gKartWario114, gKartWario115, gKartWario116, gKartWario117, gKartWario118,
    gKartWario119, gKartWario120, gKartWario121, gKartWario122, gKartWario123, gKartWario124, gKartWario125,
    gKartWario235, gKartWario236, gKartWario237, gKartWario238, gKartWario239, gKartWario240, gKartWario241,
    gKartWario242, gKartWario243, gKartWario244, gKartWario245, gKartWario246, gKartWario247, gKartWario248,
};

u8* gKartWarioGroup6[] = {
    gKartWario126, gKartWario127, gKartWario128, gKartWario129, gKartWario130, gKartWario131, gKartWario132,
    gKartWario133, gKartWario134, gKartWario135, gKartWario136, gKartWario137, gKartWario138, gKartWario139,
    gKartWario140, gKartWario141, gKartWario142, gKartWario143, gKartWario144, gKartWario145, gKartWario146,
    gKartWario255, gKartWario256, gKartWario257, gKartWario258, gKartWario259, gKartWario260, gKartWario261,
    gKartWario262, gKartWario263, gKartWario264, gKartWario265, gKartWario266, gKartWario267, gKartWario268,
};

u8* gKartWarioGroup7[] = {
    gKartWario147, gKartWario148, gKartWario149, gKartWario150, gKartWario151, gKartWario152, gKartWario153,
    gKartWario154, gKartWario155, gKartWario156, gKartWario157, gKartWario158, gKartWario159, gKartWario160,
    gKartWario161, gKartWario162, gKartWario163, gKartWario164, gKartWario165, gKartWario166, gKartWario167,
    gKartWario275, gKartWario276, gKartWario277, gKartWario278, gKartWario279, gKartWario280, gKartWario281,
    gKartWario282, gKartWario283, gKartWario284, gKartWario285, gKartWario286, gKartWario287, gKartWario288,
};

u8* gKartWarioGroup8[] = {
    gKartWario168, gKartWario169, gKartWario170, gKartWario171, gKartWario172, gKartWario173, gKartWario174,
    gKartWario175, gKartWario176, gKartWario177, gKartWario178, gKartWario179, gKartWario180, gKartWario181,
    gKartWario182, gKartWario183, gKartWario184, gKartWario185, gKartWario186, gKartWario187, gKartWario188,
    gKartWario275, gKartWario276, gKartWario277, gKartWario278, gKartWario279, gKartWario280, gKartWario281,
    gKartWario282, gKartWario283, gKartWario284, gKartWario285, gKartWario286, gKartWario287, gKartWario288,
};

u8* gKartWarioGroup9[] = {
    gKartWario189, gKartWario190, gKartWario191, gKartWario192, gKartWario193, gKartWario194, gKartWario195,
    gKartWario196, gKartWario197, gKartWario198, gKartWario199, gKartWario200, gKartWario201, gKartWario202,
    gKartWario203, gKartWario204, gKartWario205, gKartWario206, gKartWario207, gKartWario208,
};

u8* gKartWarioGroup10[] = {
    gKartWario189, gKartWario190, gKartWario191, gKartWario192, gKartWario193, gKartWario194, gKartWario195,
    gKartWario196, gKartWario197, gKartWario198, gKartWario199, gKartWario200, gKartWario201, gKartWario202,
    gKartWario203, gKartWario204, gKartWario205, gKartWario206, gKartWario207, gKartWario208,
};

u8* gKartWarioGroup11[] = {
    gKartWario209, gKartWario210, gKartWario211, gKartWario212, gKartWario213, gKartWario214, gKartWario215,
    gKartWario216, gKartWario217, gKartWario218, gKartWario219, gKartWario220, gKartWario221, gKartWario222,
    gKartWario223, gKartWario224, gKartWario225, gKartWario226, gKartWario227, gKartWario228,
};

u8* gKartWarioGroup12[] = {
    gKartWario229, gKartWario230, gKartWario231, gKartWario232, gKartWario233, gKartWario234, gKartWario235,
    gKartWario236, gKartWario237, gKartWario238, gKartWario239, gKartWario240, gKartWario241, gKartWario242,
    gKartWario243, gKartWario244, gKartWario245, gKartWario246, gKartWario247, gKartWario248,
};

u8* gKartWarioGroup13[] = {
    gKartWario229, gKartWario230, gKartWario231, gKartWario232, gKartWario233, gKartWario234, gKartWario235,
    gKartWario236, gKartWario237, gKartWario238, gKartWario239, gKartWario240, gKartWario241, gKartWario242,
    gKartWario243, gKartWario244, gKartWario245, gKartWario246, gKartWario247, gKartWario248,
};

u8* gKartWarioGroup14[] = {
    gKartWario229, gKartWario230, gKartWario231, gKartWario232, gKartWario233, gKartWario234, gKartWario235,
    gKartWario236, gKartWario237, gKartWario238, gKartWario239, gKartWario240, gKartWario241, gKartWario242,
    gKartWario243, gKartWario244, gKartWario245, gKartWario246, gKartWario247, gKartWario248,
};

u8* gKartWarioGroup15[] = {
    gKartWario249, gKartWario250, gKartWario251, gKartWario252, gKartWario253, gKartWario254, gKartWario255,
    gKartWario256, gKartWario257, gKartWario258, gKartWario259, gKartWario260, gKartWario261, gKartWario262,
    gKartWario263, gKartWario264, gKartWario265, gKartWario266, gKartWario267, gKartWario268,
};

u8* gKartWarioGroup16[] = {
    gKartWario269, gKartWario270, gKartWario271, gKartWario272, gKartWario273, gKartWario274, gKartWario275,
    gKartWario276, gKartWario277, gKartWario278, gKartWario279, gKartWario280, gKartWario281, gKartWario282,
    gKartWario283, gKartWario284, gKartWario285, gKartWario286, gKartWario287, gKartWario288,
};

u8* gKartWarioGroup17[] = {
    gKartWario269, gKartWario270, gKartWario271, gKartWario272, gKartWario273, gKartWario274, gKartWario275,
    gKartWario276, gKartWario277, gKartWario278, gKartWario279, gKartWario280, gKartWario281, gKartWario282,
    gKartWario283, gKartWario284, gKartWario285, gKartWario286, gKartWario287, gKartWario288,
};

u8** gKartMarioTable0[] = {
    gKartMarioGroup8, gKartMarioGroup7, gKartMarioGroup6, gKartMarioGroup5, gKartMarioGroup4,
    gKartMarioGroup3, gKartMarioGroup2, gKartMarioGroup1, gKartMarioGroup0,
};

u8** gKartLuigiTable0[] = {
    gKartLuigiGroup8, gKartLuigiGroup7, gKartLuigiGroup6, gKartLuigiGroup5, gKartLuigiGroup4,
    gKartLuigiGroup3, gKartLuigiGroup2, gKartLuigiGroup1, gKartLuigiGroup0,
};

u8** gKartYoshiTable0[] = {
    gKartYoshiGroup8, gKartYoshiGroup7, gKartYoshiGroup6, gKartYoshiGroup5, gKartYoshiGroup4,
    gKartYoshiGroup3, gKartYoshiGroup2, gKartYoshiGroup1, gKartYoshiGroup0,
};

u8** gKartToadTable0[] = {
    gKartToadGroup8, gKartToadGroup7, gKartToadGroup6, gKartToadGroup5, gKartToadGroup4,
    gKartToadGroup3, gKartToadGroup2, gKartToadGroup1, gKartToadGroup0,
};

u8** gKartDKTable0[] = {
    gKartDKGroup8, gKartDKGroup7, gKartDKGroup6, gKartDKGroup5, gKartDKGroup4,
    gKartDKGroup3, gKartDKGroup2, gKartDKGroup1, gKartDKGroup0,
};

u8** gKartWarioTable0[] = {
    gKartWarioGroup8, gKartWarioGroup7, gKartWarioGroup6, gKartWarioGroup5, gKartWarioGroup4,
    gKartWarioGroup3, gKartWarioGroup2, gKartWarioGroup1, gKartWarioGroup0,
};

u8** gKartPeachTable0[] = {
    gKartPeachGroup8, gKartPeachGroup7, gKartPeachGroup6, gKartPeachGroup5, gKartPeachGroup4,
    gKartPeachGroup3, gKartPeachGroup2, gKartPeachGroup1, gKartPeachGroup0,
};

u8** gKartBowserTable0[] = {
    gKartBowserGroup8, gKartBowserGroup7, gKartBowserGroup6, gKartBowserGroup5, gKartBowserGroup4,
    gKartBowserGroup3, gKartBowserGroup2, gKartBowserGroup1, gKartBowserGroup0,
};

u8** gKartMarioTable1[] = {
    gKartMarioGroup17, gKartMarioGroup16, gKartMarioGroup15, gKartMarioGroup14, gKartMarioGroup13,
    gKartMarioGroup12, gKartMarioGroup11, gKartMarioGroup10, gKartMarioGroup9,
};

u8** gKartLuigiTable1[] = {
    gKartLuigiGroup17, gKartLuigiGroup16, gKartLuigiGroup15, gKartLuigiGroup14, gKartLuigiGroup13,
    gKartLuigiGroup12, gKartLuigiGroup11, gKartLuigiGroup10, gKartLuigiGroup9,
};

u8** gKartYoshiTable1[] = {
    gKartYoshiGroup17, gKartYoshiGroup16, gKartYoshiGroup15, gKartYoshiGroup14, gKartYoshiGroup13,
    gKartYoshiGroup12, gKartYoshiGroup11, gKartYoshiGroup10, gKartYoshiGroup9,
};

u8** gKartToadTable1[] = {
    gKartToadGroup17, gKartToadGroup16, gKartToadGroup15, gKartToadGroup14, gKartToadGroup13,
    gKartToadGroup12, gKartToadGroup11, gKartToadGroup10, gKartToadGroup9,
};

u8** gKartDKTable1[] = {
    gKartDKGroup17, gKartDKGroup16, gKartDKGroup15, gKartDKGroup14, gKartDKGroup13,
    gKartDKGroup12, gKartDKGroup11, gKartDKGroup10, gKartDKGroup9,
};

u8** gKartWarioTable1[] = {
    gKartWarioGroup17, gKartWarioGroup16, gKartWarioGroup15, gKartWarioGroup14, gKartWarioGroup13,
    gKartWarioGroup12, gKartWarioGroup11, gKartWarioGroup10, gKartWarioGroup9,
};

u8** gKartPeachTable1[] = {
    gKartPeachGroup17, gKartPeachGroup16, gKartPeachGroup15, gKartPeachGroup14, gKartPeachGroup13,
    gKartPeachGroup12, gKartPeachGroup11, gKartPeachGroup10, gKartPeachGroup9,
};

u8** gKartBowserTable1[] = {
    gKartBowserGroup17, gKartBowserGroup16, gKartBowserGroup15, gKartBowserGroup14, gKartBowserGroup13,
    gKartBowserGroup12, gKartBowserGroup11, gKartBowserGroup10, gKartBowserGroup9,
};

u8*** gKartTextureTable0[] = {
    gKartMarioTable0, gKartLuigiTable0, gKartYoshiTable0, gKartToadTable0,
    gKartDKTable0,    gKartWarioTable0, gKartPeachTable0, gKartBowserTable0,
};

u8*** gKartTextureTable1[] = {
    gKartMarioTable1, gKartLuigiTable1, gKartYoshiTable1, gKartToadTable1,
    gKartDKTable1,    gKartWarioTable1, gKartPeachTable1, gKartBowserTable1,
};

u8* gKartMarioTumble[] = {
    gKartMario289, gKartMario290, gKartMario291, gKartMario292, gKartMario293, gKartMario294, gKartMario295,
    gKartMario296, gKartMario297, gKartMario298, gKartMario299, gKartMario300, gKartMario301, gKartMario302,
    gKartMario303, gKartMario304, gKartMario305, gKartMario306, gKartMario307, gKartMario308, gKartMario309,
    gKartMario310, gKartMario311, gKartMario312, gKartMario313, gKartMario314, gKartMario315, gKartMario316,
    gKartMario317, gKartMario318, gKartMario319, gKartMario320,
};

u8* gKartLuigiTumble[] = {
    gKartLuigi289, gKartLuigi290, gKartLuigi291, gKartLuigi292, gKartLuigi293, gKartLuigi294, gKartLuigi295,
    gKartLuigi296, gKartLuigi297, gKartLuigi298, gKartLuigi299, gKartLuigi300, gKartLuigi301, gKartLuigi302,
    gKartLuigi303, gKartLuigi304, gKartLuigi305, gKartLuigi306, gKartLuigi307, gKartLuigi308, gKartLuigi309,
    gKartLuigi310, gKartLuigi311, gKartLuigi312, gKartLuigi313, gKartLuigi314, gKartLuigi315, gKartLuigi316,
    gKartLuigi317, gKartLuigi318, gKartLuigi319, gKartLuigi320,
};

u8* gKartBowserTumble[] = {
    gKartBowser289, gKartBowser290, gKartBowser291, gKartBowser292, gKartBowser293, gKartBowser294, gKartBowser295,
    gKartBowser296, gKartBowser297, gKartBowser298, gKartBowser299, gKartBowser300, gKartBowser301, gKartBowser302,
    gKartBowser303, gKartBowser304, gKartBowser305, gKartBowser306, gKartBowser307, gKartBowser308, gKartBowser309,
    gKartBowser310, gKartBowser311, gKartBowser312, gKartBowser313, gKartBowser314, gKartBowser315, gKartBowser316,
    gKartBowser317, gKartBowser318, gKartBowser319, gKartBowser320,
};

u8* gKartToadTumble[] = {
    gKartToad289, gKartToad290, gKartToad291, gKartToad292, gKartToad293, gKartToad294, gKartToad295, gKartToad296,
    gKartToad297, gKartToad298, gKartToad299, gKartToad300, gKartToad301, gKartToad302, gKartToad303, gKartToad304,
    gKartToad305, gKartToad306, gKartToad307, gKartToad308, gKartToad309, gKartToad310, gKartToad311, gKartToad312,
    gKartToad313, gKartToad314, gKartToad315, gKartToad316, gKartToad317, gKartToad318, gKartToad319, gKartToad320,
};

u8* gKartYoshiTumble[] = {
    gKartYoshi289, gKartYoshi290, gKartYoshi291, gKartYoshi292, gKartYoshi293, gKartYoshi294, gKartYoshi295,
    gKartYoshi296, gKartYoshi297, gKartYoshi298, gKartYoshi299, gKartYoshi300, gKartYoshi301, gKartYoshi302,
    gKartYoshi303, gKartYoshi304, gKartYoshi305, gKartYoshi306, gKartYoshi307, gKartYoshi308, gKartYoshi309,
    gKartYoshi310, gKartYoshi311, gKartYoshi312, gKartYoshi313, gKartYoshi314, gKartYoshi315, gKartYoshi316,
    gKartYoshi317, gKartYoshi318, gKartYoshi319, gKartYoshi320,
};

u8* gKartDKTumble[] = {
    gKartDK289, gKartDK290, gKartDK291, gKartDK292, gKartDK293, gKartDK294, gKartDK295, gKartDK296,
    gKartDK297, gKartDK298, gKartDK299, gKartDK300, gKartDK301, gKartDK302, gKartDK303, gKartDK304,
    gKartDK305, gKartDK306, gKartDK307, gKartDK308, gKartDK309, gKartDK310, gKartDK311, gKartDK312,
    gKartDK313, gKartDK314, gKartDK315, gKartDK316, gKartDK317, gKartDK318, gKartDK319, gKartDK320,
};

u8* gKartPeachTumble[] = {
    gKartPeach289, gKartPeach290, gKartPeach291, gKartPeach292, gKartPeach293, gKartPeach294, gKartPeach295,
    gKartPeach296, gKartPeach297, gKartPeach298, gKartPeach299, gKartPeach300, gKartPeach301, gKartPeach302,
    gKartPeach303, gKartPeach304, gKartPeach305, gKartPeach306, gKartPeach307, gKartPeach308, gKartPeach309,
    gKartPeach310, gKartPeach311, gKartPeach312, gKartPeach313, gKartPeach314, gKartPeach315, gKartPeach316,
    gKartPeach317, gKartPeach318, gKartPeach319, gKartPeach320,
};

u8* gKartWarioTumble[] = {
    gKartWario289, gKartWario290, gKartWario291, gKartWario292, gKartWario293, gKartWario294, gKartWario295,
    gKartWario296, gKartWario297, gKartWario298, gKartWario299, gKartWario300, gKartWario301, gKartWario302,
    gKartWario303, gKartWario304, gKartWario305, gKartWario306, gKartWario307, gKartWario308, gKartWario309,
    gKartWario310, gKartWario311, gKartWario312, gKartWario313, gKartWario314, gKartWario315, gKartWario316,
    gKartWario317, gKartWario318, gKartWario319, gKartWario320,
};

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
    if (((temp & 0x80) == 0x80) || ((temp & 0x40) == 0x40) || ((temp & 0x80000) == 0x80000) ||
        ((temp & 0x800000) == 0x800000) || ((temp & 0x20000) == 0x20000) || ((player->unk_044 & 0x800) != 0)) {
        if (player->animFrameSelector[screenId] != 0) {
            osInvalDCache(&gEncodedKartTexture[index][screenId2][playerId], D_800DDEB0[player->characterId]);

#ifdef TARGET_N64
            osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ,
                         (uintptr_t) &_kart_texturesSegmentRomStart[SEGMENT_OFFSET(
                             gKartTextureTable1[player->characterId][player->animGroupSelector[screenId]]
                                               [player->animFrameSelector[screenId]])],
                         &gEncodedKartTexture[index][screenId2][playerId], D_800DDEB0[player->characterId],
                         &gDmaMesgQueue);

            osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
#else
            gEncodedKartTexture[index][screenId2][playerId].unk_00 =
                gKartTextureTable1[player->characterId][player->animGroupSelector[screenId]]
                                  [player->animFrameSelector[screenId]];
#endif
        } else {
            osInvalDCache(&gEncodedKartTexture[index][screenId2][playerId], D_800DDEB0[player->characterId]);
#ifdef TARGET_N64
            osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ,
                         (uintptr_t) &_kart_texturesSegmentRomStart[SEGMENT_OFFSET(
                             gKartTextureTable0[player->characterId][player->animGroupSelector[screenId]]
                                               [player->animFrameSelector[screenId]])],
                         &gEncodedKartTexture[index][screenId2][playerId], D_800DDEB0[player->characterId],
                         &gDmaMesgQueue);

            osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
#else
            gEncodedKartTexture[index][screenId2][playerId].unk_00 =
                gKartTextureTable0[player->characterId][player->animGroupSelector[screenId]]
                                  [player->animFrameSelector[screenId]];
#endif
        }
    } else if (((temp & 0x400) == 0x400) || ((temp & 0x01000000) == 0x01000000) ||
               ((temp & 0x02000000) == 0x02000000) || ((temp & 0x10000) == 0x10000)) {
        osInvalDCache(&gEncodedKartTexture[index][screenId2][playerId], 0x780U);
// player->unk_0A8 >> 8 converts an 8.8 fixed-point animation frame to a whole number.
#ifdef TARGET_N64
        osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ,
                     (uintptr_t) &_kart_texturesSegmentRomStart[SEGMENT_OFFSET(
                         gKartTextureTumbles[player->characterId][player->unk_0A8 >> 8])],
                     &gEncodedKartTexture[index][screenId2][playerId], 0x900, &gDmaMesgQueue);

        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
#else
        gEncodedKartTexture[index][screenId2][playerId].unk_00 =
            gKartTextureTumbles[player->characterId][player->unk_0A8 >> 8];
#endif
    } else {
        osInvalDCache(&gEncodedKartTexture[index][screenId2][playerId], D_800DDEB0[player->characterId]);

#ifdef TARGET_N64
        osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ,
                     (uintptr_t) &_kart_texturesSegmentRomStart[SEGMENT_OFFSET(
                         gKartTextureTable0[player->characterId][player->animGroupSelector[screenId]]
                                           [player->animFrameSelector[screenId]])],
                     &gEncodedKartTexture[index][screenId2][playerId], D_800DDEB0[player->characterId], &gDmaMesgQueue);

        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
#else
        gEncodedKartTexture[index][screenId2][playerId].unk_00 =
            gKartTextureTable0[player->characterId][player->animGroupSelector[screenId]]
                              [player->animFrameSelector[screenId]];
#endif
    }
}

void load_kart_texture_non_blocking(Player* player, s8 arg1, s8 arg2, s8 arg3, s8 arg4) {
    s32 temp = player->effects;

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
                                  [player->animFrameSelector[arg2]];
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
                                  [player->animFrameSelector[arg2]];
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
            gKartTextureTable0[player->characterId][player->animGroupSelector[arg2]][player->animFrameSelector[arg2]];
#endif
    }
}

void load_kart_palette(Player* player, s8 playerId, s8 screenId, s8 buffer) {
    u8* asset;
    size_t size;
#ifdef AVOID_UB
    struct_D_802F1F80* temp_s0 = &gPlayerPalettesList[buffer][screenId][playerId];
#else
    struct_D_802F1F80* temp_s0 = (struct_D_802F1F80*) &gPlayerPalettesList[buffer][screenId][playerId * 0x100];
#endif
    switch (gActiveScreenMode) {
        case SCREEN_MODE_1P:
        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:
        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            osInvalDCache(temp_s0, sizeof(struct_D_802F1F80));

#ifdef TARGET_N64
            osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ,
                         (uintptr_t) &_kart_texturesSegmentRomStart[SEGMENT_OFFSET(gKartPalettes[player->characterId])],
                         temp_s0, sizeof(struct_D_802F1F80), &gDmaMesgQueue);
            osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
#else
            size = ResourceGetTexSizeByName(gKartPalettes[player->characterId]);
            asset = (u8*) LOAD_ASSET(gKartPalettes[player->characterId]);
            memcpy(&gPlayerPalettesList[buffer][screenId][playerId].kart_palette[0], asset, size);
#endif

            break;
        case SCREEN_MODE_3P_4P_SPLITSCREEN: // Code identical to above
            osInvalDCache(temp_s0, sizeof(struct_D_802F1F80));

#ifdef TARGET_N64
            osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ,
                         (uintptr_t) &_kart_texturesSegmentRomStart[SEGMENT_OFFSET(gKartPalettes[player->characterId])],
                         temp_s0, sizeof(struct_D_802F1F80), &gDmaMesgQueue);

            osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
#else
            size = ResourceGetTexSizeByName(gKartPalettes[player->characterId]);
            asset = (u8*) LOAD_ASSET(gKartPalettes[player->characterId]);
            memcpy(&gPlayerPalettesList[buffer][screenId][playerId].kart_palette[0], asset, size);
#endif

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
