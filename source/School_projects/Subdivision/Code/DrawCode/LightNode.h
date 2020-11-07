#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
class LightNode
{
public:
	LightNode();
	void SetPosition(float x, float y, float z);
	void SetAmbient(float r, float g, float b);
	void SetDiffuse(float r, float g, float b);
	void SetSpecular(float r, float g, float b);
	void SetShineStrength(float x);
	void SetScreenGamma(float x);

	std::vector<float> GetPosition();
	std::vector<float> GetAmbient();
	std::vector<float> GetDiffuse();
	std::vector<float> GetSpecular();
	float GetShineStrength();
	float GetScreenGamma();

	void UpdateLight(GLuint program);
private:
	float position[3];
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float shineStrength;
	float screenGamma;
};