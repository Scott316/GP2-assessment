#include "GameAudioSource.h"

GameAudioSource::GameAudioSource()
{
	device = alcOpenDevice(NULL); //an OpenAL device is opened
	if (device == NULL) //If the device variable is made null, 
	{
		std::cout << "cannot open sound card" << std::endl; //an error message will appear to tell the user that the sound card cannot be opened.
	}

	context = alcCreateContext(device, NULL); //a context is created, and all the necessary data is stored within the context.

	if (context == NULL) //if the context comes back null, 
	{
		std::cout << "cannot open context" << std::endl; //an error message will appear to inform the user. 
	}

	alcMakeContextCurrent(context); //the specified context is made the current context within the game. 
}

GameAudioSource::~GameAudioSource()
{
	for (unsigned int i = 0; i < datas.size(); i++)
	{
		alDeleteSources(1, (unsigned int*)& datas[i].IDOfSource); //The alDeleteSources deletes all audio sources
		if (datas[i].IDOfBuffer != -1)
		{
			alDeleteBuffers(1, (unsigned int*)& datas[i].IDOfBuffer); //deletes all the buffers 
			delete[] datas[i].audioBuffer; //and then all associated data is also deleted
		}
	}
	alcDestroyContext(context); //the context is deleted
	alcCloseDevice(device); //this takes the name and closes the specified device
}

bool GameAudioSource::isBigEndianUsed()
{
	//checks if the devices uses Big Endian or Little Endian
	int a = 1;
	return !((char*)& a)[0];
}

int GameAudioSource::convertToIntegerType(char* buffer, int length)
{//The convertToInt() method carries out the conversion from a char array to an int array.    
	int a = 0;
	if (!isBigEndianUsed())
		for (int i = 0; i < length; i++)
			((char*)& a)[i] = buffer[i];
	else
		for (int i = 0; i < length; i++)
			((char*)& a)[3 - i] = buffer[i];
	return a;
}

char* GameAudioSource::loadWAVFileToGame(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
	//interprets the WAV file so it is possible to play it back using openAL. 
	char buffer[4];
	std::ifstream in(fn, std::ios::binary);
	in.read(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 4); //WAV
	in.read(buffer, 4);	//fmt
	in.read(buffer, 4); //16 bit
	in.read(buffer, 2); //1
	in.read(buffer, 2);
	chan = convertToIntegerType(buffer, 2);
	in.read(buffer, 4);
	samplerate = convertToIntegerType(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	bps = convertToIntegerType(buffer, 2);
	in.read(buffer, 4);
	in.read(buffer, 4);
	size = convertToIntegerType(buffer, 4);

	char* soundData = new char[size];
	in.read(soundData, size);
	return soundData;
}

unsigned int GameAudioSource::loadSoundToGame(const char* filename)
{//follows a pathway to find a file, loads it into the game. The file is loaded in and takes note of data, such as sample rate and 
//the type of audio, then saving it in a buffer. Each piece of data given an integer value to identify them.
//these integers reference each audio track, and therefore an unsigned int is used as a return type.
	bool found = false;
	unsigned int sourceID, bufferID;
	char* soundData = NULL;
	for (unsigned int i = 0; i < datas.size(); i++)
	{
		if (datas[i].audioName == filename && datas[i].IDOfBuffer != -1)
		{
			bufferID = datas[i].IDOfBuffer;
			found = true;
			break;
		}
	}
	if (!found)
	{
		int channel, sampleRate, bps, size;
		soundData = loadWAVFileToGame(filename, channel, sampleRate, bps, size);
		unsigned int format;
		alGenBuffers(1, &bufferID);

		if (channel == 1)
		{
			if (bps == 8)
			{
				format = AL_FORMAT_MONO8;
			}
			else
			{
				format = AL_FORMAT_MONO16;
			}
		}
		else
		{
			if (bps == 8)
			{
				format = AL_FORMAT_STEREO8;
			}
			else
			{
				format = AL_FORMAT_STEREO16;
			}

		}

		alBufferData(bufferID, format, soundData, size, sampleRate);
	}
	alGenSources(1, &sourceID);
	alSourcei(sourceID, AL_BUFFER, bufferID);
	alSourcef(sourceID, AL_REFERENCE_DISTANCE, 1.0f);
	datas.push_back(data(sourceID, (!found ? bufferID : -1), soundData, filename));
	return sourceID;
}

void GameAudioSource::deleteSoundFromGame(unsigned int id)
{}

void GameAudioSource::playGameSound(unsigned int id)
{//uses the integer handler to decide which audio track and the position to decide where to play the sound from.
	alSourcePlay(id);
}
void GameAudioSource::playGameSound(unsigned int id, glm::vec3& pos)
{
//uses the integer handler to decide which audio track and the position to decide where to play the sound from.
	alSource3f(id, AL_POSITION, pos.x, pos.y, pos.z);
	alSourcePlay(id);
}
void GameAudioSource::stopGameSound(unsigned int id)
{
	// uses the id of the audio tracks to decide which track to stop at what time. 
	alSourceStop(id);
}
void GameAudioSource::setAudioListener(glm::vec3& position, glm::vec3& camLookAt)
{
//determines the position of the audio listenerand sets what part of the game the listener is looking at.
	alListener3f(AL_POSITION, position.x, position.y, -position.z);
	float orinet[6] = { camLookAt.x,camLookAt.y,camLookAt.z,0,1,0 };
	alListenerfv(AL_ORIENTATION, orinet);
}