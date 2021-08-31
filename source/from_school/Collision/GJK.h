//
// Created by eliwar-5 on 2017-12-08.
//

#ifndef GSCEPT_LAB_ENV_GJKCHECK_H
#define GSCEPT_LAB_ENV_GJKCHECK_H

#include "myMathLib.h"

struct SupportPoint
{
    Vector4D vector;
    Vector4D suppA;

    SupportPoint(){}

    SupportPoint(Vector4D vector, Vector4D suppA)
    {
        this->vector = vector;
        this->suppA = suppA;
    }
};

struct Face
{
    SupportPoint a;
    SupportPoint b;
    SupportPoint c;

    Vector4D normal;
    float distance;

    Face(SupportPoint a, SupportPoint b, SupportPoint c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        normal = Vector4D(0,0,0);
        distance = -1;
    }
};

struct Edge
{
    SupportPoint a;
    SupportPoint b;

    Edge(){}

    Edge(SupportPoint a, SupportPoint b)
    {
        this->a = a;
        this->b = b;
    }
};

struct GJK
{
    static bool CheckCollision(std::vector<Vector4D> a, std::vector<Vector4D> b, Vector4D &outCollisionPoint, Vector4D &outPenetrationVector, float &outPenetrationDepth)
    {
        std::vector<SupportPoint> simplex = std::vector<SupportPoint>(); //List of simplex points
        Vector4D d = Vector4D(1,0,0); //Initial search direction

        simplex.push_back(Support(a,b,d)); //Add the first point to the simplex
        d = simplex[0].vector * -1; //Set new search direction to the negative of the previously acquired simplex point

        int loops = 0;
        while (true)
        {
            simplex.push_back(Support(a,b,d)); //Add new simplex point
            if (simplex[simplex.size()-1].vector * d < 0) //If the latest point does not reach past the origin then collision is impossible
                return false;
            if (DoSimplex(simplex, d))
            {
                EPA(a,b,simplex,outCollisionPoint,outPenetrationVector,outPenetrationDepth);

                return true;
            }
        }
    }

    static void EPA(std::vector<Vector4D> a, std::vector<Vector4D> b, std::vector<SupportPoint> simplex, Vector4D &outCollisionPoint, Vector4D &outPenetrationVector, float &outPenetrationDepth)
    {
        std::vector<Face> faces = std::vector<Face>();
        //Insert each face of the simplex into the list of faces with clockwise orientation as seen from the origin
        faces.push_back(Face(simplex[2], simplex[1], simplex[0]));
        faces.push_back(Face(simplex[3], simplex[2], simplex[0]));
        faces.push_back(Face(simplex[3], simplex[1], simplex[2]));
        faces.push_back(Face(simplex[3], simplex[0], simplex[1]));

        while (true)
        {
            int index = FindClosestFace(faces); //Get the closest face
            Face face = faces[index];
            SupportPoint p = Support(a,b,face.normal); //Get the point in the Minskowski difference farthest away in the direction of the face normal (away from the origin)
            float distanceP = p.vector * face.normal; //Get the distance to p along the face normal
            if (distanceP - face.distance < 0.00001)
            {
                //p was not further away then what the face was, this means that the face is on the edge of the
                //Minkowski difference while also being the closest face to the origin

                Vector4D barycentric = Barycentric(face);

                Vector4D test = face.normal * face.distance;
                outCollisionPoint = face.a.suppA*barycentric[0] + face.b.suppA*barycentric[1] + face.c.suppA*barycentric[2];
                outPenetrationVector = face.normal;
                outPenetrationDepth = face.distance;
                return;
            }
            else
            {
                //p was further away than the face, update the new polygon so it contains p
                std::vector<Edge> edges = std::vector<Edge>();
                for (int i = faces.size()-1; i >= 0; i--)
                {
                    //Check what faces are visible from the point p
                    if (faces[i].normal * (p.vector - faces[i].a.vector) > 0)
                    {
                        //if a face is visible then remove it and add all its edges to the list of edges that need new faces
                        AddEdge(Edge(faces[i].a, faces[i].b), edges);
                        AddEdge(Edge(faces[i].b, faces[i].c), edges);
                        AddEdge(Edge(faces[i].c, faces[i].a), edges);
                        faces.erase(faces.begin()+i);
                    }
                }

                //Add new faces from p to each of the edges
                for (int i = 0; i < edges.size(); i++)
                {
                    faces.push_back(Face(edges[i].a, edges[i].b, p));
                }
            }
        }
    }

    static void AddEdge(Edge e, std::vector<Edge> &edges)
    {
        for (int i = 0; i < edges.size(); i++)
        {
            if (e.a.vector == edges[i].b.vector && e.b.vector == edges[i].a.vector)
            {
                edges.erase(edges.begin()+i);
                return;
            }
        }
        edges.push_back(e);
    }

    static SupportPoint Support(std::vector<Vector4D> a, std::vector<Vector4D> b, Vector4D d)
    {
        int maxIndex = 0;
        float maxLength = d * a[maxIndex];
        for (int i = 1; i < a.size(); i++)
        {
            float length = d * a[i];
            if (length > maxLength)
            {
                maxIndex = i;
                maxLength = length;
            }
        }

        d = d * -1;

        int minIndex = 0;
        float minLength = d * b[minIndex];
        for (int i = 1; i < b.size(); i++)
        {
            float length = d * b[i];
            if (length > minLength)
            {
                minIndex = i;
                minLength = length;
            }
        }

        return SupportPoint(a[maxIndex] - b[minIndex], a[maxIndex]);
    }

