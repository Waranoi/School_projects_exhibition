#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "myMathLib.h"
#include "BoundingBox.h"

class MeshResource
{
private:
	int sizeOfIndexBuffer;
public:
	GLuint VBO;
	GLuint EBO;
	bool withUV;
	bool withNormal;
	MeshResource(bool withUV, bool withNormal);
	~MeshResource();
	void Draw(bool wireframe = false);
	void SetUpVBO(float* vertexBuffer, int sizeOfVertexBuffer);
	void SetUpEBO(int* indexBuffer, int sizeOfIndexBuffer);
	static MeshResource* Cube();
	static MeshResource* LoadFromOBJ(const char* filePath, bool withUV, bool withNormal);
	static void LoadFromOBJ(const char* filePath, std::vector<float>* vertexBuf, std::vector<int>* indexBuf);
	static MeshResource* LoadWithBoundingBoxFromOBJ(char* filePath, bool withUV, bool withNormal, float *boxDimensions, float *boxOrigin); //boxDimension and boxOrigin are both float[3]
	static MeshResource* LoadWithBoundingBoxFromOBJ(char* filePath, bool withUV, bool withNormal, float mass, Vector4D *centerOfMass, Matrix4D *inertiaTensor, float *boxDimensions, float *boxOrigin); //boxDimension and boxOrigin are both float[3]
	static MeshResource* LoadWithBoundingBoxFromOBJ(char* filePath, bool withUV, bool withNormal, float mass, Vector4D *centerOfMass, Matrix4D *inertiaTensor, float *boxDimensions, float *boxOrigin, std::vector<Vector4D> *polygon); //boxDimension and boxOrigin are both float[3]
};

