#pragma once
#include "MeshResource.h"
#include "TextureResource.h"
#include "ShaderObject.h"
#include "GraphicsNode.h"
#include "myMathLib.h"
#include "QuadObject.h"
#include "AABBManager.h"
#include "Object.h"
#include "Window.h"

class RayIntersection
{
public:
    bool Setup(GLFWwindow *window);
    void Run();

    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    Matrix4D CreateView();
    Matrix4D CreateProjection();
    Object* CreateObject(const char* texture, const char* vs, const char* fs, const char* mesh, bool withUV, bool withNormal, float mass);

    void DrawQuads();
    void DrawObjects();
    void DrawForceObject(float dt);

private:
    ShaderObject shader;
    float mouseX = 0.0f;
    float mouseY = 0.0f;
    float eyeX = 0.0f;
    float eyeY = 0.0f;
    float eyeZ = 0.0f;
    float matX = 0.0f;
    float matY = 0.0f;

    Vector4D eye = Vector4D(0, 0, 1.0f);
    Vector4D target = Vector4D(0, 0, 0);
    Vector4D up = Vector4D(0, 1, 0);
    Matrix4D view;
    Matrix4D projection;

    //QUADS
    std::vector<QuadObject> quads;
    bool isIntersecting = false;
    Vector4D intersection;

    //OBJECTS
    std::vector<Object*> objects;

    //FORCE OBJECT
    Object *forceObj;

    //INTERSECTION OBJECT
    Object *intersectionObj;
};