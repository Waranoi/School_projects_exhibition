//program
// Created by eliwar-5 on 2017-11-08.
//

#include "AABBManager.h"

std::vector<Object*> AABBManager::objects;

void AABBManager::AddObject(Object *obj)
{
    objects.push_back(obj);
}

bool AABBManager::RayCollision(Ray ray, Object **outResult, Vector4D *outIntersection)
{

    float distance = 10000;
    Vector4D intersection;
    Object *intersectedWith;
    //Get AABB for each object
    for (int i = 0; i < objects.size(); i++)
    {
        BoundingBox box = objects[i]->GetAABB();

        //Test for intersection with each face in the AABB
        Quad quads[6];
        quads[0] = Quad(box.origin, box.forward, box.up);
        quads[1] = Quad(box.origin + box.forward, box.right, box.up);
        quads[2] = Quad(box.origin + box.forward + box.right, box.forward * -1, box.up);
        quads[3] = Quad(box.origin + box.right, box.right * -1, box.up);
        quads[4] = Quad(box.origin, box.right, box.forward);
        quads[5] = Quad(box.origin + box.up, box.forward, box.right);

        Vector4D result;
        for (int j = 0; j < 6; j++)
        {
            if (quads[j].Intersect(ray, &result))
            {
                float d = (result - ray.point).GetLength();
                if (d < distance)
                {
                    distance = d;
                    intersection = result;
                    intersectedWith = objects[i];
                }
            }
        }
    }

    if (distance < 10000)
    {
        *outIntersection = intersection;
        *outResult = intersectedWith;
        //intersectedWith->InvertColor();
        return true;
    }
    else
        return false;
}

AABBManager::AABBManager() {}