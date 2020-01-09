#include "Window.h"


Window::Window()
{
	//initialise for debugging. 
	sdlWindow = nullptr; 
	screenWidth = 1024.0f;
	screenHeight = 768.0f; 
}

Window::~Window()
{
	//destroy context
	SDL_GL_DeleteContext(glContext); 
	//delete the window but context needs deleted first because how it is allocated
	SDL_DestroyWindow(sdlWindow); 
	SDL_Quit();
}

//getters
float Window::getWidth() { return screenWidth; } 
float Window::getHeight() { return screenHeight; }

void Window::returnError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "Press any Key to Exit";
	int in;
	std::cin >> in;
	SDL_Quit();
}

void Window::swapBuffer()
{
	//swap buffers
	SDL_GL_SwapWindow(sdlWindow); 
}

void Window::clearWindow(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	// clear colour and depth buffer - set colour to colour defined in glClearColor
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void Window::initWindow()
{
	//initalise everything
	SDL_Init(SDL_INIT_EVERYTHING); 
	//minimal amount needed to display colour 
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); // R
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);//G
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8); //B
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);// z-buffer Set
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // double Buffer Set  

	// Create the game window
	sdlWindow = SDL_CreateWindow("FPS PROTO", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenWidth, (int)screenHeight, SDL_WINDOW_OPENGL); 

	//if failed to create a window send this error
	if (sdlWindow == nullptr)
	{
		returnError("Failed create Window");
	}
	//create glContext because we created a window
	glContext = SDL_GL_CreateContext(sdlWindow);

	//if glcontext is null then failed to create context send error.
	if (glContext == nullptr)
	{
		returnError("Failed to create SDL_GL context");
	}

	//Intialize Glew
	GLenum error = glewInit();
	//if glewinit is not succesful send error.
	if (error != GLEW_OK)
	{
		returnError("Failed to intilize GLEW");
	}
	//enable z-buffering 
	glEnable(GL_DEPTH_TEST); 
	//dont draw faces that are not pointing at the camera
	glEnable(GL_CULL_FACE); 

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}