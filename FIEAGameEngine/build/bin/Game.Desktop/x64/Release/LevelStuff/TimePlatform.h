#pragma once
#include "GameObject.h"
#include "Platform.h"
#include "GameTime.h"

using namespace std::chrono_literals;

class TimePlatform : public Platform
{
	RTTI_DECLARATIONS(TimePlatform, Platform);

public:
	TimePlatform();
	virtual ~TimePlatform() = default;
	TimePlatform(const TimePlatform& other) = default;
	TimePlatform& operator=(const TimePlatform& other) = default;
	TimePlatform(TimePlatform&& other) = default;
	TimePlatform& operator=(TimePlatform&& other) = default;

	gsl::owner<TimePlatform*> Clone() const override;

	virtual void Update(const FieaGameEngine::GameTime& gameTime) override;

	static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	float DestructionTime{0};  //the time to destruct

protected:
	float _accumulateTime{0};  //the time since the platform populate
	bool _firstFrame = true;
	bool _isDestroyed = false;
	std::chrono::high_resolution_clock::time_point prevTimePoint;
};

ConcreteFactory(TimePlatform, FieaGameEngine::Scope);