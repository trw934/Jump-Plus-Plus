#pragma once
#include <SDL2/SDL_mixer.h>
#include "Vector.h"

class AudioManager final
{
public:
	AudioManager();
	~AudioManager();

	/// <summary>
	/// Add audio to the audio manager's vector
	/// </summary>
	/// <param name="path">put the path of the audio(format should be .wav)</param>
	static void AddAudio(const char* path);

	/// <summary>
	/// Play the audio via index
	/// </summary>
	/// <param name="index">the index of audio intend to play, default be 0</param>
	static void Play(const size_t index=0);

private:
	//the vector contain all the audio
	inline static FieaGameEngine::Vector<Mix_Chunk*> _AudioVector;
};