//
// Created by eliwar-5 on 2017-12-05.
//

#ifndef GSCEPT_LAB_ENV_BROADCOLLISION_H
#define GSCEPT_LAB_ENV_BROADCOLLISION_H

#include <set>
#include <vector>
#include "Object.h"
#include <algorithm>

struct Collision
{
    Object* a;
    Object* b;

    Collision(Object *a, Object *b)
    {
        this->a = a;
        this->b = b;
    }
};

struct PotentialCollisions
{
    int low;
    int high;

    PotentialCollisions(int a, int b)
    {
        if (a < b)
        {
            this->low = a;
            this->high = b;
        }
        else
        {
            this->low = b;
            this->high = a;
        }
    }
};

struct ComparePColl
{
    bool operator() (const PotentialCollisions &x, const PotentialCollisions &y) const {
        return x.low < y.low || (x.low == y.low && x.high < y.high);
    }
};

struct FloatP
{
    float value;
    int parent;
    bool startPoint;

    FloatP(float value, int parent, bool startPoint)
    {
        this->value = value;
        this->parent = parent;
        this->startPoint = startPoint;
    }
};

struct CompareFloatP
{
    bool operator() (const FloatP &x, const FloatP &y) const {
        return x.value < y.value;
    }
};

class BroadCollision
{
public:
    BroadCollision();
    ~BroadCollision();

    std::vector<Object*> objects;

    void AddObject(Object* obj);
    std::vector<Collision> GetCollisions();
};


#endif //GSCEPT_LAB_ENV_BROADCOLLISION_H
