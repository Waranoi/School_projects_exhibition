//
// Created by eliwar-5 on 2017-11-06.
//

#ifndef GSCEPT_LAB_ENV_QUAD_H
#define GSCEPT_LAB_ENV_QUAD_H

#include "myMathLib.h"
#include "MeshResource.h"
#include "TextureResource.h"
#include "ShaderObject.h"
#include "GraphicsNode.h"

struct QuadObject
{
    float height;
    float width;
    Vector4D up;
    Vector4D right;

    Matrix4D posMat;
    Matrix4D rotMat;
    Matrix4D scaleMat;
    GraphicsNode obj;

    bool invertColor;
    QuadObject(){}

    QuadObject(Vector4D origin, float height, float width)
    {
        this->height = height;
        this->width = width;
        up = Vector4D(0, 1, 0);
        right = Vector4D(1, 0, 0);

        posMat = Matrix4D::SetTranslation(origin[0], origin[1], origin[2]);
        rotMat = Matrix4D();
        scaleMat = Matrix4D::CreateScalingMatrix(width, height, 1);

        invertColor = false;
        SetUpGraphicsNode();
    }

    void Translate(Vector4D translate)
    {
        posMat = posMat * Matrix4D::SetTranslation(translate[0], translate[1], translate[2]);
    }

    void Rotate(Vector4D pivot, float xRad, float yRad, float zRad)
    {
        Matrix4D rotate = Matrix4D::CreateMatrixRotX(xRad) * Matrix4D::CreateMatrixRotY(yRad) * Matrix4D::CreateMatrixRotZ(zRad);
        up = rotate * up;
        right = rotate * right;

        rotMat = rotMat * Matrix4D::SetTranslation(pivot[0], pivot[1], pivot[2]) * rotate * Matrix4D::SetTranslation(-pivot[0], -pivot[1], -pivot[2]);
    }

    void Draw(Matrix4D viewProjection)
    {
        obj.GetShaderObject()->UseProgram();
        glUniform1i(glGetUniformLocation(obj.GetShaderObject()->GetProgram(), "invertColor"), (int)invertColor);
        obj.SetUpMatrix(viewProjection*posMat*rotMat*scaleMat);
        obj.Draw();
    }

    bool Intersect(Ray ray, Vector4D *outResult)
    {
        Vector4D intersection;
        Vector4D normal = right.CrossProduct(up);
        normal.Normalize();
        Plane plane = Plane(posMat * rotMat * Vector4D(0, 0, 0), normal);

        if (ray.Intersect(plane, &intersection)) //Get intersection point
        {
            Vector4D relativeIntersection = intersection - plane.point;
            float x = right * relativeIntersection;
            float y = up * relativeIntersection;

            if (x >= 0 && x < width && y >= 0 && y < height) //Check if intersection point is within quad boundary
            {
                *outResult = intersection;
                return true;
            }
        }
        return false;
    }

    void InvertColor()
    {
        invertColor = !invertColor;
    }

private:
    void SetUpGraphicsNode()
    {
        //Get texture and initialize texture resource
        int width, height;
        unsigned char *imgBuffer = TextureResource::Stbi_load("../source/from_school/Resources/fillerimg.jpg", &width, &height, 3);
        TextureResource *textureResource = new TextureResource();
        textureResource->LoadFromBuffer(imgBuffer, width, height);

        //Initialize shader
        ShaderObject *shader = new ShaderObject();
        shader->LoadShader("../source/from_school/Resources/vs.txt", "../source/from_school/Resources/fs.txt");

        //Create Mesh resource
        MeshResource *meshResource = MeshResource::LoadFromOBJ("../source/from_school/Resources/quad.obj", true, true);

        //Set up my graphics node
        obj.SetMeshResource(std::shared_ptr<MeshResource>(meshResource));
        obj.SetTextureResource(std::shared_ptr<TextureResource>(textureResource));
        obj.SetShaderObject(std::shared_ptr<ShaderObject>(shader));
    }
};


#endif //GSCEPT_LAB_ENV_QUAD_H
