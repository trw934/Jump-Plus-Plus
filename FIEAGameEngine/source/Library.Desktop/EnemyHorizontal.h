#pragma once
#include "EnemyBase.h"

namespace FieaGameEngine
{
	// Move left & right across the screen, with the leftmost position being the initial position
	class EnemyHorizontal final : public EnemyBase
	{
		RTTI_DECLARATIONS(EnemyHorizontal, EnemyBase);
	public:
		EnemyHorizontal(const std::string& name = "Enemy");
		~EnemyHorizontal() override = default;
		EnemyHorizontal(const EnemyHorizontal& other) = default;
		EnemyHorizontal& operator=(const EnemyHorizontal& other) = default;
		EnemyHorizontal(EnemyHorizontal&& other) noexcept = default;
		EnemyHorizontal& operator=(EnemyHorizontal&& other) noexcept = default;

		void Update(const FieaGameEngine::GameTime& gameTime) override;

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	private:
		glm::vec4 _startPosition; // Initial posiiton of the GameObject
		float _moveDistance{ 100.f }; // Distance traversed rightwards from the start position of the GameObject
		float _moveSpeed{ 15.f };

	};
	ConcreteFactory(EnemyHorizontal, FieaGameEngine::Scope);
}

