#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
class MeshResource
{
private:
	GLuint VBO;
	GLuint EBO;
	int sizeOfIndexBuffer;
	bool withUV;
	bool withNormal;
public:
	MeshResource(bool withUV, bool withNormal);
	~MeshResource();
	void Draw(bool wireframe = false);
	void SetUpVBO(float* vertexBuffer, int sizeOfVertexBuffer);
	void SetUpEBO(int* indexBuffer, int sizeOfIndexBuffer);
	static MeshResource* Cube();
	static MeshResource* LoadFromOBJ(const char* filePath, bool withUV, bool withNormal);
	static void LoadFromOBJ(const char* filePath, std::vector<float>* vertexBuf, std::vector<int>* indexBuf);
};

