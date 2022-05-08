#include "pch.h"
#include "PlayerProjectile.h"
#include "../Library.Desktop/EnemyBase.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(PlayerProjectile)

	gsl::owner<FieaGameEngine::GameObject*> FieaGameEngine::PlayerProjectile::Clone() const
	{
		return new PlayerProjectile(*this);
	}

	std::string FieaGameEngine::PlayerProjectile::ToString() const
	{
		return std::string("PlayerProjectile");
	}

	void FieaGameEngine::PlayerProjectile::Update(const FieaGameEngine::GameTime& gameTime)
	{
		gameTime;
		if (_isActive)
		{
			Renderer::GetInstance()->Draw(Sprite, static_cast<int>(GetPosition().x), static_cast<int>(GetPosition().y), SpriteWidth, SpriteHeight);
			GetPosition() += _projectileSpeed * 0.01f;
		}

		if (GetPosition().y < 0)
		{
			_isActive = false;
		}
	}
	FieaGameEngine::Vector<FieaGameEngine::Signature> PlayerProjectile::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{ "Speed", DatumTypes::Integer, 1, offsetof(PlayerProjectile, _projectileSpeed) }
		};
	}
	void PlayerProjectile::SetIsActive(bool isActive)
	{
		_isActive = isActive;
	}
}
