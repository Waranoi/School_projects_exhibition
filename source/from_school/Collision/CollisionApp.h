//
// Created by eliwar-5 on 2017-12-05.
//

#ifndef GSCEPT_LAB_ENV_COLLISIONAPP_H
#define GSCEPT_LAB_ENV_COLLISIONAPP_H

//
// Created by eliwar-5 on 2017-11-29.
//

#include "core/app.h"
#include "render/window.h"
#include "../DrawCode/MeshResource.h"
#include "../DrawCode/TextureResource.h"
#include "../DrawCode/ShaderObject.h"
#include "../DrawCode/GraphicsNode.h"
#include "../Math/myMathLib.h"
#include "QuadObject.h"
#include "AABBManager.h"
#include "Object.h"
#include "BroadCollision.h"


class CollisionApp : public Core::App
{
public:
    /// constructor
    CollisionApp();
    /// destructor
    ~CollisionApp();

    /// open app
    bool Open();
    /// run app
    void Run();

    Matrix4D CreateView();
    Matrix4D CreateProjection();
    Object* CreateObject(const char* texture, const char* vs, const char* fs, char* mesh, bool withUV, bool withNormal, float mass);
    void DrawForceObject(Object *forceObj);
    void ApplyForce(float length);
    void ApplyImpulse(float length);
private:

    ShaderObject shader;
    Display::Window* window;
    float mouseX = 0.0f;
    float mouseY = 0.0f;
    float eyeX = 0.0f;
    float eyeY = 0.0f;
    float eyeZ = 0.0f;
    float matX = 0.0f;
    float matY = 0.0f;
    float matZ = 0.0f;
    float rotX = 0.0f;
    float rotY = 0.0f;
    float rotZ = 0.0f;
    float dt = 0.0f;
    bool hold = false;
    bool test = true;
    bool c = false;

    bool enter = false;
    Vector4D collisionPoint = Vector4D(0,0,0);
    Vector4D penetrationVector = Vector4D(0,0,0);
    float penetrationDepth = 0;

    Vector4D eye = Vector4D(0, 0, 5.0f);
    Vector4D target = Vector4D(0, 0, 0);
    Vector4D up = Vector4D(0, 1, 0);
    Matrix4D view;
    Matrix4D projection;

    BroadCollision bcoll;

    //FORCE OBJECT
    Object *forceObj1;
    Object *forceObj2;
    Object *forceObj3;
};

#endif //GSCEPT_LAB_ENV_COLLISIONAPP_H
