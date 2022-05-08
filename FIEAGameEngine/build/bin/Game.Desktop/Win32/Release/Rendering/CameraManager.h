#pragma once

#include "..\Player\PlayerManager.h"
#include "SDL2/SDL.h"
#undef main
#include "SDL2/SDL_image.h"

using namespace FieaGameEngine;

class CameraManager final
{
public:
	CameraManager(std::int32_t xPos, std::int32_t yPos, PlayerManager* player);
	void SetXPos(std::int32_t xPos);
	void SetYPos(std::int32_t yPos);
	std::int32_t GetXPos();
	std::int32_t GetYPos();
	void Update();
private:
	PlayerManager* _player;
	std::int32_t _xPos = 0;
	std::int32_t _yPos = 0;
};

