@page paths Paths
The game uses multiple types of track paths

# Track Paths
Allows CPUs to follow the track and for the game to track player progress
* Up to fourth track paths are allowed and one vehicle path
* Average track uses ~800 points. Any number is allowed.
* For best results, place path points evenly apart spacing each one by around 0.20 blender units
* The first path point must be placed at (0,0,0)

# Supported Blender paths
* Bezier Curve
* Nurbs Path (just called path)

![alt text](image-23.png)

* The nurbs path is the easiest to use.
* The track path must face positive Y in blender

## Path Type
This allows the user to select which path is which. Path 1 is required and must be the main path.

The other paths must circle the entire track, splitting off wherever, and are optional.

The vehicle path is for trains, boats, cars, etc.

![alt text](image-24.png)


See the overview guide for more details