#include "pch.h"
#include "CameraManager.h"

CameraManager::CameraManager(std::int32_t xPos, std::int32_t yPos, PlayerManager* player) :
	_xPos(xPos), _yPos(yPos), _player(player)
{
}

void CameraManager::SetXPos(std::int32_t xPos)
{
	_xPos = xPos;
}

void CameraManager::SetYPos(std::int32_t yPos)
{
	_yPos = yPos;
}

std::int32_t CameraManager::GetXPos()
{
	return _xPos;
}

std::int32_t CameraManager::GetYPos()
{
	return _yPos;
}

void CameraManager::Update()
{
	std::int32_t playerCurrentY = static_cast<std::int32_t>(_player->GetPosition().y);
	_yPos = playerCurrentY;
}