//
// Created by eliwar-5 on 2017-10-31.
//

#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include <GL/glut.h>
#include <chrono>

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
    ExampleApp::ExampleApp()
    {
        // empty
    }

//------------------------------------------------------------------------------
/**
*/
    ExampleApp::~ExampleApp()
    {
        for (Object* obj : objects)
        {
            delete obj;
        }
    }

//------------------------------------------------------------------------------
/**
*/
    bool
    ExampleApp::Open()
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
                                                    matY += 0.1f;
                                                else
                                                    matY -= 0.1f;
                                                break;
                                            case GLFW_KEY_DOWN:
                                                if (action == GLFW_PRESS)
                                                    matY -= 0.1f;
                                                else
                                                    matY += 0.1f;
                                                break;
                                            case GLFW_KEY_LEFT:
                                                if (action == GLFW_PRESS)
                                                    matX -= 0.1f;
                                                else
                                                    matX += 0.1f;
                                                break;
                                            case GLFW_KEY_RIGHT:
                                                if (action == GLFW_PRESS)
                                                    matX += 0.1f;
                                                else
                                                    matX -= 0.1f;
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

                                              Matrix4D test = Matrix4D::InverseMatrix(view);
                                              ray = Matrix4D::InverseMatrix(view) * ray;
                                              ray.Normalize();


                                              isIntersecting = false;
                                              float distance = 100000.0f;
                                              int intersectIndex = -1;
                                              Vector4D newIntersection = Vector4D();
                                              Ray r = Ray(eye, ray);
                                              for (int i = 0; i < quads.size(); i++)
                                              {
                                                  if (quads[i].Intersect(r, &newIntersection))
                                                  {
                                                      if ((newIntersection - eye).GetLength() < distance)
                                                      {
                                                          distance = (newIntersection - eye).GetLength();
                                                          intersection = newIntersection;
                                                          intersectIndex = i;
                                                          isIntersecting = true;
                                                      }
                                                  }
                                              }

                                              Object *o;
                                              if (AABBManager::RayCollision(r, &o, &newIntersection))
                                              {
                                                  o->InvertColor();
                                              }

                                              if (isIntersecting == true)
                                              {
                                                  quads[intersectIndex].InvertColor();
                                              }
                                          }
                                      });

        if (this->window->Open())
        {
            // set clear color to gray
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

            glEnable(GL_CULL_FACE); //Uncomment to cull faces
            glEnable(GL_DEPTH_TEST);

            //QUADS
            quads = std::vector<QuadObject>();
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (i % 4 == 0 || j % 4 == 0)
                    {
                        float x = i * 0.4f + 0.05f - 1; //i*0.4f : x per cell, 0.05f : padding, -1 : offset
                        float y = j * 0.4f + 0.05f - 1; //j*0.4f : x per cell, 0.05f : padding, -1 : offset
                        QuadObject quad = QuadObject(Vector4D(x, y, 0), 0.3f, 0.3f);
                        quads.push_back(quad);
                    }
                }
            }

            //OBJECTS
            objects = std::vector<Object*>();
            Object *obj = CreateObject("resources/fillerimg.jpg", "resources/vs.txt", "resources/fs.txt",
                                      "resources/cube.obj", true, true, 1);
            obj->Scale(0.4f, 0.4f, 0.4f);
            objects.push_back(obj);
            AABBManager::AddObject(obj);

            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    obj = CreateObject("resources/fillerimg.jpg", "resources/vs.txt", "resources/fs.txt",
                                              "resources/cube.obj", true, true, 1);
                    obj->Scale(0.4f, 0.4f, 0.4f);
                    obj->Translate(j*0.6f-0.3f, i*0.6f-0.3f, -0.5f);
                    objects.push_back(obj);
                    AABBManager::AddObject(obj);
                }
            }

            //FORCE OBJECT
            forceObj = CreateObject("resources/fillerimg.jpg", "resources/vs.txt", "resources/fs.txt",
                                    "resources/cube.obj", true, true, 1);
            forceObj->Translate(-10, 0, -10);

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
    ExampleApp::Run()
    {
        std::chrono::time_point<std::chrono::system_clock> start;
        float dt = 0; //in seconds

        MeshResource *m = MeshResource::LoadFromOBJ("resources/tetrahedron.obj",false,false);

        while (this->window->IsOpen())
        {
            start = std::chrono::system_clock::now();

            eye = eye + Vector4D(eyeX, eyeY, eyeZ);
            view = CreateView();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            this->window->Update();

            DrawQuads();

            DrawObjects();

            this->window->SwapBuffers();

            std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - start;
            dt = elapsed_seconds.count();
        }
    }

