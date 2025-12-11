#include <libultraship.h>
#include <libultra/gbi.h>
#include "CoreMath.h"
#include <vector>
#include <memory>

#include "Harbour.h"
#include "World.h"
#include "engine/actors/Finishline.h"
#include "engine/actors/BowserStatue.h"
#include "engine/actors/Ship.h"
#include "engine/actors/SpaghettiShip.h"
#include "engine/actors/Starship.h"
#include "engine/objects/Object.h"
#include "engine/objects/BombKart.h"
#include "assets/models/tracks/mario_raceway/mario_raceway_data.h"
#include "assets/models/tracks/bowsers_castle/bowsers_castle_data.h"
#include "assets/models/tracks/bowsers_castle/bowsers_castle_displaylists.h"
#include "engine/actors/Tree.h"
#include "engine/actors/Cloud.h"
#include "engine/vehicles/Train.h"
#include "engine/objects/Trophy.h"
#include "engine/objects/CheepCheep.h"
#include "engine/objects/Snowman.h"
#include "engine/objects/TrashBin.h"
#include "engine/objects/Hedgehog.h"
#include "engine/objects/Flagpole.h"
#include "engine/objects/HotAirBalloon.h"
#include "engine/objects/Crab.h"
#include "engine/objects/Boos.h"
#include "engine/objects/GrandPrixBalloons.h"

extern "C" {
    #include "main.h"
    #include "camera.h"
    #include "course_offsets.h"
    #include "code_800029B0.h"
    #include "render_courses.h"
    #include "code_8006E9C0.h"
    #include "code_80057C60.h"
    #include "defines.h"
    #include "math_util.h"
    #include "external.h"
    #include "code_80005FD0.h"
    #include "spawn_players.h"
    #include "render_objects.h"
    #include "assets/models/common_data.h"
    #include "save.h"
    #include "replays.h"
    #include "actors.h"
    #include "collision.h"
    #include "memory.h"
    #include "courses/harbour/track.h"
}

