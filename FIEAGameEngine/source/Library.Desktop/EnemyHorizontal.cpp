#include "pch.h"
#include "EnemyHorizontal.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EnemyHorizontal);
	EnemyHorizontal::EnemyHorizontal(const std::string& name) :
		EnemyBase(name)
	{
		_startPosition = GetPosition();
	}
	void EnemyHorizontal::Update(const FieaGameEngine::GameTime& gameTime)
	{
		GetPosition().x += (_moveSpeed * 0.01f);
		if (GetPosition().x >= _startPosition.x + _moveDistance || GetPosition().x <= _startPosition.x)
		{
			_moveSpeed *= -1;
		}

		EnemyBase::Update(gameTime);
	}
	FieaGameEngine::Vector<FieaGameEngine::Signature> EnemyHorizontal::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{ "_moveDistance", FieaGameEngine::DatumTypes::Float, 1, offsetof(EnemyHorizontal, _moveDistance) },
			{ "_moveSpeed", FieaGameEngine::DatumTypes::Float, 1, offsetof(EnemyHorizontal, _moveSpeed) }
		};
	}
}