#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "LoadObject.h"

struct VertexStruct
{
public:
	VertexStruct(const glm::vec3& position, const glm::vec2& texCoordinates)
	{
		this->position = position;
		this->texCoordinates = texCoordinates;
		this->normal = normal;
	}

	glm::vec3* GetPosition() { return &position; }
	glm::vec2* GetTexCoordinates() { return &texCoordinates; }
	glm::vec3* GetNormal() { return &normal; }

private:
	glm::vec3 position;
	glm::vec2 texCoordinates;
	glm::vec3 normal;
};

struct SphereStruct
{
public:

	SphereStruct() {}

	SphereStruct(glm::vec3& position, float radius)
	{
		this->position = position;
	}

	glm::vec3 GetPosition() { return position; }
	float GetRadius() { return radius; }

	void SetPos(glm::vec3 position)
	{
		this->position = position;
	}

	void SetRadius(float radius)
	{
		this->radius = radius;
	}

private:
	glm::vec3 position;
	float radius;
};

class MeshCreationClass
{
public:
	MeshCreationClass();
	~MeshCreationClass();


	void drawMeshToGame();
	void initialiseMeshes(VertexStruct* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void loadModelToGame(const std::string& filename);
	void initialiseGameModel(const ModelThatHasBeenIndexed& currentModel);
	void updateSphereData(glm::vec3 pos, float radius);
	glm::vec3 getSpherePosition() { return sphereInMesh.GetPosition(); }
	float getSphereRadius() { return sphereInMesh.GetRadius(); }

private:



	enum
	{
		POSITION_VERTEXBUFFER,
		TEXCOORDINATES_VERTEXBUFFER,
		NORMAL_VERTEXBUFFER,
		INDEX_VERTEXBUFFER,
		NUMBEROFBUFFERS
	};

	SphereStruct sphereInMesh;
	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUMBEROFBUFFERS]; // create our array of buffers
	unsigned int amountToDraw; //how much of the vertexArrayObject do we want to draw
};