TrackPathPoint harbour_path[] = {
    {16,-121,-34,0},
    {27,-121,-60,0},
    {31,-121,-83,0},
    {34,-121,-101,0},
    {38,-121,-119,0},
    {42,-121,-132,0},
    {46,-121,-141,0},
    {53,-121,-150,0},
    {60,-121,-158,0},
    {64,-121,-171,0},
    {71,-121,-185,0},
    {79,-121,-194,0},
    {87,-121,-209,0},
    {98,-121,-225,0},
    {109,-121,-236,0},
    {120,-121,-251,0},
    {130,-121,-263,0},
    {140,-121,-273,0},
    {149,-121,-280,0},
    {157,-121,-284,0},
    {167,-121,-292,0},
    {177,-121,-301,0},
    {191,-121,-306,0},
    {203,-121,-313,0},
    {213,-121,-319,0},
    {228,-121,-325,0},
    {240,-121,-332,0},
    {251,-121,-340,0},
    {260,-121,-342,0},
    {270,-121,-345,0},
    {279,-121,-347,0},
    {289,-121,-348,0},
    {298,-121,-351,0},
    {311,-121,-356,0},
    {323,-121,-359,0},
    {332,-121,-359,0},
    {343,-121,-361,0},
    {354,-121,-363,0},
    {363,-121,-361,0},
    {373,-121,-363,0},
    {385,-121,-362,0},
    {396,-121,-360,0},
    {405,-121,-359,0},
    {418,-121,-357,0},
    {432,-120,-354,0},
    {444,-119,-349,0},
    {459,-119,-346,0},
    {467,-119,-341,0},
    {479,-119,-337,0},
    {489,-119,-332,0},
    {501,-120,-325,0},
    {513,-120,-315,0},
    {526,-121,-309,0},
    {537,-121,-300,0},
    {551,-121,-294,0},
    {561,-121,-282,0},
    {572,-120,-269,0},
    {583,-119,-259,0},
    {592,-118,-254,0},
    {598,-119,-246,0},
    {606,-119,-240,0},
    {615,-121,-228,0},
    {624,-121,-219,0},
    {637,-121,-212,0},
    {647,-121,-206,0},
    {657,-121,-193,0},
    {667,-121,-188,0},
    {674,-121,-180,0},
    {688,-122,-173,0},
    {699,-123,-160,0},
    {711,-121,-152,0},
    {722,-118,-141,0},
    {734,-115,-136,0},
    {745,-115,-130,0},
    {753,-116,-121,0},
    {768,-117,-112,0},
    {788,-116,-99,0},
    {809,-114,-85,0},
    {832,-112,-72,0},
    {849,-110,-63,0},
    {861,-110,-57,0},
    {875,-109,-51,0},
    {888,-109,-42,0},
    {899,-109,-37,0},
    {923,-110,-31,0},
    {939,-110,-25,0},
    {950,-110,-21,0},
    {958,-110,-18,0},
    {975,-109,-13,0},
    {988,-109,-8,0},
    {1017,-109,-17,0},
    {1033,-109,-25,0},
    {1055,-110,-33,0},
    {1075,-112,-38,0},
    {1091,-115,-45,0},
    {1104,-118,-55,0},
    {1112,-119,-62,0},
    {1137,-124,-75,0},
    {1160,-127,-91,0},
    {1173,-128,-101,0},
    {1184,-129,-118,0},
    {1209,-132,-147,0},
    {1224,-132,-163,0},
    {1235,-132,-190,0},
    {1244,-132,-221,0},
    {1251,-132,-257,0},
    {1255,-132,-282,0},
    {1256,-132,-327,0},
    {1253,-132,-364,0},
    {1247,-132,-405,0},
    {1236,-132,-431,0},
    {1233,-132,-473,0},
    {1228,-132,-495,0},
    {1223,-132,-513,0},
    {1218,-132,-543,0},
    {1206,-132,-561,0},
    {1197,-132,-584,0},
    {1182,-132,-613,0},
    {1170,-132,-645,0},
    {1163,-132,-660,0},
    {1158,-132,-677,0},
    {1150,-132,-689,0},
    {1138,-132,-712,0},
    {1126,-132,-737,0},
    {1114,-132,-760,0},
    {1101,-132,-775,0},
    {1085,-132,-796,0},
    {1070,-132,-823,0},
    {1058,-132,-839,0},
    {1046,-132,-861,0},
    {1024,-132,-886,0},
    {1010,-132,-906,0},
    {997,-132,-918,0},
    {989,-132,-930,0},
    {975,-132,-943,0},
    {953,-132,-971,0},
    {934,-132,-985,0},
    {919,-132,-999,0},
    {904,-132,-1014,0},
    {889,-132,-1030,0},
    {874,-132,-1044,0},
    {852,-132,-1057,0},
    {826,-132,-1074,0},
    {806,-132,-1087,0},
    {787,-132,-1104,0},
    {773,-132,-1111,0},
    {751,-132,-1125,0},
    {728,-132,-1136,0},
    {710,-132,-1148,0},
    {687,-132,-1162,0},
    {667,-132,-1174,0},
    {648,-132,-1187,0},
    {632,-132,-1190,0},
    {615,-132,-1198,0},
    {597,-132,-1205,0},
    {576,-132,-1211,0},
    {562,-132,-1216,0},
    {543,-132,-1222,0},
    {526,-132,-1227,0},
    {513,-132,-1231,0},
    {495,-132,-1236,0},
    {472,-132,-1243,0},
    {451,-132,-1243,0},
    {431,-132,-1245,0},
    {402,-132,-1250,0},
    {379,-132,-1250,0},
    {358,-132,-1252,0},
    {340,-132,-1253,0},
    {316,-132,-1255,0},
    {298,-132,-1254,0},
    {275,-132,-1252,0},
    {246,-132,-1249,0},
    {221,-132,-1242,0},
    {198,-132,-1237,0},
    {173,-132,-1232,0},
    {156,-132,-1229,0},
    {137,-132,-1221,0},
    {119,-132,-1208,0},
    {103,-132,-1195,0},
    {81,-132,-1179,0},
    {63,-132,-1161,0},
    {48,-132,-1150,0},
    {28,-132,-1125,0},
    {9,-132,-1104,0},
    {-3,-132,-1086,0},
    {-24,-132,-1064,0},
    {-38,-133,-1049,0},
    {-50,-133,-1030,0},
    {-64,-134,-1005,0},
    {-82,-135,-985,0},
    {-97,-136,-960,0},
    {-109,-137,-942,0},
    {-130,-138,-921,0},
    {-148,-139,-897,0},
    {-160,-141,-874,0},
    {-176,-142,-849,0},
    {-202,-143,-821,0},
    {-219,-145,-792,0},
    {-235,-147,-763,0},
    {-257,-149,-729,0},
    {-273,-150,-705,0},
    {-288,-152,-673,0},
    {-309,-154,-637,0},
    {-326,-155,-603,0},
    {-343,-157,-569,0},
    {-356,-158,-542,0},
    {-373,-158,-512,0},
    {-386,-160,-479,0},
    {-401,-161,-441,0},
    {-412,-163,-402,0},
    {-425,-163,-374,0},
    {-441,-164,-332,0},
    {-455,-163,-295,0},
    {-468,-163,-264,0},
    {-482,-162,-219,0},
    {-494,-162,-184,0},
    {-502,-162,-149,0},
    {-509,-162,-118,0},
    {-516,-161,-85,0},
    {-524,-160,-64,0},
    {-532,-159,-39,0},
    {-539,-159,-3,0},
    {-545,-158,28,0},
    {-551,-157,56,0},
    {-558,-156,86,0},
    {-562,-156,124,0},
    {-569,-155,152,0},
    {-574,-154,194,0},
    {-582,-153,241,0},
    {-582,-153,262,0},
    {-586,-152,298,0},
    {-590,-152,330,0},
    {-593,-152,351,0},
    {-594,-152,383,0},
    {-595,-152,415,0},
    {-595,-152,442,0},
    {-597,-152,466,0},
    {-597,-152,492,0},
    {-598,-152,530,0},
    {-597,-152,571,0},
    {-596,-152,612,0},
    {-595,-152,646,0},
    {-591,-153,683,0},
    {-591,-153,717,0},
    {-591,-152,752,0},
    {-587,-152,788,0},
    {-583,-153,820,0},
    {-580,-153,858,0},
    {-573,-154,886,0},
    {-567,-154,924,0},
    {-562,-155,952,0},
    {-554,-155,988,0},
    {-547,-156,1022,0},
    {-534,-156,1065,0},
    {-518,-157,1108,0},
    {-510,-157,1147,0},
    {-503,-158,1172,0},
    {-494,-158,1206,0},
    {-486,-157,1242,0},
    {-474,-157,1274,0},
    {-466,-157,1303,0},
    {-448,-156,1363,0},
    {-435,-156,1400,0},
    {-425,-155,1428,0},
    {-414,-154,1464,0},
    {-404,-153,1498,0},
    {-397,-150,1532,0},
    {-383,-149,1565,0},
    {-370,-148,1604,0},
    {-348,-144,1667,0},
    {-329,-142,1714,0},
    {-312,-141,1744,0},
    {-298,-138,1785,0},
    {-286,-137,1812,0},
    {-263,-135,1860,0},
    {-236,-134,1890,0},
    {-207,-134,1927,0},
    {-184,-133,1949,0},
    {-163,-133,1965,0},
    {-146,-133,1985,0},
    {-103,-132,2022,0},
    {-74,-132,2056,0},
    {-52,-132,2078,0},
    {-20,-132,2101,0},
    {6,-132,2118,0},
    {28,-132,2133,0},
    {52,-132,2154,0},
    {72,-132,2169,0},
    {89,-132,2183,0},
    {105,-132,2196,0},
    {127,-132,2215,0},
    {149,-132,2233,0},
    {169,-132,2245,0},
    {196,-132,2262,0},
    {219,-132,2277,0},
    {248,-132,2293,0},
    {284,-132,2311,0},
    {314,-132,2330,0},
    {348,-132,2339,0},
    {376,-132,2346,0},
    {395,-132,2343,0},
    {414,-132,2344,0},
    {434,-132,2342,0},
    {452,-132,2341,0},
    {486,-132,2328,0},
    {518,-132,2322,0},
    {541,-132,2312,0},
    {567,-132,2305,0},
    {590,-132,2298,0},
    {609,-132,2287,0},
    {633,-132,2277,0},
    {658,-132,2267,0},
    {686,-132,2251,0},
    {712,-132,2235,0},
    {731,-132,2227,0},
    {753,-132,2214,0},
    {774,-132,2200,0},
    {799,-132,2181,0},
    {826,-132,2160,0},
    {846,-132,2144,0},
    {867,-132,2122,0},
    {894,-132,2097,0},
    {916,-132,2070,0},
    {936,-132,2043,0},
    {960,-132,2001,0},
    {980,-132,1968,0},
    {995,-132,1936,0},
    {1008,-132,1908,0},
    {1023,-132,1882,0},
    {1048,-132,1850,0},
    {1064,-132,1820,0},
    {1077,-132,1788,0},
    {1096,-133,1757,0},
    {1113,-133,1712,0},
    {1134,-134,1667,0},
    {1150,-135,1623,0},
    {1165,-135,1582,0},
    {1186,-136,1521,0},
    {1202,-136,1480,0},
    {1219,-136,1436,0},
    {1230,-135,1397,0},
    {1249,-135,1361,0},
    {1260,-134,1327,0},
    {1270,-134,1295,0},
    {1279,-133,1258,0},
    {1292,-133,1218,0},
    {1304,-132,1177,0},
    {1305,-132,1162,0},
    {1303,-132,1146,0},
    {1302,-132,1131,0},
    {1296,-132,1115,0},
    {1291,-133,1106,0},
    {1281,-133,1097,0},
    {1275,-134,1085,0},
    {1266,-136,1074,0},
    {1254,-137,1064,0},
    {1242,-138,1060,0},
    {1225,-139,1042,0},
    {1211,-138,1034,0},
    {1196,-137,1025,0},
    {1178,-134,1018,0},
    {1165,-132,1014,0},
    {1144,-130,1009,0},
    {1124,-127,993,0},
    {1105,-126,986,0},
    {1084,-125,981,0},
    {1059,-124,980,0},
    {1039,-122,972,0},
    {1016,-121,974,0},
    {988,-120,979,0},
    {964,-120,983,0},
    {941,-121,984,0},
    {914,-124,988,0},
    {892,-124,996,0},
    {865,-123,1000,0},
    {848,-122,1004,0},
    {829,-120,1015,0},
    {811,-119,1028,0},
    {785,-115,1036,0},
    {765,-113,1044,0},
    {747,-113,1064,0},
    {724,-114,1080,0},
    {707,-115,1098,0},
    {682,-116,1109,0},
    {663,-117,1131,0},
    {645,-117,1150,0},
    {625,-117,1172,0},
    {606,-118,1183,0},
    {573,-120,1200,0},
    {554,-121,1222,0},
    {532,-121,1238,0},
    {508,-121,1258,0},
    {482,-121,1270,0},
    {461,-121,1286,0},
    {435,-121,1299,0},
    {413,-121,1307,0},
    {389,-121,1313,0},
    {369,-121,1316,0},
    {348,-121,1322,0},
    {330,-121,1325,0},
    {309,-121,1323,0},
    {284,-121,1322,0},
    {258,-121,1323,0},
    {235,-121,1313,0},
    {216,-121,1301,0},
    {186,-121,1289,0},
    {170,-121,1284,0},
    {155,-121,1271,0},
    {141,-121,1255,0},
    {126,-121,1241,0},
    {118,-121,1232,0},
    {109,-121,1217,0},
    {101,-121,1205,0},
    {94,-121,1194,0},
    {86,-121,1180,0},
    {78,-121,1164,0},
    {71,-121,1144,0},
    {63,-121,1127,0},
    {57,-121,1111,0},
    {49,-121,1088,0},
    {44,-121,1074,0},
    {38,-121,1056,0},
    {34,-121,1032,0},
    {29,-121,1010,0},
    {26,-121,986,0},
    {21,-121,968,0},
    {14,-121,940,0},
    {8,-121,918,0},
    {8,-121,893,0},
    {4,-121,872,0},
    {3,-121,851,0},
    {1,-121,829,0},
    {0,-121,803,0},
    {-1,-121,788,0},
    {-5,-121,749,0},
    {-7,-121,713,0},
    {-11,-121,678,0},
    {-8,-121,642,0},
    {-8,-121,608,0},
    {-7,-121,577,0},
    {-8,-121,556,0},
    {-11,-121,515,0},
    {-9,-121,484,0},
    {-9,-121,453,0},
    {-8,-121,416,0},
    {-6,-121,394,0},
    {-3,-121,345,0},
    {-2,-121,305,0},
    {-1,-121,251,0},
    {-1,-121,214,0},
    {-2,-121,189,0},
    {-1,-121,156,0},
    {1,-121,124,0},
    {4,-121,86,0},
    {6,-121,66,0},
    {8,-121,36,0},
    {13,-121,0,0},
    {15,-121,-14,0},
    {-32768, -32768, -32768, 0}
};

