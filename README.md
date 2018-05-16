# Ray Tracer Project

This code was created for the final project of CS488: Intro to Computer Graphics. In the end, I achieved the silver medal for my project and some fancy chocolate from the professor :) (Look at proof.png).

You can find a user friendly version at ericsraytracer.wordpress.com, where I go into a bit of detail for what my ray tracer can do and how it's implemented.
For in-depth details on implementation, file and data structures, please look at report.pdf.

Otherwise all final images can be found in Final Scene folder and each subfolder has an image depicting an objective.

## Compilation:
1. When in the A5 project folder, run 'premake4 gmake' and then 'make'.
Note that A5 executable is already placed in the folder, so no need to do the above.
Then run using:
"./A5 luaFile"
where luaFile is the name of the lua scene file
2. Lab machine used was gl16.student.cs

The renders for the objectives can be found in the respective folder in "Handin" folder.
In each objective folder, there will be the pictures showcasing the objective, usually a comparison between pictures, and a lua file used to render the scene, though parts of it may be a tad bit different than the pictures, just because I reused the same lua file for each picture.

## Objectives:
What my Ray Tracer program can do:
1. Extra primitives (Cones and Cylinders)
2. Reflection
3. Refraction
4. Soft shadows
5. Glossy Reflection
6. Glossy Transmission(Refraction)
7. Bump mapping
8. Adaptive anti-aliasing
9. Animation - Look at "Final Animation with Sound" or "Final Animation Looped" in 'Objective Animation' folder
10. Final Scene

Note that due to time constraints, instead of the animation being my final scene with parts moving, I replaced the cannon and cow with simplier objects/primitives.
Also animation has 3 distinct timed sounds (cannon fire, ball bounce, cow moo) and loops a few times.

## Extra Objectives:
- Website for my ray tracer at: ericsraytracer.wordpress.com
- Added multithreading. Chart in folder showing time as multithreading.
- Motion blur (code for that in motionblurA4.cpp)
- Depth of field effect (code for that in dofA4.cpp)
- Texture mapping

