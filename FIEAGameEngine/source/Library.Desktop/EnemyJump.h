#pragma once
#include "EnemyBase.h"

namespace FieaGameEngine
{
	class EnemyJump final : public EnemyBase
	{
		RTTI_DECLARATIONS(EnemyJump, EnemyBase);
	public:
		EnemyJump(const std::string& name = "Enemy");
		~EnemyJump() override = default;
		EnemyJump(const EnemyJump& other) = default;
		EnemyJump& operator=(const EnemyJump& other) = default;
		EnemyJump(EnemyJump&& other) noexcept = default;
		EnemyJump& operator=(EnemyJump&& other) noexcept = default;

		void Update(const FieaGameEngine::GameTime & gameTime) override;

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	private:
		glm::vec4 _startPosition; // Initial posiiton of the GameObject
		int _jumpHeight{ 100 };
		float _jumpCooldown{ 1 }; // Max time until enemy jumps again
		float _jumpTime{ 3 }; // Time until next jump
		bool _isFalling{ false };
	};
	ConcreteFactory(EnemyJump, FieaGameEngine::Scope);
}

