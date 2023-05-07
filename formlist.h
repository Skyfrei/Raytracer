#ifndef FORMLIST_H
#define FORMLIST_H

#include "form.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class FormList : public Form
{
    public:
        std::vector<shared_ptr<Form>> objects;

    public:
        FormList() {}
        FormList(shared_ptr<Form> object) { add(object); }

        void add(shared_ptr<Form> object) { objects.push_back(object); }
        void clear() { objects.clear(); }
        bool InShadow(const Ray& r, double t_min, double t_max, Hit_record& rec) const
        {
            Hit_record temp = rec;
            for(const auto& object : objects)
            {
                if (object->Hit(r, t_min, t_max, rec))
                {
                    rec.c = temp.c;
                    return true;
                }
            }
            return false;
        }
        
        virtual bool Hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const override;
};
bool FormList::Hit(const Ray& r, double t_min, double t_max, Hit_record& rec) const
{
    Hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->Hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif