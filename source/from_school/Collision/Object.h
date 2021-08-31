//
// Created by eliwar-5 on 2017-11-08.
//

#ifndef GSCEPT_LAB_ENV_OBJECT_H
#define GSCEPT_LAB_ENV_OBJECT_H

#include "GraphicsNode.h"
#include "myMathLib.h"
#include "BoundingBox.h"
#include "GJK.h"


struct Object
{
    bool invertColor;
    GraphicsNode obj;
    AABB box;
    std::vector<Vector4D> polygon;

    Matrix4D posMat;
    Matrix4D comRot;
    Matrix4D originRot;
    Matrix4D scaleMat;

    float massInverse;
    Matrix4D inertiaTensor;
    Vector4D CoM;
    Vector4D angularVelocity;
    Vector4D velocityL;

    Object() {}

    Object(GraphicsNode obj, BoundingBox box, float mass)
    {
        this->invertColor = false;
        this->obj = obj;
        this->box = AABB(box);
        this->polygon = std::vector<Vector4D>();
        this->massInverse = 1 / mass;
        this->inertiaTensor = Matrix4D();
        this->CoM = Vector4D(0,0,0);
        this->angularVelocity = Vector4D(0,0,0);
        this->velocityL = Vector4D(0, 0, 0);

        this->posMat = Matrix4D();
        this->comRot = Matrix4D();
        this->originRot = Matrix4D();
        this->scaleMat = Matrix4D();
    }

    Object(GraphicsNode obj, BoundingBox box, float mass, Vector4D CoM, Matrix4D inertiaTensor)
    {
        this->invertColor = false;
        this->obj = obj;
        this->box = AABB(box);
        this->polygon = std::vector<Vector4D>();
        this->massInverse = 1 / mass;
        this->inertiaTensor = inertiaTensor;
        this->CoM = CoM;
        this->angularVelocity = Vector4D(0,0,0);
        this->velocityL = Vector4D(0, 0, 0);

        this->posMat = Matrix4D();
        this->comRot = Matrix4D();
        this->originRot = Matrix4D();
        this->scaleMat = Matrix4D();
    }

    Object(GraphicsNode obj, BoundingBox box, std::vector<Vector4D> polygon, float mass, Vector4D CoM, Matrix4D inertiaTensor)
    {
        this->invertColor = false;
        this->obj = obj;
        this->box = AABB(box);
        this->polygon = polygon;
        this->massInverse = 1 / mass;
        this->inertiaTensor = inertiaTensor;
        this->CoM = CoM;
        this->angularVelocity = Vector4D(0,0,0);
        this->velocityL = Vector4D(0, 0, 0);

        this->posMat = Matrix4D();
        this->comRot = Matrix4D();
        this->originRot = Matrix4D();
        this->scaleMat = Matrix4D();
    }

    void Translate(float x, float y, float z)
    {
        posMat = posMat * Matrix4D::SetTranslation(x, y, z);
    }

    void Rotate(float xRad, float yRad, float zRad)
    {
        Matrix4D rotate = Matrix4D::CreateMatrixRotX(xRad) * Matrix4D::CreateMatrixRotY(yRad) * Matrix4D::CreateMatrixRotZ(zRad);
        comRot = comRot * rotate;
        originRot = originRot * rotate;
        Matrix4D test = Matrix4D::InverseMatrix(originRot);
    }

    void RotateCoM(float xRad, float yRad, float zRad)
    {
        Matrix4D rotate = Matrix4D::CreateMatrixRotX(xRad) * Matrix4D::CreateMatrixRotY(yRad) * Matrix4D::CreateMatrixRotZ(zRad);
        comRot = comRot * Matrix4D::SetTranslation(CoM[0], CoM[1], CoM[2]) * rotate * Matrix4D::SetTranslation(-CoM[0], -CoM[1], -CoM[2]);
        originRot = originRot * rotate;
    }

    void Scale(float xScale, float yScale, float zScale)
    {
        scaleMat = scaleMat * Matrix4D::CreateScalingMatrix(xScale, yScale, zScale);
    }

    void Update(float dt)
    {
        //Angular Velocity
        Vector4D dvA = angularVelocity * dt;
        RotateCoM(dvA[0], dvA[1], dvA[2]);

        //Linear Velocity
        Vector4D dvL = velocityL * dt;
        posMat.SetValue(3, posMat[3] + dvL[0]);
        posMat.SetValue(7, posMat[7] + dvL[1]);
        posMat.SetValue(11, posMat[11] + dvL[2]);
    }

    void ApplyForce(Vector4D force, float dt)
    {
        velocityL = force * massInverse * dt + velocityL;
    }

