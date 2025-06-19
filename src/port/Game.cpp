#include <libultraship.h>

#include "Game.h"
#include "port/Engine.h"

#include <graphic/Fast3D/Fast3dWindow.h>
#include "engine/World.h"
#include "engine/courses/Course.h"
#include "engine/courses/MarioRaceway.h"
#include "engine/courses/ChocoMountain.h"
#include "engine/courses/BowsersCastle.h"
#include "engine/courses/BansheeBoardwalk.h"
#include "engine/courses/YoshiValley.h"
#include "engine/courses/FrappeSnowland.h"
#include "engine/courses/KoopaTroopaBeach.h"
#include "engine/courses/RoyalRaceway.h"
#include "engine/courses/LuigiRaceway.h"
#include "engine/courses/MooMooFarm.h"
#include "engine/courses/ToadsTurnpike.h"
#include "engine/courses/KalimariDesert.h"
#include "engine/courses/SherbetLand.h"
#include "engine/courses/RainbowRoad.h"
#include "engine/courses/WarioStadium.h"
#include "engine/courses/BlockFort.h"
#include "engine/courses/Skyscraper.h"
#include "engine/courses/DoubleDeck.h"
#include "engine/courses/DKJungle.h"
#include "engine/courses/BigDonut.h"
#include "engine/courses/Harbour.h"
#include "engine/courses/TestCourse.h"
#include "engine/actors/Finishline.h"

#include "engine/courses/PodiumCeremony.h"

#include "engine/ModelLoader.h"
#include "engine/actors/BowserStatue.h"

#include "engine/GarbageCollector.h"

#include "engine/TrainCrossing.h"
#include "engine/objects/BombKart.h"
#include "engine/objects/Lakitu.h"

#include "Smoke.h"

#include "engine/HM_Intro.h"

#include "engine/editor/Editor.h"
#include "engine/editor/EditorMath.h"
#include "engine/editor/SceneManager.h"

extern "C" {
#include "main.h"
#include "audio/load.h"
#include "audio/external.h"
#include "networking/networking.h"
#include "render_courses.h"
#include "menus.h"
#include "update_objects.h"
// #include "engine/wasm.h"
}

extern "C" void Graphics_PushFrame(Gfx* data) {
    GameEngine::ProcessGfxCommands(data);
}

extern "C" void Timer_Update();

// Create the world instance
World gWorldInstance;

std::unique_ptr<PodiumCeremony> gPodiumCeremony;

Cup* gMushroomCup;
Cup* gFlowerCup;
Cup* gStarCup;
Cup* gSpecialCup;
Cup* gBattleCup;

ModelLoader gModelLoader;

HarbourMastersIntro gMenuIntro;

Editor::Editor gEditor;

s32 gTrophyIndex = NULL;

