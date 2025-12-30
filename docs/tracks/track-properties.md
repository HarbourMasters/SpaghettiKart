\page track-properties Track Properties
# Track Properties

Track Props contains the track data that the modern C++ interface sends into the C game engine. This allows complete configuration of the track from lighting, skybox, and environment, to CPU pathing, minimap, and music.

## Overview
The following settings are available. Most of these are straight forward.

![alt text](image-20.png)
# Environment
Light 1 is unused. Use Light 2.

## AI
This allows adjusting how precise the CPUs steering is, and the separation between CPUs.

* This section sets AI distance * lap. The first 8 elements are first lap 1, the next 8 lap 2, etc. The first element is for the player in-first, etc.

![alt text](image-19.png)

## Random Junk
This section effects the speed of the CPUs. Whether they should slow down or speed up in corners, and how fast they should go when they are not on the track.

![alt text](image-21.png)
