#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Window.h" 
#include "ImportShader.h"
#include "CreateMesh.h"
#include "Texture2D.h"
#include "Transform.h"
#include "SoundManager.h"

enum class GameCondition{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initGameSystems();
	void Input();
	void UpdateLoop();
	void Draw();
	bool collisDetect(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	void playSnd(unsigned int Source, glm::vec3 pos);

	Window gameWindow;
	GameCondition gameState;
	CreateMesh meshes[4];

	int rows = 16;
	int collums = 16;
	int gridSize = rows * collums;
	
	
	MainCamera myCamera;
	SoundManager sndDevice;
	

	float counter;
	unsigned int crash;
	unsigned int BGMusic;
};

