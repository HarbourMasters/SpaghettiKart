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

* Select the empty and in the object panel select `Course Root`
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

* Now you can drag using the all-axis mode (the white circle) and the points will snap to the mesh surface.
<img width="1510" height="837" alt="image" src="https://github.com/user-attachments/assets/393ae10a-4543-4155-b19f-e7f147c63651" />


* Path points are now running in the correct direction
<img width="368" height="336" alt="image" src="https://github.com/user-attachments/assets/2caf6c63-d673-4ac2-919a-34e9a452d6de" />

* Next select that last point in the path and press `E` to extrude (add) another point to the path and place it farther forward.
* Continue adding new path points until you make a complete circle.
* The average track has ~800 path points. There's no specific number required, just enough that the CPUs traverse smoothly.
  * If using the Nurbs Path, do not worry about how many path points. Just that the line connecting the points is smooth.
  * If using the bezier curve, ignore the line that connects the points. Only the points matter!
* Keep adding points until you have a complete circle
<img width="268" height="346" alt="image" src="https://github.com/user-attachments/assets/b88f1ea8-176c-47f5-bcfe-b78bfcca9248" />  

* Note that the start/end points do not need to connect to each other or anything like that. Just make them close
* If using the bezier curve you *must* place at least ten points behind the starting line in a straight line.
  * The NurbsPath requires this as well. However, you don't actually have control over the number of points. So, just make sure there's 16.8 units of space behind the finishline.

## Prepare for export
* Drag and drop the mesh and path into the empty so that they are within it. This can be done at the beginning or prior to export, either is fine.
* Hold shift while dragging an object --> place the cursor over the empty, and drop the object
<img width="158" height="90" alt="image" src="https://github.com/user-attachments/assets/e806494b-8f59-48cf-95e2-d5d1c03ba3a7" />

* Note that these objects have been named. This is not required.
* All the objects that need to be exported, must be inside the empty.

## Export
* Name the track
* Choose a mods path
* Scale should be set somewhere around 20-25. 100 results in really big models in-game
<img width="344" height="271" alt="image" src="https://github.com/user-attachments/assets/3be6300c-9125-456c-88bb-da6c231bca30" />

### Mods Path
* This should be something like `desktop/mycoolmods` or similar. All your mods will go here.
### Name
* This will determine what folders your track will be placed in.
* If you name your track `mytrack` then it will be placed in `desktop/mycoolmods/tracks/mytrack/files_are_placed_here`

* Select the empty or any object inside of the empty and click export. If it says *Success!* then you should see files in the track folder.

## Packaging
The track is now ready for packaging and import into the game for your first test!

* Find the `desktop/mycoolmods/tracks` folder and place a mods.toml file beside the tracks folder. Place the following in it and save:
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

### Game Test
* Launch the game

* Press `ESC` and enable *Debug Mode*
<img width="379" height="210" alt="image" src="https://github.com/user-attachments/assets/1a3f09ba-0743-4243-b400-2a443df78cdd" />

* Navigate away and back to the start screen and a debug menu should pop up.
Use the left/right arrow keys to switch tracks. Custom tracks are placed at the end of the list.

* Press *Launch HM64 Labs* to configure your track and place actors.
<img width="767" height="413" alt="image" src="https://github.com/user-attachments/assets/52bcde3e-0b23-4611-9417-1adb71397a5c" />

* Select your track
<img width="1301" height="275" alt="image" src="https://github.com/user-attachments/assets/5f092a60-3377-48e1-8d23-d527a0e60684" />

* Navigate to the Track Properties winow to modify track name and settings
<img width="323" height="339" alt="image" src="https://github.com/user-attachments/assets/59116891-0fca-4eb8-8050-02e70b75021d" />

### Extra info
* Goto the object menu and select a surface type (asphalt, grass, etc.)
<img width="372" height="569" alt="image" src="https://github.com/user-attachments/assets/648ae292-4d1c-4bb7-996c-5ff3ecf7310e" />

* Stuff like buildings or walls should be separate meshes to not mess with the collision generator.
* The track/pavement should be a separate mesh from the rest of the scene.
* Any number of tracks may be placed in the tracks folder
* After opening your track in the editor for the first time. A scene.json file is created in your tracks data folder.
  * If something ever goes horribly wrong you could manually edit this file or back it up to restore it later.
* If the game crashes when you try to play it. Then there is an issue with track path or mesh.
  * The game generates the collision mesh automatically.
* Lighting changes are not currently saving to the scene file yet.

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