//------------------------------------------------------------------------------
/**
*/


    Matrix4D ExampleApp::CreateView()
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

    Matrix4D ExampleApp::CreateProjection()
    {
        float fov = 90.0f;		//Field of view
        float near = 0.01f;		//Near clipping plane
        float far = 100.0f;		//Far clipping plane
        float S = 1 / (tan((fov / 2.0f) * (M_PI / 180.0f)));
        std::vector<float> w_projArr =
                {
                        S,	0,	0,	0,
                        0,	S,	0,	0,
                        0,	0,	-1 * (far / (far - near)),	-1 * ((far * near) / (far - near)),
                        0,	0,	-1,	0
                };
        return Matrix4D(w_projArr);
    }

    Object* ExampleApp::CreateObject(const char *texture, const char *vs, const char *fs, char *mesh, bool withUV,
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

        //Create Mesh resource
        float boxDimensions[3];
        float boxOrigin[3];
        MeshResource *meshResource = MeshResource::LoadWithBoundingBoxFromOBJ(mesh, withUV, withNormal, boxDimensions, boxOrigin);

        //Set up my graphics node
        GraphicsNode obj;
        obj.SetMeshResource(std::shared_ptr<MeshResource>(meshResource));
        obj.SetTextureResource(std::shared_ptr<TextureResource>(textureResource));
        obj.SetShaderObject(std::shared_ptr<ShaderObject>(shader));

        return new Object(obj, BoundingBox(Vector4D(boxOrigin[0], boxOrigin[1], boxOrigin[2]), Vector4D(boxDimensions[0], boxDimensions[1], boxDimensions[2])), mass);
    }

    void ExampleApp::DrawQuads()
    {
        for(int i = 0; i < quads.size(); i++)
        {
            quads[i].Rotate(Vector4D(0.15f, 0.15f, 0), 0, 0.005f + 0.0005f * i, 0);
            quads[i].Draw(projection*view);
        }

        if (isIntersecting == true)
        {
            glUseProgram(shader.GetProgram());
            glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "matrix"), 1, GL_TRUE, (projection*view*Matrix4D::SetTranslation(intersection[0], intersection[1], intersection[2])).GetArray());
            glutSolidSphere(0.02f, 10, 10);
        }
    }

    void ExampleApp::DrawObjects()
    {
        glEnable(GL_CULL_FACE);

        for (int i = objects.size()-1; i >= 0; i--)
        {
            objects[i]->Rotate(0, 0.005f+i*0.002f, 0);
            objects[i]->Translate(matX, matY, 0);
            objects[i]->Draw(projection*view);
            objects[i]->DrawDebug(projection*view, shader.GetProgram());
        }

        glDisable(GL_CULL_FACE);
    }

    void ExampleApp::DrawForceObject(float dt)
    {
        glEnable(GL_CULL_FACE);
        if (forceObj->posMat[3] < -10)
        {
            forceObj->ApplyForce(Vector4D(10, 0, 0), dt);
            forceObj->Update(dt);
        }
        forceObj->Draw(Matrix4D());

        glDisable(GL_CULL_FACE);
    }
} // namespace Example