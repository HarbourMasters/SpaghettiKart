#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>
#include <memory>

#include "Prison.h"
//#include "GameObject.h"
#include "World.h"
#include "engine/actors/AFinishline.h"
//#include "engine/vehicles/OBombKart.h"
#include "assets/mario_raceway_data.h"
#include "assets/bowsers_castle_data.h"
#include "assets/bowsers_castle_displaylists.h"
#include "engine/actors/ATree.h"
#include "engine/actors/ACloud.h"
#include "engine/vehicles/Train.h"
//#include "src/engine/World.h"

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
    #include "assets/common_data.h"
    #include "save.h"
    #include "staff_ghosts.h"
    #include "actors.h"
    #include "collision.h"
    #include "memory.h"
    #include "courses/Prison/grass.h"
    #include "courses/Prison/track.h"
    typedef struct {
        Gfx* addr;
        u8 surfaceType;
        u8 sectionId;
        u16 flags;
    } TrackSections;
}


TrackWaypoint prison_path[] = {
    {0, 0, 0, 0},
    {-7, 0, -38, 0},
    {-13, 0, -71, 0},
    {-18, 0, -101, 0},
    {-23, 0, -128, 0},
    {-28, 0, -153, 0},
    {-31, 0, -175, 0},
    {-35, 0, -195, 0},
    {-38, 0, -212, 0},
    {-41, 0, -228, 0},
    {-43, 0, -243, 0},
    {-46, 0, -256, 0},
    {-48, 0, -267, 0},
    {-50, 0, -278, 0},
    {-51, 0, -287, 0},
    {-66, 0, -372, 0},
    {-74, 0, -413, 0},
    {-81, 0, -449, 0},
    {-87, 0, -481, 0},
    {-93, 0, -510, 0},
    {-98, 0, -537, 0},
    {-102, 0, -560, 0},
    {-106, 0, -582, 0},
    {-110, 0, -601, 0},
    {-113, 0, -618, 0},
    {-116, 0, -634, 0},
    {-119, 0, -648, 0},
    {-121, 0, -660, 0},
    {-123, 0, -672, 0},
    {-125, 0, -682, 0},
    {-142, 0, -773, 0},
    {-142, 0, -812, 0},
    {-142, 0, -847, 0},
    {-141, 0, -878, 0},
    {-141, 0, -906, 0},
    {-141, 0, -931, 0},
    {-141, 0, -954, 0},
    {-140, 0, -974, 0},
    {-140, 0, -993, 0},
    {-140, 0, -1009, 0},
    {-140, 0, -1024, 0},
    {-140, 0, -1038, 0},
    {-140, 0, -1050, 0},
    {-140, 0, -1061, 0},
    {-139, 0, -1070, 0},
    {-138, 0, -1158, 0},
    {-112, 0, -1177, 0},
    {-88, 0, -1194, 0},
    {-67, 0, -1209, 0},
    {-48, 0, -1223, 0},
    {-30, 0, -1235, 0},
    {-15, 0, -1247, 0},
    {-1, 0, -1257, 0},
    {12, 0, -1266, 0},
    {23, 0, -1274, 0},
    {33, 0, -1281, 0},
    {43, 0, -1288, 0},
    {51, 0, -1293, 0},
    {58, 0, -1299, 0},
    {65, 0, -1303, 0},
    {126, 0, -1346, 0},
    {180, 0, -1346, 0},
    {228, 0, -1346, 0},
    {272, 0, -1346, 0},
    {311, 0, -1345, 0},
    {347, 0, -1345, 0},
    {379, 0, -1345, 0},
    {407, 0, -1345, 0},
    {433, 0, -1345, 0},
    {456, 0, -1345, 0},
    {477, 0, -1345, 0},
    {496, 0, -1344, 0},
    {513, 0, -1344, 0},
    {528, 0, -1344, 0},
    {542, 0, -1344, 0},
    {666, 0, -1343, 0},
    {739, 0, -1343, 0},
    {806, 0, -1342, 0},
    {865, 0, -1341, 0},
    {919, 0, -1340, 0},
    {967, 0, -1340, 0},
    {1011, 0, -1339, 0},
    {1050, 0, -1339, 0},
    {1085, 0, -1338, 0},
    {1117, 0, -1338, 0},
    {1146, 0, -1338, 0},
    {1171, 0, -1337, 0},
    {1194, 0, -1337, 0},
    {1215, 0, -1337, 0},
    {1234, 0, -1337, 0},
    {1403, 0, -1334, 0},
    {1446, 0, -1332, 0},
    {1486, 0, -1330, 0},
    {1521, 0, -1329, 0},
    {1553, 0, -1327, 0},
    {1581, 0, -1325, 0},
    {1607, 0, -1324, 0},
    {1630, 0, -1323, 0},
    {1651, 0, -1322, 0},
    {1670, 0, -1321, 0},
    {1687, 0, -1320, 0},
    {1702, 0, -1319, 0},
    {1716, 0, -1319, 0},
    {1728, 0, -1318, 0},
    {1740, 0, -1318, 0},
    {1840, 0, -1312, 0},
    {1841, 0, -1281, 0},
    {1842, 0, -1253, 0},
    {1843, 0, -1227, 0},
    {1844, 0, -1205, 0},
    {1845, 0, -1184, 0},
    {1846, 0, -1165, 0},
    {1847, 0, -1149, 0},
    {1847, 0, -1134, 0},
    {1848, 0, -1120, 0},
    {1849, 0, -1108, 0},
    {1849, 0, -1097, 0},
    {1850, 0, -1087, 0},
    {1850, 0, -1078, 0},
    {1850, 0, -1070, 0},
    {1854, 0, -998, 0},
    {1820, 0, -971, 0},
    {1789, 0, -946, 0},
    {1762, 0, -924, 0},
    {1738, 0, -904, 0},
    {1715, 0, -886, 0},
    {1696, 0, -870, 0},
    {1678, 0, -856, 0},
    {1662, 0, -842, 0},
    {1647, 0, -831, 0},
    {1634, 0, -820, 0},
    {1622, 0, -810, 0},
    {1612, 0, -802, 0},
    {1602, 0, -794, 0},
    {1594, 0, -787, 0},
    {1517, 0, -724, 0},
    {1477, 0, -723, 0},
    {1442, 0, -721, 0},
    {1411, 0, -719, 0},
    {1382, 0, -718, 0},
    {1356, 0, -717, 0},
    {1333, 0, -716, 0},
    {1313, 0, -715, 0},
    {1294, 0, -714, 0},
    {1277, 0, -713, 0},
    {1262, 0, -712, 0},
    {1248, 0, -711, 0},
    {1236, 0, -711, 0},
    {1225, 0, -710, 0},
    {1215, 0, -710, 0},
    {1126, 0, -705, 0},
    {1117, 0, -679, 0},
    {1110, 0, -655, 0},
    {1103, 0, -633, 0},
    {1097, 0, -614, 0},
    {1092, 0, -597, 0},
    {1087, 0, -581, 0},
    {1082, 0, -567, 0},
    {1078, 0, -554, 0},
    {1075, 0, -543, 0},
    {1071, 0, -532, 0},
    {1069, 0, -523, 0},
    {1066, 0, -515, 0},
    {1064, 0, -507, 0},
    {1061, 0, -500, 0},
    {1043, 0, -439, 0},
    {1059, 0, -408, 0},
    {1074, 0, -381, 0},
    {1087, 0, -355, 0},
    {1099, 0, -333, 0},
    {1110, 0, -313, 0},
    {1120, 0, -294, 0},
    {1129, 0, -278, 0},
    {1136, 0, -263, 0},
    {1144, 0, -250, 0},
    {1150, 0, -238, 0},
    {1156, 0, -227, 0},
    {1161, 0, -217, 0},
    {1166, 0, -208, 0},
    {1170, 0, -200, 0},
    {1208, 0, -129, 0},
    {1238, 0, -128, 0},
    {1265, 0, -127, 0},
    {1289, 0, -126, 0},
    {1311, 0, -125, 0},
    {1331, 0, -124, 0},
    {1349, 0, -123, 0},
    {1365, 0, -123, 0},
    {1379, 0, -122, 0},
    {1392, 0, -122, 0},
    {1404, 0, -121, 0},
    {1415, 0, -121, 0},
    {1424, 0, -120, 0},
    {1433, 0, -120, 0},
    {1440, 0, -119, 0},
    {1510, 0, -116, 0},
    {1519, 0, -109, 0},
    {1528, 0, -102, 0},
    {1535, 0, -96, 0},
    {1542, 0, -91, 0},
    {1548, 0, -86, 0},
    {1554, 0, -81, 0},
    {1559, 0, -77, 0},
    {1564, 0, -74, 0},
    {1568, 0, -71, 0},
    {1571, 0, -68, 0},
    {1575, 0, -65, 0},
    {1578, 0, -63, 0},
    {1580, 0, -61, 0},
    {1583, 0, -59, 0},
    {1605, 0, -41, 0},
    {1601, 0, -23, 0},
    {1597, 0, -7, 0},
    {1594, 0, 8, 0},
    {1591, 0, 21, 0},
    {1589, 0, 33, 0},
    {1587, 0, 43, 0},
    {1585, 0, 53, 0},
    {1583, 0, 62, 0},
    {1581, 0, 69, 0},
    {1580, 0, 76, 0},
    {1578, 0, 83, 0},
    {1577, 0, 88, 0},
    {1576, 0, 93, 0},
    {1575, 0, 98, 0},
    {1567, 0, 140, 0},
    {1553, 0, 155, 0},
    {1540, 0, 168, 0},
    {1529, 0, 180, 0},
    {1519, 0, 191, 0},
    {1510, 0, 201, 0},
    {1502, 0, 210, 0},
    {1495, 0, 218, 0},
    {1488, 0, 225, 0},
    {1482, 0, 232, 0},
    {1477, 0, 238, 0},
    {1472, 0, 243, 0},
    {1467, 0, 248, 0},
    {1464, 0, 252, 0},
    {1460, 0, 256, 0},
    {1429, 0, 291, 0},
    {1415, 0, 291, 0},
    {1402, 0, 291, 0},
    {1391, 0, 291, 0},
    {1381, 0, 291, 0},
    {1372, 0, 291, 0},
    {1364, 0, 291, 0},
    {1357, 0, 291, 0},
    {1350, 0, 291, 0},
    {1345, 0, 291, 0},
    {1339, 0, 291, 0},
    {1334, 0, 291, 0},
    {1330, 0, 291, 0},
    {1326, 0, 291, 0},
    {1323, 0, 291, 0},
    {1292, 0, 292, 0},
    {1259, 0, 285, 0},
    {1230, 0, 279, 0},
    {1204, 0, 274, 0},
    {1181, 0, 269, 0},
    {1160, 0, 265, 0},
    {1141, 0, 261, 0},
    {1124, 0, 258, 0},
    {1108, 0, 254, 0},
    {1094, 0, 252, 0},
    {1082, 0, 249, 0},
    {1071, 0, 247, 0},
    {1060, 0, 245, 0},
    {1051, 0, 243, 0},
    {1043, 0, 241, 0},
    {970, 0, 227, 0},
    {938, 0, 229, 0},
    {910, 0, 231, 0},
    {884, 0, 232, 0},
    {862, 0, 234, 0},
    {841, 0, 236, 0},
    {822, 0, 237, 0},
    {806, 0, 238, 0},
    {791, 0, 239, 0},
    {777, 0, 240, 0},
    {765, 0, 241, 0},
    {754, 0, 242, 0},
    {744, 0, 242, 0},
    {735, 0, 243, 0},
    {727, 0, 243, 0},
    {656, 0, 249, 0},
    {612, 0, 251, 0},
    {573, 0, 253, 0},
    {538, 0, 255, 0},
    {507, 0, 256, 0},
    {478, 0, 258, 0},
    {453, 0, 259, 0},
    {430, 0, 260, 0},
    {409, 0, 262, 0},
    {390, 0, 263, 0},
    {373, 0, 263, 0},
    {358, 0, 264, 0},
    {345, 0, 265, 0},
    {333, 0, 266, 0},
    {322, 0, 266, 0},
    {223, 0, 272, 0},
    {204, 0, 261, 0},
    {187, 0, 251, 0},
    {172, 0, 242, 0},
    {158, 0, 234, 0},
    {146, 0, 227, 0},
    {135, 0, 220, 0},
    {125, 0, 215, 0},
    {116, 0, 209, 0},
    {108, 0, 205, 0},
    {101, 0, 201, 0},
    {94, 0, 197, 0},
    {88, 0, 193, 0},
    {83, 0, 190, 0},
    {78, 0, 187, 0},
    {36, 0, 163, 0},
    {0, 0, 0},
    {-32768, -32768, -32768, 0}
};

