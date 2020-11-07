#pragma once
#include <memory>
#include "MeshResource.h"
#include "TextureResource.h"
#include "ShaderObject.h"

class GraphicsNode
{
public:
	void SetMeshResource(std::shared_ptr<MeshResource> meshResource);
	void SetTextureResource(std::shared_ptr<TextureResource> textureResource);
	void SetShaderObject(std::shared_ptr<ShaderObject> shaderObject);

	std::shared_ptr<MeshResource> GetMeshResource();
	std::shared_ptr<TextureResource> GetTextureResource();
	std::shared_ptr<ShaderObject> GetShaderObject();

	void SetUpMatrix(Matrix4D matrix);
	void Draw();
private:
	std::shared_ptr<MeshResource> meshPointer;
	std::shared_ptr<TextureResource> texturePointer;
	std::shared_ptr<ShaderObject> shaderPointer;
	Matrix4D matrix;
};