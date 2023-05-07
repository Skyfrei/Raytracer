#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "form.h"


class Triangle : public Form
{
    public:
        Material mat;
        Triangle(Point3 a, Point3 b, Point3 c, color col, Material m) : p{a, b, c}, color(col), mat(m){};
        Color color;

        virtual bool Hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const override;
        

        void GetTriangleUV(double& u, double& v, Point3& point) const
        {
            // So basically the formula to get u v and w to get each point inside the triangle is
            // u + v + w = 1
            // so w = 1 - (u + v);

            double area = cross(edge[0], edge[1]).length() / 2;
            Vec3 ap = Vec3(point.x() - p[0].x(), point.y() - p[0].y(), point.z() - p[0].z());
            Vec3 bp = Vec3(point.x() - p[1].x(), point.y() - p[1].x(), point.z() - p[1].z());
            u = cross(edge[0], ap).length() / 2;
            v = cross(edge[1], bp).length() / 2;

            double w = 1 - (u + v);
        }
        

    private:
        Vec3 p[3];
        Vec3 edge[3];
        
        
        void Calculate()
        {
            edge[0] = p[1] - p[0];   // B - A
            edge[1] = p[2] - p[1];  // C - B
            edge[2] = p[0] - p[2];  // A - C
        }
};

bool Triangle::Hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const
{
    // Calculating point of intersaction between ray and plane (same point)
    Vec3 p1p0 = p[1] - p[0];
    Vec3 p2p0 = p[2] - p[0];

    Vec3 normal = cross(p1p0, p2p0);

    double number = dot(normal, r.Direction());
    if (fabs(number) < 0)
        return false;

    double planeSlide = -1 * dot( normal, p[0]);

    double t = -1 * (dot(normal, r.Origin()) + planeSlide) / number;
    if (t < 0 )
        return false;
    
    rec.t = t;
    rec.p = r.At(rec.t);

    Vec3 c = cross(edge[0], rec.p - p[0]);
    Vec3 c1 = cross(edge[1], rec.p - p[1]);
    Vec3 c2 = cross(edge[2], rec.p - p[2]);

    if (dot(normal, c) < 0 || dot(normal, c1) < 0 || dot(normal, c2))
        return false;

    rec.normal = normal;
    rec.c = color;
    this->GetTriangleUV(rec.u, rec.v, rec.p);
    rec.c = color * mat.texture.Value(rec.u, rec.v, rec.p);

    return true;

}

#endif