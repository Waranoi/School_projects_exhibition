#include "LightNode.h"

LightNode::LightNode()
{
	SetPosition(0, 0, -0.1);
	SetAmbient(0, 0, 0);
	SetDiffuse(0.5f, 0.5f, 0.5f);
	SetSpecular(1, 1, 1);
	SetShineStrength(16);
	SetScreenGamma(2.5f);
}

void LightNode::UpdateLight(GLuint program)
{
	glUniform3fv(glGetUniformLocation(program, "lightPos"), 1, position);
	glUniform3fv(glGetUniformLocation(program, "ambientColor"), 1, ambient);
	glUniform3fv(glGetUniformLocation(program, "diffuseColor"), 1, diffuse);
	glUniform3fv(glGetUniformLocation(program, "specColor"), 1, specular);
	glUniform1f(glGetUniformLocation(program, "shininess"), shineStrength);
	glUniform1f(glGetUniformLocation(program, "screenGamma"), screenGamma);
}

void LightNode::SetPosition(float x, float y, float z)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
}

void LightNode::SetAmbient(float r, float g, float b)
{
	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
}

void LightNode::SetDiffuse(float r, float g, float b)
{
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
}

void LightNode::SetSpecular(float r, float g, float b)
{
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
}

void LightNode::SetShineStrength(float strength)
{
	shineStrength = strength;
}

void LightNode::SetScreenGamma(float screenGamma)
{
	this->screenGamma = screenGamma;
}

std::vector<float> LightNode::GetPosition()
{
	return{ position[0], position[1], position[2] };
}

std::vector<float> LightNode::GetAmbient()
{
	return{ ambient[0], ambient[1], ambient[2] };
}

std::vector<float> LightNode::GetDiffuse()
{
	return{ diffuse[0], diffuse[1], diffuse[2] };
}

std::vector<float> LightNode::GetSpecular()
{
	return{ specular[0], specular[1], specular[2] };
}

float LightNode::GetShineStrength()
{
	return shineStrength;
}

float LightNode::GetScreenGamma()
{
	return screenGamma;
}