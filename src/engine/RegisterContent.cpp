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

/**
 * @file RegisterContent
 * This file is responsible for registering the games stock content
 */

void RegisterActors(Registry<ActorInfo, const SpawnParams&>& r) {
    ActorInfo info;

    info = { .ResourceName = "mk:item_box" };

    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            spawn_item_box(pos);
        }
    );

    info = { .ResourceName = "mk:fake_item_box" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            spawn_fake_item_box(pos);
        }
    );

    info = { .ResourceName = "mk:thwomp" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OThwomp(params));
        }
    );

    info = { .ResourceName = "mk:snowman" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OSnowman(params));
        }
    );

    info = { .ResourceName = "mk:hot_air_balloon" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OHotAirBalloon(params));
        }
    );

    info = { .ResourceName = "mk:hedgehog" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OHedgehog(params));
        }
    );

    info = { .ResourceName = "mk:grand_prix_balloons" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OGrandPrixBalloons(params));
        }
    );

    info = { .ResourceName = "mk:flagpole" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OFlagpole(params));
        }
    );

    info = { .ResourceName = "mk:crab" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OCrab(params));
        }
    );

    info = { .ResourceName = "mk:cheep_cheep" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OCheepCheep(params));
        }
    );

    info = { .ResourceName = "mk:bomb_kart" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OBombKart(params));
        }
    );

    info = { .ResourceName = "mk:bat" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OBat(params));
        }
    );

    info = { .ResourceName = "mk:boos" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OBoos(params));
        }
    );

    info = { .ResourceName = "mk:trophy" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OTrophy(params));
        }
    );

    info = { .ResourceName = "mk:trash_bin" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OTrashBin(params));
        }
    );

    info = { .ResourceName = "mk:seagull" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OSeagull(params));
        }
    );

    info = { .ResourceName = "mk:chain_chomp" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OChainChomp());
        }
    );

    info = { .ResourceName = "mk:podium" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OPodium(params));
        }
    );

    info = { .ResourceName = "mk:penguin" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddObject(new OPenguin(params));
        }
    );

    info = { .ResourceName = "mk:banana" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ABanana(params));
        }
    );

    info = { .ResourceName = "mk:mario_sign" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AMarioSign(params));
        }
    );

    info = { .ResourceName = "mk:wario_sign" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AWarioSign(params));
        }
    );

    info = { .ResourceName = "mk:falling_rock" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AFallingRock(params));
        }
    );

    info = { .ResourceName = "mk:yoshi_egg" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_YOSHI_EGG);
        }
    );

    info = { .ResourceName = "mk:piranha_plant" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_PIRANHA_PLANT);
        }
    );

    info = { .ResourceName = "mk:tree_mario_raceway" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_MARIO_RACEWAY);
        }
    );

    info = { .ResourceName = "mk:tree_yoshi_valley" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_YOSHI_VALLEY);
        }
    );

    info = { .ResourceName = "mk:tree_royal_raceway" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_ROYAL_RACEWAY);
        }
    );

    info = { .ResourceName = "mk:tree_moo_moo_farm" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_MOO_MOO_FARM);
        }
    );

    info = { .ResourceName = "mk:palm_tree" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_PALM_TREE);
        }
    );

    info = { .ResourceName = "mk:unknown_0x1a" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_LUIGI_RACEWAY);
        }
    );

    info = { .ResourceName = "mk:unknown_0x1b" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_UNKNOWN_0x1B);
        }
    );

    info = { .ResourceName = "mk:tree_peach_castle" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_PEACH_CASTLE);
        }
    );

    info = { .ResourceName = "mk:tree_frappe_snowland" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_FRAPPE_SNOWLAND);
        }
    );

    info = { .ResourceName = "mk:cactus1_kalamari_desert" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_CACTUS1_KALAMARI_DESERT);
        }
    );

    info = { .ResourceName = "mk:cactus2_kalamari_desert" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_CACTUS2_KALAMARI_DESERT);
        }
    );

    info = { .ResourceName = "mk:cactus3_kalamari_desert" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_CACTUS3_KALAMARI_DESERT);
        }
    );

    info = { .ResourceName = "mk:bush_bowsers_castle" };
    r.Add(info,
        [](const SpawnParams& params) {
            FVector loc = params.Location.value_or(FVector{0, 0, 0});
            Vec3f pos = { loc.x, loc.y, loc.z };
            Vec3s rot = {0, 0, 0};
            Vec3f vel = {0, 0, 0};
            add_actor_to_empty_slot(pos, rot, vel, ACTOR_BUSH_BOWSERS_CASTLE);
        }
    );

    info = { .ResourceName = "mk:train" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ATrain(params));
        }
    );

    info = { .ResourceName = "mk:paddle_boat" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ABoat(params));
        }
    );

    info = { .ResourceName = "mk:car" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ACar(params));
        }
    );

    info = { .ResourceName = "mk:truck" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ATankerTruck(params));
        }
    );

    info = { .ResourceName = "mk:tanker_truck" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ATankerTruck(params));
        }
    );

    info = { .ResourceName = "mk:bus" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ATankerTruck(params));
        }
    );

    info = { .ResourceName = "hm:spaghetti_ship" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ASpaghettiShip(params));
        }
    );

    info = { .ResourceName = "hm:ship" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AShip(params));
        }
    );

    info = { .ResourceName = "hm:starship" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AStarship(params));
        }
    );

    info = { .ResourceName = "hm:cloud" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new ACloud(params));
        }
    );

    info = { .ResourceName = "hm:text" };
    r.Add(info,
        [](const SpawnParams& params) {
            gWorldInstance.AddActor(new AText(params));
        }
    );
}

