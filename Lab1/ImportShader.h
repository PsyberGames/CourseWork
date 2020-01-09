#pragma once
#include <string>
#include <GL\glew.h>
#include "transform.h"
#include "MainCamera.h"

class ImportShader
{
public:
	ImportShader(const std::string& filename);
	//bind shader to gpu, allows use of shader with the gpu
	void Bind(); 
	void Update(const Transform& transform, const MainCamera& camera);

	std::string ImportShader::importShader(const std::string& fileName);
	void ImportShader::ShaderErrorCall(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint ImportShader::ShaderCreate(const std::string& text, unsigned int type);

    ~ImportShader();


protected:
private:
	// total number of shaders
	static const unsigned int SHADER_COUNT = 2; 

	enum
	{
		S_TRANSFORM,

		UNIFORM_COUNT
	};
	//shader program
	GLuint ShaderApp; 
	//array of shaders
	GLuint AllShaders[SHADER_COUNT]; 
	//array of the uniforms
	GLuint AllUniforms[UNIFORM_COUNT]; 
};
