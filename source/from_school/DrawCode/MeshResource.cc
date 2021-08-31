#include "MeshResource.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

MeshResource::MeshResource(bool withUV, bool withNormal)
{
	this->withUV = withUV;
	this->withNormal = withNormal;
}

MeshResource::~MeshResource()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void MeshResource::SetUpVBO(float* vertexBuffer, int sizeOfVertexBuffer)
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertexBuffer * sizeof(float), vertexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshResource::SetUpEBO(int* indexBuffer, int sizeOfIndexBuffer)
{
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexBuffer * sizeof(int), indexBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	this->sizeOfIndexBuffer = sizeOfIndexBuffer;
}

void MeshResource::Draw(bool wireframe)
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glEnableVertexAttribArray(0);
	int i = 0;
	if (withUV)
	{
		glEnableVertexAttribArray(1);
		i += 2;
	}
	if (withNormal)
	{
		glEnableVertexAttribArray(2);
		i += 3;
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (3 + i), nullptr);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * (3 + i), (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (3 + i), (GLvoid*)(sizeof(float) * i));

	glDrawElements(GL_TRIANGLES, sizeOfIndexBuffer, GL_UNSIGNED_INT, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

MeshResource* MeshResource::Cube()
{
	MeshResource* mesh = new MeshResource(true, false);

	float buf[] = {
		0.5f,	-0.5f,	-0.5f, //fram
		0.0f,	1.0f,
		0.5f,	-0.5f,	0.5f,
		1.0f,	1.0f,
		-0.5f,	-0.5f,	0.5f,
		1.0f,	0.0f,
		-0.5f,	-0.5f,	-0.5f,
		0.0f,	0.0f,

		0.5f,	0.5f,	-0.5f, //bak
		1.0f,	1.0f,
		0.5f,	0.5f,	0.5f,
		0.0f,	1.0f,
		-0.5f,	0.5f,	0.5f,
		0.0f,	0.0f,
		-0.5f,	0.5f,	-0.5f,
		1.0f,	0.0f
	};

	int indices[] = {
		4, 0, 3,
		4, 3, 7,
		2, 6, 7,
		2, 7, 3,
		1, 5, 2,
		5, 6, 2,
		0, 4, 1,
		4, 5, 1,
		4, 7, 5,
		7, 6, 5,
		0, 1, 2,
		0, 2, 3 };

	(*mesh).SetUpVBO(buf, 56);
	(*mesh).SetUpEBO(indices, 36);
	return mesh;
}

MeshResource* MeshResource::LoadFromOBJ(const char* filePath, bool withUV, bool withNormal)
{
	std::ifstream fileStream(filePath, std::ios::in);
	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist, crash imminent kek." << std::endl;
	}

	std::vector<float> position;
	std::vector<float> textureCoord;
	std::vector<float> normal;
	std::vector<float> completeBuffer;
	int indiceCounter = 0;
	std::vector<int> indices;
	//Loop through obj file
	while (!fileStream.eof()) {
		std::string line = "";
		std::getline(fileStream, line);
		if (line == "")
			continue;
		
		int i = 0;
		std::string task = "";
		for (; i < line.length(); i++)
		{
			if (line.at(i) == ' ' && task == "")
				continue;
			else if (line.at(i) == ' ')
				break;
			else
				task += line.at(i);
		}
		/////////////////////////////////////////////////////////////
		if (task == "v")
		{
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == ' ')
				{
					position.push_back(std::stof(item));
					item = "";
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
					position.push_back(std::stof(item));
			}
		}/////////////////////////////////////////////////////////////
		else if (task == "vt")
		{
			int itemSaved = 0;
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (itemSaved == 2)
					break;
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == ' ')
				{
					textureCoord.push_back(std::stof(item));
					item = "";
					itemSaved++;
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
					textureCoord.push_back(std::stof(item));
			}
		}/////////////////////////////////////////////////////////////
		else if (task == "vn")
		{
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == ' ')
				{
					normal.push_back(std::stof(item));
					item = "";
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
					normal.push_back(std::stof(item));
			}
		}/////////////////////////////////////////////////////////////
		else if (task == "f")
		{
			int counter = 0; //counter; 0 == vector, 1 == texture coordinate, 2 == normal
			int sides = 0;
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == '/' && item == "")
				{
					counter++;
					continue;
				}
				else if (line.at(i) == '/')
				{
					int index = std::stoi(item) - 1;
					switch (counter)
					{
						case 0:
							completeBuffer.push_back(position.at(index * 3));
							completeBuffer.push_back(position.at(index * 3 + 1));
							completeBuffer.push_back(position.at(index * 3 + 2));
							if (sides < 3)
								indices.push_back(indiceCounter);
							else
							{
								indices.push_back(indiceCounter - 3);
								indices.push_back(indiceCounter - 1);
								indices.push_back(indiceCounter);
							}
							indiceCounter++;
							break;
						case 1:
							completeBuffer.push_back(textureCoord.at(index * 2));
							completeBuffer.push_back(textureCoord.at(index * 2 + 1));
							break;
					}
					counter++;
					item = "";
				}
				else if (line.at(i) == ' ')
				{
					int index = std::stoi(item) - 1;
					switch (counter)
					{
						case 0:
							completeBuffer.push_back(position.at(index * 3));
							completeBuffer.push_back(position.at(index * 3 + 1));
							completeBuffer.push_back(position.at(index * 3 + 2));
							if (sides < 3)
								indices.push_back(indiceCounter);
							else
							{
								indices.push_back(indiceCounter - 3);
								indices.push_back(indiceCounter - 1);
								indices.push_back(indiceCounter);
							}
							indiceCounter++;
							break;
						case 1:
							completeBuffer.push_back(textureCoord.at(index * 2));
							completeBuffer.push_back(textureCoord.at(index * 2 + 1));
							break;
						case 2:
							completeBuffer.push_back(normal.at(index * 3));
							completeBuffer.push_back(normal.at(index * 3 + 1));
							completeBuffer.push_back(normal.at(index * 3 + 2));
							break;
					}
					counter = 0;
					item = "";
					sides++;
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
				{
					int index = std::stoi(item) - 1;
					switch (counter)
					{
					case 0:
						completeBuffer.push_back(position.at(index * 3));
						completeBuffer.push_back(position.at(index * 3 + 1));
						completeBuffer.push_back(position.at(index * 3 + 2));
						if (sides < 3)
							indices.push_back(indiceCounter);
						else
						{
							indices.push_back(indiceCounter - 3);
							indices.push_back(indiceCounter - 1);
							indices.push_back(indiceCounter);
						}
						indiceCounter++;
						break;
					case 1:
						completeBuffer.push_back(textureCoord.at(index * 2));
						completeBuffer.push_back(textureCoord.at(index * 2 + 1));
						break;
					case 2:
						completeBuffer.push_back(normal.at(index * 3));
						completeBuffer.push_back(normal.at(index * 3 + 1));
						completeBuffer.push_back(normal.at(index * 3 + 2));
						break;
					}
				}
			}
		}
	}
	//End loop
	fileStream.close();

	MeshResource* mesh = new MeshResource(withUV, withNormal);
	(*mesh).SetUpVBO(completeBuffer.data(), completeBuffer.size());
	(*mesh).SetUpEBO(indices.data(), indices.size());
	return mesh;
}