    static bool DoSimplex(std::vector<SupportPoint> &simplex, Vector4D &d)
    {
        int size = simplex.size();
        if (size == 2)
        {
            //d = AB x AO x AB where A = simplex[1], B = simplex[0], O = (0,0,0)
            Vector4D AB = simplex[0].vector - simplex[1].vector;
            d = (AB.CrossProduct(simplex[1].vector * -1)).CrossProduct(AB);
            //If d is an invalid direction
            if (d == Vector4D(0,0,0))
            {
                //Get line perpendicular AB, perp vector P is solved by using the dot product. AB * P = 0, if P.x and P.y = 1 then P.z = -(AB.x+AB.y)/AB.z
                if (AB[0] != 0)
                    d = Vector4D(-(AB[1]+AB[2])/AB[0],1,1);
                else if (AB[1] != 0)
                    d = Vector4D(1,-(AB[0]+AB[2])/AB[1],1);
                else //AB[2] != 0
                    d = Vector4D(1,1,-(AB[0]+AB[1])/AB[2]);
            }
            return false;
        }
        else if (size == 3)
        {
            //A = simplex[2], B = simplex[1], C = simplex[0], O = (0,0,0)
            Vector4D AO = simplex[2].vector * -1;
            Vector4D AB = simplex[1].vector - simplex[2].vector;
            Vector4D AC = simplex[0].vector - simplex[2].vector;

            //Test which side of AB origin is
            Vector4D ABperp = AC.CrossProduct(AB).CrossProduct(AB);
            if (simplex[2].vector * ABperp < 0)
            {
                //Origin is outside the triangle
                d = AB.CrossProduct(AO).CrossProduct(AB);
                simplex.erase(simplex.begin()+0);
                return false;
            }

            //Test which side of AC origin is
            Vector4D ACperp = AB.CrossProduct(AC).CrossProduct(AC);
            if (simplex[2].vector * ACperp < 0)
            {
                //Origin is outside the triangle
                d = AC.CrossProduct(AO).CrossProduct(AC);
                simplex.erase(simplex.begin()+1);
                return false;
            }

            //Else origin is in triangle
            //Test which side of the triangle origin is
            Vector4D ABCperp = AC.CrossProduct(AB);
            if (simplex[2].vector * ABCperp < 0)
            {
                //Origin is in the direction of ABCperp
                d = ABCperp;
            }
            else
            {
                //Origin is in the opposite direction of ABCperp
                d = ABCperp * -1;
                Vector4D B = simplex[1].vector;
                simplex[1].vector = simplex[2].vector;
                simplex[2].vector = B;
            }
            return false;
        }
        else //size == 4
        {
            //A = simplex[3], B = simplex[2], C = simplex[1], D = simplex[0], O = (0,0,0)
            Vector4D A0 = simplex[3].vector * -1;
            Vector4D AB = simplex[2].vector - simplex[3].vector;
            Vector4D AC = simplex[1].vector - simplex[3].vector;
            Vector4D AD = simplex[0].vector - simplex[3].vector;

            Vector4D ABCperp = AC.CrossProduct(AB);
            if (simplex[3].vector * ABCperp < 0)
            {
                //Origin is outside the tetrahedron
                d = ABCperp;
                simplex.erase(simplex.begin()+0);
                return false;
            }

            Vector4D ACDperp = AD.CrossProduct(AC);
            if (simplex[3].vector * ACDperp < 0)
            {
                //Origin is outside the tetrahedron
                d = ACDperp;
                simplex.erase(simplex.begin()+2);
                return false;
            }

            Vector4D ADBperp = AB.CrossProduct(AD);
            if (simplex[3].vector * ADBperp < 0)
            {
                //Origin is outside the tetrahedron
                d = ADBperp;
                simplex.erase(simplex.begin()+1);
                Vector4D C = simplex[1].vector;
                simplex[1].vector = simplex[2].vector;
                simplex[2].vector = C;
                return false;
            }

            //else origin is in the triangle
            return true;
        }
    }

    static int FindClosestFace(std::vector<Face> &faces)
    {
        int closestFace = 0;
        for (int i = 0; i < faces.size(); i++)
        {
            Vector4D AB = faces[i].b.vector - faces[i].a.vector;
            Vector4D AC = faces[i].c.vector - faces[i].a.vector;

            //if (faces[i].distance == -1)
            faces[i].normal = AB.CrossProduct(AC);
            faces[i].normal.Normalize();
            faces[i].distance = faces[i].a.vector * faces[i].normal;

            if (faces[i].distance < faces[closestFace].distance)
                closestFace = i;
        }
        return closestFace;
    }

    static Vector4D Barycentric(Face face)
    {
        Vector4D v0 = face.b.vector - face.a.vector;
        Vector4D v1 = face.c.vector - face.a.vector;
        Vector4D v2 = face.normal*face.distance - face.a.vector;

        float d00 = v0*v0;
        float d01 = v0*v1;
        float d11 = v1*v1;
        float d20 = v2*v0;
        float d21 = v2*v1;
        float denom = d00 * d11 - d01 * d01;
        float v = (d11 * d20 - d01 * d21) / denom;
        float w = (d00 * d21 - d01 * d20) / denom;
        float u = 1.0f - v - w;

        return Vector4D(u,v,w);
    }
};

#endif //GSCEPT_LAB_ENV_GJKCHECK_H