void RegisterTracks(Registry<TrackInfo>& r) {
    TrackInfo info;

    info = {
        .ResourceName = "mk:mario_raceway",
        .Name = "mario raceway",
        .DebugName = "m circuit",
        .Length = "567m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<MarioRaceway>());
        }
    );

    info = {
        .ResourceName = "mk:choco_mountain",
        .Name = "choco mountain",
        .DebugName = "mountain",
        .Length = "687m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<ChocoMountain>());
        }
    );

    info = {
        .ResourceName = "mk:bowsers_castle",
        .Name = "bowser's castle",
        .DebugName = "castle",
        .Length = "777m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<BowsersCastle>());
        }
    );

    info = {
        .ResourceName = "mk:banshee_boardwalk",
        .Name = "banshee boardwalk",
        .DebugName = "ghost",
        .Length = "747m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<BansheeBoardwalk>());
        }
    );

    info = {
        .ResourceName = "mk:yoshi_valley",
        .Name = "yoshi valley",
        .DebugName = "maze",
        .Length = "772m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<YoshiValley>());
        }
    );

    info = {
        .ResourceName = "mk:frappe_snowland",
        .Name = "frappe snowland",
        .DebugName = "snow",
        .Length = "734m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<FrappeSnowland>());
        }
    );

    info = {
        .ResourceName = "mk:koopa_beach",
        .Name = "koopa troopa beach",
        .DebugName = "beach",
        .Length = "691m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<KoopaTroopaBeach>());
        }
    );

    info = {
        .ResourceName = "mk:royal_raceway",
        .Name = "royal raceway",
        .DebugName = "p circuit",
        .Length = "1025m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<RoyalRaceway>());
        }
    );

    info = {
        .ResourceName = "mk:luigi_raceway",
        .Name = "luigi raceway",
        .DebugName = "l circuit",
        .Length = "717m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<LuigiRaceway>());
        }
    );

    info = {
        .ResourceName = "mk:moo_moo_farm",
        .Name = "moo moo farm",
        .DebugName = "farm",
        .Length = "527m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<MooMooFarm>());
        }
    );

    info = {
        .ResourceName = "mk:toads_turnpike",
        .Name = "toad's turnpike",
        .DebugName = "highway",
        .Length = "1036m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<ToadsTurnpike>());
        }
    );

    info = {
        .ResourceName = "mk:kalimari_desert",
        .Name = "kalimari desert",
        .DebugName = "desert",
        .Length = "753m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<KalimariDesert>());
        }
    );

    info = {
        .ResourceName = "mk:sherbet_land",
        .Name = "sherbet land",
        .DebugName = "sherbet",
        .Length = "756m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<SherbetLand>());
        }
    );

    info = {
        .ResourceName = "mk:rainbow_road",
        .Name = "rainbow road",
        .DebugName = "rainbow",
        .Length = "2000m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<RainbowRoad>());
        }
    );

    info = {
        .ResourceName = "mk:wario_stadium",
        .Name = "wario stadium",
        .DebugName = "stadium",
        .Length = "1591m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<WarioStadium>());
        }
    );

    info = {
        .ResourceName = "mk:block_fort",
        .Name = "block fort",
        .DebugName = "block",
        .Length = "",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<BlockFort>());
        }
    );

    info = {
        .ResourceName = "mk:skyscraper",
        .Name = "skyscraper",
        .DebugName = "skyscraper",
        .Length = "",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<Skyscraper>());
        }
    );

    info = {
        .ResourceName = "mk:double_deck",
        .Name = "double deck",
        .DebugName = "deck",
        .Length = "",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<DoubleDeck>());
        }
    );

    info = {
        .ResourceName = "mk:dk_jungle",
        .Name = "d.k.'s jungle parkway",
        .DebugName = "jungle",
        .Length = "893m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<DKJungle>());
        }
    );

    info = {
        .ResourceName = "mk:big_donut",
        .Name = "big donut",
        .DebugName = "doughnut",
        .Length = "",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<BigDonut>());
        }
    );

    info = {
        .ResourceName = "mk:test_track",
        .Name = "test track",
        .DebugName = "test track",
        .Length = "100m",
    };

    r.Add(info,
        []() {
            gWorldInstance.SetCurrentTrack(std::make_shared<TestTrack>());
        }
    );
}
