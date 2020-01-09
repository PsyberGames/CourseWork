#pragma once
#include <GL/glew.h>
#include <glm\glm.hpp>

class SkyBox
{
public:
	SkyBox();
	~SkyBox();

private:
	GLuint vbo;
	GLuint vao;
};