PrisonCourse::PrisonCourse() {
    this->gfxSize = 100;
    this->textures = NULL;
    Props.MinimapTexture = gTextureCourseOutlineMarioRaceway;
    Props.MinimapDimensions = IVector2D(ResourceGetTexWidthByName(Props.MinimapTexture), ResourceGetTexHeightByName(Props.MinimapTexture));

    Props.Id = "mk:prison_course";
    Props.Name = "Prison";
    Props.DebugName = "prison";
    Props.CourseLength = "100m";
    Props.AIBehaviour = D_0D008F28;
    Props.AIMaximumSeparation = 50.0f;
    //Props.AIMaximumSeparation = 2.0f;
    Props.AIMinimumSeparation = 0.3f;
    Props.SomePtr = D_800DCB34;
    Props.AISteeringSensitivity = 48;

    Props.NearPersp = 9.0f;
    Props.FarPersp = 4500.0f;

    //Props.PathSizes = {51, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};
    Props.PathSizes = {6672, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0};

    Props.D_0D009418[0] = 4.1666665f;
    Props.D_0D009418[1] = 5.5833334f;
    Props.D_0D009418[2] = 6.1666665f;
    Props.D_0D009418[3] = 6.75f;

    Props.D_0D009568[0] = 3.75f;
    Props.D_0D009568[1] = 5.1666665f;
    Props.D_0D009568[2] = 5.75f;
    Props.D_0D009568[3] = 6.3333334f;

    Props.D_0D0096B8[0] = 3.3333332f;
    Props.D_0D0096B8[1] = 3.9166667f;
    Props.D_0D0096B8[2] = 4.5f;
    Props.D_0D0096B8[3] = 5.0833334f;

    Props.D_0D009808[0] = 3.75f;
    Props.D_0D009808[1] = 5.1666665f;
    Props.D_0D009808[2] = 5.75f;
    Props.D_0D009808[3] = 6.3333334f;

    Props.PathTable[0] = prison_path;
    Props.PathTable[1] = NULL;
    Props.PathTable[2] = NULL;
    Props.PathTable[3] = NULL;

    Props.PathTable2[0] = prison_path;
    Props.PathTable2[1] = NULL;
    Props.PathTable2[2] = NULL;
    Props.PathTable2[3] = NULL;

    Props.Clouds = NULL;
    Props.CloudList = NULL;
    Props.MinimapFinishlineX = 0;
    Props.MinimapFinishlineY = 0;

    Props.Skybox.TopRight = {48, 8, 120};
    Props.Skybox.BottomRight = {255, 0, 0};
    Props.Skybox.BottomLeft = {255, 0, 0};
    Props.Skybox.TopLeft = {48, 8, 120};
    Props.Skybox.FloorTopRight = {0, 0, 0};
    Props.Skybox.FloorBottomRight = {0, 0, 0};
    Props.Skybox.FloorBottomLeft = {0, 0, 0};
    Props.Skybox.FloorTopLeft = {0, 0, 0};
}