void CustomEngineInit() {
    /* Add all courses to the global course list */
    Course* mario         = gWorldInstance.AddCourse(std::make_unique<MarioRaceway>());
    Course* choco         = gWorldInstance.AddCourse(std::make_unique<ChocoMountain>());
    Course* bowser        = gWorldInstance.AddCourse(std::make_unique<BowsersCastle>());
    Course* banshee       = gWorldInstance.AddCourse(std::make_unique<BansheeBoardwalk>());
    Course* yoshi         = gWorldInstance.AddCourse(std::make_unique<YoshiValley>());
    Course* frappe        = gWorldInstance.AddCourse(std::make_unique<FrappeSnowland>());
    Course* koopa         = gWorldInstance.AddCourse(std::make_unique<KoopaTroopaBeach>());
    Course* royal         = gWorldInstance.AddCourse(std::make_unique<RoyalRaceway>());
    Course* luigi         = gWorldInstance.AddCourse(std::make_unique<LuigiRaceway>());
    Course* mooMoo        = gWorldInstance.AddCourse(std::make_unique<MooMooFarm>());
    Course* toads         = gWorldInstance.AddCourse(std::make_unique<ToadsTurnpike>());
    Course* kalimari      = gWorldInstance.AddCourse(std::make_unique<KalimariDesert>());
    Course* sherbet       = gWorldInstance.AddCourse(std::make_unique<SherbetLand>());
    Course* rainbow       = gWorldInstance.AddCourse(std::make_unique<RainbowRoad>());
    Course* wario         = gWorldInstance.AddCourse(std::make_unique<WarioStadium>());
    Course* block         = gWorldInstance.AddCourse(std::make_unique<BlockFort>());
    Course* skyscraper    = gWorldInstance.AddCourse(std::make_unique<Skyscraper>());
    Course* doubleDeck    = gWorldInstance.AddCourse(std::make_unique<DoubleDeck>());
    Course* dkJungle      = gWorldInstance.AddCourse(std::make_unique<DKJungle>());
    Course* bigDonut      = gWorldInstance.AddCourse(std::make_unique<BigDonut>());
//    Course* harbour       = gWorldInstance.AddCourse(std::make_unique<Harbour>());
    Course* testCourse    = gWorldInstance.AddCourse(std::make_unique<TestCourse>());

    gPodiumCeremony = std::make_unique<PodiumCeremony>();

    // Construct cups with vectors of Course* (non-owning references)
    gMushroomCup = new Cup("mk:mushroom_cup", "Mushroom Cup", {
        luigi, mooMoo, koopa, kalimari
    });

    gFlowerCup = new Cup("mk:flower_cup", "Flower Cup", {
        toads, frappe, choco, mario
    });

    gStarCup = new Cup("mk:star_cup", "Star Cup", {
        wario, sherbet, royal, bowser
    });

    gSpecialCup = new Cup("mk:special_cup", "Special Cup", {
        dkJungle, yoshi, banshee, rainbow
    });

    gBattleCup = new Cup("mk:battle_cup", "Battle Cup", {
        bigDonut, block, doubleDeck, skyscraper
    });

    /* Instantiate Cups */
    gWorldInstance.AddCup(gMushroomCup);
    gWorldInstance.AddCup(gFlowerCup);
    gWorldInstance.AddCup(gStarCup);
    gWorldInstance.AddCup(gSpecialCup);
    gWorldInstance.AddCup(gBattleCup);

    /* Set default course; mario raceway */
    //SelectMarioRaceway(); // This results in a nullptr
    gWorldInstance.CurrentCourse = mario;
    gWorldInstance.CurrentCup = gMushroomCup;
    gWorldInstance.CurrentCup->CursorPosition = 3;
    gWorldInstance.CupIndex = 0;

    // ModelLoader::LoadModelList bowserStatueList = {
    //     .course = gBowsersCastle,
    //     .gfxBuffer = &gBowserStatueGfx[0],
    //     .gfxBufferSize = 162,
    //     .gfxStart = (0x2BB8 / 8), // 0x2BB8 / sizeof(OldGfx)
    //     .vtxBuffer = &gBowserStatueVtx[0],
    //     .vtxBufferSize = 717,
    //     .vtxStart = 1942,
    // };

    // Model loader systems allows cutting pieces out of courses and making them actors.
    // Commented out due to alleged stability issues.
    // gModelLoader.Add(bowserStatueList);

    // gModelLoader.Load();
}

void CustomEngineDestroy() {
    delete gMushroomCup;
    delete gFlowerCup;
    delete gStarCup;
    delete gSpecialCup;
    delete gBattleCup;
}

extern "C" {

void HM_InitIntro() {
    gMenuIntro.HM_InitIntro();
}

void HM_TickIntro() {
    gMenuIntro.HM_TickIntro();
}

void HM_DrawIntro() {
    gMenuIntro.HM_DrawIntro();
}

void CM_SpawnFromLevelProps() {
    // Spawning actors needs to be delayed to the correct time.
    // And loadlevel needs to happen asap

    //Editor::LoadLevel(nullptr);
   // Editor::SpawnFromLevelProps();
}

World* GetWorld(void) {
    return &gWorldInstance;
}

u32 WorldNextCup(void) {
    return gWorldInstance.NextCup();
}

u32 WorldPreviousCup(void) {
    return gWorldInstance.PreviousCup();
}

void CM_SetCup(void* cup) {
    gWorldInstance.SetCup((Cup*) cup);
}

void* GetCup() {
    return gWorldInstance.CurrentCup;
}

u32 GetCupIndex(void) {
    return gWorldInstance.GetCupIndex();
}

void CM_SetCupIndex(size_t index) {
    gWorldInstance.SetCupIndex(index);
}

const char* GetCupName(void) {
    return gWorldInstance.CurrentCup->Name;
}

void LoadCourse() {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->Load();
    }
}

size_t GetCourseIndex() {
    return gWorldInstance.CourseIndex;
}

void SetCourse(const char* name) {
    gWorldInstance.SetCourse(name);
}

void NextCourse() {
    gWorldInstance.NextCourse();
}

