#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"

struct Light
{
    Color color;
    double intensity = 10;
};

class ParallelLight : virtual public Light
{
    public:
        Vec3 direction;

        ParallelLight(Color c, Vec3 dir) : direction(dir)
        {
            color = c;
        }
        
};

class PointLight : virtual public Light
{
    public: 
        Point3 origin;

        PointLight(Color c, Point3 o)
        {
            color = c;
            origin = o;
        }
};

#endif