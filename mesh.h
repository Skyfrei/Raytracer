#ifndef MESH_H
#define MESH_H

#include "triangle.h"
#include "vector"
#include "vec3.h"
 
class Mesh : public Form
{
    public:
        Material mat;
        Color color;
        std::vector<Triangle> GetTriangles()
        {
            return triangles;
        }
    public:
        Mesh(){}
        Mesh(std::vector<Vec3> pos, std::vector<Vec3> norm, std::vector<Vec3> text, Material m, Color col) : mat(m), color(col)
        {
            for(int i = 0; i < pos.size() - 1; i = i + 3)
            {
                Triangle obj(pos[i], pos[i+1], pos[i+2], color, mat);
                triangles.push_back(obj);
            }
        }
        virtual bool Hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const override;



    private:
        std::vector<Triangle> triangles;
    
};
bool Mesh::Hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const
{
    return false;
}


#endif