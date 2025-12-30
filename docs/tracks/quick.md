\page quick Quick Reference
# Quick Reference

Important details without the steps

## Track Details
Path Points: ~800 (any number is fine)
Triangle Count: Original tracks average ~6000 triangles
  * SpaghettiKart will start losing fps after ~100k triangles

Starting Line Width: 1.75 units  
Track Boundaries: +-32767.0 in Blender units this is +-1310.68 (32767 / 25)
  * Note that this allows a very big track

Track Widths:
* Wide: 1.5 points to 2.7 points.
* Medium: 1.5 points to 1 point.
* Narrow: 1 point to 0.5.*

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

