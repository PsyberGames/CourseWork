#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct MainCamera
{
public:
	MainCamera()
	{
	}

	void initCamera(const glm::vec3& pos, float fov, float aspect, float nearClip, float farClip)
	{
		//sets up camera pos , direction and projection
		this->pos = pos;
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, nearClip, farClip);
	}
	void setPos(float move)
	{
		// allows us to change the camera position
		this->pos = glm::vec3(pos.x + move, pos.y, pos.z);
	}
	glm::vec3 getPos()
	{
		// gets the camera position
		return this->pos;
	}

	inline glm::mat4 GetViewProjection() const
	{
		//get camera view projection not MVP
		return projection * glm::lookAt(pos, pos + forward, up);
	}

	void RotateY(float angle)
	{
		// allows us to rotate the camera on the y axis
		static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

		glm::mat4 rotation = glm::rotate(angle, UP);

		forward = glm::vec3(glm::normalize(rotation * glm::vec4(forward, 0.0)));
		up = glm::vec3(glm::normalize(rotation * glm::vec4(up, 0.0)));
	}
	void MoveForward(float amt)
	{
		//move camera forward
		pos += forward * amt;
	}

	void MoveRight(float amt)
	{
		//move the camera right
		pos += glm::cross(up, forward) * amt;
	}

	void Pitch(float angle)
	{
		//allows us to rotate the camera upwards or downwards
		glm::vec3 right = glm::normalize(glm::cross(up, forward));

		forward = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(forward, 0.0)));
		up = glm::normalize(glm::cross(forward, right));
	}

	

protected:
private:
	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
};


