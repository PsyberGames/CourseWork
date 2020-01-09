#include "Texture2D.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>

Texture2D::Texture2D(const std::string& fileName)
{
	//width, height, and no of components of image
	int width, height, numComponents; 
	//load the image and store the data
	unsigned char* imageData = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4); 

	if (imageData == NULL)
	{
		std::cerr << "texture load failed" << fileName << std::endl;
	}
	// number of and address of textures
	glGenTextures(1, &texture2DHandler); 
	//bind texture - define type 
	glBindTexture(GL_TEXTURE_2D, texture2DHandler); 
	// wrap texture outside width
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	// wrap texture outside height
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	// linear filtering for minification (texture is smaller than area)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	// linear filtering for magnifcation (texture is larger)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	//Target, Mipmapping Level, Pixel Format, Width, Height, Border Size, Input Format, Data Type of Texture, Image Data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData); 
	//clear the image
	stbi_image_free(imageData);
}

Texture2D::~Texture2D()
{
	// number of and address of textures
	glDeleteTextures(1, &texture2DHandler); 
}

void Texture2D::Bind(unsigned int unit)
{
	// check we are working with one of the 32 textures
	assert(unit >= 0 && unit <= 31); 
	//set acitve texture unit
	glActiveTexture(GL_TEXTURE0 + unit); 
	//type of and texture to bind to unit
	glBindTexture(GL_TEXTURE_2D, texture2DHandler); 
}
