#include "GameDisplay.h"


GameDisplay::GameDisplay()
{
	sdlGameDWindow = nullptr; //initialise to generate null access violation for debugging. 
	screenGameDisplayWidth = 1024.0f; //The width of the display window is set to 1024 pixels.
	screenGameDisplayHeight = 768.0f; //The height of the display window is set to 768 pixels. 
}

GameDisplay::~GameDisplay()
{
	SDL_GL_DeleteContext(glGameDisplayContext); // deletes the specified OpenGL context
	SDL_DestroyWindow(sdlGameDWindow); // removes the window 
	SDL_Quit(); //clear all initialised methods. 

}

float GameDisplay::getGameDisplayWidth() { return screenGameDisplayWidth; } //retrieve width information and returns it. 
float GameDisplay::getGameDisplayHeight() { return screenGameDisplayHeight; } //retrieve height information and returns it. 

void GameDisplay::returnGameDisplayError(std::string errorString)
{
	//error message window created and message displayed on screen.
	//user then prompted to press any key to make the error message disappear and the method is then stopped from running.
	std::cout << errorString << std::endl;
	std::cout << "press any  key to quit...";
	int in;
	std::cin >> in;
	SDL_Quit();
}

void GameDisplay::swapGameDisplayBuffer()
{
	SDL_GL_SwapWindow(sdlGameDWindow); //swap the OpenGL buffers for the specified game display window. 
}

void GameDisplay::clearGameDisplay(float red, float green, float blue, float alpha)
{
	glClearColor(red, green, blue, alpha); //declares red, green, blue and alpha values used to clear game’s colour buffers and the values will be used by glClear. 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //used to reset specified buffers to valuesand GL_COLOR_BUFFER_BIT used to identify which buffers can be editedand GL_DEPTH_BUFFER_BIT indicates the depth buffer.
}

void GameDisplay::initialiseGameDisplay()
{

	SDL_Init(SDL_INIT_EVERYTHING); //uses the SDL_Init function to initialise everything.

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //used to set attributes before game window is created. minimum number of bits required to display red is 8
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8); //minimum number of bits required to display green is 8
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8); //minimum number of bits required to display blue is 8
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);// the depth size of the z - buffer is set to 16
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // and the double buffer is set up  

	//SDL_CREATEWINDOW function used to create  window using specified information. the title will be “Game Display Window”, window positioned at the centre of the screen and will the dimension that were specified earlier
	sdlGameDWindow = SDL_CreateWindow("Game Display Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenGameDisplayWidth, (int)screenGameDisplayHeight, SDL_WINDOW_OPENGL); // create window

	if (sdlGameDWindow == nullptr) //If no pointers are seen to point to a window,
	{
		returnGameDisplayError("failed to create window"); // an error message will be displayed.
	}

	glGameDisplayContext = SDL_GL_CreateContext(sdlGameDWindow); //Context is then created for window to be used

	if (glGameDisplayContext == nullptr) //if this fails,
	{
		returnGameDisplayError("failed to create SDL_GL context"); //an error message appears.
	}

	GLenum error = glewInit(); //GLEW is then initialised
	if (error != GLEW_OK)
	{
		returnGameDisplayError("failed to initialise GLEW");
	}

	glEnable(GL_DEPTH_TEST); //Z - buffering enabled
	glEnable(GL_CULL_FACE); //back face culling is enabled


	glClearColor(0.0f, 1.0f, 1.0f, 1.0f); //sets the red value to 0.0 and the values of green, blueand alpha to 1.0.
}