    void ApplyImpulse(Vector4D impulse, Vector4D impulsePoint)
    {
        //impulse = Vector4D(0, 0.102965f, -0.994685f);
        //impulsePoint = Vector4D(0, 0.365820f, 0.500000f);
        impulse = Vector4D(-0.436f, 0.124f, -1);
        impulsePoint = Vector4D(-0.1f, 0.3f, 0.500000f);
        //Angular velocity
        Vector4D dir = impulse;
        dir.Normalize();

        Vector4D hypotenuse = (posMat * CoM) - impulsePoint;
        Vector4D adjacent = dir * (hypotenuse * dir);
        Vector4D lever = adjacent - hypotenuse;
        Vector4D angularMomentum = lever.CrossProduct(impulse);
        angularVelocity = Matrix4D::InverseMatrix(originRot * inertiaTensor) * angularMomentum + angularVelocity;
        printf("ang vel: %f, %f, %f\n", angularVelocity[0], angularVelocity[1], angularVelocity[2]);
        printf("impulse: %f, %f, %f\n", impulse[0], impulse[1], impulse[2]);
        printf("impulsePoint: %f, %f, %f\n", impulsePoint[0], impulsePoint[1], impulsePoint[2]);

        //Linear velocity
        velocityL = impulse * massInverse + velocityL;
    }

    void Draw(Matrix4D viewProjection)
    {
        obj.GetShaderObject()->UseProgram();
        glUniform1i(glGetUniformLocation(obj.GetShaderObject()->GetProgram(), "invertColor"), (int)invertColor);
        obj.SetUpMatrix(viewProjection*posMat*comRot*scaleMat);
        obj.Draw();
    }
    ///Kolla så att angular velocity är korrekt för både applyimpulse och respons.
    void InvertColor()
    {
        invertColor = !invertColor;
    }

    BoundingBox GetAABB()
    {
        BoundingBox b = box.GetAABB(originRot, comRot);
        b.origin = posMat * scaleMat * b.origin;
        b.up = scaleMat * b.up;
        b.right = scaleMat * b.right;
        b.forward = scaleMat * b.forward;

        return b;
    }

    static bool GJKCollision(Object &a, Object &b, Vector4D &outCollisionPoint, Vector4D &outPenetrationVector, float &outPenetrationDepth)
    {
        std::vector<Vector4D> polygonA = std::vector<Vector4D>();
        Matrix4D modelA = a.posMat*a.comRot*a.scaleMat;
        for (int i = 0; i < a.polygon.size(); i++)
        {
            polygonA.push_back(modelA * a.polygon[i]);
        }

        std::vector<Vector4D> polygonB = std::vector<Vector4D>();
        Matrix4D modelB = b.posMat*b.comRot*b.scaleMat;
        for (int i = 0; i < b.polygon.size(); i++)
        {
            polygonB.push_back(modelB * b.polygon[i]);
        }

        if (GJK::CheckCollision(polygonA, polygonB, outCollisionPoint, outPenetrationVector, outPenetrationDepth) == false)
            return false;

        ///Respons
        //Vectors from center of mass to collision point
        Vector4D rA = outCollisionPoint - a.CoM;
        Vector4D rB = outCollisionPoint - b.CoM;

        //Rate of change for collision point A and B
        Vector4D deltaACP = a.velocityL + a.angularVelocity.CrossProduct(rA);
        Vector4D deltBACP = b.velocityL + b.angularVelocity.CrossProduct(rB);

        //Get relative velocity between the two colliding objects
        float vRel = outPenetrationVector * (deltBACP - deltaACP);
        if (vRel > -0.00001) //If the relative velocity is above the threshold the objects are either resting or separating, don't move them apart then
            return true;

        float bounce = 1;
        Matrix4D aWorldIT = Matrix4D::InverseMatrix(a.comRot*a.scaleMat)*a.inertiaTensor*Matrix4D::Transpose(Matrix4D::InverseMatrix(a.comRot*a.scaleMat));
        Matrix4D bWorldIT = Matrix4D::InverseMatrix(b.comRot*b.scaleMat)*b.inertiaTensor*Matrix4D::Transpose(Matrix4D::InverseMatrix(b.comRot*b.scaleMat));
        float impulseLength = -(1+bounce)*vRel / (a.massInverse+b.massInverse + outPenetrationVector*( Matrix4D::InverseMatrix(aWorldIT)*(rA.CrossProduct(outPenetrationVector)).CrossProduct(rA)  +  Matrix4D::InverseMatrix(bWorldIT)*(rB.CrossProduct(outPenetrationVector)).CrossProduct(rB) ));

        Vector4D impulseA = outPenetrationVector*-impulseLength;
        Vector4D impulseB = outPenetrationVector*impulseLength;

        printf("HEJ\n");

        a.velocityL = a.velocityL + impulseA*a.massInverse;
        b.velocityL = b.velocityL + impulseB*b.massInverse;

        a.angularVelocity = a.angularVelocity + Matrix4D::InverseMatrix(a.originRot * a.inertiaTensor) * rA.CrossProduct(impulseA);
        Vector4D t = a.angularVelocity + Matrix4D::InverseMatrix(aWorldIT) * rA.CrossProduct(impulseA);
        b.angularVelocity = b.angularVelocity + Matrix4D::InverseMatrix(b.originRot * b.inertiaTensor) * rB.CrossProduct(impulseB);
    }

