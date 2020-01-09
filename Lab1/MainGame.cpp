#include "MainGame.h"
#include "MainCamera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	gameState = GameCondition::PLAY;
	//new window
	Window* gameWindow = new Window(); 
	CreateMesh* meshes();
	
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initGameSystems(); 
	UpdateLoop();
}

void MainGame::initGameSystems()
{
	gameWindow.initWindow();
	crash = sndDevice.soundLoad("..\\res\\crash.wav");
	BGMusic = sndDevice.soundLoad("..\\res\\bg.wav");
	
	meshes[0].LoadMesh("..\\res\\boat.obj");
	meshes[1].LoadMesh("..\\res\\cube.obj");
	meshes[2].LoadMesh("..\\res\\tree.obj");
	meshes[3].LoadMesh("..\\res\\skybox.obj");
	
	myCamera.initCamera(glm::vec3(0, 8, -15), 70.0f, (float)gameWindow.getWidth()/ gameWindow.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;
}

void MainGame::UpdateLoop()
{
	while (gameState != GameCondition::EXIT)
	{
		Input();
		Draw();
		collisDetect(meshes[0].getSpherePos(), meshes[0].getSphereRadius(), meshes[1].getSpherePos(), meshes[1].getSphereRadius());
		playSnd(BGMusic, glm::vec3(0.0f,0.0f,0.0f));
	}
}

void MainGame::Input()
{
	SDL_Event evntHandler;

	//get and process events
	while(SDL_PollEvent(&evntHandler))
	{
		//Keyboard input
		if (evntHandler.type == SDL_KEYDOWN)
		{
			if (evntHandler.key.keysym.sym == SDLK_ESCAPE)
			{
				gameState = GameCondition::EXIT;
			}
			switch (evntHandler.key.keysym.sym)
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
		if (evntHandler.button.x < (1024/2))
		{		
		myCamera.RotateY(0.033);			
		}
		if (evntHandler.button.x > (1024/2))
		{			
		myCamera.RotateY(-0.033);			
		}
		if (evntHandler.button.y != 0)
		{
			if (evntHandler.button.y > (768 / 2))
			{
			myCamera.Pitch(0.033);
			}
			if (evntHandler.button.y < (768 / 2))
			{
			myCamera.Pitch(-0.033);
			}
		}
		
	}
	
}


bool MainGame::collisDetect(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		//add bool to mesh
		sndDevice.listenerSet(myCamera.getPos(), m1Pos);
		playSnd(crash, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}

void MainGame::playSnd(unsigned int Source, glm::vec3 pos)
{
	//state all within openAL
	//AL_INITIAL
	//AL_STOPPED
	//AL_PLAYING
	//AL_PAUSED
	ALint state; 
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	
	if (AL_PLAYING != state)
	{
		sndDevice.soundPlay(Source, pos);
	}
}

void MainGame::Draw()
{
	gameWindow.clearWindow(0.0f, 0.0f, 0.0f, 1.0f);
	//new shader
	ImportShader shader("..\\res\\shader"); 
	//load texture
	Texture2D texture("..\\res\\grass.jpg"); 
	//load texture
	Texture2D texture1("..\\res\\water.jpg"); 
	////load texture
	Texture2D texture2("..\\res\\tree.jpg");
	////load texture
	Texture2D texture3("..\\res\\boat.jpg");

	//Level Generation below
	//water
	for (int i = 0; i < rows; i++)
	{
		for (int t = 0; t < collums; t++)
		{
			transform.SetPos(glm::vec3(-5 + (i * 2), -2, -6 + (t*2)));
			transform.SetRot(glm::vec3(0.0, 0.0, 0));
			transform.SetScale(glm::vec3(1, 1, 1));
			// bind the shader to that mesh 
			shader.Bind();
			//update the shader for that mesh
			shader.Update(transform, myCamera);
			//bind the texture to the mesh 
			texture1.Bind(0);
			//draw the mesh to window
			meshes[1].Draw();

			
			
		}
		
	}
	//land
	for (int i = 0; i < rows; i++)
	{
		for (int t = 0; t < collums; t++)
		{
			
			if (i <= 2 || i >= 13 || t <= 2 || t >= 11)
			{

			}
			else {
				transform.SetPos(glm::vec3(-2 + (i * 2), -0.5, -2 + (t * 2)));
				transform.SetRot(glm::vec3(0.0, 0.0, 0));
				transform.SetScale(glm::vec3(1, 0.5, 1));
				// bind the shader to that mesh 
				shader.Bind();
				//update the shader for that mesh
				shader.Update(transform, myCamera);
				//bind the texture to the mesh 
				texture.Bind(0);
				//draw the mesh to window
				meshes[1].Draw();
			}


		}
		
	}
	//trees
	for (int i = 0; i < rows; i++)
	{
		for (int t = 0; t < collums; t++)
		{

			if (i <= 3 || i >= 10 || t <= 3 || t >= 10)
			{

			}
			else {
				transform.SetPos(glm::vec3(0 + (i * 2), -0.2, 0 + (t * 2)));
				transform.SetRot(glm::vec3(0.0, 0.0, 0));
				transform.SetScale(glm::vec3(1, 1, 1));
				// bind the shader to that mesh 
				shader.Bind();
				//update the shader for that mesh
				shader.Update(transform, myCamera);
				//bind the texture to the mesh 
				texture2.Bind(0);
				//draw the mesh to window
				meshes[2].Draw();
			}


		}

	}
	
	//ship 1 setting transform and rotation and scale etc.
	transform.SetPos(glm::vec3(sinf(counter), -0.5, 0.0));
	transform.SetRot(glm::vec3(0.0, -80, 0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	// bind the shader to that mesh 
	shader.Bind();
	//update the shader for that mesh
	shader.Update(transform, myCamera);
	//bind the texture to the mesh 
	texture3.Bind(0);
	//draw the mesh to window
	meshes[0].Draw();
	//update the mesh sphere collider data so we can detect collisions.
	meshes[0].UpdateSphereColData(*transform.GetPos(), 0.62f);
	
	//ship 2 setting transform and rotation and scale etc.
	transform.SetPos(glm::vec3(-sinf(counter), -0.5, -sinf(counter)*5));
	transform.SetRot(glm::vec3(0.0, 0.0, 0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));
	// bind the shader to that mesh 
	shader.Bind();
	//update the shader for that mesh
	shader.Update(transform, myCamera);
	//bind the texture to the mesh 
	texture3.Bind(0);
	//draw the mesh to window
	meshes[0].Draw();
	//update the mesh sphere collider data so we can detect collisions.
	meshes[0].UpdateSphereColData(*transform.GetPos(), 0.62f);
	counter = counter + 0.05f;


	//cheap version of a skybox ............... really simple and basic but this can be improved on quite easily ***!!!!!!
	transform.SetPos(glm::vec3(-5 , -2, -6));
	transform.SetRot(glm::vec3(0.0, 0.0, 0));
	transform.SetScale(glm::vec3(-100, -100, -100));
	// bind the shader to that mesh 
	shader.Bind();
	//update the shader for that mesh
	shader.Update(transform, myCamera);
	//bind the texture to the mesh 
	texture1.Bind(0);

	meshes[1].Draw();

				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	gameWindow.swapBuffer();
} 