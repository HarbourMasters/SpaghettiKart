\page quick Quick Reference
# Quick Reference

Important details without the steps

## Template Project
* See the template below containing
  * A finishline model for use as as reference
  * A starting road (probably delete and make your own)
  * Road has enough length behind the finishline for the racers
  * A starting path

[template 1.0.blend](<template 1.0.blend>)

## Track Details
Path Points: ~800 (any number is fine)
* For best results, place path points evenly apart spacing each one by around 0.20 blender units
Triangle Count: Original tracks average ~6000 triangles
  * SpaghettiKart will start losing fps after ~100k triangles

Starting Line Width: 1.75 units  
Track Boundaries: +-32767.0 in Blender units this is +-1310.68 (32767 / 25)
  * Note that this allows a very big track

Track Widths:
* Luigi Raceway track width: 1.13 points
* Mario Raceway track width: 1.0 points
* Wide: 1.1 points to 2.0 points
* Medium: 0.8 points to 1.1 points
* Narrow: 0.5 point to 0.8 points

## The Laws of SpaghettiKart
* Track geography must be a plane, not a box
  * A flat track with a basic plane (square), needs to be turned into triangules and/or subdivided a few times, otherwise the collision generation will 'wig out', placing the racers incorrectly
* The starting line must face north
  * In Blender: Positive Green Y Axis
  * In game: Negative Z axis
* The meshes anchor needs to be center of mass or at 0,0,0
    * Otherwise the mesh will have a weird offset.
* Do not draw your path backwards (In blender turn on normals on the bezier curve to see the direction)
* The first path point is set at 0,0,0
* Recommend a scaling of 25 in the F3D Exporter window
* Must be 10 path points behind the starting line


# Collision Surface Extra Types
Colouring vertices the following colours will set these actions for that area.
* Player Tumbles: RGB(153, 0, 153)
* No Collision: RGB(0, 153, 153)
* Darkens the player: RGB(255, 0, 0)
* Out of Bounds: RGB(230, 204, 0)

