#pragma once
#include "GameObject.h"
#include "Platform.h"
#include "../Output/AudioManager.h"

class DestructablePlatform : public Platform
{
	RTTI_DECLARATIONS(DestructablePlatform, Platform);

public:
	DestructablePlatform();
	virtual ~DestructablePlatform() = default;
	DestructablePlatform(const DestructablePlatform& other) = default;
	DestructablePlatform& operator=(const DestructablePlatform& other) = default;
	DestructablePlatform(DestructablePlatform&& other) = default;
	DestructablePlatform& operator=(DestructablePlatform&& other) = default;

	gsl::owner<DestructablePlatform*> Clone() const override;

	virtual void Update(const FieaGameEngine::GameTime& gameTime) override;

	static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	bool wasLandedOn{ false };  //bool for checking if the player landed on the platform
};

ConcreteFactory(DestructablePlatform, FieaGameEngine::Scope);