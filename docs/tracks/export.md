\page export Export
# Export

## Quick Overview
A basic track needs the following:
* A surface mesh built using a plane (a flat square, not a cube)
  * Large enough to encompass the spawn area from (0,0,0) to (0, -16.8, 0) in blender, in-game (0, 0, -420)
* A path built using a Path or bezier curve


## Setup
* Naming the objects in blender is not required.
* To export a track all the objects need to be placed inside the empty like so:
![](image-1.png)

* This can be done by dragging mesh or path into the empty while holding `SHIFT`

## Export Panel
![alt text](image-2.png)

Featureset
* Must be set to HM64
Name
* The name of your track
Mods Path
* The mods folder where the data will output too
  * `desktop/mycoolmods` will output as `desktop/mycoolmods/tracks/mytrackname/stuff_here`
Export
* Press this button to export the track!

## Packaging

* Find the `desktop/mycoolmods/tracks` folder and place a mods.toml file beside the tracks folder. Place the following in mods.toml
```toml
[mod]
name = "mymod"
version = "1.0.0"
```
* Highlight the tracks folder and the mods.toml file.
<img width="120" height="74" alt="image" src="https://github.com/user-attachments/assets/8842a528-ef67-4ae6-81cd-802f133bbea1" />

* Right-click --> *Add To Archive* and turn into a stored zip archive.
  * This file should *not* be compressed.
* If you wish, you may rename this file to mod_name.o2r or mod_name.zip

If you open the zip folder, you should immediately see the `tracks` folder and the mods.toml file. Inside `tracks` should be a folder with the name of your track, and some files inside of that folder.

If it does not contain any files similar to the below then something has gone wrong.

<img width="289" height="466" alt="image" src="https://github.com/user-attachments/assets/ccef558b-ac9a-42ed-bc85-e27da4f16598" />

* If all checks out, go to your game executable
* Add a `mods` folder. Drag and drop your mod into the mods folder.
* Any number of tracks may be placed in the tracks folder
  * This allows map packs