void MeshResource::LoadFromOBJ(const char* filePath, std::vector<float>* vertexBuf, std::vector<int>* indexBuf)
{
	std::ifstream fileStream(filePath, std::ios::in);
	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist, crash imminent kek." << std::endl;
	}

	std::vector<float> position;
	std::vector<float> textureCoord;
	std::vector<float> normal;
	std::vector<float> completeBuffer;
	int indiceCounter = 0;
	std::vector<int> indices;
	//Loop through obj file
	while (!fileStream.eof()) {
		std::string line = "";
		std::getline(fileStream, line);
		if (line == "")
			continue;

		int i = 0;
		std::string task = "";
		for (; i < line.length(); i++)
		{
			if (line.at(i) == ' ' && task == "")
				continue;
			else if (line.at(i) == ' ')
				break;
			else
				task += line.at(i);
		}
		/////////////////////////////////////////////////////////////
		if (task == "v")
		{
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == ' ')
				{
					position.push_back(std::stof(item));
					item = "";
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
					position.push_back(std::stof(item));
			}
		}/////////////////////////////////////////////////////////////
		else if (task == "vt")
		{
			int itemSaved = 0;
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (itemSaved == 2)
					break;
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == ' ')
				{
					textureCoord.push_back(std::stof(item));
					item = "";
					itemSaved++;
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
					textureCoord.push_back(std::stof(item));
			}
		}/////////////////////////////////////////////////////////////
		else if (task == "vn")
		{
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == ' ')
				{
					normal.push_back(std::stof(item));
					item = "";
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
					normal.push_back(std::stof(item));
			}
		}/////////////////////////////////////////////////////////////
		else if (task == "f")
		{
			int counter = 0; //counter; 0 == vector, 1 == texture coordinate, 2 == normal
			int sides = 0;
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == '/' && item == "")
				{
					counter++;
					continue;
				}
				else if (line.at(i) == '/')
				{
					int index = std::stoi(item) - 1;
					switch (counter)
					{
					case 0:
						completeBuffer.push_back(position.at(index * 3));
						completeBuffer.push_back(position.at(index * 3 + 1));
						completeBuffer.push_back(position.at(index * 3 + 2));
						if (sides < 3)
							indices.push_back(indiceCounter);
						else
						{
							indices.push_back(indiceCounter - 3);
							indices.push_back(indiceCounter - 1);
							indices.push_back(indiceCounter);
						}
						indiceCounter++;
						break;
					case 1:
						completeBuffer.push_back(textureCoord.at(index * 2));
						completeBuffer.push_back(textureCoord.at(index * 2 + 1));
						break;
					}
					counter++;
					item = "";
				}
				else if (line.at(i) == ' ')
				{
					int index = std::stoi(item) - 1;
					switch (counter)
					{
					case 0:
						completeBuffer.push_back(position.at(index * 3));
						completeBuffer.push_back(position.at(index * 3 + 1));
						completeBuffer.push_back(position.at(index * 3 + 2));
						if (sides < 3)
							indices.push_back(indiceCounter);
						else
						{
							indices.push_back(indiceCounter - 3);
							indices.push_back(indiceCounter - 1);
							indices.push_back(indiceCounter);
						}
						indiceCounter++;
						break;
					case 1:
						completeBuffer.push_back(textureCoord.at(index * 2));
						completeBuffer.push_back(textureCoord.at(index * 2 + 1));
						break;
					case 2:
						completeBuffer.push_back(normal.at(index * 3));
						completeBuffer.push_back(normal.at(index * 3 + 1));
						completeBuffer.push_back(normal.at(index * 3 + 2));
						break;
					}
					counter = 0;
					item = "";
					sides++;
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
				{
					int index;
					try {
						index = std::stoi(item) - 1;
					}
					catch (std::invalid_argument) {
						break;
					}

					switch (counter)
					{
					case 0:
						completeBuffer.push_back(position.at(index * 3));
						completeBuffer.push_back(position.at(index * 3 + 1));
						completeBuffer.push_back(position.at(index * 3 + 2));
						if (sides < 3)
							indices.push_back(indiceCounter);
						else
						{
							indices.push_back(indiceCounter - 3);
							indices.push_back(indiceCounter - 1);
							indices.push_back(indiceCounter);
						}
						indiceCounter++;
						break;
					case 1:
						completeBuffer.push_back(textureCoord.at(index * 2));
						completeBuffer.push_back(textureCoord.at(index * 2 + 1));
						break;
					case 2:
						completeBuffer.push_back(normal.at(index * 3));
						completeBuffer.push_back(normal.at(index * 3 + 1));
						completeBuffer.push_back(normal.at(index * 3 + 2));
						break;
					}
				}
			}
		}
	}
	//End loop
	fileStream.close();

	(*vertexBuf) = completeBuffer;
	(*indexBuf) = indices;
}