    void DrawDebug(Matrix4D viewProjection, GLuint rayProgram)
    {
        glUseProgram(rayProgram);
        glUniform4fv(glGetUniformLocation(rayProgram, "color"), 1, Vector4D(1, 0, 0).GetArray());
        glUniformMatrix4fv(glGetUniformLocation(rayProgram, "matrix"), 1, GL_TRUE, (viewProjection).GetArray());
        BoundingBox a = box.base.Rotate(originRot, comRot); //OOBB
        a.origin = posMat * scaleMat * a.origin;
        a.up = scaleMat * a.up;
        a.right = scaleMat * a.right;
        a.forward = scaleMat * a.forward;
        DrawBox(a);

        glUniform4fv(glGetUniformLocation(rayProgram, "color"), 1, Vector4D(0, 0, 1).GetArray());
        BoundingBox b = GetAABB(); //AABB
        DrawBox(b);
    }

private:
    void DrawBox(BoundingBox b)
    {
        glLineWidth(2.5f);
        glBegin(GL_LINE_STRIP);
        //lower quad
        glVertex3f(b.origin[0], b.origin[1], b.origin[2]);
        glVertex3f(b.origin[0]+b.right[0], b.origin[1]+b.right[1], b.origin[2]+b.right[2]);
        glVertex3f(b.origin[0]+b.right[0]+b.forward[0], b.origin[1]+b.right[1]+b.forward[1], b.origin[2]+b.right[2]+b.forward[2]);
        glVertex3f(b.origin[0]+b.forward[0], b.origin[1]+b.forward[1], b.origin[2]+b.forward[2]);
        glVertex3f(b.origin[0], b.origin[1], b.origin[2]);

        //upper quad
        glVertex3f(b.origin[0]+b.up[0], b.origin[1]+b.up[1], b.origin[2]+b.up[2]);
        glVertex3f(b.origin[0]+b.up[0]+b.right[0], b.origin[1]+b.up[1]+b.right[1], b.origin[2]+b.up[2]+b.right[2]);
        glVertex3f(b.origin[0]+b.up[0]+b.right[0]+b.forward[0], b.origin[1]+b.up[1]+b.right[1]+b.forward[1], b.origin[2]+b.up[2]+b.right[2]+b.forward[2]);
        glVertex3f(b.origin[0]+b.up[0]+b.forward[0], b.origin[1]+b.up[1]+b.forward[1], b.origin[2]+b.up[2]+b.forward[2]);
        glVertex3f(b.origin[0]+b.up[0], b.origin[1]+b.up[1], b.origin[2]+b.up[2]);
        glEnd();

        glBegin(GL_LINES);
        //connections between upper and lower quad
        glVertex3f(b.origin[0]+b.right[0], b.origin[1]+b.right[1], b.origin[2]+b.right[2]);
        glVertex3f(b.origin[0]+b.up[0]+b.right[0], b.origin[1]+b.up[1]+b.right[1], b.origin[2]+b.up[2]+b.right[2]);

        glVertex3f(b.origin[0]+b.right[0]+b.forward[0], b.origin[1]+b.right[1]+b.forward[1], b.origin[2]+b.right[2]+b.forward[2]);
        glVertex3f(b.origin[0]+b.up[0]+b.right[0]+b.forward[0], b.origin[1]+b.up[1]+b.right[1]+b.forward[1], b.origin[2]+b.up[2]+b.right[2]+b.forward[2]);

        glVertex3f(b.origin[0]+b.forward[0], b.origin[1]+b.forward[1], b.origin[2]+b.forward[2]);
        glVertex3f(b.origin[0]+b.up[0]+b.forward[0], b.origin[1]+b.up[1]+b.forward[1], b.origin[2]+b.up[2]+b.forward[2]);
        glEnd();
    }
};

#endif //GSCEPT_LAB_ENV_OBJECT_H
