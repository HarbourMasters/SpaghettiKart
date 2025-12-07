# Modding
* Very focused on ease of modding without weird formats or obscure tools.
* Early stages of modding and as such some features are not fully implemented yet
* SpaghettiKart does not use Retro

## General Structure

Mods in SpaghettiKart are packaged as `.o2r` or `.zip` archives, or as folders placed in the `mods/` directory.

```
mods/
├── MyTexturePack.o2r
├── CustomTracks.zip
└── MyDevMod/           <- Folder-based mod for development
    ├── mods.toml
    └── textures/
```

Every mod **must** include a `mods.toml` file at its root. This file contains metadata about the mod (name, version, dependencies). See [mods.toml File Structure](mods-toml.md) for details.

### Supported Formats

| Format | Extension | Description |
|--------|-----------|-------------|
| O2R Archive | `.o2r` | Recommended format for distribution |
| ZIP Archive | `.zip` | Standard zip file |
| Folder | - | Useful for development |
| Disabled | `.disabled` | Renamed archives are skipped |

### Mod Loading Order

Mods are loaded in dependency order. If mod A depends on mod B, then B will be loaded first. Mods loaded later can override resources from earlier mods.

## Getting Started

* [mods.toml File Structure](mods-toml.md) - Required metadata file for all mods
* [Migration Guide](migrations.md) - Migrate existing mods to the new structure

## Mod Types

* Textures Pack
    * Some texture might not be possible to change yet, but most of them can be changed.
    * [link](textures-pack.md)
* Custom Tracks (CT)
    * Custom tracks are reserved for advanced users. We plan to simplify the process in the future.
    * [link](custom-track.md)
* Custom Characters
    * Custom characters can only replace existing characters for now. We plan to allow to add new characters in the future.
    * [link](custom-characters.md)
* Custom Audio
    * Only custom sequences are supported, not custom samples (like voices or sound effects).
    * [link](custom-audio.md)
* Scripting
    * Add logic to the game are not possible yet but we plan to add support for scripting in the future. We will use [WebAssembly](https://en.wikipedia.org/wiki/WebAssembly) with [component model](https://component-model.bytecodealliance.org/) this will allow you to choose your language (Rust, C, C++, Python, JS, etc.) and compile it to WebAssembly.
* 3D Models For Characters
    * Custom 3D models for characters are not supported yet. Put high quality character can be really close of 3d model but easily add a lot of size to the mod. We plan to add support for custom 3D models in the future.
* Custom Sounds
    * Custom sounds are not supported yet. We plan to add support for custom sounds in the future.
* Custom Menus
    * Custom menus are not supported yet. We plan to add support for custom menus in the future.
