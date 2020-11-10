//
// Created by eliwar-5 on 2017-11-08.
//

#ifndef GSCEPT_LAB_ENV_AABBMANAGER_H
#define GSCEPT_LAB_ENV_AABBMANAGER_H

#include <vector>
#include "BoundingBox.h"
#include "Object.h"
#include "../Math/myMathLib.h"
#include "Quad.h"

class AABBManager
{
public:
    static std::vector<Object*> objects;
    static void AddObject(Object *obj);
    //VARFÖR BEHÖVER JAG EN DUBBEL POINTER? FÖRSTÅR INTE FÖRKLARINGEN HELT HÄR : https://stackoverflow.com/questions/11842416/function-does-not-change-passed-pointer-c
    static bool RayCollision(Ray ray, Object **outResult, Vector4D *outIntersection);

private:
    AABBManager();
};

#endif //GSCEPT_LAB_ENV_AABBMANAGER_H
