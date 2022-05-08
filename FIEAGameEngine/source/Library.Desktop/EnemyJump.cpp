#include "pch.h"
#include "EnemyJump.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EnemyJump)
	EnemyJump::EnemyJump(const std::string& name) :
		EnemyBase(name)
	{
		_startPosition = GetPosition();
	}
	void EnemyJump::Update(const FieaGameEngine::GameTime& gameTime)
	{
		glm::vec4& position = GetPosition();
		if (!_isFalling && _jumpTime > 0)
		{
			position -= 0.01f;
		}
		else if (_isFalling)
		{
			position += 0.01f;
		}

		if (position.y < _startPosition.y + _jumpHeight)
		{
			_isFalling = true;
		}
		else if (position.y > _startPosition.y)
		{
			_jumpTime = _jumpCooldown;
			_isFalling = false;
		}

		if (_jumpTime > 0)
		{
			_jumpTime -= 0.01f;
		}

		EnemyBase::Update(gameTime);
	}
	FieaGameEngine::Vector<FieaGameEngine::Signature> EnemyJump::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			/*{ "_moveDistance", FieaGameEngine::DatumTypes::Integer, 1, offsetof(EnemyJump, _jumpHeight) },
			{ "_moveDistance", FieaGameEngine::DatumTypes::Float, 1, offsetof(EnemyJump, _jumpCooldown) },
			{ "_moveSpeed", FieaGameEngine::DatumTypes::Float, 1, offsetof(EnemyJump, _jumpTime) }*/
		};
	}
}