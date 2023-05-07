#ifndef MESHPARSER_H
#define MESHPARSER_H

#include <iostream>
#include <fstream>
#include <vector>

inline std::vector<std::string> SplitString(std::string line, std::string delimiter)
{
    std::vector<std::string> strVec;
    std::string str; 
    for(int i = 0; i < line.length(); i++)
    {
        if (std::string(1,line[i]) == delimiter)
        {
            strVec.push_back(str);
            str = "";
            continue;
        }
        if (i == line.length() - 1)
        {
            str += line[i];
            strVec.push_back(str);
        }
        str += line[i];
    }

    return strVec;
}


class MeshParser
{
    public:
        MeshParser(){}

        std::vector<Vec3> GetPosition()
        {
            std::vector<Vec3> temp = positionCoords;
            positionCoords.clear();
            return temp;
        }
        std::vector<Vec3> GetNormals()
        {
            std::vector<Vec3> temp = normals;
            normals.clear();
            return temp;
        }
        std::vector<Vec3> GetTexture()
        {
            std::vector<Vec3> temp = textures;
            textures.clear();
            return temp;
        }
        
        void ParseFile(std::string filePath)
        {
            std::string l;
            std::ifstream myFile;
            std::vector<std::string> vec;
            myFile.open(filePath);
            std::vector<Vec3> pc;
            std::vector<Vec3> norm;
            std::vector<Vec3> tex;

            for (std::string line; getline(myFile, line);)
            {
                vec.clear();

                std::string fL = std::string(1, line[0]);
                std::string sL = std::string(1, line[1]);
                if (sL != " ") fL += sL;
                if (fL != "f" && fL != "vn" && fL != "vt" && fL != "v")
                    continue;
                else
                {
                    vec = SplitString(line, " "); 
                    vec.erase(vec.begin());

                    if (fL == "v")
                    {
                        Point3 point(std::stod(vec[0]), std::stod(vec[1]), std::stod(vec[2]));
                        pc.push_back(point);
                        continue;
                    }
                    else if (fL == "vn")
                    {
                        Point3 point(std::stod(vec[0]), std::stod(vec[1]), std::stod(vec[2]));
                        norm.push_back(point);
                        continue;
                    }
                    else if (fL == "vt")
                    {
                        Point3 point(std::stod(vec[0]), std::stod(vec[1]), 0);
                        tex.push_back(point);
                        continue;
                    }
                    else if (fL == "f")
                    {
                        int a = vec.size();
                        for (int j = 0; j < a; j++)
                        {
                            auto f = SplitString(vec[j], "/");
                             
                            positionCoords.push_back(pc[std::stod(f[0]) - 1]);
                            textures.push_back(tex[std::stod(f[1]) - 1]);
                            normals.push_back(norm[std::stod(f[2]) - 1]);
                        }
                    }
                }
            }
        }

    private:
        std::vector<Vec3> positionCoords;
        std::vector<Vec3> normals;
        std::vector<Vec3> textures;
};

#endif