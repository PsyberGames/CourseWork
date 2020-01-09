#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;


class Window
{
public:
	Window();
	~Window();
	void initWindow();
	void swapBuffer();
	void clearWindow(float r, float g, float b, float a);

	float getWidth();
	float getHeight();

private:

	void returnError(std::string errorString);
	//global variable to hold the context
	SDL_GLContext glContext; 
	//holds pointer to out window
	SDL_Window* sdlWindow; 
	float screenWidth;
	float screenHeight;
};

