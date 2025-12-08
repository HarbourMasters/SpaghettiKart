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

void RegisterGameActors(Registry<SpawnParams&>& r) {
    r.Add("mk:item_box",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            spawn_item_box(pos);
        }
    );

    r.Add("mk:fake_item_box",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            spawn_fake_item_box(pos);
        }
    );

    r.Add("mk:thwomp",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OThwomp(params));
        }
    );

    r.Add("mk:snowman",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OSnowman(params));
        }
    );

    r.Add("mk:hot_air_balloon",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OHotAirBalloon(params));
        }
    );

    r.Add("mk:hedgehog",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OHedgehog(params));
        }
    );

    r.Add("mk:grand_prix_balloons",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OGrandPrixBalloons(params));
        }
    );

    r.Add("mk:flagpole",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OFlagpole(params));
        }
    );

    r.Add("mk:crab",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OCrab(params));
        }
    );

    r.Add("mk:cheep_cheep",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OCheepCheep(params));
        }
    );

    r.Add("mk:bomb_kart",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OBombKart(params));
        }
    );

    r.Add("mk:bat",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OBat(params));
        }
    );

    r.Add("mk:boos",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OBoos(params));
        }
    );

    r.Add("mk:trophy",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OTrophy(params));
        }
    );

    r.Add("mk:trash_bin",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OTrashBin(params));
        }
    );

    r.Add("mk:seagull",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OSeagull(params));
        }
    );

    r.Add("mk:chain_chomp",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OChainChomp());
        }
    );

    r.Add("mk:podium",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OPodium(params));
        }
    );

    r.Add("mk:penguin",
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OPenguin(params));
        }
    );

    r.Add("mk:banana",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ABanana(params));
        }
    );

    r.Add("mk:mario_sign",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AMarioSign(params));
        }
    );

    r.Add("mk:wario_sign",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AWarioSign(params));
        }
    );

    r.Add("mk:falling_rock",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AFallingRock(params));
        }
    );

    r.Add("mk:yoshi_egg",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_YOSHI_EGG);
        }
    );

    r.Add("mk:piranha_plant",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_PIRANHA_PLANT);
        }
    );

    r.Add("mk:tree_mario_raceway",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_MARIO_RACEWAY);
        }
    );

    r.Add("mk:tree_yoshi_valley",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_YOSHI_VALLEY);
        }
    );

    r.Add("mk:tree_royal_raceway",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_ROYAL_RACEWAY);
        }
    );

    r.Add("mk:tree_moo_moo_farm",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_MOO_MOO_FARM);
        }
    );

    r.Add("mk:palm_tree",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_PALM_TREE);
        }
    );

    r.Add("mk:unknown_0x1a",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_LUIGI_RACEWAY);
        }
    );

    r.Add("mk:unknown_0x1b",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_UNKNOWN_0x1B);
        }
    );

    r.Add("mk:tree_peach_castle",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_PEACH_CASTLE);
        }
    );

    r.Add("mk:tree_frappe_snowland",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_FRAPPE_SNOWLAND);
        }
    );

    r.Add("mk:cactus1_kalamari_desert",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_CACTUS1_KALAMARI_DESERT);
        }
    );

    r.Add("mk:cactus2_kalamari_desert",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_CACTUS2_KALAMARI_DESERT);
        }
    );

    r.Add("mk:cactus3_kalamari_desert",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_CACTUS3_KALAMARI_DESERT);
        }
    );

    r.Add("mk:bush_bowsers_castle",
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_BUSH_BOWSERS_CASTLE);
        }
    );

    r.Add("mk:train",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ATrain(params));
        }
    );

    r.Add("mk:paddle_boat",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ABoat(params));
        }
    );

    r.Add("mk:car",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ACar(params));
        }
    );

    r.Add("mk:truck",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ATankerTruck(params));
        }
    );

    r.Add("mk:tanker_truck",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ATankerTruck(params));
        }
    );

    r.Add("mk:bus",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ATankerTruck(params));
        }
    );

    r.Add("hm:spaghetti_ship",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ASpaghettiShip(params));
        }
    );

    r.Add("hm:ship",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AShip(params));
        }
    );

    r.Add("hm:starship",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AStarship(params));
        }
    );

    r.Add("hm:cloud",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ACloud(params));
        }
    );

    r.Add("hm:text",
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AText(params));
        }
    );
}

void RegisterTracks(Registry<>& r) {
    r.Add("mk:mario_raceway",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<MarioRaceway>());
        }
    );
    r.Add("mk:choco_mountain",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<ChocoMountain>());
        }
    );
    r.Add("mk:bowsers_castle",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<BowsersCastle>());
        }
    );
    r.Add("mk:banshee_boardwalk",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<BansheeBoardwalk>());
        }
    );
    r.Add("mk:yoshi_valley",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<YoshiValley>());
        }
    );
    r.Add("mk:frappe_snowland",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<FrappeSnowland>());
        }
    );
    r.Add("mk:koopa_troopa_beach",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<KoopaTroopaBeach>());
        }
    );
    r.Add("mk:royal_raceway",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<RoyalRaceway>());
        }
    );
    r.Add("mk:luigi_raceway",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<LuigiRaceway>());
        }
    );
    r.Add("mk:moo_moo_farm",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<MooMooFarm>());
        }
    );
    r.Add("mk:toads_turnpike",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<ToadsTurnpike>());
        }
    );
    r.Add("mk:kalimari_desert",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<KalimariDesert>());
        }
    );
    r.Add("mk:sherbet_land",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<SherbetLand>());
        }
    );
    r.Add("mk:rainbow_road",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<RainbowRoad>());
        }
    );
    r.Add("mk:wario_stadium",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<WarioStadium>());
        }
    );
    r.Add("mk:block_fort",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<BlockFort>());
        }
    );
    r.Add("mk:skyscraper",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<Skyscraper>());
        }
    );
    r.Add("mk:double_deck",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<DoubleDeck>());
        }
    );
    r.Add("mk:dk_jungle",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<DKJungle>());
        }
    );
    r.Add("mk:big_donut",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<BigDonut>());
        }
    );

    r.Add("mk:test_track",
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<TestTrack>());
        }
    );
}
