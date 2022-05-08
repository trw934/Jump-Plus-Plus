#include "pch.h"
#include "EnemyBase.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EnemyBase);
	EnemyBase::EnemyBase(const std::string& name) : GameObject(EnemyBase::TypeIdClass())
	{
		Name = name;
	}
	gsl::owner<FieaGameEngine::GameObject*> EnemyBase::Clone() const
	{
		return new EnemyBase(*this);
	}
	bool EnemyBase::Equals(const RTTI* other) const
	{
		const EnemyBase* otherCast = other->As<EnemyBase>();
		if (otherCast == nullptr)
		{
			return false;
		}

		return operator==(*otherCast);
	}
	std::string EnemyBase::ToString() const
	{
		return std::string("EnemyBase");
	}
	void EnemyBase::Update(const FieaGameEngine::GameTime& gameTime)
	{
		gameTime;
		glm::vec4& position = GetPosition();
		Renderer::GetInstance()->Draw(Sprite, static_cast<int>(position.x), static_cast<int>(position.y), SpriteWidth, SpriteHeight);
	}
	FieaGameEngine::Vector<FieaGameEngine::Signature> EnemyBase::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{ "Health", FieaGameEngine::DatumTypes::Integer, 1, offsetof(EnemyBase, _health) }
		};
	}
	void EnemyBase::TakeDamage(int damage)
	{
		_health -= damage;
		if (_health <= 0)
		{
			this->~EnemyBase();
		}
	}

	int EnemyBase::Health() const
	{
		return _health;
	}
}