void PreviousCourse() {
    gWorldInstance.PreviousCourse();
}

void SetCourseById(s32 course) {
    if (course < 0 || course >= gWorldInstance.Courses.size()) {
        return;
    }
    gWorldInstance.CourseIndex = course;
    gWorldInstance.CurrentCourse = gWorldInstance.Courses[gWorldInstance.CourseIndex].get();
}

void CM_VehicleCollision(s32 playerId, Player* player) {
    for (auto& actor : gWorldInstance.Actors) {
        if (actor) {
            actor->VehicleCollision(playerId, player);
        }
    }
}

void CM_BombKartsWaypoint(s32 cameraId) {
    for (auto& object : gWorldInstance.Objects) {
        if (auto kart = dynamic_cast<OBombKart*>(object)) {
            if (kart) {
                kart->Waypoint(cameraId);
            }
        }
    }
}

void CM_DisplayBattleBombKart(s32 playerId, s32 primAlpha) {

    if ((playerId < 0) || (playerId > 4)) {
        return;
    }

    if (primAlpha == 0) {
        gWorldInstance.playerBombKart[playerId].state = PlayerBombKart::PlayerBombKartState::DISABLED;
        gWorldInstance.playerBombKart[playerId]._primAlpha = primAlpha;
    } else {
        gWorldInstance.playerBombKart[playerId].state = PlayerBombKart::PlayerBombKartState::ACTIVE;
        gWorldInstance.playerBombKart[playerId]._primAlpha = primAlpha;
    }
}

void CM_DrawBattleBombKarts(s32 cameraId) {
    for (size_t i = 0; i < gPlayerCount; i++) {
        gWorldInstance.playerBombKart[i].Draw(i, cameraId);
    }
}

void CM_ClearVehicles(void) {
    gWorldInstance.Crossings.clear();
}

void CM_CrossingTrigger() {
    for (auto& crossing : gWorldInstance.Crossings) {
        if (crossing) {
            crossing->CrossingTrigger();
        }
    }
}

void CM_AICrossingBehaviour(s32 playerId) {
    for (auto& crossing : gWorldInstance.Crossings) {
        if (crossing) {
            crossing->AICrossingBehaviour(playerId);
        }
    }
}

s32 CM_GetCrossingOnTriggered(uintptr_t* crossing) {
    TrainCrossing* ptr = (TrainCrossing*) crossing;
    if (ptr) {
        return ptr->OnTriggered;
    }
}

void CM_LoadTextures() {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->LoadTextures();
    }
}

void CM_RenderCourse(struct UnkStruct_800DC5EC* arg0) {
    if (gWorldInstance.CurrentCourse->IsMod() == false) {
        if ((CVarGetInteger("gFreecam", 0) == true)) {
            // Render credits courses
            //gSPClearGeometryMode(gDisplayListHead++, G_LIGHTING);
            //gSPSetGeometryMode(gDisplayListHead++, G_SHADE | G_CULL_BACK | G_SHADING_SMOOTH);
            render_credits();
            return;
        }
    }

    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->Render(arg0);
    }
}

void CM_RenderCredits() {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->RenderCredits();
    }
}

void CM_TickActors() {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.TickActors();
    }
}

void CM_DrawActors(Camera* camera, struct Actor* actor) {
    AActor* a = gWorldInstance.ConvertActorToAActor(actor);
    if (a->IsMod()) {
        a->Draw(camera);
    }
}

void CM_DrawStaticMeshActors() {
    gWorldInstance.DrawStaticMeshActors();
}

void CM_BeginPlay() {
    Course* course = gWorldInstance.CurrentCourse;

    if (course) {
        // Do not spawn finishline in credits or battle mode. And if bSpawnFinishline.
        if ((gGamestate != CREDITS_SEQUENCE) && (gModeSelection != BATTLE)) {
            if (course->bSpawnFinishline) {
                gWorldInstance.AddActor(new AFinishline(course->FinishlineSpawnPoint));
            }
        }
        gEditor.AddLight("Sun", nullptr, D_800DC610[1].l->l.dir);

        course->BeginPlay();
    }
}

void CM_TickObjects() {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.TickObjects();
    }
}

// A couple objects such as lakitu are ticked inside of process_game_tick which support 60fps.
// This is a fallback to support that.
void CM_TickObjects60fps() {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.TickObjects60fps();
    }
}

void CM_DrawObjects(s32 cameraId) {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.DrawObjects(cameraId);
    }
}

