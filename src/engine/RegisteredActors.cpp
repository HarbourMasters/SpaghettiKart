#include "SpawnParams.h"
#include "engine/CoreMath.h"
#include "Registry.h"
#include "engine/World.h"

#include "AllTracks.h"
#include "AllActors.h"

extern "C" {
#include "common_structs.h"
#include "actors.h"
#include "actor_types.h"
}

void RegisterGameActors(Registry& r) {
    r.AddActor("mk:item_box",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            spawn_item_box(pos);
        }
    );

    r.AddActor("mk:fake_item_box",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            spawn_fake_item_box(pos);
        }
    );

    r.AddActor("mk:thwomp",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OThwomp(params));
        }
    );

    r.AddActor("mk:snowman",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OSnowman(params));
        }
    );

    r.AddActor("mk:hot_air_balloon",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OHotAirBalloon(params));
        }
    );

    r.AddActor("mk:hedgehog",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OHedgehog(params));
        }
    );

    r.AddActor("mk:grand_prix_balloons",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OGrandPrixBalloons(params));
        }
    );

    r.AddActor("mk:flagpole",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OFlagpole(params));
        }
    );

    r.AddActor("mk:crab",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OCrab(params));
        }
    );

    r.AddActor("mk:cheep_cheep",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OCheepCheep(params));
        }
    );

    r.AddActor("mk:bomb_kart",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OBombKart(params));
        }
    );

    r.AddActor("mk:bat",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OBat(params));
        }
    );

    r.AddActor("mk:boos",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OBoos(params));
        }
    );

    r.AddActor("mk:trophy",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OTrophy(params));
        }
    );

    r.AddActor("mk:trash_bin",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OTrashBin(params));
        }
    );

    r.AddActor("mk:seagull",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OSeagull(params));
        }
    );

    r.AddActor("mk:chain_chomp",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OChainChomp());
        }
    );

    r.AddActor("mk:podium",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OPodium(params));
        }
    );

    r.AddActor("mk:penguin",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OPenguin(params));
        }
    );

    r.AddActor("mk:banana",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ABanana(params));
        }
    );

    r.AddActor("mk:mario_sign",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AMarioSign(params));
        }
    );

    r.AddActor("mk:wario_sign",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AWarioSign(params));
        }
    );

    r.AddActor("mk:falling_rock",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AFallingRock(params));
        }
    );

    r.AddActor("mk:yoshi_egg",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_YOSHI_EGG);
        }
    );

    r.AddActor("mk:piranha_plant",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_PIRANHA_PLANT);
        }
    );

    r.AddActor("mk:tree_mario_raceway",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_MARIO_RACEWAY);
        }
    );

    r.AddActor("mk:tree_yoshi_valley",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_YOSHI_VALLEY);
        }
    );

    r.AddActor("mk:tree_royal_raceway",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_ROYAL_RACEWAY);
        }
    );

    r.AddActor("mk:tree_moo_moo_farm",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_MOO_MOO_FARM);
        }
    );

    r.AddActor("mk:palm_tree",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_PALM_TREE);
        }
    );

    r.AddActor("mk:unknown_0x1a",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_LUIGI_RACEWAY);
        }
    );

    r.AddActor("mk:unknown_0x1b",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_UNKNOWN_0x1B);
        }
    );

    r.AddActor("mk:tree_peach_castle",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_PEACH_CASTLE);
        }
    );

    r.AddActor("mk:tree_frappe_snowland",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_FRAPPE_SNOWLAND);
        }
    );

    r.AddActor("mk:cactus1_kalamari_desert",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_CACTUS1_KALAMARI_DESERT);
        }
    );

    r.AddActor("mk:cactus2_kalamari_desert",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_CACTUS2_KALAMARI_DESERT);
        }
    );

    r.AddActor("mk:cactus3_kalamari_desert",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_CACTUS3_KALAMARI_DESERT);
        }
    );

    r.AddActor("mk:bush_bowsers_castle",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_BUSH_BOWSERS_CASTLE);
        }
    );

    r.AddActor("mk:train",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ATrain(params));
        }
    );

    r.AddActor("mk:paddle_boat",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ABoat(params));
        }
    );

    r.AddActor("mk:car",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ACar(params));
        }
    );

    r.AddActor("mk:truck",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ATankerTruck(params));
        }
    );

    r.AddActor("mk:tanker_truck",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ATankerTruck(params));
        }
    );

    r.AddActor("mk:bus",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ATankerTruck(params));
        }
    );

    r.AddActor("hm:spaghetti_ship",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ASpaghettiShip(params));
        }
    );

    r.AddActor("hm:ship",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AShip(params));
        }
    );

    r.AddActor("hm:starship",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AStarship(params));
        }
    );

    r.AddActor("hm:cloud",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ACloud(params));
        }
    );

    r.AddActor("hm:text",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AText(params));
        }
    );
}

void RegisterTracks(Registry& r) {
    r.AddTrack("mk:mario_raceway",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<MarioRaceway>());
        }
    );
    r.AddTrack("mk:choco_mountain",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<ChocoMountain>());
        }
    );
    r.AddTrack("mk:bowsers_castle",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<BowsersCastle>());
        }
    );
    r.AddTrack("mk:banshee_boardwalk",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<BansheeBoardwalk>());
        }
    );
    r.AddTrack("mk:yoshi_valley",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<YoshiValley>());
        }
    );
    r.AddTrack("mk:frappe_snowland",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<FrappeSnowland>());
        }
    );
    r.AddTrack("mk:koopa_troopa_beach",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<KoopaTroopaBeach>());
        }
    );
    r.AddTrack("mk:royal_raceway",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<RoyalRaceway>());
        }
    );
    r.AddTrack("mk:luigi_raceway",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<LuigiRaceway>());
        }
    );
    r.AddTrack("mk:moo_moo_farm",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<MooMooFarm>());
        }
    );
    r.AddTrack("mk:toads_turnpike",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<ToadsTurnpike>());
        }
    );
    r.AddTrack("mk:kalimari_desert",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<KalimariDesert>());
        }
    );
    r.AddTrack("mk:sherbet_land",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<SherbetLand>());
        }
    );
    r.AddTrack("mk:rainbow_road",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<RainbowRoad>());
        }
    );
    r.AddTrack("mk:wario_stadium",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<WarioStadium>());
        }
    );
    r.AddTrack("mk:block_fort",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<BlockFort>());
        }
    );
    r.AddTrack("mk:skyscraper",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<Skyscraper>());
        }
    );
    r.AddTrack("mk:double_deck",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<DoubleDeck>());
        }
    );
    r.AddTrack("mk:dk_jungle",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<DKJungle>());
        }
    );
    r.AddTrack("mk:big_donut",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<BigDonut>());
        }
    );

    r.AddTrack("mk:test_track",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<TestTrack>());
        }
    );
}
