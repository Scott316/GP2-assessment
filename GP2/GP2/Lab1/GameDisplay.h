#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;
class GameDisplay
{
public:
	GameDisplay();
	~GameDisplay();
	void initialiseGameDisplay();
	void swapGameDisplayBuffer();
	void clearGameDisplay(float red, float green, float blue, float alpha);

	float getGameDisplayWidth();
	float getGameDisplayHeight();

private:

	void returnGameDisplayError(std::string errorString);

	SDL_GLContext glGameDisplayContext; //global variable to hold the context
	SDL_Window* sdlGameDWindow; //holds pointer to out window
	float screenGameDisplayWidth;
	float screenGameDisplayHeight;
};