MeshResource* MeshResource::LoadWithBoundingBoxFromOBJ(const char* filePath, bool withUV, bool withNormal, float *boxDimensions, float *boxOrigin)
{
	std::ifstream fileStream(filePath, std::ios::in);
	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist, crash imminent kek." << std::endl;
	}

	std::vector<float> position;
	std::vector<float> textureCoord;
	std::vector<float> normal;
	std::vector<float> completeBuffer;
	int indiceCounter = 0;
	std::vector<int> indices;
	//Loop through obj file
	while (!fileStream.eof()) {
		std::string line = "";
		std::getline(fileStream, line);
		if (line == "")
			continue;

		int i = 0;
		std::string task = "";
		for (; i < line.length(); i++)
		{
			if (line.at(i) == ' ' && task == "")
				continue;
			else if (line.at(i) == ' ')
				break;
			else
				task += line.at(i);
		}
		/////////////////////////////////////////////////////////////
		if (task == "v")
		{
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == ' ')
				{
					position.push_back(std::stof(item));
					item = "";
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
					position.push_back(std::stof(item));
			}
		}/////////////////////////////////////////////////////////////
		else if (task == "vt")
		{
			int itemSaved = 0;
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (itemSaved == 2)
					break;
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == ' ')
				{
					textureCoord.push_back(std::stof(item));
					item = "";
					itemSaved++;
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
					textureCoord.push_back(std::stof(item));
			}
		}/////////////////////////////////////////////////////////////
		else if (task == "vn")
		{
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == ' ')
				{
					normal.push_back(std::stof(item));
					item = "";
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
					normal.push_back(std::stof(item));
			}
		}/////////////////////////////////////////////////////////////
		else if (task == "f")
		{
			int counter = 0; //counter; 0 == vector, 1 == texture coordinate, 2 == normal
			int sides = 0;
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == '/' && item == "")
				{
					counter++;
					continue;
				}
				else if (line.at(i) == '/')
				{
					int index = std::stoi(item) - 1;
					switch (counter)
					{
						case 0:
							completeBuffer.push_back(position.at(index * 3));
							completeBuffer.push_back(position.at(index * 3 + 1));
							completeBuffer.push_back(position.at(index * 3 + 2));
							if (sides < 3)
								indices.push_back(indiceCounter);
							else
							{
								indices.push_back(indiceCounter - 3);
								indices.push_back(indiceCounter - 1);
								indices.push_back(indiceCounter);
							}
							indiceCounter++;
							break;
						case 1:
							completeBuffer.push_back(textureCoord.at(index * 2));
							completeBuffer.push_back(textureCoord.at(index * 2 + 1));
							break;
					}
					counter++;
					item = "";
				}
				else if (line.at(i) == ' ')
				{
					int index = std::stoi(item) - 1;
					switch (counter)
					{
						case 0:
							completeBuffer.push_back(position.at(index * 3));
							completeBuffer.push_back(position.at(index * 3 + 1));
							completeBuffer.push_back(position.at(index * 3 + 2));
							if (sides < 3)
								indices.push_back(indiceCounter);
							else
							{
								indices.push_back(indiceCounter - 3);
								indices.push_back(indiceCounter - 1);
								indices.push_back(indiceCounter);
							}
							indiceCounter++;
							break;
						case 1:
							completeBuffer.push_back(textureCoord.at(index * 2));
							completeBuffer.push_back(textureCoord.at(index * 2 + 1));
							break;
						case 2:
							completeBuffer.push_back(normal.at(index * 3));
							completeBuffer.push_back(normal.at(index * 3 + 1));
							completeBuffer.push_back(normal.at(index * 3 + 2));
							break;
					}
					counter = 0;
					item = "";
					sides++;
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
				{
					int index = std::stoi(item) - 1;
					switch (counter)
					{
						case 0:
							completeBuffer.push_back(position.at(index * 3));
							completeBuffer.push_back(position.at(index * 3 + 1));
							completeBuffer.push_back(position.at(index * 3 + 2));
							if (sides < 3)
								indices.push_back(indiceCounter);
							else
							{
								indices.push_back(indiceCounter - 3);
								indices.push_back(indiceCounter - 1);
								indices.push_back(indiceCounter);
							}
							indiceCounter++;
							break;
						case 1:
							completeBuffer.push_back(textureCoord.at(index * 2));
							completeBuffer.push_back(textureCoord.at(index * 2 + 1));
							break;
						case 2:
							completeBuffer.push_back(normal.at(index * 3));
							completeBuffer.push_back(normal.at(index * 3 + 1));
							completeBuffer.push_back(normal.at(index * 3 + 2));
							break;
					}
				}
			}
		}
	}
	//End loop
	fileStream.close();

    //Up, Right, Forward
    float min[3] = {position[0], position[1], position[2]};
    float max[3] = {position[0], position[1], position[2]};

	for (int j = 0; j < position.size(); j+=3)
	{
        float corner[3] = {position[j], position[j+1], position[j+2]};
        for (int i = 0; i < 3; i++)
        {
            if (corner[i] < min[i])
                min[i] = corner[i];
            else if (corner[i] > max[i])
                max[i] = corner[i];
        }
	}

    boxDimensions[0] = max[0]-min[0];
    boxDimensions[1] = max[1]-min[1];
    boxDimensions[2] = max[2]-min[2];

    boxOrigin[0] = min[0];
    boxOrigin[1] = min[1];
    boxOrigin[2] = min[2];

	MeshResource* mesh = new MeshResource(withUV, withNormal);
	(*mesh).SetUpVBO(completeBuffer.data(), completeBuffer.size());
	(*mesh).SetUpEBO(indices.data(), indices.size());
	return mesh;
}

