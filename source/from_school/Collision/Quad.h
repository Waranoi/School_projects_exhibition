//
// Created by eliwar-5 on 2017-11-10.
//

#ifndef GSCEPT_LAB_ENV_QUAD_H
#define GSCEPT_LAB_ENV_QUAD_H

#include "myMathLib.h"

struct Quad
{
    Plane plane;
    Vector4D right;
    Vector4D up;
    float width;
    float height;

    Quad() {}

    Quad(Vector4D origin, Vector4D right, Vector4D up)
    {
        Vector4D normal = right.CrossProduct(up);
        normal.Normalize();

        plane = Plane(origin, normal);
        this->right = right;
        this->up = up;
        width = right.GetLength();
        height = up.GetLength();

        this->right.Normalize();
        this->up.Normalize();
    }

    bool Intersect(Ray ray, Vector4D *outResult)
    {
        if (ray.Intersect(plane, outResult)) //Get intersection point
        {
            Vector4D relativeIntersection = *outResult - plane.point;
            float x = right * relativeIntersection;
            float y = up * relativeIntersection;

            if (x >= 0 && x < width && y >= 0 && y < height) //Check if intersection point is within quad boundary
                return true;
        }
        return false;
    }
};

#endif //GSCEPT_LAB_ENV_QUAD_H
