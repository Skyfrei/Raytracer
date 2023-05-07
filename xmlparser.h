#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "./pugixml/src/pugixml.hpp"
#include <iostream>
#include <vector>
#include "vec3.h"
#include "scene.h"
#include "light.h"
#include "sphere.h"
#include "camera.h"
#include "sphere.h"
#include "triangle.h"
#include "mesh.h"
#include "meshparser.h"

class Xmlparser
{
    public:
        char* filePath;
        Vec3 cameraPos;
        Vec3 cameraLookat;
        Vec3 cameraUp;
        int depth;
        double image_width, image_height;
        std::vector<Mesh> meshVector;

        std::vector<Sphere> spheres;
        std::vector<std::string> pathToMesh;
        Camera cam;
        Scene sc;


    public:
        Xmlparser(char* p) : filePath(p)
        {
            result = doc.load_file(filePath);
            scene = doc.child("scene");
            camera = scene.child("camera");
            light = scene.child("lights");
            surfaces = scene.child("surfaces");
            background = scene.child("background_color");


            Vec3 bgColor = Vec3(atof(background.attribute("r").value()),
                        atof(background.attribute("g").value()),
                        atof(background.attribute("b").value()));

            //Camera
            Vec3 pos = Vec3(atof(camera.child("position").attribute("x").value()),
                        atof(camera.child("position").attribute("y").value()),
                        atof(camera.child("position").attribute("z").value()));

            Vec3 lookat = Vec3(atof(camera.child("lookat").attribute("x").value()),
                        atof(camera.child("lookat").attribute("y").value()),
                        atof(camera.child("lookat").attribute("z").value()));

            Vec3 up = Vec3(atof(camera.child("up").attribute("x").value()),
                        atof(camera.child("up").attribute("y").value()),
                        atof(camera.child("up").attribute("z").value()));
        
            int depth = atof(camera.child("max_bounces").attribute("n").value());
            int angle = atof(camera.child("horizontal_fov").attribute("angle").value());
            double image_width = atof(camera.child("resolution").attribute("horizontal").value());
            double image_height = atof(camera.child("resolution").attribute("vertical").value());

            sc = Scene(image_width, image_height, image_width/image_height, depth, bgColor);


            cam = Camera(pos, lookat, up, angle, image_width / image_height, 0.2, 1000);
            sc.cam = cam;

            //Objects
            double radius, reflectance, trasmittance, refraction, factor;
            Vec3 surfPos, matColor, phong;
            int iterator;

            for (pugi::xml_node sphere = surfaces.child("sphere"); sphere; sphere = sphere.next_sibling("sphere"))
            {
                
                radius = atof(sphere.attribute("radius").value());
                surfPos = Vec3(atof(sphere.child("position").attribute("x").value()),
                        atof(sphere.child("position").attribute("y").value()),
                        atof(sphere.child("position").attribute("z").value()));
        
                auto material = sphere.child("material_solid");
                matColor = Vec3(atof(material.child("color").attribute("r").value()),
                        atof(material.child("color").attribute("g").value()),
                        atof(material.child("color").attribute("b").value()));

                phong = Vec3(atof(material.child("phong").attribute("ka").value()),
                        atof(material.child("phong").attribute("kd").value()),
                        atof(material.child("phong").attribute("ks").value()));
                auto exponent = atof(material.child("phong").attribute("exponent").value());

                refraction = atof(material.attribute("refraction").value());
                trasmittance = atof(material.attribute("transmittance").value());
                reflectance = atof(material.attribute("reflectance").value());

                auto texture = sphere.child("texture").attribute("name").value();
                
                Material m(phong, exponent);
                spheres.push_back(Sphere(surfPos, radius, matColor, m));
                iterator++;
            }

            for (pugi::xml_node mesh = surfaces.child("mesh"); mesh; mesh = mesh.next_sibling("mesh"))
            {
                Mesh meshobj;
                MeshParser mParser;
                std::string name = mesh.attribute("name").value();
                auto material = mesh.child("material_solid");

                color matColor = Vec3(atof(material.child("color").attribute("r").value()),
                        atof(material.child("color").attribute("g").value()),
                        atof(material.child("color").attribute("b").value()));
                

                auto texture = "./scenes/MarbleBeige.png";

                phong = Vec3(atof(material.child("phong").attribute("ka").value()),
                        atof(material.child("phong").attribute("kd").value()),
                        atof(material.child("phong").attribute("ks").value()));
                auto exponent = atof(material.child("phong").attribute("exponent").value());
                Material mat(phong, exponent);
                mat.texture = texture;

                mParser.ParseFile("./scenes/"+name);

                meshobj = Mesh(mParser.GetPosition(), mParser.GetNormals(), mParser.GetTexture(), mat, matColor);

                meshVector.push_back(meshobj);

                auto refraction = material.child("refraction").attribute("iof").value();
                auto reflectance = material.child("reflectance").attribute("r").value();
                auto transm = material.child("transmittance").attribute("t").value();
                
            }

            for (pugi::xml_node l = light.child("parallel_light"); l; l = light.next_sibling("parallel_light"))
            {
                Color lightColor = Vec3(atof(l.child("color").attribute("r").value()),
                        atof(l.child("color").attribute("g").value()),
                        atof(l.child("color").attribute("b").value()));

                Vec3 lightDir = Vec3(atof(l.child("direction").attribute("x").value()),
                        atof(l.child("direction").attribute("y").value()),
                        atof(l.child("direction").attribute("z").value()));

                ParallelLight plight(lightColor, lightDir);
                sc.plights.push_back(plight);
            }


            for (pugi::xml_node l = light.child("point_light"); l; l = light.next_sibling("point_light"))
            {
                Vec3 lightColor = Vec3(atof(l.child("color").attribute("r").value()),
                        atof(l.child("color").attribute("g").value()),
                        atof(l.child("color").attribute("b").value()));

                Color lightPos = Vec3(atof(l.child("position").attribute("x").value()),
                        atof(l.child("position").attribute("y").value()),
                        atof(l.child("position").attribute("z").value()));

                PointLight dlight(lightColor, lightPos);
                sc.dotlights.push_back(dlight);
            }

            for (pugi::xml_node l = light.child("ambient_light"); l; l = light.next_sibling("ambient_light"))
            {
                Vec3 lightColor = Vec3(atof(l.child("color").attribute("r").value()),
                        atof(l.child("color").attribute("g").value()),
                        atof(l.child("color").attribute("b").value()));

                sc.ambientLight = lightColor;
            }
        }



        std::vector<Sphere> GetForms()
        {
            return spheres;
        }
        std::vector<Mesh>GetMeshes()
        {
            return meshVector;
            
        }
        Camera GetCamera()
        {
            return cam;
        }

        Scene GetScene()
        {
            return sc;
        }

    private:
        pugi::xml_document doc;
        pugi::xml_parse_result result;
        pugi::xml_node scene;
        pugi::xml_node camera;
        pugi::xml_node light;
        pugi::xml_node surfaces;
        pugi::xml_node background;
};




#endif