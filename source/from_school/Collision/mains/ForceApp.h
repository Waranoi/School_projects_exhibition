//
// Created by eliwar-5 on 2017-11-29.
//

#ifndef GSCEPT_LAB_ENV_FORCEAPP_H
#define GSCEPT_LAB_ENV_FORCEAPP_H

//
// Created by eliwar-5 on 2017-10-31.
//

#include "core/app.h"
#include "render/window.h"
#include "MeshResource.h"
#include "TextureResource.h"
#include "ShaderObject.h"
#include "GraphicsNode.h"
#include "myMathLib.h"
#include "QuadObject.h"
#include "AABBManager.h"
#include "Object.h"
#include "BroadCollision.h"


class ForceApp : public Core::App
{
public:
    /// constructor
    ForceApp();
    /// destructor
    ~ForceApp();

    /// open app
    bool Open();
    /// run app
    void Run();

    Matrix4D CreateView();
    Matrix4D CreateProjection();
    Object* CreateObject(const char* texture, const char* vs, const char* fs, char* mesh, bool withUV, bool withNormal, float mass);
    void DrawForceObject();
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
    float dt = 0.0f;
    bool hold = false;
    bool test = true;

    Vector4D eye = Vector4D(0, 0, 1.0f);
    Vector4D target = Vector4D(0, 0, 0);
    Vector4D up = Vector4D(0, 1, 0);
    Matrix4D view;
    Matrix4D projection;

    //FORCE OBJECT
    Object *forceObj;
};

#endif //GSCEPT_LAB_ENV_FORCEAPP_H
