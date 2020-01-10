#pragma once

#include <glm\glm.hpp>
#include <AL\al.h>
#include <AL\alc.h>
#include <vector>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <SDL\SDL.h>
#include <glm/gtc/type_ptr.hpp>

class GameAudioSource
{
	struct data {
		int IDOfSource; //creates an ineger variable to store the ID of an audio source
		int IDOfBuffer; //creates an ineger variable to store the ID of a buffer
		char* audioBuffer;
		std::string audioName;
		data(unsigned int sourceID, unsigned int bufferID, char* buffer, const char* name)
		{
			this->IDOfSource = sourceID;
			this->IDOfBuffer = bufferID;
			this->audioBuffer = buffer;
			this->audioName = name;
		}

	};

	struct Vector3
	{
		float x, y, z;
	};
	std::vector<data> datas;
	ALCcontext* context;
	ALCdevice* device;
	bool isBigEndianUsed();
	int convertToIntegerType(char* buffer, int length);
	char* loadWAVFileToGame(const char* fn, int& chan, int& samplerate, int& bps, int& size);

public:
	GameAudioSource();
	~GameAudioSource();
	unsigned int loadSoundToGame(const char* filename);
	void deleteSoundFromGame(unsigned int soundID);
	void playGameSound(unsigned int soundID);
	void playGameSound(unsigned int soundID, glm::vec3& position);
	void stopGameSound(unsigned int soundID);
	void setAudioListener(glm::vec3& position, glm::vec3& camLookAt);

private:

};
