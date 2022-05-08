#include "pch.h"
#include "DestructablePlatform.h"

using namespace FieaGameEngine;
RTTI_DEFINITIONS(DestructablePlatform);

DestructablePlatform::DestructablePlatform() :
	Platform(DestructablePlatform::TypeIdClass())
{
}

gsl::owner<DestructablePlatform*> DestructablePlatform::Clone() const
{
	return new DestructablePlatform(*this);
}

void DestructablePlatform::Update(const GameTime& gameTime)
{
	if (wasLandedOn)
	{
		AudioManager::Play(1);
		this->~DestructablePlatform();
	}
	else
	{
		Platform::Update(gameTime);
	}
}

Vector<Signature> DestructablePlatform::Signatures()
{
	return Vector<Signature>
	{
	};
}