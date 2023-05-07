What ive implemented

I took the opportunity to start over after the previous 3a assignement and removed most uneccessary stuff. So i had to rewrite everything start to finish from 3a
after realizing that i needed 40% of the overall points and not just 40% on 1 of the assignements.


What I have redone and done in 3b
-   Texture mapping spheres
-   Rendering triangles and meshes
-   Reflectance and refraction
-   Importing dotlights and parallel lights (different functions, i left them both in ray_color and boom_color)
-   Shading 

What i would have liked to add:
-   The mesh parser is not perfect (actually, im not sure why its not working properly :( )
-   Triangles are rendered nicely alone (there is a commented line which adds a random triangle in Task3, but im unsure why when mesh rendered the mesh looks bad or incomplete)
-   Because i didn't have the time i had to implement reflectance and refraction without taking into consideration object cooeficients
    so, i just made them all the same coeficient of 0.8


Classes:
-   Xmlparser
    -   Parses the xml
-   Meshparser
    -   Parses the mesh received as argument from xmlparser and returns triangles
-   stb_image
    -   Is just an external library i use to load images for texture mappign spheres
-   The rest of the classes are a modification of what previously existed or a completely new implementation.
-   The only thing i have stuck with from the previous assignement are the Vec3 class everything else is pretty much changed to the core


I would have honestly loved to have worked more on this assignement but sadly I had a really busy schedule as I have had quite a few exams
and also have been changing my home. Although unimportant, I thought id write it down :D