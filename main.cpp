#include "vec3.h"
#include "camera.h"
#include "scene.h"
#include "sphere.h"
#include "formlist.h"
#include "xmlparser.h"
#include "triangle.h"
#include "meshparser.h"
#include "mesh.h"
#include "loader.h"
#include <iostream>

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(g * scale);
    b = sqrt(b * scale);

    // Red - Green - Blue 
    out << static_cast<int> (255.999 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(255.999 * clamp(b, 0.0, 0.999)) << '\n';
}

Vec3 Reflect(Vec3& normalizedVec, Vec3& normal)
{
    return normalizedVec - 2 * dot(normalizedVec, normal) * normal;
}

color Illuminate(const Ray& r, Hit_record& rec)
{
    Vec3 closeness = std::max(0.0, dot(rec.normal, - 1 *r.Direction())) * rec.c;
    return closeness;
}

Vec3 Refract(const Vec3& ray, Vec3& normal, double matCoo)
{
        double cos = clamp(-1, 1, dot(ray, normal));
        double e = 1;
        double e2 = matCoo;
        Vec3 tempnormal = normal;
        if (cos < 0) { cos = -cos; }
        else
        {
            double tempE = e2;
            e2 = e;
            e = tempE;
            tempnormal = -normal;
        }
        double theta = e / e2;
        double k = 1 - theta * theta * (1 - cos * cos);
        return k < 0 ? Vec3(0, 0, 0) : theta * ray + (theta * cos - sqrt(k)) * tempnormal;
}

color boom_color(const Ray& r, const FormList& world, int depth, Scene& sc)
{
    Hit_record rec;
    Color clr;
    Color reflectedColor;
    Color refractedColor;

    if (world.Hit(r, 0.001, infinity, rec))
    {
        if (depth <= 0)
            return clr;
        for(int i = 0; sc.dotlights.size(); i++)
        {
            Ray reverseRay(sc.dotlights[i].origin, -1 * (sc.dotlights[i].origin - rec.p));
            if (world.InShadow(reverseRay, 0.001, reverseRay.Direction().length(), rec) == true)
            {
                Vec3 shadowClr = Illuminate(reverseRay, rec);
                return 0.4 * sc.AmbientLight() * shadowClr;
            }
            else
            {
                clr += Illuminate(Ray(sc.dotlights[i].origin, sc.dotlights[i].origin - rec.p), rec) * sc.dotlights[i].color;
                Ray reflected(rec.p, reflect(sc.dotlights[i].origin - rec.p, rec.normal));  
                reflectedColor += 0.8 * boom_color(reflected, world, depth - 1, sc);

                Ray refracted(rec.p, Refract(sc.dotlights[i].origin - rec.p, rec.normal, 0.7));
                clr += 0.8 * boom_color (refracted, world, depth - 1, sc);
            }
        }
    }
    else
        return sc.background;

    return (clr + reflectedColor) * sc.AmbientLight();
}


// Returns a vector duh
color ray_color(const Ray& r, const FormList& world, int depth, color background, Scene& sc)
{
    Hit_record rec;
    Ray ParallelRay;
    std::vector<Ray> reverseRays;
    std::vector<Ray> pointRay;
    std::vector<Ray> reflectedRay;
    Vec3 diffuse;
    Color clr;
    bool allfalse = false;

    
    if (depth <= 0)
        return background;

    //r instead of pointray
    if (!world.Hit(r, 0.001, infinity, rec))
        //return background;

    if (sc.dotlights.size() > 0)
    {
        for(int i = 0; i < sc.dotlights.size(); i++)
        {
            pointRay.push_back(Ray(sc.dotlights[i].origin, sc.dotlights[i].origin - rec.p));

            reverseRays.push_back(Ray(rec.p, pointRay[i].Direction() * -1));
        }
    }




    double closeness = 0;
    if (sc.plights.size() > 0)
        closeness = std::max(0.0, dot(rec.normal,-1 * ParallelRay.Direction()));

    for (int i = 0; i < pointRay.size(); i++)
        closeness += std::max(0.0, dot(rec.normal, -1 * pointRay[i].Direction()));


    //Parallel light
    if (sc.plights.size() > 0)
    {
        ParallelRay = Ray(Point3(infinity, infinity, infinity), sc.plights[0].direction);
        //Reverse rays for shadows
        reverseRays.push_back(Ray(rec.p, ParallelRay.Direction() * -1));
    }


    // Reflection
    for(int i = 0; i < reverseRays.size(); i++)
    {
        if (world.InShadow(reverseRays[i], 0.001, infinity, rec) == true)
        {
            closeness += std::max(0.0, dot(rec.normal, reverseRays[i].Direction()));
            if (i == reverseRays.size() - 1)
                return rec.c * 0.4 * sc.AmbientLight() * closeness + clr;
        }
    }     

    //Reflection
    for (int i = 0; i < pointRay.size(); i++)
    {
        Vec3 reflected = reflect(pointRay[i].Direction(), rec.normal);
        reflectedRay.push_back(Ray(rec.p, reflected));
        // clr += 0.8 * ray_color(pointRay[i], world, depth - 1, background, sc);

        clr += ray_color(Ray(rec.p ,reflected), world, depth -1, background, sc);
    }


    return (sc.AmbientLight() * rec.c * closeness + clr );
}

