//
// Created by eliwar-5 on 2017-10-31.
//

#ifndef GSCEPT_LAB_ENV_EXAMPLEAPP_H
#define GSCEPT_LAB_ENV_EXAMPLEAPP_H

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

namespace Example
{
    class ExampleApp : public Core::App
    {
    public:
        /// constructor
        ExampleApp();
        /// destructor
        ~ExampleApp();

        /// open app
        bool Open();
        /// run app
        void Run();

        Matrix4D CreateView();
        Matrix4D CreateProjection();
        Object* CreateObject(const char* texture, const char* vs, const char* fs, char* mesh, bool withUV, bool withNormal, float mass);
        void DrawQuads();
        void DrawObjects();
        void DrawForceObject(float dt);
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
    };
} // namespace Example

#endif //GSCEPT_LAB_ENV_EXAMPLEAPP_H
