#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "mesh_loader.h"

struct Vert
{
public:
	Vert(const glm::vec3& pos, const glm::vec2& texCoord)
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
	}

	glm::vec3* GetPos() { return &pos; }
	glm::vec2* GetTexCoord() { return &texCoord; }
	glm::vec3* GetNormal() { return &normal; }

private:
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

struct SphereCollider
{
public:

	SphereCollider() {}

	SphereCollider(glm::vec3& pos, float radius)
	{
		this->pos = pos;
	}

	glm::vec3 GetPos() { return pos; }
	float GetRadius() { return radius; }

	void SetPos(glm::vec3 pos)
	{
		this->pos = pos;
	}

	void SetRadius(float radius)
	{
		this->radius = radius;
	}

private:
	glm::vec3 pos;
	float radius;
};

class CreateMesh
{
public:
	CreateMesh();
	~CreateMesh();


	void Draw();
	void init(Vert* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void LoadMesh(const std::string& filename);
	void initModel(const IndexedMesh& model);
	void UpdateSphereColData(glm::vec3 pos, float radius);
	glm::vec3 getSpherePos() { return sphereCollider.GetPos(); }
	float getSphereRadius() { return sphereCollider.GetRadius(); }

private:



	enum
	{
		POSITION_VERTEXBUFFER,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	SphereCollider sphereCollider;
	GLuint vertexArrayObject;
	// create our array of buffers
	GLuint vertexArrayBuffers[NUM_BUFFERS]; 
	//how much of the vert Array Object do we want to draw
	unsigned int drawCount; 
};