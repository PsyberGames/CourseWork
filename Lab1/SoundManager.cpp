#include "SoundManager.h"


SoundManager::SoundManager()
{
	//open sound card
	audioDevice = alcOpenDevice(NULL); 
	if (audioDevice == NULL)
	{
		// if cant find sound card
		std::cout << "cannot open sound card" << std::endl;
	}
	// create a audio content to hold the audio data.
	audioContext = alcCreateContext(audioDevice, NULL);

	if (audioContext == NULL)
	{
		// if it can not open the audio context
		std::cout << "cannot open context" << std::endl;
	}
	//set the newly create audio content to current
	alcMakeContextCurrent(audioContext);
}

SoundManager::~SoundManager()
{
	//destroys all the audio data and context
	for (unsigned int i = 0; i < audioDatas.size(); i++)
	{
		alDeleteSources(1, (unsigned int*)&audioDatas[i].IDsource);
		if (audioDatas[i].IDbuffer != -1)
		{
			alDeleteBuffers(1, (unsigned int*)&audioDatas[i].IDbuffer);
			delete[] audioDatas[i].datBuf;
		}
	}
	alcDestroyContext(audioContext);
	alcCloseDevice(audioDevice);
}

bool SoundManager::isBigEndian()
{
	//dealing with bigEndian sequence
	int bE = 1;
	return !((char*)&bE)[0];
}

int SoundManager::convertToInt(char* buffer, int length)
{
	//input char array 
	int a = 0;
	if (!isBigEndian())
		for (int i = 0; i < length; i++)
			((char*)&a)[i] = buffer[i];
	else
		for (int i = 0; i < length; i++)
			((char*)&a)[3 - i] = buffer[i];
	return a;//output int array this is needing done because OpenAL uses int within it parameters
}

char* SoundManager::loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
	//take all the data stored within given file and converts it into ints so we can us the sound data with OPENAL
	char datBuf[4];
	std::ifstream in(fn, std::ios::binary);
	in.read(datBuf, 4);
	in.read(datBuf, 4);
	in.read(datBuf, 4); //WAV files
	in.read(datBuf, 4);	//fmt files
	in.read(datBuf, 4); //16 bit files
	in.read(datBuf, 2); //1 
	in.read(datBuf, 2);
	chan = convertToInt(datBuf, 2);
	in.read(datBuf, 4);
	samplerate = convertToInt(datBuf, 4);
	in.read(datBuf, 4);
	in.read(datBuf, 2);
	in.read(datBuf, 2);
	bps = convertToInt(datBuf, 2);
	in.read(datBuf, 4);
	in.read(datBuf, 4);
	size = convertToInt(datBuf, 4);

	char* soundData = new char[size];
	in.read(soundData, size);
	return soundData;
}

unsigned int SoundManager::soundLoad(const char* filename)
{
	//load given sound file from location of specific file
	bool found = false;
	unsigned int IDsource, IDbuffer;
	char* soundData = NULL;
	for (unsigned int i = 0; i < audioDatas.size(); i++)
	{
		if (audioDatas[i].Name == filename && audioDatas[i].IDbuffer != -1)
		{
			IDbuffer = audioDatas[i].IDbuffer;
			found = true;
			break;
		}
	}
	if(!found)
		{	
			int channel, sampleRate, bps, size;
			soundData = loadWAV(filename, channel, sampleRate, bps, size);
			unsigned int format;
			alGenBuffers(1, &IDbuffer);

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

			alBufferData(IDbuffer, format, soundData, size, sampleRate);
		}
	alGenSources(1, &IDsource);
	alSourcei(IDsource, AL_BUFFER, IDbuffer);
	alSourcef(IDsource, AL_REFERENCE_DISTANCE, 1.0f);
	audioDatas.push_back(data(IDsource, (!found ? IDbuffer : -1), soundData, filename));
	return IDsource;
}

void SoundManager::soundDelete(unsigned int ID)
{}

void SoundManager::soundPlay(unsigned int ID, glm::vec3& POS)
{
	//play sound using the ID and play at the 3d position that is going to be playing the sound
	alSource3f(ID, AL_POSITION, POS.x, POS.y, POS.z);
	alSourcePlay(ID);
}
void SoundManager::soundStop(unsigned int ID)
{
	//stops the sound using the ID and making a call to OPENAL
	alSourceStop(ID);
}
void SoundManager::listenerSet(glm::vec3& POS, glm::vec3& CAMDIRECTION)
{
	//set position of the listener and where the listener is looking
	alListener3f(AL_POSITION, POS.x, POS.y, -POS.z);
	float orinet[6] = { CAMDIRECTION.x,CAMDIRECTION.y,CAMDIRECTION.z,0,1,0 };
	alListenerfv(AL_ORIENTATION, orinet);
}