#pragma once
#include <string>
#include <GL\glew.h>

class Texture2D
{
public:
	Texture2D(const std::string& fileName);

	void Bind(unsigned int unit); // bind upto 32 textures

	~Texture2D();

protected:
private:

	GLuint texture2DHandler;
};