TrackSections prisonSections[] = {
    { PrisonCourse_PrisonCourse_mesh, 255,  255, ASPHALT },
    { PrisonGrass_PrisonGrass_mesh, 255,  255, GRASS },
    { 0x00000000, 0, 0, 0x00000 },
};

void PrisonCourse::Load() {
    Course::Load(PrisonCourse_PrisonCourse_mesh_vtx_0, NULL);

    generate_collision_mesh_with_default_section_id(PrisonCourse_PrisonCourse_mesh, ASPHALT);
    generate_collision_mesh_with_default_section_id(PrisonGrass_PrisonGrass_mesh, GRASS);

    parse_course_displaylists((TrackSectionsI*)prisonSections);
    func_80295C6C();
    D_8015F8E4 = gCourseMinY - 10.0f;
}

void PrisonCourse::LoadTextures() {
    dma_textures(gTextureTrees1, 0x0000035BU, 0x00000800U);
    D_802BA058 = dma_textures(gTexturePiranhaPlant1, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant2, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant3, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant4, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant5, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant6, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant7, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant8, 0x000003E8U, 0x00000800U);
    dma_textures(gTexturePiranhaPlant9, 0x000003E8U, 0x00000800U);
}

void PrisonCourse::BeginPlay() {
    //Finishline spawner
    //ItemBoxes spawner
    struct ActorSpawnData itemboxes[] = {
        {   792,    0, -1315,    0},
        {   792,    0, -1272,    0},
        {   792,    0, -1372,    0},
        {   792,    0, -1419,    0},
        {   -38,    0, -801,     0},
        {     0,    0, -801,     0},
        {  1293,    0,  290,     0},
        {  1293,    0,  267,     0},
        {  1293,    0,  310,     0},
        {  1144,    0, -442,     0},
        {  1111,    0, -442,     0},
        {   960,    0, -442,     0},
        {   928,    0, -442,     0},
        {  -172,    0, -646,     0},
        {  -200,    0, -646,     0},
        {{ -32768,  0,    0 }, {0}},
    };
    spawn_all_item_boxes(itemboxes);
    
    //Tree spawner
    Vec3f tree_pos[] = {
        {-71, 0, 131},
        {194, 0, 147},
        {439, 0, 383},
        {968, 18, 65},
        {1252, 0, 395},
        {1289, 0, 35},
        {1689, 0, -150},
    };

    Vec3s rot = {0, 0, 0};
    Vec3f vel = {0, 0, 0};
    int i = 0;
    for (i = 0; i < ARRAY_COUNT(tree_pos); i++) {
        add_actor_to_empty_slot(tree_pos[i], rot, vel, ACTOR_TREE_MARIO_RACEWAY);
    }

    //Thwomp Spawners
    typedef struct {
        s16 X;
        s16 Z;
        s16 Direction;
        f32 Scale;
        uint16_t Behavior;
        s16 PrimAlpha;
        u16 BoxSize;
    } PrisonThwomp;

    //North Rotation : 0x8000
    //South Rotation : 0x0000
    //East Rotation  : 0x4000 
    //West Rotation  : 0xC000

    PrisonThwomp TWList[] = {
        {-189, -646, 0x0000, 3.0f, 4, 0, 20}, {-21, -805, 0x0000, 3.0f, 4, 0, 20}, //Entrance
        {1128, -433, 0x8000, 3.0f, 4, 1, 10}, {945, -433, 0x8000, 3.0f, 4, 1, 20}, //Blue Room
        {487, -1409, 0xC000, 2.0f, 2, 2, 10}, {1872, -903, 0x4000, 2.0f, 2, 2, 10} //Hallway
    };

    for (i = 0; i < ARRAY_COUNT(TWList); i++) {
        gWorldInstance.AddObject(new OThwomp(TWList[i].X, TWList[i].Z, TWList[i].Direction, TWList[i].Scale, TWList[i].Behavior, TWList[i].PrimAlpha, TWList[i].BoxSize));
    };
}

