#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vec3.h"

class Camera
{
    public:
        point3 origin;
        point3 lower_left_corner;
        Vec3 horizontal;
        Vec3 vertical;
        Vec3 u, v, w;
        double lens_radius;
        
    public:
        Camera(){}
        Camera(point3 lookfrom, point3 lookat, Vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist) 
        {
            auto theta = deg_to_radians(vfov);
            auto viewport_height = 3 * theta;
            auto viewport_width = aspect_ratio * viewport_height;

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focus_dist * viewport_width * u;
            vertical = focus_dist * viewport_height * v;
            lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

            lens_radius = aperture / 2;

        }

        Ray get_ray(double s, double t) const 
        {

            return Ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
        }
};

#endif