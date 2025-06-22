/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include "stb_image.h"

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;
std::map<std::string, MeshData>		ResourceManager::Models;

Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, std::string name)
{
	Textures[name] = loadTextureFromFile(file);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

bool ResourceManager::DoesTextureExist(const char* _textureName)
{
	if (Textures.find(_textureName) == Textures.end())
		return false;
	else
		return true;

}

bool ResourceManager::DoesShaderExist(const char* _shaderName)
{
	if (Shaders.find(_shaderName) == Shaders.end())
		return false;
	else
		return true;

}

MeshData ResourceManager::LoadModel(const char* _filePath, std::string _name)
{
	Assimp::Importer importer;

	if (!std::filesystem::exists(_filePath)) { VERTEX_ERROR("Failed to load Model: " + std::string(_filePath));   return MeshData(); }

	const aiScene* scene = importer.ReadFile(_filePath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

	if (scene == nullptr) return MeshData();

	aiMesh* mesh = *scene->mMeshes;
	MeshData MeshTemp;
	// gather all data for the mesh
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vert;
		// Vertices
		vert.position.x = mesh->mVertices[i].x;
		vert.position.y = mesh->mVertices[i].y;
		vert.position.z = mesh->mVertices[i].z;

		// Normals
		vert.Normal.x = mesh->mNormals[i].x;
		vert.Normal.y = mesh->mNormals[i].y;
		vert.Normal.z = mesh->mNormals[i].z;

		// Uvs 
		for (unsigned int j = 0; j < mesh->GetNumUVChannels(); j++) {
			vert.TexCoord.x = mesh->mTextureCoords[j][i].x;
			vert.TexCoord.y = mesh->mTextureCoords[j][i].y;
		}

		// Tangents & Bi Tangents
		if (mesh->HasTangentsAndBitangents()) {

			vert.Tangent.x = mesh->mTangents[i].x;
			vert.Tangent.y = mesh->mTangents[i].y;
			vert.Tangent.z = mesh->mTangents[i].z;

			vert.BitTangent.x = mesh->mBitangents[i].x;
			vert.BitTangent.y = mesh->mBitangents[i].y;
			vert.BitTangent.z = mesh->mBitangents[i].z;
		}

		MeshTemp.vertices.push_back(vert);
	}

	// Faces
	if (mesh->HasFaces()) {
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			MeshTemp.indices.push_back((unsigned short)mesh->mFaces[i].mIndices[0]);
			MeshTemp.indices.push_back((unsigned short)mesh->mFaces[i].mIndices[1]);
			MeshTemp.indices.push_back((unsigned short)mesh->mFaces[i].mIndices[2]);
		}
	}

	// Keep the loaded model stored in the resource manager for later use.
	Models[_name] = MeshTemp;
	return MeshTemp;
}

MeshData ResourceManager::GetModel(std::string _name)
{
	return Models[_name];
}

void ResourceManager::Clear()
{
	// (properly) delete all shaders	
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	// (properly) delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		// open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// if geometry shader path is present, also load a geometry shader
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		VERTEX_ERROR("Shader " + std::string(vShaderFile) + " failed to read. Please check spelling or corrupt files.");
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();
	// 2. now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* file)
{
	Texture2D texture;
	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);


	if (nrChannels == 3)
	{
		texture.Internal_Format = GL_RGB;
		texture.Image_Format = GL_RGB;
	}
	else if (nrChannels == 4)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	else {

		VERTEX_ERROR("Texture " + std::string(file) + " failed to read. Please check spelling or corrupt files.");
	}

	texture.Generate(width, height, data);

	stbi_image_free(data);
	return texture;
}