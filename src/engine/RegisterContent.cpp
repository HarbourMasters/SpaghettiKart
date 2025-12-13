#include "SpawnParams.h"
#include "engine/CoreMath.h"
#include "Registry.h"
#include "engine/World.h"
#include "RegisterContent.h"

#include "AllTracks.h"
#include "AllActors.h"

extern "C" {
#include "common_structs.h"
#include "actors.h"
#include "actor_types.h"
#include "assets/textures/other_textures.h"
}

/**
 * @file RegisterContent
 * This file is responsible for registering the games stock content
 */


/**
 * 
 * DO NOT RE-ORDER ANYTHING IN THIS FILE.
 * IT WILL MIX UP THE IDS
 * 
 * Add new content to the end of the function
 */


void RegisterActors(Registry<ActorInfo, const SpawnParams&>& r) {
    ActorInfo info;

    info = { .ResourceName = "mk:item_box", .Name = "Item Box" };

    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        spawn_item_box(pos);
    });

    info = { .ResourceName = "mk:fake_item_box", .Name = "Fake Item Box" };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        spawn_fake_item_box(pos);
    });

    info = { .ResourceName = "mk:thwomp", .Name = "Thwomp" };
    r.Add(info, AddObjectToWorld<OThwomp>);

    info = { .ResourceName = "mk:snowman", .Name = "Snowman" };
    r.Add(info, AddObjectToWorld<OSnowman>);

    info = { .ResourceName = "mk:hot_air_balloon", .Name = "Hot Air Balloon" };
    r.Add(info, AddObjectToWorld<OHotAirBalloon>);

    info = { .ResourceName = "mk:hedgehog", .Name = "Hedgehog" };
    r.Add(info, AddObjectToWorld<OHedgehog>);

    info = { .ResourceName = "mk:grand_prix_balloons", .Name = "Grand Prix Balloons" };
    r.Add(info, AddObjectToWorld<OGrandPrixBalloons>);

    info = { .ResourceName = "mk:flagpole", .Name = "Flagpole", .Tags = { "sign" } };
    r.Add(info, AddObjectToWorld<OFlagpole>);

    info = { .ResourceName = "mk:crab", .Name = "Crab" };
    r.Add(info, AddObjectToWorld<OCrab>);

    info = { .ResourceName = "mk:cheep_cheep", .Name = "Cheep Cheep", .Tags = { "passive" } };
    r.Add(info, AddObjectToWorld<OCheepCheep>);

    info = { .ResourceName = "mk:bomb_kart", .Name = "Bomb Kart", .Tags = { "vehicle" } };
    r.Add(info, AddObjectToWorld<OBombKart>);

    info = { .ResourceName = "mk:bat", .Name = "Bat" };
    r.Add(info, AddObjectToWorld<OBat>);

    info = { .ResourceName = "mk:boos", .Name = "Boos" };
    r.Add(info, AddObjectToWorld<OBoos>);

    info = { .ResourceName = "mk:trophy", .Name = "Trophy" };
    r.Add(info, AddObjectToWorld<OTrophy>);

    info = { .ResourceName = "mk:trash_bin", .Name = "Trash Bin" };
    r.Add(info, AddObjectToWorld<OTrashBin>);

    info = { .ResourceName = "mk:seagull", .Name = "Seagull", .Tags = { "passive" } };
    r.Add(info, AddObjectToWorld<OSeagull>);

    info = { .ResourceName = "mk:chain_chomp", .Name = "Chain Chomp", .Tags = { "vehicle" } };
    r.Add(info, AddObjectToWorld<OChainChomp>);

    info = { .ResourceName = "mk:podium", .Name = "Podium" };
    r.Add(info, AddObjectToWorld<OPodium>);

    info = { .ResourceName = "mk:penguin", .Name = "Penguin" };
    r.Add(info, AddObjectToWorld<OPenguin>);

    info = { .ResourceName = "mk:banana", .Name = "Banana" };
    r.Add(info, AddActorToWorld<ABanana>);

    info = { .ResourceName = "mk:mario_sign", .Name = "Mario Sign", .Tags = { "sign" } };
    r.Add(info, AddActorToWorld<AMarioSign>);

    info = { .ResourceName = "mk:wario_sign", .Name = "Wario Sign", .Tags = { "sign" } };
    r.Add(info, AddActorToWorld<AWarioSign>);

    info = { .ResourceName = "mk:falling_rock", .Name = "Falling Rock", .Tags = { "obstacle" } };
    r.Add(info, AddActorToWorld<AFallingRock>);

    info = { .ResourceName = "mk:yoshi_egg", .Name = "Yoshi Egg", .Tags = { "obstacle" } };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        Vec3s rot = { 0, 0, 0 };
        Vec3f vel = { 0, 0, 0 };
        add_actor_to_empty_slot(pos, rot, vel, ACTOR_YOSHI_EGG);
    });

    info = { .ResourceName = "mk:piranha_plant", .Name = "Piranha Plant", .Tags = { "foliage", "obstacle" } };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        Vec3s rot = { 0, 0, 0 };
        Vec3f vel = { 0, 0, 0 };
        add_actor_to_empty_slot(pos, rot, vel, ACTOR_PIRANHA_PLANT);
    });

    info = { .ResourceName = "mk:tree_mario_raceway", .Name = "Tree (Mario Raceway)", .Tags = { "foliage" } };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        Vec3s rot = { 0, 0, 0 };
        Vec3f vel = { 0, 0, 0 };
        add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_MARIO_RACEWAY);
    });

    info = { .ResourceName = "mk:tree_yoshi_valley", .Name = "Tree (Yoshi Valley)", .Tags = { "foliage" } };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        Vec3s rot = { 0, 0, 0 };
        Vec3f vel = { 0, 0, 0 };
        add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_YOSHI_VALLEY);
    });

    info = { .ResourceName = "mk:tree_royal_raceway", .Name = "Tree (Royal Raceway)", .Tags = { "foliage" } };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        Vec3s rot = { 0, 0, 0 };
        Vec3f vel = { 0, 0, 0 };
        add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_ROYAL_RACEWAY);
    });

    info = { .ResourceName = "mk:tree_moo_moo_farm", .Name = "Tree (Moo Moo Farm)", .Tags = { "foliage" } };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        Vec3s rot = { 0, 0, 0 };
        Vec3f vel = { 0, 0, 0 };
        add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_MOO_MOO_FARM);
    });

    info = { .ResourceName = "mk:palm_tree", .Name = "Palm Tree", .Tags = { "foliage" } };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        Vec3s rot = { 0, 0, 0 };
        Vec3f vel = { 0, 0, 0 };
        add_actor_to_empty_slot(pos, rot, vel, ACTOR_PALM_TREE);
    });

    info = { .ResourceName = "mk:tree_luigi_raceway", .Name = "Tree (Luigi Raceway)", .Tags = { "foliage" } };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        Vec3s rot = { 0, 0, 0 };
        Vec3f vel = { 0, 0, 0 };
        add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_LUIGI_RACEWAY);
    });

    info = { .ResourceName = "mk:unknown_0x1b", .Name = "Unknown Plant (0x1B)", .Tags = { "foliage" } };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        Vec3s rot = { 0, 0, 0 };
        Vec3f vel = { 0, 0, 0 };
        add_actor_to_empty_slot(pos, rot, vel, ACTOR_UNKNOWN_0x1B);
    });

    info = { .ResourceName = "mk:tree_peach_castle", .Name = "Tree (Peach Castle)", .Tags = { "foliage" } };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        Vec3s rot = { 0, 0, 0 };
        Vec3f vel = { 0, 0, 0 };
        add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_PEACH_CASTLE);
    });

    info = { .ResourceName = "mk:tree_frappe_snowland", .Name = "Tree (Frappe Snowland)", .Tags = { "foliage" } };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        Vec3s rot = { 0, 0, 0 };
        Vec3f vel = { 0, 0, 0 };
        add_actor_to_empty_slot(pos, rot, vel, ACTOR_TREE_FRAPPE_SNOWLAND);
    });

    info = { .ResourceName = "mk:cactus1_kalamari_desert",
             .Name = "Cactus 1 (Kalamari Desert)",
             .Tags = { "foliage" } };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        Vec3s rot = { 0, 0, 0 };
        Vec3f vel = { 0, 0, 0 };
        add_actor_to_empty_slot(pos, rot, vel, ACTOR_CACTUS1_KALAMARI_DESERT);
    });

    info = { .ResourceName = "mk:cactus2_kalamari_desert",
             .Name = "Cactus 2 (Kalamari Desert)",
             .Tags = { "foliage" } };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        Vec3s rot = { 0, 0, 0 };
        Vec3f vel = { 0, 0, 0 };
        add_actor_to_empty_slot(pos, rot, vel, ACTOR_CACTUS2_KALAMARI_DESERT);
    });

    info = { .ResourceName = "mk:cactus3_kalamari_desert",
             .Name = "Cactus 3 (Kalamari Desert)",
             .Tags = { "foliage" } };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        Vec3s rot = { 0, 0, 0 };
        Vec3f vel = { 0, 0, 0 };
        add_actor_to_empty_slot(pos, rot, vel, ACTOR_CACTUS3_KALAMARI_DESERT);
    });

    info = { .ResourceName = "mk:bush_bowsers_castle", .Name = "Bush (Bowser's Castle)" };
    r.Add(info, [](const SpawnParams& params) {
        FVector loc = params.Location.value_or(FVector{ 0, 0, 0 });
        Vec3f pos = { loc.x, loc.y, loc.z };
        Vec3s rot = { 0, 0, 0 };
        Vec3f vel = { 0, 0, 0 };
        add_actor_to_empty_slot(pos, rot, vel, ACTOR_BUSH_BOWSERS_CASTLE);
    });

    info = { .ResourceName = "mk:finishline", .Name = "Finishline" };
    r.Add(info, AddActorToWorld<AFinishline>);

    info = { .ResourceName = "mk:train", .Name = "Train", .Tags = { "vehicle" } };
    r.Add(info, AddActorToWorld<ATrain>);

    info = { .ResourceName = "mk:paddle_boat", .Name = "Paddle Boat", .Tags = { "vehicle" } };
    r.Add(info, AddActorToWorld<ABoat>);

    info = { .ResourceName = "mk:car", .Name = "Car", .Tags = { "vehicle" } };
    r.Add(info, AddActorToWorld<ACar>);

    info = { .ResourceName = "mk:truck", .Name = "Truck", .Tags = { "vehicle" } };
    r.Add(info, AddActorToWorld<ATankerTruck>);

    info = { .ResourceName = "mk:tanker_truck", .Name = "Tanker Truck", .Tags = { "vehicle" } };
    r.Add(info, AddActorToWorld<ATankerTruck>);

    info = { .ResourceName = "mk:bus", .Name = "Bus", .Tags = { "vehicle" } };
    r.Add(info, AddActorToWorld<ATankerTruck>);

    info = { .ResourceName = "hm:spaghetti_ship", .Name = "Spaghetti Ship" };
    r.Add(info, AddActorToWorld<ASpaghettiShip>);

    info = { .ResourceName = "hm:ship", .Name = "Ghostship (HM64)" };
    r.Add(info, AddActorToWorld<AShip>);

    info = { .ResourceName = "hm:starship", .Name = "Starship (HM64)" };
    r.Add(info, AddActorToWorld<AStarship>);

    info = { .ResourceName = "hm:cloud", .Name = "Cloud (HM64)", .Tags = { "item" } };
    r.Add(info, AddActorToWorld<ACloud>);

    info = { .ResourceName = "hm:text", .Name = "Text (HM64)" };
    r.Add(info, AddActorToWorld<AText>);

    info = { .ResourceName = "mk:bowser_statue", .Name = "Bowser Statue" };
    r.Add(info, AddActorToWorld<ABowserStatue>);
}