void CM_TickEditor() {
    gEditor.Tick();
}

void CM_DrawEditor() {
    gEditor.Draw();
}

void CM_Editor_SetLevelDimensions(s16 minX, s16 maxX, s16 minZ, s16 maxZ, s16 minY, s16 maxY) {
    gEditor.SetLevelDimensions(minX, maxX, minZ, maxZ, minY, maxY);
}

void CM_TickParticles() {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.TickParticles();
    }
}

void CM_DrawParticles(s32 cameraId) {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.DrawParticles(cameraId);
    }
}

// Helps prevents users from forgetting to add a finishline to their course
bool CM_DoesFinishlineExist() {
    for (AActor* actor : gWorldInstance.Actors) {
        if (dynamic_cast<AFinishline*>(actor)) {
            return true;
        }
    }
    return false;
}

void CM_InitClouds() {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->InitClouds();
    }
}

void CM_UpdateClouds(s32 arg0, Camera* camera) {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->UpdateClouds(arg0, camera);
    }
}

void CM_Waypoints(Player* player, int8_t playerId) {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->Waypoints(player, playerId);
    }
}

void CM_SomeCollisionThing(Player* player, Vec3f arg1, Vec3f arg2, Vec3f arg3, f32* arg4, f32* arg5, f32* arg6,
                           f32* arg7) {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->SomeCollisionThing(player, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    }
}

void CM_InitCourseObjects() {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->InitCourseObjects();
    }
}

void CM_UpdateCourseObjects() {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->UpdateCourseObjects();
    }
    TrainSmokeTick();
}

void CM_RenderCourseObjects(s32 cameraId) {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->RenderCourseObjects(cameraId);
    }

    TrainSmokeDraw(cameraId);
}

void CM_SomeSounds() {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->SomeSounds();
    }
}

void CM_CreditsSpawnActors() {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->CreditsSpawnActors();
    }
}

void CM_WhatDoesThisDo(Player* player, int8_t playerId) {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->WhatDoesThisDo(player, playerId);
    }
}

void CM_WhatDoesThisDoAI(Player* player, int8_t playerId) {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->WhatDoesThisDoAI(player, playerId);
    }
}

void CM_SetStaffGhost() {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->SetStaffGhost();
    }
}

Properties* CM_GetProps() {
    if (gWorldInstance.CurrentCourse) {
        return &gWorldInstance.CurrentCourse->Props;
    }
    return NULL;
}

Properties* CM_GetPropsCourseId(s32 courseId) {
    return &gWorldInstance.Courses[courseId]->Props;
}

void CM_ScrollingTextures() {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->ScrollingTextures();
    }
}

void CM_DrawWater(struct UnkStruct_800DC5EC* screen, uint16_t pathCounter, uint16_t cameraRot,
                  uint16_t playerDirection) {
    if (gWorldInstance.CurrentCourse) {
        gWorldInstance.CurrentCourse->DrawWater(screen, pathCounter, cameraRot, playerDirection);
    }
}

/**
 * This should only be ran once per course, otherwise animation/timings might become sped up.
 */
void CM_SpawnStarterLakitu() {
    if ((gDemoMode) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }

    for (size_t i = 0; i < gPlayerCountSelection1; i++) {
        OLakitu* lakitu = new OLakitu(i, OLakitu::LakituType::STARTER);
        gWorldInstance.Lakitus[i] = lakitu;
        gWorldInstance.AddObject(lakitu);
    }
}

// Checkered flag lakitu
void CM_ActivateFinishLakitu(s32 playerId) {
    if ((gDemoMode) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }
    gWorldInstance.Lakitus[playerId]->Activate(OLakitu::LakituType::FINISH);
}

void CM_ActivateSecondLapLakitu(s32 playerId) {
    if ((gDemoMode) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }
    gWorldInstance.Lakitus[playerId]->Activate(OLakitu::LakituType::SECOND_LAP);
}

void CM_ActivateFinalLapLakitu(s32 playerId) {
    if ((gDemoMode) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }
    gWorldInstance.Lakitus[playerId]->Activate(OLakitu::LakituType::FINAL_LAP);
}

void CM_ActivateReverseLakitu(s32 playerId) {
    if ((gDemoMode) || (gGamestate == CREDITS_SEQUENCE)) {
        return;
    }
    gWorldInstance.Lakitus[playerId]->Activate(OLakitu::LakituType::REVERSE);
}

size_t GetCupCursorPosition() {
    return gWorldInstance.CurrentCup->CursorPosition;
}

