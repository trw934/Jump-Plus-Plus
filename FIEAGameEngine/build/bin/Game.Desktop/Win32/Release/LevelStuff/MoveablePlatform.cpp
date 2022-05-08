#include "pch.h"
#include "MoveablePlatform.h"
#include <iostream>
#include <cmath>

using namespace FieaGameEngine;
RTTI_DEFINITIONS(MoveablePlatform);

MoveablePlatform::MoveablePlatform() :
	Platform(MoveablePlatform::TypeIdClass())
{
}

gsl::owner<MoveablePlatform*> MoveablePlatform::Clone() const
{
	return new MoveablePlatform(*this);
}

void MoveablePlatform::Update(const GameTime&)
{
	if (!_cachedStartPos)
	{
		_startPosition = GetPosition();
		_cachedStartPos = true;
	}
	if (!_isReached)
	{
		Move(_startPosition, MovePosition);
	}
	else
	{
		Move(MovePosition, _startPosition);
	}
	glm::vec4& position = GetPosition();
	Renderer::GetInstance()->Draw(Sprite, static_cast<int>(position.x), static_cast<int>(position.y), _spriteWidth, _spriteHeight);
}

void MoveablePlatform::Move(glm::vec4 startPos, glm::vec4 endPos)
{
	glm::vec4 directionVector = endPos - startPos;
	static float speed = MoveSpeed * _movementVariable;
	GetPosition() += (directionVector * speed);

	if (directionVector.x < 0 || directionVector.y < 0)
	{
		if (GetPosition().x <= endPos.x && GetPosition().y <= endPos.y )
		{
			_isReached = !_isReached;
			return;
		}
	}
	else if (directionVector.x > 0 || directionVector.y > 0)
	{
		if (GetPosition().x >= endPos.x && GetPosition().y >= endPos.y)
		{
			_isReached = !_isReached;
			return;
		}
	}
}

Vector<Signature> MoveablePlatform::Signatures()
{
	return Vector<Signature>
	{
		{ "MoveSpeed", DatumTypes::Float, 1, offsetof(MoveablePlatform, MoveSpeed)},
		{ "MovePosition", DatumTypes::Vector, 1, offsetof(MoveablePlatform, MovePosition) }
	};
}