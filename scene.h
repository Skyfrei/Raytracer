#ifndef SCENE_H
#define SCENE_H

#include "vec3.h"
#include "light.h"
#include <vector>

class Scene
{
    public:
        int image_height, image_width;
        double aspect_ratio;
        int samplesPerPixel = 50;
        int depth;
        Color background;
        std::vector<ParallelLight> plights;
        std::vector<PointLight> dotlights;
        std::vector<Ray> rays;
        Color ambientLight;
        double ambientCooef = 0.2;
        Camera cam;

    public:
        Scene(){}
        Scene(int w, int h, double asc, int d, Color b) :
            image_width(w), image_height(h), aspect_ratio(asc),
            depth(d), background(b){}
        Color AmbientLight()
        {
            return ambientCooef * ambientLight;
        }



};


#endif