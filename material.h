#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"

class Material
{
    public: 
        Material(){}
        Material(Vec3 p, double expo) : phong(p), exponent(expo)
        {

        }
        Material(Vec3 p, double expo, Texture t) : phong(p), exponent(expo), texture(t)
        {

        }

    public:
        Vec3 phong;
        double exponent; 
        Texture texture;
};


#endif