MeshResource* MeshResource::LoadWithBoundingBoxFromOBJ(const char* filePath, bool withUV, bool withNormal, float mass, Vector4D *centerOfMass, Matrix4D *inertiaTensor, float *boxDimensions, float *boxOrigin)
{
	std::ifstream fileStream(filePath, std::ios::in);
	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist, crash imminent kek." << std::endl;
	}

	std::vector<float> position;
	std::vector<float> textureCoord;
	std::vector<float> normal;
	std::vector<float> completeBuffer;
	int indiceCounter = 0;
	std::vector<int> indices;
	//Loop through obj file
	while (!fileStream.eof()) {
		std::string line = "";
		std::getline(fileStream, line);
		if (line == "")
			continue;

		int i = 0;
		std::string task = "";
		for (; i < line.length(); i++)
		{
			if (line.at(i) == ' ' && task == "")
				continue;
			else if (line.at(i) == ' ')
				break;
			else
				task += line.at(i);
		}
		/////////////////////////////////////////////////////////////
		if (task == "v")
		{
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == ' ')
				{
					position.push_back(std::stof(item));
					item = "";
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
					position.push_back(std::stof(item));
			}
		}/////////////////////////////////////////////////////////////
		else if (task == "vt")
		{
			int itemSaved = 0;
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (itemSaved == 2)
					break;
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == ' ')
				{
					textureCoord.push_back(std::stof(item));
					item = "";
					itemSaved++;
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
					textureCoord.push_back(std::stof(item));
			}
		}/////////////////////////////////////////////////////////////
		else if (task == "vn")
		{
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == ' ')
				{
					normal.push_back(std::stof(item));
					item = "";
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
					normal.push_back(std::stof(item));
			}
		}/////////////////////////////////////////////////////////////
		else if (task == "f")
		{
			int counter = 0; //counter; 0 == vector, 1 == texture coordinate, 2 == normal
			int sides = 0;
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == '/' && item == "")
				{
					counter++;
					continue;
				}
				else if (line.at(i) == '/')
				{
					int index = std::stoi(item) - 1;
					switch (counter)
					{
						case 0:
							completeBuffer.push_back(position.at(index * 3));
							completeBuffer.push_back(position.at(index * 3 + 1));
							completeBuffer.push_back(position.at(index * 3 + 2));
							if (sides < 3)
								indices.push_back(indiceCounter);
							else
							{
								indices.push_back(indiceCounter - 3);
								indices.push_back(indiceCounter - 1);
								indices.push_back(indiceCounter);
							}
							indiceCounter++;
							break;
						case 1:
							completeBuffer.push_back(textureCoord.at(index * 2));
							completeBuffer.push_back(textureCoord.at(index * 2 + 1));
							break;
					}
					counter++;
					item = "";
				}
				else if (line.at(i) == ' ')
				{
					int index = std::stoi(item) - 1;
					switch (counter)
					{
						case 0:
							completeBuffer.push_back(position.at(index * 3));
							completeBuffer.push_back(position.at(index * 3 + 1));
							completeBuffer.push_back(position.at(index * 3 + 2));
							if (sides < 3)
								indices.push_back(indiceCounter);
							else
							{
								indices.push_back(indiceCounter - 3);
								indices.push_back(indiceCounter - 1);
								indices.push_back(indiceCounter);
							}
							indiceCounter++;
							break;
						case 1:
							completeBuffer.push_back(textureCoord.at(index * 2));
							completeBuffer.push_back(textureCoord.at(index * 2 + 1));
							break;
						case 2:
							completeBuffer.push_back(normal.at(index * 3));
							completeBuffer.push_back(normal.at(index * 3 + 1));
							completeBuffer.push_back(normal.at(index * 3 + 2));
							break;
					}
					counter = 0;
					item = "";
					sides++;
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
				{
					int index = std::stoi(item) - 1;
					switch (counter)
					{
						case 0:
							completeBuffer.push_back(position.at(index * 3));
							completeBuffer.push_back(position.at(index * 3 + 1));
							completeBuffer.push_back(position.at(index * 3 + 2));
							if (sides < 3)
								indices.push_back(indiceCounter);
							else
							{
								indices.push_back(indiceCounter - 3);
								indices.push_back(indiceCounter - 1);
								indices.push_back(indiceCounter);
							}
							indiceCounter++;
							break;
						case 1:
							completeBuffer.push_back(textureCoord.at(index * 2));
							completeBuffer.push_back(textureCoord.at(index * 2 + 1));
							break;
						case 2:
							completeBuffer.push_back(normal.at(index * 3));
							completeBuffer.push_back(normal.at(index * 3 + 1));
							completeBuffer.push_back(normal.at(index * 3 + 2));
							break;
					}
				}
			}
		}
	}
	//End loop
	fileStream.close();

	//Bounding box values
	float min[3] = {position[0], position[1], position[2]};
	float max[3] = {position[0], position[1], position[2]};

	//Inertia tensor variabler
	Matrix4D tensor;
	tensor.SetValue(0, 0);
	tensor.SetValue(5, 0);
	tensor.SetValue(10, 0);
	float pointMass = mass / (position.size() / 3);

	//Center of mass
	Vector4D CoM = Vector4D(0,0,0);
	int points = 0;

	for (int j = 0; j < position.size(); j+=3)
	{
		float corner[3] = {position[j], position[j+1], position[j+2]};
		CoM = CoM + Vector4D(corner[0], corner[1], corner[2]);
		points++;

		tensor.SetValue(0, tensor[0] +  pointMass * (corner[1]*corner[1] + corner[2]*corner[2]));
		tensor.SetValue(1, tensor[1] -  pointMass * corner[0] * corner[1]);
		tensor.SetValue(2, tensor[2] -  pointMass * corner[0] * corner[2]);
		tensor.SetValue(4, tensor[4] -  pointMass * corner[0] * corner[1]);
		tensor.SetValue(5, tensor[5] +  pointMass * (corner[0]*corner[0] + corner[2]*corner[2]));
		tensor.SetValue(6, tensor[6] -  pointMass * corner[1] * corner[2]);
		tensor.SetValue(8, tensor[8] -  pointMass * corner[0] * corner[2]);
		tensor.SetValue(9, tensor[9] -  pointMass * corner[1] * corner[2]);
		tensor.SetValue(10, tensor[10] +  pointMass * (corner[0]*corner[0] + corner[1]*corner[1]));

		for (int i = 0; i < 3; i++)
		{
			if (corner[i] < min[i])
				min[i] = corner[i];
			else if (corner[i] > max[i])
				max[i] = corner[i];
		}
	}

	CoM = CoM * (1.0f/points);
	*centerOfMass = CoM;

	*inertiaTensor = tensor;

	boxDimensions[0] = max[0]-min[0];
	boxDimensions[1] = max[1]-min[1];
	boxDimensions[2] = max[2]-min[2];

	boxOrigin[0] = min[0];
	boxOrigin[1] = min[1];
	boxOrigin[2] = min[2];

	MeshResource* mesh = new MeshResource(withUV, withNormal);
	(*mesh).SetUpVBO(completeBuffer.data(), completeBuffer.size());
	(*mesh).SetUpEBO(indices.data(), indices.size());
	return mesh;
}

