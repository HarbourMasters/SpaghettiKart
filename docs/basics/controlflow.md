@page controlflow Codebase Overview

[TOC]

# Paradigm
The developers wrote mk64 using a state-driven paradigm. Gamestate, gamemode, course, actors, structs, and more decide which branches code should follow. As an example, a condition can check the current course to load that courses resources or run logic based on that.

# Threading
After boot, the game begins by setting up its four threads; [idle](@ref thread1_idle), [video](@ref thread3_video), [audio](@ref thread4_audio), and the [game loop](@ref thread5_game_loop).

The [idle thread](@ref thread1_idle) allows the cpu to sleep. Without it, if at any time execution of all threads were paused, the cpu would never be able to continue. The idle thread is active if all the other threads are paused.

As such, the [idle thread](@ref thread1_idle) runs the following loop: `while(true);` (it runs in a perpetual loop of nothing; sleep). In mips assembly it looks like this:
```
.L800005B8:
b     .L800005B8
nop   
```
b stands for branch which acts akin to a goto. In this case, branch to the label `.L800005B8`. This creates an infinite loop. Whenever the cpu branches it always runs the next instruction as well which is called a delay slot. This means the cpu will continuously branch then run `nop` or `no operation` with no method of escaping the loop (except for when another thread has a higher priority which means the cpu switches to that thread and stops running the idle thread).

N64 threads are ran based on priority running whichever thread holds the most of it. Threads can also pause and wait for events. Note that the N64 is not multi-threaded by modern standards as the other threads contain specific purposes which slightly differs from the concept of splitting a single program into multiple processes for efficiency.

# Overall Control Flow
order of initialisation of thread:
boot:
* init_threads:<br />
  * [idle](@ref thread1_idle), [video](@ref thread3_video), [audio](@ref thread4_audio), [game loop](@ref thread5_game_loop)<br />
* [game loop](@ref thread5_game_loop):<br />
  * [audio](@ref thread4_audio)<br />
  * [jumpTo a specific menu or race based on a gameState flag.](@ref update_gamestate)<br />
  * [profiler](@ref profiler_log_thread5_time)<br />
  * [config_gfx_pool](@ref config_gfx_pool)<br />
  * [read_controllers](@ref read_controllers)<br />
  * [game_state_handler](@ref game_state_handler)<br />
  * [endDL/vsync](@ref display_and_vsync)<br />
  * [game_state_handler](@ref game_state_handler):<br />
    * switch([gGamestate](@ref gGamestate))<br />
      * [menus](@ref update_menus) -> switch([menu](@ref gMenuSelection)) { // do menu stuff }<br />
      * [race_logic_loop](@ref race_logic_loop) -> spaghetti<br />
      * [podium_ceremony](@ref podium_ceremony_loop)<br />
      * [credits](@ref credits_loop)<br />
* [video](@ref thread3_video):<br />
  handles interaction between video/audio threads.
  handles vblanking and some elements pertaining to framebuffer
  most of all, handles which step of rendering a frame the cpu is in.
  Checks when new to start new sp tasks


If mk64 is in a menu state it will branch off to the menu code, running relevant bits of code based on more flags such as which particular menu the user is in. This will loop until the state changes to a different one such as race mode. If mk64 is in a race state, then race related code is ran and it spaghetti's off into a wide series of branches. This may include concepts such as `isLinedUp, isRacing, isRaceFinished, gotoNextCourse, isHuman, and isAI`.

This relatively primitive design could be defined as a state machine from an abstract point of view. This would differ from an OOP design that uses objects and hierarchy. You will become very familiar with this design principle as you explore the code-base. During any step of the game loop, a switch can be setup to check a flag then run code relevant to the situation. For instance, a flag can check whether a race is in-progress or complete. If in-progress set the player to human controlled. If complete, set player to AI controlled.

# Segments
mk64 code is split into three sections for now.
|Segment|desc|
|---|---|
|Main|[Menus](@ref menus.c), [audio](@ref audio), [libultra](@ref PR), rsp (include in libultra)|
|Racing|[Memory management](@ref memory.c), [courses](@ref render_courses.c), [players](player_controller.c), [actors](@ref actors.c), [skybox](@ref skybox_and_splitscreen.c), [collision](@ref collision.c), [math](@ref math_util.c) |
|Ending|[Podium ceremony](@ref ending), [credits](@ref credits.c)|

Racing segment loads after selecting a grand prix. Note that it is always reloaded preventing randomization of cpu's at the start of the first race due to no time for the random seed to actually become random.

# Code Breakdown

## Menus
|file|desc|
|---|---|
|[menus](@ref menus.c)| Menu related code |
|[save](@ref save.c)| Save related |
|[startup_logo](@ref data/startup_logo.c)|Startup logo|

## Players
Player related code resides in
|file|desc|
|---|---|
|[player_controller](@ref player_controller.c)| Applies physics to players |
|[camera](@ref camera.c)| Player cameras |

## Actors
Actor related code resides in
|file|desc|
|---|---|
|[actors](@ref actors.c)|Variety of [actors](actorsmenu.html), see link for specifics.|
|[actors_extended](@ref actors_extended.c)|Shells and bananas|
|[code_80005FD0](@ref code_80005FD0.c)| Vehicles; trains, cars, trucks, boats |

## Objects
Object related code resides in
|file|desc|
|---|---|
|[code_80057C60](@ref code_80057C60.c)|Objects|
|[update_objects](@ref update_objects.c)|Objects|
|[code_80086E70](@ref code_80086E70.c)|Objects|

Documentation of the specifics still in-progress. See [actors](actorsmenu.html) for more information.

## Courses
Course related code resides in
|file|desc|
|---|---|
|[memory](@ref memory.c)| Loads and extracts course data|
|[render_courses](@ref render_courses.c)|Renders course content|
|[common_textures](@ref common_textures.c)|Content accessible to every course such as items and portraits|

### Course Folder
[course folder link][@ref courses]
|file|desc|
|---|---|
|courses/course_name/course_data.c|Course data|
|courses/course_name/course_vertices.inc.c|Course vertices (this + _displaylists.inc.c makes the course geography)|
|courses/course_name/course_displaylists.inc.c|Course displaylists (these get compressed during compile)|
|courses/course_name/course_offsets.c|Textures used in the course|

## UI & Other 2D Screen data
|file|desc|
|---|---|
|[render_objects](@ref render_objects.c)|ItemWindows|

## Engine
|file|desc|
|---|---|
|[collision](@ref collision.c)| |
|[code_80005FD0](@ref code_80005FD0.c)| Pathing code |

## Math
Math related code resides in
|file|desc|
|---|---|
|[math_util](@ref math_util.c)| |
|[math_util_2](@ref math_util_2.c)| |  

## Ending
|file|desc|
|---|---|
|[ceremony_and_credits](@ref ceremony_and_credits.c)|Camera rails for podium ceremony and credits|
|[podium_ceremony_actors](@ref podium_ceremony_actors.c)|Fireworks and balloons for podium ceremony|
|[ceremony_data](@ref ceremony_data.c)|Trophies and podium models. Paths.|
|[credits](@ref credits.c)|Credits text and UI positions|  
