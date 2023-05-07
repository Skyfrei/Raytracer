#ifndef FORM_H
#define FORM_H

#include "material.h"
#include "ray.h"


struct Hit_record
{
    point3 p;
    Vec3 normal;
    Color c;
    double t;
    bool front_face;
    bool inShadow = false;
    double u;
    double v;
    Vec3 albedo;
    Material mat;


    inline void set_face_normal(const Ray& r, const Vec3& outward_normal)
    {
        front_face = dot(r.Direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
    
    
};

class Form
{
    public:
        Vec3 albedo = Vec3(0.15, 0.15, 0.15);
        virtual bool Hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const = 0;
};

#endif