void RegisterTracks(Registry<TrackInfo>& r) {
    TrackInfo info;

    info = {
        .ResourceName = "mk:mario_raceway",
        .Name = "mario raceway",
        .DebugName = "m circuit",
        .Length = "567m",
        .MinimapTexture = minimap_mario_raceway,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<MarioRaceway>()); });

    info = {
        .ResourceName = "mk:choco_mountain",
        .Name = "choco mountain",
        .DebugName = "mountain",
        .Length = "687m",
        .MinimapTexture = minimap_choco_mountain,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<ChocoMountain>()); });

    info = {
        .ResourceName = "mk:bowsers_castle",
        .Name = "bowser's castle",
        .DebugName = "castle",
        .Length = "777m",
        .MinimapTexture = minimap_bowsers_castle,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<BowsersCastle>()); });

    info = {
        .ResourceName = "mk:banshee_boardwalk",
        .Name = "banshee boardwalk",
        .DebugName = "ghost",
        .Length = "747m",
        .MinimapTexture = minimap_banshee_boardwalk,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<BansheeBoardwalk>()); });

    info = {
        .ResourceName = "mk:yoshi_valley",
        .Name = "yoshi valley",
        .DebugName = "maze",
        .Length = "772m",
        .MinimapTexture = minimap_yoshi_valley,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<YoshiValley>()); });

    info = {
        .ResourceName = "mk:frappe_snowland",
        .Name = "frappe snowland",
        .DebugName = "snow",
        .Length = "734m",
        .MinimapTexture = minimap_frappe_snowland,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<FrappeSnowland>()); });

    info = {
        .ResourceName = "mk:koopa_beach",
        .Name = "koopa troopa beach",
        .DebugName = "beach",
        .Length = "691m",
        .MinimapTexture = minimap_koopa_troopa_beach,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<KoopaTroopaBeach>()); });

    info = {
        .ResourceName = "mk:royal_raceway",
        .Name = "royal raceway",
        .DebugName = "p circuit",
        .Length = "1025m",
        .MinimapTexture = minimap_royal_raceway,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<RoyalRaceway>()); });

    info = {
        .ResourceName = "mk:luigi_raceway",
        .Name = "luigi raceway",
        .DebugName = "l circuit",
        .Length = "717m",
        .MinimapTexture = minimap_luigi_raceway,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<LuigiRaceway>()); });

    info = {
        .ResourceName = "mk:moo_moo_farm",
        .Name = "moo moo farm",
        .DebugName = "farm",
        .Length = "527m",
        .MinimapTexture = minimap_moo_moo_farm,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<MooMooFarm>()); });

    info = {
        .ResourceName = "mk:toads_turnpike",
        .Name = "toad's turnpike",
        .DebugName = "highway",
        .Length = "1036m",
        .MinimapTexture = minimap_toads_turnpike,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<ToadsTurnpike>()); });

    info = {
        .ResourceName = "mk:kalimari_desert",
        .Name = "kalimari desert",
        .DebugName = "desert",
        .Length = "753m",
        .MinimapTexture = minimap_kalimari_desert,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<KalimariDesert>()); });

    info = {
        .ResourceName = "mk:sherbet_land",
        .Name = "sherbet land",
        .DebugName = "sherbet",
        .Length = "756m",
        .MinimapTexture = minimap_sherbet_land,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<SherbetLand>()); });

    info = {
        .ResourceName = "mk:rainbow_road",
        .Name = "rainbow road",
        .DebugName = "rainbow",
        .Length = "2000m",
        .MinimapTexture = minimap_rainbow_road,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<RainbowRoad>()); });

    info = {
        .ResourceName = "mk:wario_stadium",
        .Name = "wario stadium",
        .DebugName = "stadium",
        .Length = "1591m",
        .MinimapTexture = minimap_wario_stadium,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<WarioStadium>()); });

    info = {
        .ResourceName = "mk:block_fort",
        .Name = "block fort",
        .DebugName = "block",
        .Length = "",
        .MinimapTexture = minimap_block_fort,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<BlockFort>()); });

    info = {
        .ResourceName = "mk:skyscraper",
        .Name = "skyscraper",
        .DebugName = "skyscraper",
        .Length = "",
        .MinimapTexture = minimap_skyscraper,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<Skyscraper>()); });

    info = {
        .ResourceName = "mk:double_deck",
        .Name = "double deck",
        .DebugName = "deck",
        .Length = "",
        .MinimapTexture = minimap_double_deck,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<DoubleDeck>()); });

    info = {
        .ResourceName = "mk:dk_jungle",
        .Name = "d.k.'s jungle parkway",
        .DebugName = "jungle",
        .Length = "893m",
        .MinimapTexture = minimap_dks_jungle_parkway,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<DKJungle>()); });

    info = {
        .ResourceName = "mk:big_donut",
        .Name = "big donut",
        .DebugName = "doughnut",
        .Length = "",
        .MinimapTexture = minimap_big_donut,
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<BigDonut>()); });

    info = {
        .ResourceName = "mk:test_track",
        .Name = "test track",
        .DebugName = "test track",
        .Length = "100m",
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<TestTrack>()); });
}