void SetCupCursorPosition(size_t position) {
    gWorldInstance.CurrentCup->SetCourse(position);
    // gWorldInstance.CurrentCup->CursorPosition = position;
}

size_t GetCupSize() {
    return gWorldInstance.CurrentCup->GetSize();
}

void SetCourseFromCup() {
    gWorldInstance.CurrentCourse = gWorldInstance.CurrentCup->GetCourse();
}

void* GetCourse(void) {
    return gWorldInstance.CurrentCourse;
}

struct Actor* CM_GetActor(size_t index) {
    if (index < gWorldInstance.Actors.size()) {
        AActor* actor = gWorldInstance.Actors[index];
        return reinterpret_cast<struct Actor*>(reinterpret_cast<char*>(actor) + sizeof(void*));
    } else {
        // throw std::runtime_error("GetActor() index out of bounds");
        return NULL;
    }
}

size_t CM_FindActorIndex(Actor* actor) {
    // Move the ptr back to look at the vtable.
    // This gets us the proper C++ class instead of just the variables used in C.
    AActor* a = reinterpret_cast<AActor*>((char*) actor - sizeof(void*));
    auto actors = gWorldInstance.Actors;

    auto it = std::find(actors.begin(), actors.end(), static_cast<AActor*>(a));
    if (it != actors.end()) {
        return std::distance(actors.begin(), it);
    }
    printf("FindActorIndex() actor not found\n");
    return 0;
}

void CM_DeleteActor(size_t index) {
    std::vector<AActor*> actors = gWorldInstance.Actors;
    if (index < actors.size()) {
        actors.erase(actors.begin() + index);
    }
}

/**
 * Clean up actors and other game objects.
 */
void CM_CleanWorld(void) {
    World* world = &gWorldInstance;
    for (auto& actor : world->Actors) {
        delete actor;
    }

    for (auto& object : world->Objects) {
        delete object;
    }

    for (auto& emitter : world->Emitters) {
        delete emitter;
    }

    for (auto& actor : world->StaticMeshActors) {
        delete actor;
    }

    gEditor.ClearObjects();
    gWorldInstance.Actors.clear();
    gWorldInstance.StaticMeshActors.clear();
    gWorldInstance.Objects.clear();
    gWorldInstance.Emitters.clear();
    gWorldInstance.Lakitus.clear();
    gWorldInstance.Reset();
}

struct Actor* CM_AddBaseActor() {
    return (struct Actor*) gWorldInstance.AddBaseActor();
}

void CM_AddEditorObject(struct Actor* actor, const char* name) {
    gWorldInstance.AddEditorObject(actor, name);
}

void Editor_AddLight(s8* direction) {
    static size_t i = 0;
    gEditor.AddLight(("Light "+std::to_string(i)).c_str(), nullptr, direction);
    i += 1;
}

void Editor_ClearMatrix() {
    gEditor.ClearMatrixPool();
}

size_t CM_GetActorSize() {
    return gWorldInstance.Actors.size();
}

void CM_ActorCollision(Player* player, Actor* actor) {
    AActor* a = gWorldInstance.ConvertActorToAActor(actor);

    if (a->IsMod()) {
        a->Collision(player, a);
    }
}

f32 CM_GetWaterLevel(Vec3f pos, Collision* collision) {
    FVector fPos = {pos[0], pos[1], pos[2]};
    return gWorldInstance.CurrentCourse->GetWaterLevel(fPos, collision);
}

