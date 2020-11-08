#include "GraphicsNode.h"

void GraphicsNode::SetMeshResource(std::shared_ptr<MeshResource> meshResource)
{
	meshPointer = meshResource;
}

void GraphicsNode::SetTextureResource(std::shared_ptr<TextureResource> textureResource)
{
	texturePointer = textureResource;
}

void GraphicsNode::SetShaderObject(std::shared_ptr<ShaderObject> shaderObject)
{
	shaderPointer = shaderObject;
}

std::shared_ptr<MeshResource> GraphicsNode::GetMeshResource()
{
	return meshPointer;
}

std::shared_ptr<TextureResource> GraphicsNode::GetTextureResource()
{
	return texturePointer;
}

std::shared_ptr<ShaderObject> GraphicsNode::GetShaderObject()
{
	return shaderPointer;
}

void GraphicsNode::SetUpMatrix(Matrix4D matrix)
{
	this->matrix = matrix;
}

void GraphicsNode::Draw()
{
	shaderPointer->UseProgram();
	shaderPointer->UpdateRotation(matrix);
	meshPointer->Draw();
}