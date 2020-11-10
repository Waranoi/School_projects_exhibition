#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class TextureResource
{
	GLuint TBO;
private:
public:
	TextureResource();
	~TextureResource();
	void LoadFromFile(const char* filename);
	void LoadFromBuffer(unsigned char* imageBuffer, int width, int height);
	void BindTexture();
	static unsigned char* Stbi_load(const char* filename, int* widthOUT, int* heightOUT, int channels);
};