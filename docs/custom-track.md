# Custom Tracks
This guide is
* Hastily written
* Made for advanced users
* A work-in-progress

This guide is not all encompassing and may be missing important information. A better manual will be drafted in the future.

## Dependencies
* Blender v4.3 (older should work fine too).
* Harbour Masters Fast64 https://github.com/HarbourMasters/fast64


## How to use
* Add an empty <-- All of your meshes and path will go here. Place it at coordinate 0,0,0
* Add a bezier curve <-- This will be the track path. Place it at coordinate 0,0,0
* Add some mesh <-- The drivable surface should be a flat-plane, not a box.
  * Goto the object menu and select a surface type (asphalt, grass, etc.)
  * Stuff like buildings or walls should be separate meshes to not mess with the collision generator.
  * The track/pavement should be a separate mesh from the rest of the scene.

## Materials
* Youtube tutorials discuss how materials work.
* Fast64 often defaults to a CI8 palette texture, always change `Format` to `RGBA-16 bit` or `RGBA-32 bit`
The Colour Index (CI8) format can cause issues if not used correctly. It's easy to confuse this as directly below this box it says `RGBA 16-bit`
Example of incorrect texture format:
<img width="677" height="777" alt="image" src="https://github.com/user-attachments/assets/518bd16e-3d16-43f9-9767-fc73ea2ab5f8" />

Example of correct texture format
<img width="627" height="205" alt="image" src="https://github.com/user-attachments/assets/2cce483b-7fcb-435b-924e-3443e2976e95" />

## Export
* Check `Ignore Textures Restrictions` failing to do so may result in errors
<img width="318" height="315" alt="image" src="https://github.com/user-attachments/assets/60f084d3-aef4-429c-889f-2e2d74473e1a" />


* FAST64 MUST BE SET TO F3DEX MODE (The others *should* work but they don't)
* Select the empty and click the SpaghettiKart and Harbour Masters profiles

* Name: Track name here
* internal_game_path: must have `tracks/your_track_name_here`
* export_path: Choose an export location
* decomp_path: Leave blank
* Click Export

## Preparation
* Place files in folder `tracks/your_track_name_here/the_files_here`
* Right click on the folder `tracks`, and turn into stored zip archive.
  * This file should *not* be compressed.
* If you wish, you may rename this file to mod_name.o2r or mod_name.zip

## Import
* Place mod in `mods` folder beside game executable.
* Launch the game
* Enable HM64 Labs in the ESC enhancements menu
* Enable Debug Mode in the ESC Developer menu (only required if using the debug menu to play tracks)
  * If you are at the start screen, navigate away and come back, the debug menu will appear.
* In the Content Browser click `Tracks`
* Your track should be in here with an exclamation mark. Your track needs to be initialized.

## Track Initialization
* An initialized track has a scene.json file in it. The game creates this file automatically.
* In the Track Properties window setup how you want and then click on your track in the Content Browser.
* The scene file should now be generated. If the game crashes, just restart.
* If the track no longer has the exclamation mark, your scene file has successfully been created.
* The track should now show up in the debug track list.
  * You can use the debug menu to play your track.
  * You can click on your track in the Content Browser under `Tracks` to play your track.

## Playing Your Custom Track
* If the game crashes when you select it. Issue with track path or mesh is most common cause.
  * The game generates collision mesh automatically.
* Adjust the settings in the track properties to whatever.
* Add actors or more mesh using the Content Browser
* Actors/Objects and Lighting changes are not currently saving to the scene file yet.
  * You can place stuff. But it will not save.
  
# Tips

**The Laws of SpaghettiKart**
* Track geography must be a plane, not a box
  * A flat course with a basic plane (square), needs to be turned into triangules and/or subdivided a few times, otherwise the collision generation will 'wig out', placing the racers incorrectly
* The starting line must face north
  * In Blender: Positive Green Y Axis
  * In game: Negative Z axis
* The meshes anchor needs to be center of mass or at 0,0,0
    * Otherwise the mesh will have a weird offset.
* Don't draw your path backwards (In blender turn on normals on the bezier curve to see the direction)
* The first path point is set at 0,0,0
* Recommend a scaling of 20 in the F3D Exporter window
* Must be 10 path points behind the starting line

**Minimap**
* Must be named: minimap.png
  
Two options for making minimap textures:
* Make the background full black (this will be transparent in-game). Use White for the track.
  * This option is easiest to see the track.
* Use FULL alpha for the background. Use white for the track.
*Extra details:*
* Save .png as 32bit. 32x32 (other resolutions should work if needed, but recommend no bigger than 128x128)
* Turn off brush anti-aliasing.
  * Semi-transparent pixels will be rendered full white. So your lines will end up thicker.

## Extra information that may be helpful
https://github.com/DeadHamster35/Tarmac64/wiki/Troubleshooting

Applicable only to Overkart 64, but some concepts may transfer to SpaghettiKart
https://github.com/DeadHamster35/Tarmac64/wiki/Blender-Reference-Guide
