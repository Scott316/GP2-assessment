#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "GameDisplay.h" 
#include "ShaderCreationClass.h"
#include "MeshCreationClass.h"
#include "TextureCreationClass.h"
#include "transform.h"
#include "GameAudioSource.h"

enum class GamePlayState { PLAY, EXIT };
class MainCodeOfGame
{
public:
	MainCodeOfGame();
	~MainCodeOfGame();
	float carX = 0.0f;
	float carY = 0.0f;
	float carZ = 0.0f;
	bool moveRight = true;
	bool playermoveLeft = false;
	void runMainCodeOfGame();

private:

	void initMainGameSystems();
	void processMainGameInput();
	void MainGamePlayLoop();
	void drawMainGameToScreen();
	bool collisionDetection(glm::vec3 mesh1Position, float mesh1Radius, glm::vec3 mesh2Position, float mesh2Radius, glm::vec3 mesh3Position, float mesh3Radius);
	void playAudioInGame(unsigned int audioSource, glm::vec3 position);

	GameDisplay _displayOfGamePlay;
	GamePlayState _stateOfGamePlay;
	MeshCreationClass carMesh;
	MeshCreationClass houseMesh;
	MeshCreationClass tireMesh;
	GameCamera myGamePlayCamera;
	GameAudioSource audioPlayDevice;

	float counter;
	unsigned int soundEffect;
	unsigned int backGroundMusic;

};