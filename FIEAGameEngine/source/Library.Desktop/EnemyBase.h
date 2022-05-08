#pragma once
#include "GameObject.h"
#include "..\Game.Desktop\Rendering\Renderer.h"

// Base class for enemy; creates an enemy that stands in place
namespace FieaGameEngine
{
	class EnemyBase : public FieaGameEngine::GameObject
	{
		RTTI_DECLARATIONS(EnemyBase, FieaGameEngine::GameObject);
	public:
		EnemyBase(const std::string& name = "Enemy");
		virtual ~EnemyBase() = default;
		EnemyBase(const EnemyBase& other) = default;
		EnemyBase& operator=(const EnemyBase& other) = default;
		EnemyBase(EnemyBase&& other) noexcept = default;
		EnemyBase& operator=(EnemyBase&& other) noexcept = default;

		gsl::owner<FieaGameEngine::GameObject*> Clone() const override;
		bool Equals(const RTTI* other) const override;
		std::string ToString() const override;

		virtual void Update(const FieaGameEngine::GameTime& gameTime) override;

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
		virtual void TakeDamage(int damage = 1);

		// Accessors
		int Health() const;

		// TODO: subscribe to collision events, and take damage
		// TODO: Destroy enemy when HP is 0
	protected:
		int _health = 1;
	};
	ConcreteFactory(EnemyBase, FieaGameEngine::Scope);
}

