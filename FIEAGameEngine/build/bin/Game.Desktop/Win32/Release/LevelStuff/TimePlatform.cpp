#include "pch.h"
#include "TimePlatform.h"
#include <math.h>
#include <iostream>

using namespace FieaGameEngine;
RTTI_DEFINITIONS(TimePlatform);

TimePlatform::TimePlatform() :
	Platform(TimePlatform::TypeIdClass())
{
	//assign the _clockDestructionTime to the float we did
}

gsl::owner<TimePlatform*> TimePlatform::Clone() const
{
	return new TimePlatform(*this);
}

void TimePlatform::Update(const GameTime& gameTime)
{
	
	Platform::Update(gameTime);
	if (!_firstFrame && !_isDestroyed)
	{
		if (_accumulateTime < DestructionTime)
		{
			auto f_ms = gameTime.CurrentTime() - prevTimePoint;
			auto f = f_ms.count() * pow(10, -9.0f);
			_accumulateTime += static_cast<float>(f);
		}
		else
		{
			//Destroy the platform
			//Clean the render of the object
			//Clean the collision of the object
			this->~TimePlatform();
			_isDestroyed = true;
		}
	}
	prevTimePoint = gameTime.CurrentTime();
	_firstFrame = false;
}

Vector<Signature> TimePlatform::Signatures()
{
	return Vector<Signature>
	{
		{ "DestructionTime", DatumTypes::Float, 1, offsetof(TimePlatform, DestructionTime)}
	};
}