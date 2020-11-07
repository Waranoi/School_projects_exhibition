#include "TextureResource.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureResource::TextureResource():TBO(0)
{

}

TextureResource::~TextureResource()
{
	glDeleteTextures(1, &TBO);
	glDeleteBuffers(1, &TBO);
}

void TextureResource::LoadFromFile(const char* filename)
{
	int width;
	int height;
	int comp;
	unsigned char* data = stbi_load(filename, &width, &height, &comp, 4);
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &TBO);
	glBindTexture(GL_TEXTURE_2D, TBO);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);


	glGenerateMipmap(GL_TEXTURE_2D);
}

void TextureResource::LoadFromBuffer(unsigned char* imageBuffer, int width, int height)
{
	if (TBO == 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &TBO);
	}
	glBindTexture(GL_TEXTURE_2D, TBO);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageBuffer);

	glGenerateMipmap(GL_TEXTURE_2D);
}

unsigned char* TextureResource::Stbi_load(const char* filename, int* widthOUT, int* heightOUT, int channels)
{
	int comp;
	return stbi_load(filename, widthOUT, heightOUT, &comp, channels);
}