// Likely sets minimap boundaries
void PrisonCourse::MinimapSettings() {
    D_8018D220 = reinterpret_cast<uint8_t (*)[1024]>(dma_textures(gTextureExhaust5, 0x443, 0x1000));
    D_8018D2A0 = 0.022f;
    D_8018D2E0 = 6;
    D_8018D2E8 = 28;
    D_8018D2C0[0] = 260;
    D_8018D2D8[0] = 170;
    D_80165718 = 0;
    D_80165720 = 5;
    D_80165728 = -240;
}

Path2D prison_course_path2D[] = {
    {    0, 0},
    {    0, -100},
    {    0, -200},
    {    0, -300},
    {    0, -400},
    {    0, -500},
    {    0, -600},
    {    0, -700},
    {    0, -800},
    {    0, -900},
    {    0, -1000},
    {    0, -1096},    // Main point 1
    {  100, -1090},
    {  200, -1085},
    {  300, -1080},
    {  400, -1075},
    {  500, -1072},    // Curve begins to smooth here
    {  600, -1068},
    {  700, -1065},
    {  800, -1063},
    {  900, -1061},
    {  984, -1060},    // Main point 2
    {  990, -900},
    {  995, -800},
    {  997, -700},
    {  998, -600},
    {  999, -500},
    {  999, -400},
    {  999, -300},
    {  999, -200},
    {  999, -100},
    {  999, 0},
    {  999, 100},
    {  999, 200},
    {  999, 300},
    {  999, 400},
    {  999, 500},
    {  999, 600},
    {  999, 700},
    {  999, 800},
    {  999, 900},
    {  999, 940},      // Main point 3
    {  900, 945},
    {  800, 945},
    {  700, 947},
    {  600, 948},
    {  500, 949},
    {  400, 949},
    {  300, 949},
    {  200, 950},
    {  100, 950},
    {    0, 950},      // Main point 4

    // End of path
    { -32768, -32768 } // Terminator
};

