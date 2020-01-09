#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "MainCamera.h"

struct Transform
{
public:
	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f))
	{
		this->pos = pos;
		this->rot = rot;
		this->scale = scale;
	}

	inline glm::mat4 GetMesh() const
	{
		glm::mat4 posMaterial = glm::translate(pos);
		glm::mat4 scaleMaterial = glm::scale(scale);
		glm::mat4 rotationX = glm::rotate(rot.x, glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotationY = glm::rotate(rot.y, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotationZ = glm::rotate(rot.z, glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 rotationMaterial = rotationX * rotationY * rotationZ;

		return posMaterial * rotationMaterial * scaleMaterial;
	}

	//getters

	inline glm::vec3* GetPos() { return &pos; } 
	inline glm::vec3* GetRot() { return &rot; }
	inline glm::vec3* GetScale() { return &scale; }
	// setters
	inline void SetPos(glm::vec3& pos) { this->pos = pos; } 
	inline void SetRot(glm::vec3& rot) { this->rot = rot; }
	inline void SetScale(glm::vec3& scale) { this->scale = scale; }
protected:
private:
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
};


