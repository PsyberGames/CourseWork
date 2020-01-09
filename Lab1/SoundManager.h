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

class SoundManager
{
	//structfor the sound data
	struct data {
		int IDsource, IDbuffer;
		char* datBuf;
		std::string Name;
		data(unsigned int Is, unsigned int Ib, char* buff, const char* name)
		{
			this->IDsource = Is;
			this->IDbuffer = Ib;
			this->datBuf = buff;
			this->Name = name;
		}

	};
	//use vector 3 struct here to handle multople inputs from the sound data
	struct Vector3
	{
		float x, y, z;
	};
	std::vector<data> audioDatas;
	ALCcontext* audioContext;
	ALCdevice *audioDevice;
	bool isBigEndian();
	int convertToInt(char* buffer, int length);
	char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size);

public:
	SoundManager();
	~SoundManager();
	unsigned int soundLoad(const char* filename);
	void soundDelete(unsigned int id);
	void soundPlay(unsigned int id, glm::vec3& pos);
	void soundStop(unsigned int id);
	void listenerSet(glm::vec3& pos, glm::vec3& camLookAt);

private:
	
};

