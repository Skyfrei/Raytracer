#ifndef SPHERE_H
#define SPHERE_H

#include "form.h"
#include "vec3.h"
#include "ray.h"
#include "texture.h"

class Sphere : public Form
{
    public:
        point3 center;
        double radius;
        color sphereColor;
        Material mat;

    public: 
        Sphere() {}
        Sphere(point3 center, double r, color c, Material m) : center(center), radius(r), sphereColor(c), mat(m) {};

        void Texture_Sphere(Vec3& normal, double& u, double& v) const
        {
            
        }

        static void GetSphereUV(double& u, double& v, point3& p) {
            auto theta = acos(-p.y());
            auto phi = atan2(-p.z(), p.x()) + pi;

            u = phi / (2 * pi);
            v = theta / pi;
        }


        virtual bool Hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const override;

    private:
        
    
};




bool Sphere::Hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const
{
    Vec3 sv = r.Origin() - center;
    auto a = r.Direction().length_squared();
    auto halfb = dot(sv, r.Direction());
    auto c = sv.length_squared() - radius * radius;
    auto quadraticFormula = (halfb * halfb) -  (a * c);

    if (quadraticFormula < 0)
        return false;
    auto sqr = sqrt(quadraticFormula);

    auto root = (-halfb - sqr) / a;
    if (root < t_min || t_max < root)
    {
        root = (-halfb + sqr) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.At(rec.t);
    Vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.albedo = albedo;

    GetSphereUV(rec.u, rec.v, rec.p);
    rec.c = sphereColor * mat.texture.Value(rec.u, rec.v, rec.p);

    return true;

}

#endif