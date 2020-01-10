#include "MainCodeOfGame.h"
#include "GameCamera.h"
#include <iostream>
#include <string>
Transform transform;

MainCodeOfGame::MainCodeOfGame()
{
	// Finally, an audio device is created using information in the GameAudioSource class and will be used to deal with the audio later. 
	_stateOfGamePlay = GamePlayState::PLAY; //the state of the gameplay is changed from exit to playing
	GameDisplay* _gameDisplay = new GameDisplay(); ///a new game display created using GameDisplay() class.
	MeshCreationClass* carMesh(); //three new mesh variables are created calling information from the MeshCreationClass class.
	MeshCreationClass* houseMesh();
	MeshCreationClass* tireMesh();
	GameAudioSource* audioDevice(); //audio device created using information in GameAudioSource class and will be used to deal with the audio later.

}

MainCodeOfGame::~MainCodeOfGame()
{
}

void MainCodeOfGame::runMainCodeOfGame()
{
	
	initMainGameSystems(); //calls and runs the initMainGameSystems() method
	MainGamePlayLoop(); //the MainGamePlayLoop() method is also called and run. 
}

void MainCodeOfGame::initMainGameSystems()
{

	_displayOfGamePlay.initialiseGameDisplay(); //the initialiseGameDisplay() method is called from the GameDisplay classand runs it to initialise the display.
	soundEffect = audioPlayDevice.loadSoundToGame("..\\res\\bang.wav"); //load the bang sound effect to the game from its file
	backGroundMusic = audioPlayDevice.loadSoundToGame("..\\res\\background.wav"); //load the background music to the game from its file

	carMesh.loadModelToGame("..\\res\\car.obj"); //calls the loadModelToGame method to load in the car object
	houseMesh.loadModelToGame("..\\res\\house.obj"); //calls the loadModelToGame method to load in the house object
	tireMesh.loadModelToGame("..\\res\\tire.obj"); //calls the loadModelToGame method to load in the tire object
	//calls the initialiseGameCamera method to set up the camera with the specified position, field of view, camera aspect, near clipand far clip.
	myGamePlayCamera.initialiseGameCamera(glm::vec3(0, 0, -5), 70.0f, (float)_displayOfGamePlay.getGameDisplayWidth() / _displayOfGamePlay.getGameDisplayHeight(), 0.01f, 1000.0f);

}

void MainCodeOfGame::MainGamePlayLoop()
{
	while (_stateOfGamePlay != GamePlayState::EXIT) //while the gameplay is still running,
	{
		processMainGameInput(); //the processMainGameInput() method is called and run
		drawMainGameToScreen(); //the drawMainGameToScreen() method is called and run
		//The collisionDection() method is also called and run
		collisionDetection(carMesh.getSpherePosition(), carMesh.getSphereRadius(), houseMesh.getSpherePosition(), houseMesh.getSphereRadius(), tireMesh.getSpherePosition(), tireMesh.getSphereRadius());
		//the playAudioInGame(backGroundMusic, glm::vec3(1.0f, 5.0f, 6.0f)) method is run
		playAudioInGame(backGroundMusic, glm::vec3(1.0f,5.0f,6.0f));
	}
}


void MainCodeOfGame::processMainGameInput()
{
	//all key presses and inputs are handled. 
	
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) 
	{
		switch (evnt.type)
		{
		case SDL_QUIT: //If the red X at the top of the screen is pressed, 
			_stateOfGamePlay = GamePlayState::EXIT; //the state of gameplay will be changed to quit, and the game will stop. 
			break;
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym) {
			case SDLK_ESCAPE: //if the escape key is pressed,
				_stateOfGamePlay = GamePlayState::EXIT; //the gameplay will be stopped and the program will quit
			case SDLK_w: //Pressing the W key,
				myGamePlayCamera.MoveForward(0.5f); //moves the camera along the z-axis,
				std::cout << "Move forward" << std::endl; //displays a message to shows the user what direction the camera and car move
				carZ += 0.5f; //moves the car along the z-axis,
				break;
			case SDLK_a: //if the "A" key is pressed,
				myGamePlayCamera.MoveRight(0.5f);//moves the camera along the x-axis,
				std::cout << "Move left" << std::endl; //displays a message to shows the user what direction the camera and car move
				carX += 0.5f; //moves the car along the x-axis,
				break;
			case SDLK_s: //if the "S" key is pressed,
				myGamePlayCamera.back(0.5f); //moves the camera along the z-axis,
				std::cout << "Move back" << std::endl; //displays a message to shows the user what direction the camera and car move
				carZ -= 0.5f; //moves the car along the z-axis,
				break;
			case SDLK_d: //if the "D" key is pressed,
				myGamePlayCamera.left(0.5f); //moves the camera along the x-axis,
				std::cout << "Move Right" << std::endl; //displays a message to shows the user what direction the camera and car move
				carX -= 0.5f; //moves the car along the z-axis,
				break;
			case SDLK_UP:
				myGamePlayCamera.MoveUp(0.5f); //moves the camera along the y-axis,
				std::cout << "Move Up" << std::endl; //displays a message to shows the user what direction the camera and car move
				carY += 0.5f; //moves the car along the y-axis,
				break;
			case SDLK_DOWN:
				myGamePlayCamera.MoveDown(0.5f); //moves the camera along the y-axis,
				std::cout << "Move Down" << std::endl; //displays a message to shows the user what direction the camera and car move
				carY -= 0.5f; //moves the car along the y-axis,
				break;
			case SDLK_q: //when the q key is held down,
				audioPlayDevice.stopGameSound(backGroundMusic); //  the background music would stop. 

				break;
			default:
				break;
			}

		}


	}
}