MeshResource* MeshResource::LoadWithBoundingBoxFromOBJ(const char* filePath, bool withUV, bool withNormal, float mass, Vector4D *centerOfMass, Matrix4D *inertiaTensor, float *boxDimensions, float *boxOrigin, std::vector<Vector4D> *polygon)
{
	std::ifstream fileStream(filePath, std::ios::in);
	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist, crash imminent kek." << std::endl;
	}

	std::vector<float> position;
	std::vector<float> textureCoord;
	std::vector<float> normal;
	std::vector<float> completeBuffer;
	int indiceCounter = 0;
	std::vector<int> indices;
	//Loop through obj file
	while (!fileStream.eof()) {
		std::string line = "";
		std::getline(fileStream, line);
		if (line == "")
			continue;

		int i = 0;
		std::string task = "";
		for (; i < line.length(); i++)
		{
			if (line.at(i) == ' ' && task == "")
				continue;
			else if (line.at(i) == ' ')
				break;
			else
				task += line.at(i);
		}
		/////////////////////////////////////////////////////////////
		if (task == "v")
		{
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == ' ')
				{
					position.push_back(std::stof(item));
					item = "";
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
					position.push_back(std::stof(item));
			}
		}/////////////////////////////////////////////////////////////
		else if (task == "vt")
		{
			int itemSaved = 0;
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (itemSaved == 2)
					break;
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == ' ')
				{
					textureCoord.push_back(std::stof(item));
					item = "";
					itemSaved++;
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
					textureCoord.push_back(std::stof(item));
			}
		}/////////////////////////////////////////////////////////////
		else if (task == "vn")
		{
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == ' ')
				{
					normal.push_back(std::stof(item));
					item = "";
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
					normal.push_back(std::stof(item));
			}
		}/////////////////////////////////////////////////////////////
		else if (task == "f")
		{
			int counter = 0; //counter; 0 == vector, 1 == texture coordinate, 2 == normal
			int sides = 0;
			std::string item = "";
			for (; i < line.length(); i++)
			{
				if (line.at(i) == ' ' && item == "")
					continue;
				else if (line.at(i) == '/' && item == "")
				{
					counter++;
					continue;
				}
				else if (line.at(i) == '/')
				{
					int index = std::stoi(item) - 1;
					switch (counter)
					{
						case 0:
							completeBuffer.push_back(position.at(index * 3));
							completeBuffer.push_back(position.at(index * 3 + 1));
							completeBuffer.push_back(position.at(index * 3 + 2));
							if (sides < 3)
								indices.push_back(indiceCounter);
							else
							{
								indices.push_back(indiceCounter - 3);
								indices.push_back(indiceCounter - 1);
								indices.push_back(indiceCounter);
							}
							indiceCounter++;
							break;
						case 1:
							completeBuffer.push_back(textureCoord.at(index * 2));
							completeBuffer.push_back(textureCoord.at(index * 2 + 1));
							break;
					}
					counter++;
					item = "";
				}
				else if (line.at(i) == ' ')
				{
					int index = std::stoi(item) - 1;
					switch (counter)
					{
						case 0:
							completeBuffer.push_back(position.at(index * 3));
							completeBuffer.push_back(position.at(index * 3 + 1));
							completeBuffer.push_back(position.at(index * 3 + 2));
							if (sides < 3)
								indices.push_back(indiceCounter);
							else
							{
								indices.push_back(indiceCounter - 3);
								indices.push_back(indiceCounter - 1);
								indices.push_back(indiceCounter);
							}
							indiceCounter++;
							break;
						case 1:
							completeBuffer.push_back(textureCoord.at(index * 2));
							completeBuffer.push_back(textureCoord.at(index * 2 + 1));
							break;
						case 2:
							completeBuffer.push_back(normal.at(index * 3));
							completeBuffer.push_back(normal.at(index * 3 + 1));
							completeBuffer.push_back(normal.at(index * 3 + 2));
							break;
					}
					counter = 0;
					item = "";
					sides++;
				}
				else
					item += line.at(i);

				if (i == line.length() - 1 && item != "")
				{
					int index = std::stoi(item) - 1;
					switch (counter)
					{
						case 0:
							completeBuffer.push_back(position.at(index * 3));
							completeBuffer.push_back(position.at(index * 3 + 1));
							completeBuffer.push_back(position.at(index * 3 + 2));
							if (sides < 3)
								indices.push_back(indiceCounter);
							else
							{
								indices.push_back(indiceCounter - 3);
								indices.push_back(indiceCounter - 1);
								indices.push_back(indiceCounter);
							}
							indiceCounter++;
							break;
						case 1:
							completeBuffer.push_back(textureCoord.at(index * 2));
							completeBuffer.push_back(textureCoord.at(index * 2 + 1));
							break;
						case 2:
							completeBuffer.push_back(normal.at(index * 3));
							completeBuffer.push_back(normal.at(index * 3 + 1));
							completeBuffer.push_back(normal.at(index * 3 + 2));
							break;
					}
				}
			}
		}
	}
	//End loop
	fileStream.close();

	//Bounding box values
	float min[3] = {position[0], position[1], position[2]};
	float max[3] = {position[0], position[1], position[2]};

	//Inertia tensor variabler
	Matrix4D tensor;
	tensor.SetValue(0, 0);
	tensor.SetValue(5, 0);
	tensor.SetValue(10, 0);
	float pointMass = mass / (position.size() / 3);

	//Center of mass
	Vector4D CoM = Vector4D(0,0,0);
	int points = 0;

	//List of corners
	std::vector<Vector4D> corners = std::vector<Vector4D>();

	for (int j = 0; j < position.size(); j+=3)
	{
		float corner[3] = {position[j], position[j+1], position[j+2]};
		CoM = CoM + Vector4D(corner[0], corner[1], corner[2]);
		points++;

		corners.push_back(Vector4D(corner[0],corner[1],corner[2]));

		tensor.SetValue(0, tensor[0] +  pointMass * (corner[1]*corner[1] + corner[2]*corner[2]));
		tensor.SetValue(1, tensor[1] -  pointMass * corner[0] * corner[1]);
		tensor.SetValue(2, tensor[2] -  pointMass * corner[0] * corner[2]);
		tensor.SetValue(4, tensor[4] -  pointMass * corner[0] * corner[1]);
		tensor.SetValue(5, tensor[5] +  pointMass * (corner[0]*corner[0] + corner[2]*corner[2]));
		tensor.SetValue(6, tensor[6] -  pointMass * corner[1] * corner[2]);
		tensor.SetValue(8, tensor[8] -  pointMass * corner[0] * corner[2]);
		tensor.SetValue(9, tensor[9] -  pointMass * corner[1] * corner[2]);
		tensor.SetValue(10, tensor[10] +  pointMass * (corner[0]*corner[0] + corner[1]*corner[1]));

		for (int i = 0; i < 3; i++)
		{
			if (corner[i] < min[i])
				min[i] = corner[i];
			else if (corner[i] > max[i])
				max[i] = corner[i];
		}
	}

	CoM = CoM * (1.0f/points);
	*centerOfMass = CoM;

	*inertiaTensor = tensor;

	*polygon = corners;

	boxDimensions[0] = max[0]-min[0];
	boxDimensions[1] = max[1]-min[1];
	boxDimensions[2] = max[2]-min[2];

	boxOrigin[0] = min[0];
	boxOrigin[1] = min[1];
	boxOrigin[2] = min[2];

	MeshResource* mesh = new MeshResource(withUV, withNormal);
	(*mesh).SetUpVBO(completeBuffer.data(), completeBuffer.size());
	(*mesh).SetUpEBO(indices.data(), indices.size());
	return mesh;
}