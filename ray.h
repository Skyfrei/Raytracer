#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray
{
    public:
        Ray(){}
        Ray(const Point3 origin, const Vec3 direction)
        {
            o = origin;
            dir = direction;
        }

        Point3 Origin() const
        {
            return o;
        }

        Vec3 Direction() const
        {
            return dir;
        }

        Point3 At(double t) const
        {
            return o + t * dir;            
        }

    private: 
        Point3 o;
        Vec3 dir;


};




#endif
