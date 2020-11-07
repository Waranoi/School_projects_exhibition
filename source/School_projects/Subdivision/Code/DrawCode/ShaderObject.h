#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "myMathLib.h"
#include <string>

class ShaderObject
{
public:
	void LoadShader(const char* vsPath, const char* fsPath);
	void UseProgram();
	void UpdateRotation(Matrix4D matrix);
	GLuint GetMatrixID();
	GLuint GetProgram();
private:
	int index;
	GLuint program;
	GLuint matrixID;
	GLuint vectorID;
	std::string ReadFile(const char* filePath);
};