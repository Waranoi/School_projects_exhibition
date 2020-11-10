//
// Created by eliwar-5 on 2017-11-08.
//

#ifndef GSCEPT_LAB_ENV_BOUNDINGBOX_H
#define GSCEPT_LAB_ENV_BOUNDINGBOX_H

#include "myMathLib.h"

struct BoundingBox
{
    //Up, Right, Forward
    Vector4D origin;
    Vector4D up;
    Vector4D right;
    Vector4D forward;

    BoundingBox()
    {
        origin = Vector4D();
        up = Vector4D(0, 1, 0);
        right = Vector4D(1, 0, 0);
        forward = Vector4D(0, 0, 1);
    }

    BoundingBox(Vector4D origin, Vector4D dimensions)
    {
        this->origin = origin;
        up = Vector4D(0, dimensions[1], 0);
        right = Vector4D(dimensions[0], 0, 0);
        forward = Vector4D(0, 0, dimensions[2]);
    }

    BoundingBox(Vector4D origin, Vector4D up, Vector4D right, Vector4D forward)
    {
        this->origin = origin;
        this->up = up;
        this->right = right;
        this->forward = forward;
    }

    BoundingBox Rotate(Matrix4D orientation)
    {
        return BoundingBox(orientation*origin, orientation*up, orientation*right, orientation*forward);
    }

    BoundingBox Rotate(Matrix4D originRotation, Matrix4D pivotRotation)
    {
        return BoundingBox(pivotRotation*origin, originRotation*up, originRotation*right, originRotation*forward);
    }
};

struct AABB //Axis Aligned Bounding Box
{
    BoundingBox base;

    AABB()
    {
        base = BoundingBox();
    }

    AABB(BoundingBox box)
    {
        base = box;
    }

    BoundingBox GetAABB(Matrix4D centerRotation, Matrix4D pivotRotation)
    {
        BoundingBox box = base.Rotate(centerRotation, pivotRotation);

        //Up, Right, Forward
        float min[3] = {box.origin[0],box.origin[1],box.origin[2]};
        float max[3] = {box.origin[0],box.origin[1],box.origin[2]};

        UpdateMinMax(box.origin + box.up ,min, max);
        UpdateMinMax(box.origin + box.up + box.right ,min, max);

        UpdateMinMax(box.origin + box.right ,min, max);
        UpdateMinMax(box.origin + box.right + box.forward ,min, max);

        UpdateMinMax(box.origin + box.forward ,min, max);
        UpdateMinMax(box.origin + box.forward + box.up ,min, max);
        UpdateMinMax(box.origin + box.forward + box.up + box.right ,min, max);

        return BoundingBox(Vector4D(min[0],min[1],min[2]), Vector4D(max[0]-min[0],max[1]-min[1],max[2]-min[2]));
    }

private:
    void UpdateMinMax(Vector4D corner, float *min, float *max)
    {
        for (int i = 0; i < 3; i++)
        {
            if (corner[i] < min[i])
                min[i] = corner[i];
            else if (corner[i] > max[i])
                max[i] = corner[i];
        }
    }
};

#endif //GSCEPT_LAB_ENV_BOUNDINGBOX_H
