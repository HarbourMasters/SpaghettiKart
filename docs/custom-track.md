# Custom Track Overview

This guide is not all-encompassing but rather covers the basics for track creation.

## Dependencies
* Blender v4.3 or older.
* Harbour Masters Fast64 https://github.com/HarbourMasters/fast64

## Configuration
* Set Fast64 Global Settings to `MK64`
* Set F3D Microcode to `F3DEX`
<img width="362" height="416" alt="image" src="https://github.com/user-attachments/assets/99b50f7c-28db-4300-b49a-892fc86ce894" />

## Adding the surface

* Add an empty and place it at coordinates 0, 0, 0
<img width="482" height="532" alt="image" src="https://github.com/user-attachments/assets/ab1475c9-9610-4735-8083-d1586bd76cd7" />

* In the object panel select `Course Root`
  * This is the root of the custom track. All mesh and path are placed within.
<img width="342" height="372" alt="image" src="https://github.com/user-attachments/assets/f1ac68f0-2beb-4078-af62-52d9925e56f4" />

* Remove the default cube
<img width="316" height="359" alt="image" src="https://github.com/user-attachments/assets/bce1d0f3-fceb-4f33-8997-6e66fb45d517" />

* Add a plane and place it at coordinates 0, 0, 0
<img width="609" height="121" alt="image" src="https://github.com/user-attachments/assets/660e0ecc-3f89-4722-b1f0-73f2e253b1fd" />
* The drivable surface *must* be a flat mesh. It cannot be a cube.
* Select the plane and scale it to a reasonable size by pressing the `s` key and dragging with the mouse.
  * Z coordinate 420.0f must have mesh under it for the players to spawn correctly.
  * This is approximately -16.8 Y in Blender units (420 / scale of 25)

* One method to test the plane size is by adding a cube
<img width="1294" height="801" alt="image" src="https://github.com/user-attachments/assets/71566a5b-06b5-4a95-829a-1f9edde3c530" />
* You will likely want the surface mesh to be a bit bigger. However, this size will meet the spawn requirements.
* Delete the cube

You now have a valid surface for racers to drive on. Multiple surfaces, hills, bumps, cubes, walls, spheres, etc. are all applicable types of mesh to add to the track. The specifics of crafting the track will be covered in another guide. Just know that there are proper ways to connect mesh together so the racers do not fall through while traversing from one mesh to another mesh.

### Adding a Path
* Allows CPUs to follow the track
* Allows game to track players progress as they navigate the track

* Add a nurbs path or bezier curve
  * The nurbs path is easier to use and outputs very smooth results
  * The steps below apply to *both* types of curve/path
 <img width="452" height="272" alt="image" src="https://github.com/user-attachments/assets/7654445c-16e9-4ae8-a328-52c727f01643" />

* Select the path
* Enter `Edit` mode (Press tab)
* Enable Normals
<img width="734" height="240" alt="image" src="https://github.com/user-attachments/assets/568e1937-be7d-4daa-9ada-f8238e9e7efe" />
* Normals shows these arrows which tell us which way is forward.
<img width="1023" height="387" alt="image" src="https://github.com/user-attachments/assets/be868cd9-1e04-4818-afa3-25c2c4031242" />
Thus, the point marked as `1` is the first path point. This must be placed at 0, 0, 0. If it were to be placed at 5, 0, 0, it would have an unexpected offset in-game.

* Move the other points farther forward so that they do not get in the way, and then set the first point at 0, 0, 0.
<img width="1061" height="623" alt="image" src="https://github.com/user-attachments/assets/89db120b-1789-45a9-80ab-01f7a11c7b86" />
* If you zoom out, this point should be placed at the very center of the plane. If it is not at the center, press `tab` to switch to `object` mode, and move the whole path to 0,0,0. Then switch back to `edit` mode and place the first path point at 0, 0, 0

* The point should now be at the center (presuming the plane is also centered at 0, 0, 0)

* This next step is incredibly important
<img width="978" height="540" alt="image" src="https://github.com/user-attachments/assets/5918325a-0b0e-46b3-b433-96bf61b2092f" />
* See the green Y in the compass at the top right corner? This shows the positive Y-axis. This is the direction players are facing when they spawn. As such, your path must go in this direction. If it goes in any other direction the CPUs will turn 90 degrees and drive off the track.

#### Path Point Alignment
* Enter *Quad View* as this allows moving points from the top-down
<img width="911" height="676" alt="image" src="https://github.com/user-attachments/assets/521756d5-e8af-4d3a-ae4e-2029cf93021d" />
* Next enable *Magnet* mode (shift-tab) and *Snap Target* set to *Face*
<img width="353" height="352" alt="image" src="https://github.com/user-attachments/assets/ae4aeeae-1744-45f2-88a0-6afb8a045ec2" />

* Now you can drag using the all-axis mode (the white circle)

* Path points now running in the correct direction


The path must be placed with positive Y as forward. At the start of the race, when you press A to go, you are facing in the positive Y direction in blender. In-game it's actually negative Z

## How to use
* Add an empty <-- All of your meshes and path will go here. Place it at coordinate 0,0,0
* Add a bezier curve <-- This will be the track path. Place it at coordinate 0,0,0

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
  * A flat track with a basic plane (square), needs to be turned into triangules and/or subdivided a few times, otherwise the collision generation will 'wig out', placing the racers incorrectly
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
