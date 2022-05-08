#include "pch.h"
#include "GameTime.h"

using namespace std::chrono;

namespace FieaGameEngine
{
	const high_resolution_clock::time_point& GameTime::CurrentTime() const
	{
		return mCurrentTime;
	}

	void GameTime::SetCurrentTime(const high_resolution_clock::time_point& currentTime)
	{
		mCurrentTime = currentTime;
	}

	const milliseconds& GameTime::TotalGameTime() const
	{
		return mTotalGameTime;
	}

	void GameTime::SetTotalGameTime(const milliseconds& totalGameTime)
	{
		mTotalGameTime = totalGameTime;
	}

	const milliseconds& GameTime::ElapsedGameTime() const
	{
		return mElapsedGameTime;
	}

	void GameTime::SetElapsedGameTime(const milliseconds& elapsedGameTime)
	{
		mElapsedGameTime = elapsedGameTime;
	}

	duration<float> GameTime::TotalGameTimeSeconds() const
	{
		return duration_cast<duration<float>>(mTotalGameTime);
	}

	duration<float> GameTime::ElapsedGameTimeSeconds() const
	{
		return duration_cast<duration<float>>(mElapsedGameTime);
	}

	void GameTime::UpdateDeltaTime(std::chrono::high_resolution_clock::time_point prevTimePoint)
	{
		auto f_ms = CurrentTime() - prevTimePoint;
		auto f = f_ms.count() * pow(10, -9.0f);
		_deltaTime = static_cast<float>(f);
	}
}