FormList Task3(Camera& cam, Scene& sc)
{
    char filepath[255];
    std::cin>>filepath;

    Xmlparser parser(filepath);

    FormList world;
    //Get forms from parser
    
    cam = parser.GetCamera();
    sc = parser.GetScene();
    sc.cam = cam;
    
    for(int i = 0; i < parser.GetForms().size(); i++)
    {
        Sphere s = parser.spheres[i];
        s.mat.texture.LoadTexture("./scenes/mramor6x6.png");
        world.add(make_shared<Sphere>(s));
    }

    
    Material matt(Vec3(0.2, 0.3, 0.5), 200);
    matt.texture.LoadTexture("./scenes/mramor6x6.png");
    //world.add(make_shared<Triangle>(Point3(-1.0, 1, -5.0), Point3(0, 0.7, -3.0), Point3(3.0, 0.4, -4), color(234, 200, 0), matt));
    
    for (int i = 0; i < parser.GetMeshes().size(); i++)
    {
            std::vector<Triangle> obj = parser.GetMeshes()[i].GetTriangles();
            for (int t = 0; t < obj.size(); t++)
                world.add(make_shared<Triangle>(obj[t]));
    }
    //
    return world;
}

FormList Task4(Camera& cam, Scene& sc)
{
    char filepath[255];
    std::cin>>filepath;

    Xmlparser parser(filepath);

    FormList world;
    //Get forms from parser
    
    cam = parser.GetCamera();
    sc = parser.GetScene();
    sc.cam = cam;
    
    
    Material matt(Vec3(0.2, 0.3, 0.5), 200);
    matt.texture.LoadTexture("./scenes/mramor6x6.png");
    //world.add(make_shared<Triangle>(Point3(-1.0, 1, -5.0), Point3(0, 0.7, -3.0), Point3(3.0, 0.4, -4), color(234, 200, 0), matt));
    
    for (int i = 0; i < parser.GetMeshes().size(); i++)
    {
            std::vector<Triangle> obj = parser.GetMeshes()[i].GetTriangles();
            for (int t = 0; t < obj.size(); t++)
                world.add(make_shared<Triangle>(obj[t]));
    }
    //
    return world;
}

int main()
{
    
    const int samples_per_pixel = 50;

    //World
    //auto world = Task2();
    
    Camera cam;
    Scene sc;


    FormList world = Task3(cam, sc);


    const auto aspect_ratio = sc.image_width / sc.image_height;
    const int image_width = sc.image_width;
    const int image_height = sc.image_height;
    const int max_depth = sc.depth;

    std::cout<<"P3\n" << image_width << ' ' << image_height << "\n255\n";
    

    for (int j = image_height - 1; j >= 0; j--)
    {
        std::cerr<< "\r ScanLines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; i++)
        {
            color pixel_color(0, 0, 0);
            for(int s = 0; s < samples_per_pixel; s++)
            {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);

                Ray r = cam.get_ray(u, v);

                pixel_color += boom_color(r, world, max_depth, sc);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr<< "\nDone\n";

    return 0;
}