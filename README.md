[comment]: <> (Todo: Make Light Mode Image)
[comment]: <> (Todo: Make Dark Mode Image)

# SpaghettiKart

Developed by: 
* [MegaMech](https://www.github.com/MegaMech)
* [Coco](https://www.github.com/coco875)

## Discord

Official Discord: https://discord.com/invite/shipofharkinian

If you're having any trouble after reading through this `README`, feel free ask for help in the SpaghettiKart Support text channels. Please keep in mind that we do not condone piracy.

# Quick Start

SpaghettiKart does not include any copyrighted assets.  You are required to provide a supported copy of the game.

### 1. Verify your ROM dump
The supported ROM is the USA 1.1 Rev A version. You can verify you have dumped a supported copy of the game by using the SHA-1 File Checksum Online at https://www.romhacking.net/hash/. The hash for a US 1.0 ROM is SHA-1: 579C48E211AE952530FFC8738709F078D5DD215E.

### 2. Verify your ROM is in .z64 format
Your ROM needs to be in .z64 format. If it's in .n64 format, use the following to convert it to a .z64: https://hack64.net/tools/swapper.php

### 2. Download SpaghettiKart from [Releases](https://github.com/HarbourMasters/SpaghettiKart/releases)

### 3. Generating the O2R from the ROM
#### Windows
* Extract every file from the zip into a folder of your choosing.
* Copy your ROM to the root of the folder you extracted the zip to.
* Run "generate_o2r.bat"

#### MacOS
* Extract every file from the zip into a folder of your choosing.
* Copy your ROM to the root of the folder you extracted the zip to.
* Run "generate_o2r.sh"

### 4. Play!
* Launch `SpaghettiKart.exe`
Congratulations, you are now sailing with SpaghettiKart! Have fun!

# Configuration

### Default keyboard configuration
| N64 | A | B | Z | Start | Analog stick | C buttons | D-Pad |
| - | - | - | - | - | - | - | - |
| Keyboard | X | C | Z | Space | WASD | Arrow keys | TFGH |

### Other shortcuts
| Keys | Action |
| - | - |
| F1 | Toggle menubar |
| F11 | Fullscreen |
| Tab | Toggle Alternate assets |
| Ctrl+R | Reset |

### Graphics Backends
Currently, there are three rendering APIs supported: DirectX11 (Windows), OpenGL (all platforms), and Metal (macOS). You can change which API to use in the `Settings` menu of the menubar, which requires a restart.  If you're having an issue with crashing, you can change the API in the `spaghettify.cfg.json` file by finding the line `"Backend":{`... and changing the `id` value to `3` and set the `Name` to `OpenGL`. `DirectX 11` with id `2` is the default on Windows. `Metal` with id `4` is the default on macOS.

# Custom Assets
Custom assets are packed in `.o2r` or `.otr` files. To use custom assets, place them in the `mods` folder.

If you're interested in creating and/or packing your own custom asset `.o2r`/`.otr` files, check out the following tools:
* [**retro - OTR and O2R generator**](https://github.com/HarbourMasters64/retro)
* [**fast64 - Blender plugin (Note that SF64 is not supported at this time)**](https://github.com/HarbourMasters/fast64)

# Development
### Building

If you want to manually compile SpaghettiKart, please consult the [building instructions](https://github.com/HarbourMasters/SpaghettiKart/blob/main/docs/BUILDING.md).

### Playtesting
If you want to playtest a continuous integration build, you can find them at the links below. Keep in mind that these are for playtesting only, and you will likely encounter bugs and possibly crashes. 

* [Windows](https://nightly.link/HarbourMasters/SpaghettiKart/workflows/main/main/spaghettify-windows.zip)
* [macOS](https://nightly.link/HarbourMasters/SpaghettiKart/workflows/main/main/spaghettify-mac-x64.zip)
* [Linux](https://nightly.link/HarbourMasters/SpaghettiKart/workflows/main/main/SpaghettiKart-linux.zip)


<a href="https://github.com/Kenix3/libultraship/">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="./docs/poweredbylus.darkmode.png">
    <img alt="Powered by libultraship" src="./docs/poweredbylus.lightmode.png">
  </picture>
</a>