Harbour::Harbour() {
    Props.Minimap.Texture = minimap_mario_raceway;
    Props.Minimap.Width = ResourceGetTexWidthByName(Props.Minimap.Texture);
    Props.Minimap.Height = ResourceGetTexHeightByName(Props.Minimap.Texture);
    Props.Minimap.Pos[0].X = 260;
    Props.Minimap.Pos[0].Y = 170;
    Props.Minimap.PlayerX = 6;
    Props.Minimap.PlayerY = 28;
    Props.Minimap.PlayerScaleFactor = 0.022f;
    ResizeMinimap(&Props.Minimap);

    ResourceName = "mk:harbour";
    Props.SetText(Props.Name, "Harbour", sizeof(Props.Name));
    Props.SetText(Props.DebugName, "harbour", sizeof(Props.DebugName));
    Props.SetText(Props.TrackLength, "99m", sizeof(Props.TrackLength));

    Props.AIBehaviour = D_0D008F28;
    Props.AIMaximumSeparation = 50.0f;
    Props.AIMinimumSeparation = 0.3f;
    Props.AISteeringSensitivity = 48;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    Props.PathSizes = {459, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

    Props.CurveTargetSpeed[0] = 4.1666665f;
    Props.CurveTargetSpeed[1] = 5.5833334f;
    Props.CurveTargetSpeed[2] = 6.1666665f;
    Props.CurveTargetSpeed[3] = 6.75f;

    Props.NormalTargetSpeed[0] = 3.75f;
    Props.NormalTargetSpeed[1] = 5.1666665f;
    Props.NormalTargetSpeed[2] = 5.75f;
    Props.NormalTargetSpeed[3] = 6.3333334f;

    Props.D_0D0096B8[0] = 3.3333332f;
    Props.D_0D0096B8[1] = 3.9166667f;
    Props.D_0D0096B8[2] = 4.5f;
    Props.D_0D0096B8[3] = 5.0833334f;

    Props.OffTrackTargetSpeed[0] = 3.75f;
    Props.OffTrackTargetSpeed[1] = 5.1666665f;
    Props.OffTrackTargetSpeed[2] = 5.75f;
    Props.OffTrackTargetSpeed[3] = 6.3333334f;

    Props.PathTable[0] = harbour_path;
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = harbour_path;
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.CloudTexture = (u8*) gTextureExhaust5;
    Props.Clouds = gKalimariDesertClouds;
    Props.CloudList = gLuigiRacewayClouds;
    Props.Minimap.FinishlineX = 0;
    Props.Minimap.FinishlineY = 0;

    Props.Skybox.TopRight = {120, 140, 188};
    Props.Skybox.BottomRight = {216, 232, 248};
    Props.Skybox.BottomLeft = {216, 232, 248};
    Props.Skybox.TopLeft = {40, 184, 208};
    Props.Skybox.FloorTopRight = {0, 0, 0};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {0, 0, 0};
    Props.Sequence = MusicSeq::MUSIC_SEQ_WARIO_STADIUM;
}

TrackSections harbour_surfaces[] = {
    { (uintptr_t) road_map_001_mesh, 1,  255, 0x0000 },
    { (uintptr_t) ground_map_mesh, 8,  255, 0x0000 },
    { 0x00000000, 0, 0, 0x00000 },
};

void Harbour::Load() {
    Track::Load(road_map_001_mesh_vtx_0, NULL);

    if (gIsMirrorMode != 0) {
        InvertTriangleWinding(ground_map_mesh);
        InvertTriangleWinding(road_map_001_mesh);
        InvertTriangleWinding(bush_map_004_mesh);
        InvertTriangleWinding(castle_map_002_mesh);
        InvertTriangleWinding(statue_map_005_mesh);
        InvertTriangleWinding(trees_map_003_mesh);
        InvertTriangleWinding(water_water1_mesh);
        InvertTriangleWinding(moon_moon_mesh);
    }

    // The light gets overridden in hm_intro, reset to normal
    ground_f3d_material_013_lights = gdSPDefLights1(
        0x7F, 0x7F, 0x7F,
        0xFF, 0xFF, 0xFF, 0x49, 0x49, 0x49
    );

    generate_collision_mesh_with_default_section_id(ground_map_mesh, 8);
    generate_collision_mesh_with_default_section_id(road_map_001_mesh, 1);
    generate_collision_mesh_with_defaults(castle_map_002_mesh);
    generate_collision_mesh_with_defaults(trees_map_003_mesh);
    generate_collision_mesh_with_defaults(bush_map_004_mesh);
    generate_collision_mesh_with_defaults(statue_map_005_mesh);

    parse_track_displaylists((TrackSections*)harbour_surfaces);
    func_80295C6C();
    Props.WaterLevel = gTrackMinY - 10.0f;
}

void Harbour::BeginPlay() {
    struct ActorSpawnData itemboxes[] = {
        {   200, 1500, 200 , 0},
        {   350, 2500, 300 , 1},
        {   400, 2000, 350 , 2},
        {    40, 0, -800, 0},
        {    -40, 0, -800, 0},
        {    0, 0, -800, 0},
        {    999, 6, 482, 0},
        {    1064, 8, 275, {0}},
        {   1028, 5, -39 , {0}},
        {    320, 0, 1020, {0}},
        {   293, 0, 950, {0}},
        {{ -32768, 0,    0 }, {0}},
    };

    struct ActorSpawnData rocks[] = {
        {{   200, 1500, 200 }, {0}},
        {{   350, 2500, 300 }, {1}},
        {{   400, 2000, 350 }, {2}},
        {{ -32768,   0,   0 }, {0}},
    };

    //spawn_all_item_boxes(itemboxes);

    Vec3f test = {-100, 0, -150};
    Vec3s rot = {0, 0, 0};
    Vec3f vel = {0, 0, 0};

    //add_actor_to_empty_slot(test, rot, vel, ACTOR_TREE_MARIO_RACEWAY);


    Vec3f pos = {0, 80, 0};
    // GetWorld()->AddActor(new ACloud(pos));

    // GetWorld()->AddActor(new OSeagull(0, pos));
    // GetWorld()->AddActor(new OSeagull(1, pos));
    // GetWorld()->AddActor(new OSeagull(2, pos));
    // GetWorld()->AddActor(new OSeagull(3, pos));
    // GetWorld()->AddObject(new OCheepCheep(FVector(0, 40, 0), OCheepCheep::CheepType::RACE, IPathSpan(0, 10)));
    // GetWorld()->AddObject(new OTrophy(FVector(0,0,0), OTrophy::TrophyType::GOLD, OTrophy::Behaviour::GO_FISH));
    //GetWorld()->AddObject(new OSnowman(FVector(0, 0, 0)));
    //GetWorld()->AddObject(new OTrashBin(FVector(0.0f, 0.0f, 0.0f), IRotator(0, 90, 0), 1.0f, OTrashBin::Behaviour::MUNCHING));

    //GetWorld()->AddObject(new OHedgehog(FVector(0, 0, 0), FVector2D(0, -200), 9));
    //GetWorld()->AddObject(new OFlagpole(FVector(0, 0, -200), 0x400));
//    GetWorld()->AddObject(new OHotAirBalloon(FVector(0.0, 20.0f, -200.0f)));

    //GetWorld()->AddObject(new OCrab(FVector2D(0, 0), FVector2D(0, -200)));
//    GetWorld()->AddActor(new ABowserStatue(FVector(-200, 0, 0), ABowserStatue::Behaviour::CRUSH));

//    GetWorld()->AddObject(new OBoos(10, IPathSpan(0, 5), IPathSpan(18, 23), IPathSpan(25, 50)));

    //GetWorld()->AddActor(new AShip(FVector(-1694, -111, 1451), AShip::Skin::GHOSTSHIP));
    //GetWorld()->AddActor(new AShip(FVector(2811, -83, 966), AShip::Skin::SHIP2));                                                                                                                                                
}

void Harbour::WhatDoesThisDo(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x19B) &&
        ((s16) gNearestPathPointByPlayerId[playerId] < 0x1B9)) {
        if (D_80165300[playerId] != 1) {
            func_800CA288(playerId, 0x55);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA2B8(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

void Harbour::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (((s16) gNearestPathPointByPlayerId[playerId] >= 0x19B) &&
        ((s16) gNearestPathPointByPlayerId[playerId] < 0x1B9)) {
        if (D_80165300[playerId] != 1) {
            func_800CA2E4(playerId, 0x55);
        }
        D_80165300[playerId] = 1;
    } else {
        if (D_80165300[playerId] != 0) {
            func_800CA30C(playerId);
            D_80165300[playerId] = 0;
        }
    }
}

void Harbour::Draw(ScreenContext* arg0) {
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    set_track_light_direction(D_800DC610, D_802B87D4, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_big_donut_packed_dl_DE8
    }
    gSPDisplayList(gDisplayListHead++, ground_map_mesh);
    gSPDisplayList(gDisplayListHead++, road_map_001_mesh);
    gSPDisplayList(gDisplayListHead++, bush_map_004_mesh);
    gSPDisplayList(gDisplayListHead++, castle_map_002_mesh);
    gSPDisplayList(gDisplayListHead++, statue_map_005_mesh);
    gSPDisplayList(gDisplayListHead++, trees_map_003_mesh);
    gSPDisplayList(gDisplayListHead++, water_water1_mesh);
    gSPDisplayList(gDisplayListHead++, moon_moon_mesh);
}

bool Harbour::IsMod() {
    return true;
}