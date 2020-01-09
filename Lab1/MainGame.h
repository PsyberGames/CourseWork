#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Window.h" 
#include "ImportShader.h"
#include "CreateMesh.h"
#include "Texture2D.h"
#include "Transform.h"
#include "SoundManager.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	void playAudio(unsigned int Source, glm::vec3 pos);

	Window _gameDisplay;
	GameState _gameState;
	CreateMesh meshes[2];

	int rows = 16;
	int collums = 16;
	int gridSize = rows * collums;
	
	
	MainCamera myCamera;
	SoundManager soundDevice;
	

	float counter;
	unsigned int whistle;
	unsigned int backGroundMusic;
};

