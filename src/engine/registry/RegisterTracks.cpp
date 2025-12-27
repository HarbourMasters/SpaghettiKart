#include "engine/registry/RegisterContent.h"
#include <memory>

#include "port/Game.h"
#include "engine/AllTracks.h"

/**
 * DO NOT RE-ORDER ANYTHING IN THESE FILES
 * 
 * It will result in differing Ids
 */

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
        .ResourceName = "mk:koopa_troopa_beach",
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
        .ResourceName = "hm:test_track",
        .Name = "test track",
        .DebugName = "test track",
        .Length = "100m",
    };

    r.Add(info, []() { GetWorld()->SetCurrentTrack(std::make_unique<TestTrack>()); });
}
