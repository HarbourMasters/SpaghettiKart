#include <libultraship.h>

#include "Course.h"
#include "MarioRaceway.h"

extern "C" {
    #include "main.h"
    #include "memory.h"
    #include "common_structs.h"
    #include "course_offsets.h"
}

Course::Course() {}

void Course::Load(const char* vtx, const char* gfx) {
    gSegmentTable[4] = reinterpret_cast<uintptr_t>(&vtx[0]);
    gSegmentTable[7] = reinterpret_cast<uintptr_t>(&gfx[0]);
}

void Course::Load(const char* courseVtx, course_texture* textures, const char* displaylists, size_t dlSize) {

    size_t vtxSize = (ResourceGetSizeByName(courseVtx) / sizeof(CourseVtx)) * sizeof(Vtx);
    size_t texSegSize;

    // Convert course vtx to vtx
    Vtx* vtx = reinterpret_cast<Vtx*>(allocate_memory(vtxSize));
    gSegmentTable[4] = reinterpret_cast<uintptr_t>(&vtx[0]);
    printf("\nVtxsize: 0x%X\n\n",vtxSize);
    func_802A86A8(reinterpret_cast<CourseVtx*>(LOAD_ASSET_RAW(courseVtx)), vtx, vtxSize / sizeof(Vtx));

    // Load and allocate memory for course textures
    course_texture* asset = textures;
    u8* freeMemory = NULL;
    u8* texture = NULL;
    size_t size = 0;
    texSegSize = 0;
    while (asset->addr) {
        size = ResourceGetTexSizeByName(asset->addr);
        freeMemory = (u8*) allocate_memory(size);

        texture = reinterpret_cast<u8*>(LOAD_ASSET_RAW(asset->addr));
        if (texture) {
            if (asset == &textures[0]) {
                gSegmentTable[5] = reinterpret_cast<uintptr_t>(&freeMemory[0]);
            }
            memcpy(freeMemory, texture, size);
            texSegSize += size;
            // printf("Texture Addr: 0x%llX, size 0x%X\n", &freeMemory[0], size);
        }
        asset++;
    }

    // Extract packed DLs
    u8* packed = reinterpret_cast<u8*>(LOAD_ASSET_RAW(displaylists));
    Gfx* gfx = (Gfx*) allocate_memory(sizeof(Gfx) * dlSize); // Size of unpacked DLs
    assert(gfx != NULL);
    gSegmentTable[7] = reinterpret_cast<uintptr_t>(&gfx[0]);
    displaylist_unpack(reinterpret_cast<uintptr_t *>(gfx), reinterpret_cast<uintptr_t>(packed), 0);
}

void Course::LoadTextures() { }
void Course::Init() {  }
void Course::BeginPlay() {  }
void Course::Render(struct UnkStruct_800DC5EC* arg0) { }
void Course::Collision() {}
void Course::Destroy() { }

Course* currentCourse = nullptr;

extern "C" {
    void CourseManager_LoadCourse(const char* courseVtx, course_texture* textures, const char* displaylists, size_t dlSize) {

    delete currentCourse;

    //switch (0) {
      //  case COURSE_MARIO_RACEWAY:
            currentCourse = new CourseMarioRaceway();
      //      break;

        // case COURSE_CHOCO_MOUNTAIN:
        //     currentCourse = new CourseChocoMountain();
        //     break;

        // case COURSE_BOWSER_CASTLE:
        //     currentCourse = new CourseBowsersCastle();
        //     break;

        // case COURSE_BANSHEE_BOARDWALK:
        //     currentCourse = new CourseBansheeBoardwalk();
        //     break;

        // case COURSE_YOSHI_VALLEY:
        //     currentCourse = new CourseYoshiValley();
        //     break;

        // case COURSE_FRAPPE_SNOWLAND:
        //     currentCourse = new CourseFrappeSnowland();
        //     break;

        // case COURSE_KOOPA_BEACH:
        //     currentCourse = new CourseKoopaBeach();
        //     break;

        // case COURSE_ROYAL_RACEWAY:
        //     currentCourse = new CourseRoyalRaceway();
        //     break;

        // case COURSE_LUIGI_RACEWAY:
        //     currentCourse = new CourseLuigiRaceway();
        //     break;

        // case COURSE_MOO_MOO_FARM:
        //     currentCourse = new CourseMooMooFarm();
        //     break;

        // case COURSE_TOADS_TURNPIKE:
        //     currentCourse = new CourseToadsTurnpike();
        //     break;

        // case COURSE_KALAMARI_DESERT:
        //     currentCourse = new CourseKalamariDesert();
        //     break;

        // case COURSE_SHERBET_LAND:
        //     currentCourse = new CourseSherbetLand();
        //     break;

        // case COURSE_RAINBOW_ROAD:
        //     currentCourse = new CourseRainbowRoad();
        //     break;

        // case COURSE_WARIO_STADIUM:
        //     currentCourse = new CourseWarioStadium();
        //     break;

        // case COURSE_BLOCK_FORT:
        //     currentCourse = new CourseBlockFort();
        //     break;

        // case COURSE_SKYSCRAPER:
        //     currentCourse = new CourseSkyscraper();
        //     break;

        // case COURSE_DOUBLE_DECK:
        //     currentCourse = new CourseDoubleDeck();
        //     break;

        // case COURSE_DK_JUNGLE:
        //     currentCourse = new CourseDKJungle();
        //     break;

        // case COURSE_BIG_DONUT:
        //     currentCourse = new CourseBigDonut();
        //     break;

      //  default:
            // Handle invalid course ID
   //         return;
   // }

        currentCourse->Load(courseVtx, textures, displaylists, dlSize);
    }

    void CourseManager_RenderCourse(struct UnkStruct_800DC5EC* arg0) {
        if (currentCourse) {
            currentCourse->Render(arg0);
        }
    }
}