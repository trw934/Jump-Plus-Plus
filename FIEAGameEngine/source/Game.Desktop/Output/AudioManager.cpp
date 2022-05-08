#include "pch.h"
#include "AudioManager.h"
#include <SDL2/SDL.h>

AudioManager::AudioManager()
{
	SDL_Init(SDL_INIT_AUDIO);

	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init audio: %s", Mix_GetError());
		exit(-1);
	}
}

AudioManager::~AudioManager()
{
	SDL_Quit();
}


void AudioManager::AddAudio(const char* path)
{
	Mix_Chunk* tmpChunk = Mix_LoadWAV(path);

	if (tmpChunk != nullptr)
	{
		_AudioVector.PushBack(tmpChunk);
	}
	else
	{
		throw std::runtime_error("Couldn't initialize the audio");
	}
}

void AudioManager::Play(const size_t index)
{
	if (index > _AudioVector.Size() - 1)
	{
		throw std::runtime_error("Audio index is out of range");
	}

	Mix_PlayChannel(-1,_AudioVector[index], 0);
}