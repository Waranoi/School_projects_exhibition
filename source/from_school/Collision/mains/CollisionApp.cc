//
// Created by eliwar-5 on 2017-12-05.
//

// Created by eliwar-5 on 2017-11-29.
//

//
// Created by eliwar-5 on 2017-10-31.
//

#include "config.h"
#include "CollisionApp.h"
#include "BroadCollision.h"
#include <cstring>
#include <GL/glut.h>
#include <chrono>

using namespace Display;

//------------------------------------------------------------------------------
/**
*/
CollisionApp::CollisionApp()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CollisionApp::~CollisionApp()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
CollisionApp::Open()
{
    App::Open();
    this->window = new Display::Window;
    this->view = CreateView();
    this->projection = CreateProjection();

    window->SetKeyPressFunction([this](int32 key, int32 scancode, int32 action, int32 mods)
                                {
                                    if (action == GLFW_REPEAT)
                                        return;

                                    switch (key)
                                    {
                                        case GLFW_KEY_W:
                                            if (action == GLFW_PRESS)
                                                eyeY += 0.1f;
                                            else
                                                eyeY -= 0.1f;
                                            break;
                                        case GLFW_KEY_S:
                                            if (action == GLFW_PRESS)
                                                eyeY -= 0.1f;
                                            else
                                                eyeY += 0.1f;
                                            break;
                                        case GLFW_KEY_A:
                                            if (action == GLFW_PRESS)
                                                eyeX -= 0.1f;
                                            else
                                                eyeX += 0.1f;
                                            break;
                                        case GLFW_KEY_D:
                                            if (action == GLFW_PRESS)
                                                eyeX += 0.1f;
                                            else
                                                eyeX -= 0.1f;
                                            break;
                                        case GLFW_KEY_E:
                                            if (action == GLFW_PRESS)
                                                eyeZ += 0.1f;
                                            else
                                                eyeZ -= 0.1f;
                                            break;
                                        case GLFW_KEY_Q:
                                            if (action == GLFW_PRESS)
                                                eyeZ -= 0.1f;
                                            else
                                                eyeZ += 0.1f;
                                            break;
                                        case GLFW_KEY_UP:
                                            if (action == GLFW_PRESS)
                                                matY += 0.01f;
                                            else
                                                matY -= 0.01f;
                                            break;
                                        case GLFW_KEY_DOWN:
                                            if (action == GLFW_PRESS)
                                                matY -= 0.01f;
                                            else
                                                matY += 0.01f;
                                            break;
                                        case GLFW_KEY_LEFT:
                                            if (action == GLFW_PRESS)
                                                matX -= 0.01f;
                                            else
                                                matX += 0.01f;
                                            break;
                                        case GLFW_KEY_RIGHT:
                                            if (action == GLFW_PRESS)
                                                matX += 0.01f;
                                            else
                                                matX -= 0.01f;
                                            break;
                                        case GLFW_KEY_PAGE_UP:
                                            if (action == GLFW_PRESS)
                                                matZ -= 0.01f;
                                            else
                                                matZ += 0.01f;
                                            break;
                                        case GLFW_KEY_PAGE_DOWN:
                                            if (action == GLFW_PRESS)
                                                matZ += 0.01f;
                                            else
                                                matZ -= 0.01f;
                                            break;case GLFW_KEY_KP_8:
                                            if (action == GLFW_PRESS)
                                                rotX += 0.01f;
                                            else
                                                rotX -= 0.01f;
                                            break;
                                        case GLFW_KEY_KP_5:
                                            if (action == GLFW_PRESS)
                                                rotX -= 0.01f;
                                            else
                                                rotX += 0.01f;
                                            break;
                                        case GLFW_KEY_KP_4:
                                            if (action == GLFW_PRESS)
                                                rotY -= 0.01f;
                                            else
                                                rotY += 0.01f;
                                            break;
                                        case GLFW_KEY_KP_6:
                                            if (action == GLFW_PRESS)
                                                rotY += 0.01f;
                                            else
                                                rotY -= 0.01f;
                                            break;
                                        case GLFW_KEY_KP_7:
                                            if (action == GLFW_PRESS)
                                                rotZ -= 0.01f;
                                            else
                                                rotZ += 0.01f;
                                            break;
                                        case GLFW_KEY_KP_9:
                                            if (action == GLFW_PRESS)
                                                rotZ += 0.01f;
                                            else
                                                rotZ -= 0.01f;
                                            break;
                                        case GLFW_KEY_ENTER:
                                            if (action == GLFW_PRESS)
                                                enter = true;
                                            break;
                                    }
                                });

    window->SetMouseMoveFunction([this](float64 x, float64 y)
                                 {
                                     mouseX = x;
                                     mouseY = y;
                                 });

    window->SetMousePressFunction([this](int32 button, int32 state, int32)
                                  {
                                      if (button == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_PRESS)
                                      {
                                          ApplyImpulse(1);
                                      }
                                      else if (button == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_RELEASE)
                                      {
                                      }
                                  });

    if (this->window->Open())
    {
        // set clear color to gray
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        glEnable(GL_CULL_FACE); //Uncomment to cull faces
        glEnable(GL_DEPTH_TEST);

        bcoll = BroadCollision();

        //FORCE OBJECT
        forceObj1 = CreateObject("resources/fillerimg.jpg", "resources/vs.txt", "resources/fs.txt",
                                "resources/cube.obj", true, true, 1);
        forceObj2 = CreateObject("resources/fillerimg.jpg", "resources/vs.txt", "resources/fs.txt",
                                 "resources/cube.obj", true, true, 1);
        forceObj1->Translate(0, 0, 0);
        forceObj2->Translate(0, 0, 2);
        bcoll.AddObject(forceObj1);
        bcoll.AddObject(forceObj2);
        AABBManager::AddObject(forceObj1);
        AABBManager::AddObject(forceObj2);

        shader.LoadShader("resources/vs.txt", "resources/rayfs.txt");
        glUseProgram(shader.GetProgram());
        glUniform4fv(glGetUniformLocation(shader.GetProgram(), "color"), 1, Vector4D(1, 0, 0).GetArray());
        shader.UpdateRotation(projection*view);

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
CollisionApp::Run()
{
    std::chrono::time_point<std::chrono::system_clock> start;

    while (this->window->IsOpen())
    {
        start = std::chrono::system_clock::now();

        eye = eye + Vector4D(eyeX, eyeY, eyeZ);
        view = CreateView();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->window->Update();

        forceObj2->Translate(matX, matY, matZ);
        forceObj2->Rotate(rotX, rotY, rotZ);

        DrawForceObject(forceObj1);
        DrawForceObject(forceObj2);

        bool c2 = false;
        std::vector<Collision> coll = bcoll.GetCollisions();
        for(int i = 0; i < coll.size(); i++)
        {
            if(Object::GJKCollision(*coll[i].a, *coll[i].b, collisionPoint, penetrationVector, penetrationDepth))
            {
                c2 = true;
            }
        }

        if (c2 == true)
        {
            if(c == false)
                printf("[START] Collision\n");
            c = true;
        }
        else
        {
            if(c == true)
                printf("[STOP] Collision\n");
            c = false;
        }
        if (enter == true)
        {
            enter = false;
            printf("Collision Point: %f, %f, %f | Penetration Vector: %f, %f, %f | Penetration Depth %f\n", collisionPoint[0], collisionPoint[1], collisionPoint[2], penetrationVector[0], penetrationVector[1], penetrationVector[2], penetrationDepth);
        }

        this->window->SwapBuffers();

        std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - start;
        dt = elapsed_seconds.count();
    }
}

//------------------------------------------------------------------------------
/**
*/


Matrix4D CollisionApp::CreateView()
{

    Vector4D zAxis(eye - target);
    zAxis.Normalize();
    Vector4D xAxis = up.CrossProduct(zAxis);
    xAxis.Normalize();
    Vector4D yAxis = zAxis.CrossProduct(xAxis);

    std::vector<float> viewArr =
            {
                    xAxis[0],	xAxis[1],	xAxis[2],	-(xAxis * eye),
                    yAxis[0],	yAxis[1],	yAxis[2],	-(yAxis * eye),
                    zAxis[0],	zAxis[1],	zAxis[2],	-(zAxis * eye),
                    0,			0,			0,			1
            };
    return Matrix4D(viewArr);
}

Matrix4D CollisionApp::CreateProjection()
{
    float fov = 90.0f;		//Field of view
    float near = 0.01f;		//Near clipping plane
    float far = 100.0f;		//Far clipping plane
    float S = 1.0f / (tan((fov / 2.0f) * (M_PI / 180.0f)));
    std::vector<float> w_projArr =
            {
                    S,	0,	0,	0,
                    0,	S,	0,	0,
                    0,	0,	-1 * (far / (far - near)),	-1 * ((far * near) / (far - near)),
                    0,	0,	-1,	0
            };
    return Matrix4D(w_projArr);
}

Object* CollisionApp::CreateObject(const char *texture, const char *vs, const char *fs, char *mesh, bool withUV,
                               bool withNormal, float mass)
{
    //Get texture and initialize texture resource if object has
    TextureResource *textureResource;
    if (withUV == true)
    {
        int width, height;
        unsigned char *imgBuffer = TextureResource::Stbi_load(texture, &width, &height, 3);
        textureResource = new TextureResource();
        textureResource->LoadFromBuffer(imgBuffer, width, height);
    }

    //Initialize shader
    ShaderObject *shader = new ShaderObject();
    shader->LoadShader(vs, fs);

    //Position vector
    std::vector<Vector4D> polygon = std::vector<Vector4D>();

    //Create Mesh resource
    float boxDimensions[3];
    float boxOrigin[3];
    Matrix4D inertiaTensor = Matrix4D();
    Vector4D centerOfMass = Vector4D();
    MeshResource *meshResource = MeshResource::LoadWithBoundingBoxFromOBJ(mesh, withUV, withNormal, mass, &centerOfMass, &inertiaTensor, boxDimensions, boxOrigin, &polygon);

    //Set up my graphics node
    GraphicsNode obj;
    obj.SetMeshResource(std::shared_ptr<MeshResource>(meshResource));
    obj.SetTextureResource(std::shared_ptr<TextureResource>(textureResource));
    obj.SetShaderObject(std::shared_ptr<ShaderObject>(shader));

    return new Object(obj, BoundingBox(Vector4D(boxOrigin[0], boxOrigin[1], boxOrigin[2]), Vector4D(boxDimensions[0], boxDimensions[1], boxDimensions[2])), polygon, mass, centerOfMass, inertiaTensor);
}

void CollisionApp::DrawForceObject(Object *forceObj)
{
    glEnable(GL_CULL_FACE);

    forceObj->Update(dt);
    forceObj->Draw(projection*view);
    forceObj->DrawDebug(projection*view, shader.GetProgram());

    glDisable(GL_CULL_FACE);
}

void CollisionApp::ApplyForce(float length)
{
    int width, height;
    window->GetWindowDimensions(&width, &height);

    //Convert mouse positions from screen space to NDC space
    float x, y;
    x = 2.0f * mouseX / width - 1;
    y = 1 - 2.0f * mouseY / height;

    //http://antongerdelan.net/opengl/raycasting.html
    Vector4D ray = Vector4D(x, y, -1.0f);
    ray = Matrix4D::InverseMatrix(projection) * ray;
    ray = Vector4D(ray[0], ray[1], -1.0f, 0.0f);

    ray = Matrix4D::InverseMatrix(view) * ray;
    ray.Normalize();
    Ray r = Ray(eye, ray);

    Object *o;
    Vector4D point = Vector4D();
    if (AABBManager::RayCollision(r, &o, &point))
    {
        o->ApplyForce(ray * length, dt);
    }
}

void CollisionApp::ApplyImpulse(float length)
{
    int width, height;
    window->GetWindowDimensions(&width, &height);

    //Convert mouse positions from screen space to NDC space
    float x, y;
    x = 2.0f * mouseX / width - 1;
    y = 1 - 2.0f * mouseY / height;

    //http://antongerdelan.net/opengl/raycasting.html
    Vector4D ray = Vector4D(x, y, -1.0f);
    ray = Matrix4D::InverseMatrix(projection) * ray;
    ray = Vector4D(ray[0], ray[1], -1.0f, 0.0f);

    ray = Matrix4D::InverseMatrix(view) * ray;
    ray.Normalize();
    Ray r = Ray(eye, ray);

    Object *o;
    Vector4D point = Vector4D();
    if (AABBManager::RayCollision(r, &o, &point))
    {
        o->ApplyImpulse(ray * length, point);
    }
}