bool MainCodeOfGame::collisionDetection(glm::vec3 mesh1Position, float mesh1Radius, glm::vec3 mesh2Position, float mesh2Radius, glm::vec3 mesh3Position, float mesh3Radius)
{
	float distance = glm::sqrt((mesh2Position.x - mesh1Position.x) * (mesh2Position.x - mesh1Position.x) + (mesh2Position.y - mesh1Position.y) * (mesh2Position.y - mesh1Position.y) + (mesh2Position.z - mesh1Position.z) * (mesh2Position.z - mesh1Position.z));
	float distance2 = glm::sqrt((mesh3Position.x - mesh1Position.x) * (mesh3Position.x - mesh1Position.x) + (mesh3Position.y - mesh1Position.y) * (mesh3Position.y - mesh1Position.y) + (mesh3Position.z - mesh1Position.z) * (mesh3Position.z - mesh1Position.z));
	float distance3 = glm::sqrt((mesh3Position.x - mesh2Position.x) * (mesh3Position.x - mesh2Position.x) + (mesh3Position.y - mesh2Position.y) * (mesh3Position.y - mesh2Position.y) + (mesh3Position.z - mesh2Position.z) * (mesh3Position.z - mesh2Position.z));
	if (distance < (mesh1Radius + mesh2Radius))
	{
		audioPlayDevice.setAudioListener(myGamePlayCamera.getPosition(), mesh1Position); //add bool to mesh
		playAudioInGame(soundEffect, mesh1Position);
		return true;
	}
	else if (distance2 < (mesh1Radius + mesh3Radius))
	{
		audioPlayDevice.setAudioListener(myGamePlayCamera.getPosition(), mesh1Position); //add bool to mesh
		playAudioInGame(soundEffect, mesh1Position);
		return true;
	}
	else if (distance3 < (mesh2Radius + mesh3Radius))
	{
		audioPlayDevice.setAudioListener(myGamePlayCamera.getPosition(), mesh2Position); //add bool to mesh
		playAudioInGame(soundEffect, mesh2Position);
		return true;
	}
	else 
	{
		return false;
	}
}

void MainCodeOfGame::playAudioInGame(unsigned int Source, glm::vec3 pos)
{

	ALint state;
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	
	if (AL_PLAYING != state)
	{
		audioPlayDevice.playGameSound(Source, pos);
	}
}

void MainCodeOfGame::drawMainGameToScreen()
{
	_displayOfGamePlay.clearGameDisplay(0.0f, 0.0f, 0.0f, 1.0); //clears the game display and sets the background to yellow

	ShaderCreationClass shader("..\\res\\shader"); //new shader
	TextureCreation texture("..\\res\\red.jfif"); //loads the red texture to the game from file

    
	transform.SetPos(glm::vec3(carX, carY, carZ));//sets the position of the car to be 0.0 on the x-axis, 0.0 on the y-axis and 0.0 on the z-axis, making it dead centre on the screen
	transform.SetRot(glm::vec3(0.0, 20.0, 0.0));//sets the rotation of the car to be 0.0 on the x - axis, 20.0 on the y-axis and 1.0 on the z-axis, facing the front of the car to the left of the screen
	transform.SetScale(glm::vec3(0.01, 0.01, 0.01));//sets the scale of the car to be 0.01 on the x - axis, 0.01 on the y - axis and 0.01 on the z - axis

	shader.BindGameShaders();
	shader.UpdateGameShaders(transform, myGamePlayCamera);
	texture.BindOurTexture(0);
	carMesh.drawMeshToGame();
	carMesh.updateSphereData(*transform.GetPos(), 0.62f);

	

	TextureCreation texture2("..\\res\\bricks.jpg"); //load texture
	
	

	transform.SetPos(glm::vec3(-2.5, 0.0, 0.0));
	transform.SetRot(glm::vec3(0.0, 5.0, 0.0));
	transform.SetScale(glm::vec3(0.015, 0.015, 0.015));

	shader.BindGameShaders();
	shader.UpdateGameShaders(transform, myGamePlayCamera);
	texture.BindOurTexture(1);
	houseMesh.drawMeshToGame();
	houseMesh.updateSphereData(*transform.GetPos(), 0.62f);
	counter = counter + 0.05f;

	ShaderCreationClass shader2("..\\res\\shader"); //new shader
	TextureCreation texture3("..\\res\\yellow.jpg"); //load texture
	

	transform.SetPos(glm::vec3(2.0, 0.25, 0.0)); //sets the position of the tire to be 2.0 on the x-axis, 1.0 on the y-axis and 1.0 on the z-axis
	transform.SetRot(glm::vec3(0.0, 5.0, 0.0)); //sets the rotation of the tire to be 0.0 on the x-axis, 5.0 on the y-axis and 0.0 on the z-axis
	transform.SetScale(glm::vec3(0.04, 0.04, 0.04)); //sets the scale of the tire to be 0.1 on the x-axis, 0.1 on the y-axis and 0.1 on the z-axis


	shader.BindGameShaders();
	shader.UpdateGameShaders(transform, myGamePlayCamera);
	texture.BindOurTexture(2);
	tireMesh.drawMeshToGame();
	tireMesh.updateSphereData(*transform.GetPos(), 0.62f);
	
	processMainGameInput();
	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_displayOfGamePlay.swapGameDisplayBuffer();
}
