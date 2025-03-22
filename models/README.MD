# Model Extract
this folder contain all element to extract interesting Gfx

## Instruction
you need blender between 3.6 and 4.2.1

run `make model_extract -j` to extract assets in multithreaded. And waits (for me it take around 20 minutes so it take his time).

you can run `make fast64_blender` to run blender with fast64 enable without downloading it.

## Organisation
In Tracks there is all track json and folder (once extracted). In each track folder you can find three folder objects, sections, preview, collision_sections. Objects are all assets who are use for object/actor. Sections are all sections reference in a Gfx list who are use with render_course_segments. And preview are the version of the track who are use in render_credits who the complete track or a big part (some are broken). Collision sections are just collision who are use in mk64.