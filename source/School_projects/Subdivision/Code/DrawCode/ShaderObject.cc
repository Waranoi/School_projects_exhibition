#include "ShaderObject.h"
#include <fstream>

void ShaderObject::LoadShader(const char* vsPath, const char* fsPath)
{
	std::string vsString = ReadFile(vsPath);
	std::string fsString = ReadFile(fsPath);
	const char* vertexShader = vsString.c_str();
	const char* fragmentShader = fsString.c_str();

	GLuint vertexBuffer = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentBuffer = glCreateShader(GL_FRAGMENT_SHADER);

	int logLength;

	glShaderSource(vertexBuffer, 1, &vertexShader, NULL);
	glCompileShader(vertexBuffer);
	glGetShaderiv(vertexBuffer, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		GLchar* buf = new GLchar[logLength];
		glGetShaderInfoLog(vertexBuffer, logLength, NULL, buf);
		printf("[VERTEX SHADER COMPILE ERROR]: %s", buf);
		delete[] buf;
	}

	glShaderSource(fragmentBuffer, 1, &fragmentShader, NULL);
	glCompileShader(fragmentBuffer);
	glGetShaderiv(fragmentBuffer, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		GLchar* buf = new GLchar[logLength];
		glGetShaderInfoLog(fragmentBuffer, logLength, NULL, buf);
		printf("[FRAGMENT SHADER COMPILE ERROR]: %s", buf);
		delete[] buf;
	}

	program = glCreateProgram();
	glAttachShader(program, vertexBuffer);
	glAttachShader(program, fragmentBuffer);
	glLinkProgram(program);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		GLchar* buf = new GLchar[logLength];
		glGetProgramInfoLog(program, logLength, NULL, buf);
		printf("[PROGRAM LINK ERROR]: %s", buf);
		delete[] buf;
	}

	UseProgram();
	matrixID = glGetUniformLocation(program, "matrix");

	glDeleteShader(vertexBuffer);
	glDeleteShader(fragmentBuffer);
}

std::string ShaderObject::ReadFile(const char* filePath)
{
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "Read failed";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void ShaderObject::UpdateRotation(Matrix4D matrix)
{
	glUniformMatrix4fv(matrixID, 1, GL_TRUE, matrix.GetArray());
}

void ShaderObject::UseProgram()
{
	glUseProgram(program);
}

GLuint ShaderObject::GetMatrixID()
{
	return matrixID;
}

GLuint ShaderObject::GetProgram()
{
	return program;
}