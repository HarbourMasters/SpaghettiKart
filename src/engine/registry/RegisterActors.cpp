#include "engine/registry/RegisterContent.h"
#include "engine/CoreMath.h"
#include "engine/AllActors.h"

extern "C" {
#include "common_structs.h"
#include "actors.h"
#include "actor_types.h"
#include "assets/textures/other_textures.h"
}

/**
 * DO NOT RE-ORDER ANYTHING IN THESE FILES
 * 
 * It will result in differing Ids
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