// clang-format off
bool IsMarioRaceway()     { return dynamic_cast<MarioRaceway*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsLuigiRaceway()     { return dynamic_cast<LuigiRaceway*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsChocoMountain()    { return dynamic_cast<ChocoMountain*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsBowsersCastle()    { return dynamic_cast<BowsersCastle*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsBansheeBoardwalk() { return dynamic_cast<BansheeBoardwalk*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsYoshiValley()      { return dynamic_cast<YoshiValley*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsFrappeSnowland()   { return dynamic_cast<FrappeSnowland*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsKoopaTroopaBeach() { return dynamic_cast<KoopaTroopaBeach*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsRoyalRaceway()     { return dynamic_cast<RoyalRaceway*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsMooMooFarm()       { return dynamic_cast<MooMooFarm*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsToadsTurnpike()    { return dynamic_cast<ToadsTurnpike*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsKalimariDesert()   { return dynamic_cast<KalimariDesert*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsSherbetLand()      { return dynamic_cast<SherbetLand*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsRainbowRoad()      { return dynamic_cast<RainbowRoad*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsWarioStadium()     { return dynamic_cast<WarioStadium*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsBlockFort()        { return dynamic_cast<BlockFort*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsSkyscraper()       { return dynamic_cast<Skyscraper*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsDoubleDeck()       { return dynamic_cast<DoubleDeck*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsDkJungle()         { return dynamic_cast<DKJungle*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsBigDonut()         { return dynamic_cast<BigDonut*>(gWorldInstance.CurrentCourse) != nullptr; }
bool IsPodiumCeremony()   { return dynamic_cast<PodiumCeremony*>(gWorldInstance.CurrentCourse) != nullptr; }

void SelectMarioRaceway()       { gWorldInstance.SetCourseByType<MarioRaceway>(); }
void SelectLuigiRaceway()       { gWorldInstance.SetCourseByType<LuigiRaceway>(); }
void SelectChocoMountain()      { gWorldInstance.SetCourseByType<ChocoMountain>(); }
void SelectBowsersCastle()      { gWorldInstance.SetCourseByType<BowsersCastle>(); }
void SelectBansheeBoardwalk()   { gWorldInstance.SetCourseByType<BansheeBoardwalk>(); }
void SelectYoshiValley()        { gWorldInstance.SetCourseByType<YoshiValley>(); }
void SelectFrappeSnowland()     { gWorldInstance.SetCourseByType<FrappeSnowland>(); }
void SelectKoopaTroopaBeach()   { gWorldInstance.SetCourseByType<KoopaTroopaBeach>(); }
void SelectRoyalRaceway()       { gWorldInstance.SetCourseByType<RoyalRaceway>(); }
void SelectMooMooFarm()         { gWorldInstance.SetCourseByType<MooMooFarm>(); }
void SelectToadsTurnpike()      { gWorldInstance.SetCourseByType<ToadsTurnpike>(); }
void SelectKalimariDesert()     { gWorldInstance.SetCourseByType<KalimariDesert>(); }
void SelectSherbetLand()        { gWorldInstance.SetCourseByType<SherbetLand>(); }
void SelectRainbowRoad()        { gWorldInstance.SetCourseByType<RainbowRoad>(); }
void SelectWarioStadium()       { gWorldInstance.SetCourseByType<WarioStadium>(); }
void SelectBlockFort()          { gWorldInstance.SetCourseByType<BlockFort>(); }
void SelectSkyscraper()         { gWorldInstance.SetCourseByType<Skyscraper>(); }
void SelectDoubleDeck()         { gWorldInstance.SetCourseByType<DoubleDeck>(); }
void SelectDkJungle()           { gWorldInstance.SetCourseByType<DKJungle>(); }
void SelectBigDonut()           { gWorldInstance.SetCourseByType<BigDonut>(); }
void SelectPodiumCeremony()     { gWorldInstance.CurrentCourse = gPodiumCeremony.get(); }
// clang-format on

void* GetMushroomCup(void) {
    return gMushroomCup;
}

void* GetFlowerCup(void) {
    return gFlowerCup;
}

void* GetStarCup(void) {
    return gStarCup;
}

void* GetSpecialCup(void) {
    return gSpecialCup;
}

void* GetBattleCup(void) {
    return gBattleCup;
}

// End of frame cleanup of actors, objects, etc.
void CM_RunGarbageCollector(void) {
    RunGarbageCollector();
}
}

void push_frame() {
    GameEngine::StartAudioFrame();
    GameEngine::Instance->StartFrame();
    thread5_iteration();
    GameEngine::EndAudioFrame();
    // thread5_game_loop();
    // Graphics_ThreadUpdate();w
    // Timer_Update();
}

#ifdef _WIN32
int SDL_main(int argc, char** argv) {
#else
#if defined(__cplusplus) && defined(PLATFORM_IOS)
extern "C"
#endif
    int
    main(int argc, char* argv[]) {
#endif
    // load_wasm();
    GameEngine::Create();
    audio_init();
    sound_init();

    CustomEngineInit();

    if (CVarGetInteger("gEnableDebugMode", 0) == true) {
        gMenuSelection = START_MENU;
    }

    thread5_game_loop();
    gEditor.Load();
    while (WindowIsRunning()) {
        push_frame();
    }
    CustomEngineDestroy();
    // GameEngine::Instance->ProcessFrame(push_frame);
    GameEngine::Instance->Destroy();
    return 0;
}
