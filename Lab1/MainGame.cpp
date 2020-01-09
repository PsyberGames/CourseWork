#include "MainGame.h"
#include "MainCamera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Window* _gameDisplay = new Window(); //new display
	CreateMesh* meshes();
	
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initWindow();
	whistle = soundDevice.soundLoad("..\\res\\bang.wav");
	backGroundMusic = soundDevice.soundLoad("..\\res\\background.wav");
	
	meshes[0].LoadMesh("..\\res\\monkey3.obj");
	meshes[1].LoadMesh("..\\res\\cube.obj");
	
	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
		collision(meshes[0].getSpherePos(), meshes[0].getSphereRadius(), meshes[1].getSpherePos(), meshes[1].getSphereRadius());
		playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	//get and process events
	while(SDL_PollEvent(&evnt)) 
	{
		//Keyboard input
		if (evnt.type == SDL_KEYDOWN)
		{
			if (evnt.key.keysym.sym == SDLK_ESCAPE)
			{
				_gameState = GameState::EXIT;
			}
			switch (evnt.key.keysym.sym)
			{
			case SDLK_w:
				myCamera.MoveForward(0.5f);
				break;
			case SDLK_s:
				myCamera.MoveForward(-0.5f);
				break;
			case SDLK_a:
				myCamera.MoveRight(0.25f);
				break;
			case SDLK_d:
				myCamera.MoveRight(-0.25f);
				break;

			}
		}
		// Mouse input
		if (evnt.button.x < (1024/2))
		{		
		myCamera.RotateY(0.033);			
		}
		if (evnt.button.x > (1024/2))
		{			
		myCamera.RotateY(-0.033);			
		}
		if (evnt.button.y != 0)
		{
			if (evnt.button.y > (768 / 2))
			{
			myCamera.Pitch(0.013);
			}
			if (evnt.button.y < (768 / 2))
			{
			myCamera.Pitch(-0.013);
			}
		}
		
	}
	
}


bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		soundDevice.listenerSet(myCamera.getPos(), m1Pos); //add bool to mesh
		playAudio(whistle, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}

void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
{
	
	ALint state; 
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	/*
	Possible values of state
	AL_INITIAL
	AL_STOPPED
	AL_PLAYING
	AL_PAUSED
	*/
	if (AL_PLAYING != state)
	{
		soundDevice.soundPlay(Source, pos);
	}
}

void MainGame::drawGame()
{
	_gameDisplay.clearWindow(0.0f, 0.0f, 0.0f, 1.0f);
	
	ImportShader shader("..\\res\\shader"); //new shader
	Texture2D texture("..\\res\\grass.jpg"); //load texture
	Texture2D texture1("..\\res\\water.jpg"); //load texture
	for (int i = 0; i < rows; i++)
	{
		for (int t = 0; t < collums; t++)
		{
			transform.SetPos(glm::vec3(0 + (i * 2), -2, 0 + (t*2)));
			transform.SetRot(glm::vec3(0.0, 0.0, 0));
			transform.SetScale(glm::vec3(1, 1, 1));
			
			shader.Bind();
			shader.Update(transform, myCamera);
			texture.Bind(0);
			meshes[1].Draw();
			
		}
		
	}
	
	

	transform.SetPos(glm::vec3(sinf(counter), 0.5, 0.0));
	transform.SetRot(glm::vec3(0.0, 0.0, counter * 5));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	shader.Bind();
	shader.Update(transform, myCamera);
	texture.Bind(0);
	meshes[0].Draw();
	meshes[0].UpdateSphereColData(*transform.GetPos(), 0.62f);
	

	transform.SetPos(glm::vec3(-sinf(counter), -0.5, -sinf(counter)*5));
	transform.SetRot(glm::vec3(0.0, 0.0, counter * 5));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	shader.Bind();
	shader.Update(transform, myCamera);
	texture.Bind(0);
	meshes[0].Draw();
	meshes[0].UpdateSphereColData(*transform.GetPos(), 0.62f);
	counter = counter + 0.05f;

				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 