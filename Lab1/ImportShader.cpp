#include "ImportShader.h"
#include <iostream>
#include <fstream>

ImportShader::ImportShader(const std::string& filename)
{
	// create shader program (openGL saves as ref number)
	ShaderApp = glCreateProgram();
	// create vertex shader
	AllShaders[0] = ShaderCreate(importShader("..\\res\\shader.vert"), GL_VERTEX_SHADER);
	// create fragment shader
	AllShaders[1] = ShaderCreate(importShader("..\\res\\shader.frag"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < SHADER_COUNT; i++)
	{
		//add all our shaders to the shader program "shaders" 
		glAttachShader(ShaderApp, AllShaders[i]);
	}
	// associate attribute variable with our shader program attribute (in this case attribute vec3 position;)
	glBindAttribLocation(ShaderApp, 0, "position");
	glBindAttribLocation(ShaderApp, 1, "texCoord");
	glBindAttribLocation(ShaderApp, 2, "normals");
	//create executables that will run on the GPU shaders
	glLinkProgram(ShaderApp);
	// check for error
	ShaderErrorCall(ShaderApp, GL_LINK_STATUS, true, "Error: Shader program linking failed");

	glValidateProgram(ShaderApp); //check the entire program is valid
	ShaderErrorCall(ShaderApp, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");
	// associate with the location of uniform variable within a program
	AllUniforms[S_TRANSFORM] = glGetUniformLocation(ShaderApp, "transform");
}


ImportShader::~ImportShader()
{
	for (unsigned int i = 0; i < SHADER_COUNT; i++)
	{
		//detach shader from program
		glDetachShader(ShaderApp, AllShaders[i]);
		//delete the shaders
		glDeleteShader(AllShaders[i]);
	}
	// delete the program
	glDeleteProgram(ShaderApp);
}

void ImportShader::Bind()
{
	//installs the program object specified by program as part of rendering state
	glUseProgram(ShaderApp);
}

void ImportShader::Update(const Transform& transform, const MainCamera& camera)
{
	glm::mat4 mvp = camera.GetViewProjection() * transform.GetMesh();
	glUniformMatrix4fv(AllUniforms[S_TRANSFORM], 1, GLU_FALSE, &mvp[0][0]);
}


GLuint ImportShader::ShaderCreate(const std::string& text, unsigned int type)
{
	//create shader based on specified type
	GLuint shader = glCreateShader(type); 

	//if == 0 shader no created
	if (shader == 0) 
		std::cerr << "Error type creation failed " << type << std::endl;
	//convert strings into list of c-strings
	const GLchar* stringSource[1]; 
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();
	//send source code to opengl
	glShaderSource(shader, 1, stringSource, lengths); 
	//get open gl to compile shader code
	glCompileShader(shader); 
	//check for compile error
	ShaderErrorCall(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	return shader;
}

std::string ImportShader::importShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void ImportShader::ShaderErrorCall(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

