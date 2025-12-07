# Migration: Beta to v1.0

This guide explains how to migrate your existing resource packs and mods from the **Beta version** to the **future first stable release (v1.0)** of SpaghettiKart.

## Overview

With the v1.0 release approaching, the YAML structure and resource paths have been reorganized to be more logical and maintainable. If you have existing mods or texture packs created for the Beta version, you will need to update them to work with v1.0 and later versions.

## Migration Script

A Python migration script ([`migration.py`](https://gist.github.com/coco875/5865b6a8e480990e4f75752206e9c728)) is provided at the root of the repository to help automate the migration process. You can customize the `folder` variable to point to your mod folder.

### Usage

1. Download the [migration.py](https://gist.github.com/coco875/5865b6a8e480990e4f75752206e9c728) script
2. Edit `migration.py` and set the `folder` variable to your mod folder name
3. Run the script:
```bash
python migration.py
```
4. The migrated mod will be created in a new folder with `_Migrated` suffix

### Generated Files

The migration script will automatically generate a `mods.toml` file at the root of your migrated mod. This file is required for SpaghettiKart to properly recognize and load your mod. See [mods.toml documentation](mods-toml.md) for more details about this file.

## Path Changes

### Textures

The main change is that textures are now organized into a `textures/` subfolder with proper categorization.

#### Track Textures

| Old Path | New Path |
|----------|----------|
| `banshee_boardwalk_data/` | `textures/tracks/banshee_boardwalk/banshee_boardwalk_data/` |
| `bowsers_castle_data/` | `textures/tracks/bowsers_castle/bowsers_castle_data/` |
| `choco_mountain_data/` | `textures/tracks/choco_mountain/choco_mountain_data/` |
| `dks_jungle_parkway_data/` | `textures/tracks/dks_jungle_parkway/dks_jungle_parkway_data/` |
| `frappe_snowland_data/` | `textures/tracks/frappe_snowland/frappe_snowland_data/` |
| `kalimari_desert_data/` | `textures/tracks/kalimari_desert/kalimari_desert_data/` |
| `koopa_troopa_beach_data/` | `textures/tracks/koopa_troopa_beach/koopa_troopa_beach_data/` |
| `luigi_raceway_data/` | `textures/tracks/luigi_raceway/luigi_raceway_data/` |
| `mario_raceway_data/` | `textures/tracks/mario_raceway/mario_raceway_data/` |
| `moo_moo_farm_data/` | `textures/tracks/moo_moo_farm/moo_moo_farm_data/` |
| `rainbow_road_data/` | `textures/tracks/rainbow_road/rainbow_road_data/` |
| `royal_raceway_data/` | `textures/tracks/royal_raceway/royal_raceway_data/` |
| `sherbet_land_data/` | `textures/tracks/sherbet_land/sherbet_land_data/` |
| `toads_turnpike_data/` | `textures/tracks/toads_turnpike/toads_turnpike_data/` |
| `wario_stadium_data/` | `textures/tracks/wario_stadium/wario_stadium_data/` |
| `yoshis_valley_data/` | `textures/tracks/yoshis_valley/yoshis_valley_data/` |

#### Kart Textures

| Old Path | New Path |
|----------|----------|
| `bowser_kart/` | `textures/karts/bowser_kart/` |
| `donkeykong_kart/` | `textures/karts/donkeykong_kart/` |
| `luigi_kart/` | `textures/karts/luigi_kart/` |
| `mario_kart/` | `textures/karts/mario_kart/` |
| `peach_kart/` | `textures/karts/peach_kart/` |
| `toad_kart/` | `textures/karts/toad_kart/` |
| `wario_kart/` | `textures/karts/wario_kart/` |
| `yoshi_kart/` | `textures/karts/yoshi_kart/` |

#### Other Textures

| Old Path | New Path |
|----------|----------|
| `boo_frames/` | `textures/boo_frames/` |
| `ceremony_data/` | `textures/ceremony_data/` |
| `common_data/` | `textures/common_data/` |
| `other_textures/` | `textures/other_textures/` |
| `player_selection/` | `textures/player_selection/` |
| `startup_logo/` | `textures/startup_logo/` |
| `texture_data_2/` | `textures/texture_data_2/` |
| `texture_tkmk00/` | `textures/texture_tkmk00/` |

## Kart Frame Textures

Kart textures have been split into separate wheel textures. Each kart frame now has 4 associated wheel textures. That makes them possible to spin.

### Old Structure
```
mario_kart_frame001.png
```

### New Structure
```
mario_kart_frame001_wheel0.png
mario_kart_frame001_wheel1.png
mario_kart_frame001_wheel2.png
mario_kart_frame001_wheel3.png
```

This applies to all 289 frames (frame000 to frame288) for each kart:
- `bowser_kart`
- `donkeykong_kart`
- `luigi_kart`
- `mario_kart`
- `peach_kart`
- `toad_kart`
- `wario_kart`
- `yoshi_kart`

The migration script automatically duplicates your original kart frame texture to all 4 wheel variants.

## New Folder Structure

The new YAML and resource structure is organized as follows:

```
yamls/us/
├── models/
│   ├── ceremony_data.yml
│   ├── common_data.yml
│   ├── data_800E8700.yml
│   ├── data_segment2.yml
│   ├── startup_logo.yml
│   └── tracks/
│       ├── banshee_boardwalk/
│       ├── big_donut/
│       ├── block_fort/
│       ├── bowsers_castle/
│       ├── choco_mountain/
│       └── ...
├── other/
│   ├── ceremony_data.yml
│   ├── common_data.yml
│   ├── startup_logo.yml
│   └── tracks/
│       └── ...
├── sound/
│   ├── banks.yml
│   ├── root.yml
│   ├── samples.yml
│   └── sequences.yml
└── textures/
    ├── boo_frames.yml
    ├── ceremony_data.yml
    ├── common_data.yml
    ├── data_800E45C0.yml
    ├── other_textures.yml
    ├── player_selection.yml
    ├── some_data.yml
    ├── startup_logo.yml
    ├── texture_data_2.yml
    ├── texture_tkmk00.yml
    ├── karts/
    │   ├── bowser_kart.yml
    │   ├── donkeykong_kart.yml
    │   ├── luigi_kart.yml
    │   ├── mario_kart.yml
    │   ├── peach_kart.yml
    │   ├── toad_kart.yml
    │   ├── wario_kart.yml
    │   └── yoshi_kart.yml
    └── tracks/
        ├── banshee_boardwalk/
        ├── bowsers_castle/
        ├── choco_mountain/
        └── ...
```

## Manual Migration

If you prefer to migrate manually:

1. Create the new folder structure in your mod
2. Move texture files according to the path mapping tables above
3. For kart textures, duplicate each frame texture to the 4 wheel variants
4. Create a `mods.toml` file at the root of your mod (see [mods.toml documentation](mods-toml.md))
5. Test your mod with the new version of SpaghettiKart

## Troubleshooting

- **Textures not loading**: Verify that your paths match exactly with the new structure
- **Missing wheel textures**: Make sure all 4 wheel variants exist for each kart frame
- **Wrong track textures**: Double-check the track name in the path (e.g., `yoshi_valley` vs `yoshis_valley`)

## See Also

- [mods.toml File Structure](mods-toml.md) - Required metadata file for mods
- [Texture Pack Guide](textures-pack.md) - How to create texture packs
- [Modding Guide](modding.md) - General modding information