// void PrisonCourse::SpawnVehicles() {

// }

// void PrisonCourse::UpdateVehicles() {
//     //update_vehicle_trains();
// }

// void PrisonCourse::UpdateCourseObjects() {
//     func_80081208();
// }

// void PrisonCourse::InitCourseObjects() {

// }

// void PrisonCourse::SomeSounds() {}

void PrisonCourse::WhatDoesThisDo(Player* player, int8_t playerId) {
    if (((s16) gNearestWaypointByPlayerId[playerId] >= 0x19B) &&
        ((s16) gNearestWaypointByPlayerId[playerId] < 0x1B9)) {
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

void PrisonCourse::WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (((s16) gNearestWaypointByPlayerId[playerId] >= 0x19B) &&
        ((s16) gNearestWaypointByPlayerId[playerId] < 0x1B9)) {
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

// Positions the finishline on the minimap
void PrisonCourse::MinimapFinishlinePosition() {
    //! todo: Place hard-coded values here.
    draw_hud_2d_texture_8x8(this->Props.MinimapFinishlineX, this->Props.MinimapFinishlineY, (u8*) common_texture_minimap_finish_line);
}

void PrisonCourse::Render(struct UnkStruct_800DC5EC* arg0) {
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);
    gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
    func_802B5D64(D_800DC610, D_802B87D4, 0, 1);
    gSPTexture(gDisplayListHead++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gSPSetGeometryMode(gDisplayListHead++, G_SHADING_SMOOTH);

    if (func_80290C20(arg0->camera) == 1) {
        gDPSetCombineMode(gDisplayListHead++, G_CC_SHADE, G_CC_SHADE);
        gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
        // d_course_big_donut_packed_dl_DE8
    }
    gSPDisplayList(gDisplayListHead++, PrisonCourse_PrisonCourse_mesh);
    gSPDisplayList(gDisplayListHead++, PrisonGrass_PrisonGrass_mesh);
}

void PrisonCourse::RenderCourseObjects(s32 cameraId) {
    //render_object_thwomps(cameraId);
}

// void PrisonCourse::RenderCredits() {
// }

// void PrisonCourse::Collision() {}

// void PrisonCourse::Destroy() { }

bool PrisonCourse::IsMod() {
    return true;
}