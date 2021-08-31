//
// Created by eliwar-5 on 2017-12-05.
//

#include "BroadCollision.h"

BroadCollision::BroadCollision()
{
    objects = std::vector<Object*>();
}

BroadCollision::~BroadCollision()
{

}

void BroadCollision::AddObject(Object *obj)
{
    objects.push_back(obj);
}

std::vector<Collision> BroadCollision::GetCollisions()
{
    std::multiset<FloatP, CompareFloatP> queueX = std::multiset<FloatP, CompareFloatP>();
    std::multiset<FloatP, CompareFloatP> queueY = std::multiset<FloatP, CompareFloatP>();
    std::multiset<FloatP, CompareFloatP> queueZ = std::multiset<FloatP, CompareFloatP>();
    std::multiset<FloatP, CompareFloatP>::iterator itQueue;

    for (int i = 0; i < objects.size(); i++)
    {
        BoundingBox aabb = objects[i]->GetAABB();

        //Add end and start point of a line parallel to the x-axis
        queueX.insert(FloatP(aabb.origin[0], i, true));
        queueX.insert(FloatP((aabb.origin + aabb.right)[0], i, false));

        //Add end and start point of a line parallel to the y-axis
        queueY.insert(FloatP(aabb.origin[1], i, true));
        queueY.insert(FloatP((aabb.origin + aabb.up)[1], i, false));

        //Add end and start point of a line parallel to the y-axis
        queueZ.insert(FloatP(aabb.origin[2], i, true));
        queueZ.insert(FloatP((aabb.origin + aabb.forward)[2], i, false));
    }

    std::set<PotentialCollisions, ComparePColl> collisions = std::set<PotentialCollisions, ComparePColl>();
    std::set<PotentialCollisions, ComparePColl> pColl = std::set<PotentialCollisions, ComparePColl>();
    std::set<PotentialCollisions, ComparePColl>::iterator itColl;

    std::set<int> candidates = std::set<int>();
    std::set<int>::iterator itCandid;

    //Check collision in x-axis
    for (itQueue = queueX.begin(); itQueue != queueX.end(); ++itQueue)
    {
        FloatP f = *itQueue; // Note the "*" here
        if (f.startPoint == true)
        {
            for (itCandid = candidates.begin(); itCandid != candidates.end(); ++itCandid)
            {
                collisions.insert(PotentialCollisions(f.parent, *itCandid));
            }
            candidates.insert(f.parent);
        }
        else
        {
            candidates.erase(f.parent);
        }
    }

    //Check collision in y-axis
    for (itQueue = queueY.begin(); itQueue != queueY.end(); ++itQueue)
    {
        FloatP f = *itQueue; // Note the "*" here
        if (f.startPoint == true)
        {
            for (itCandid = candidates.begin(); itCandid != candidates.end(); ++itCandid)
            {
                PotentialCollisions p = PotentialCollisions(f.parent, *itCandid);
                if (collisions.find(p) != collisions.end()) //If p already is in the collisions set, i.e. if p also intersects through the x-axis then add it as a potential collision
                    pColl.insert(p);
            }
            candidates.insert(f.parent);
        }
        else
        {
            candidates.erase(f.parent);
        }
    }
    collisions = pColl;
    pColl.clear();

    //Check collision in z-axis
    for (itQueue = queueZ.begin(); itQueue != queueZ.end(); ++itQueue)
    {
        FloatP f = *itQueue; // Note the "*" here
        if (f.startPoint == true)
        {
            for (itCandid = candidates.begin(); itCandid != candidates.end(); ++itCandid)
            {
                PotentialCollisions p = PotentialCollisions(f.parent, *itCandid);
                if (collisions.find(p) != collisions.end()) //If p already is in the collisions set, i.e. if p also intersects through the x and y-axis then add it as a potential collision
                    pColl.insert(p);
            }
            candidates.insert(f.parent);
        }
        else
        {
            candidates.erase(f.parent);
        }
    }
    collisions = pColl;


    std::vector<Collision> result = std::vector<Collision>();
    for (itColl = collisions.begin(); itColl != collisions.end(); ++itColl)
    {
        Collision c = Collision(objects[itColl->low], objects[itColl->high]);
        result.push_back(c);
    }

    return result;
}
