#pragma once
#include "GameObject.h"
#include "EventSubscriber.h"
#include "Event.h"
#include "..\Input\InputEventPayload.h"
#include "..\\Rendering\Renderer.h"
#include <iostream>

namespace FieaGameEngine
{
	class PlayerProjectile : public GameObject
	{
		RTTI_DECLARATIONS(PlayerProjectile, GameObject)

	public:
		PlayerProjectile() : GameObject(PlayerProjectile::TypeIdClass())
		{
			Sprite = "bullet";
			SpriteWidth = 16;
			SpriteHeight = 16;
		};
		virtual ~PlayerProjectile() = default;
		PlayerProjectile(const PlayerProjectile& other) = default;
		PlayerProjectile& operator=(const PlayerProjectile& other) = default;
		PlayerProjectile(PlayerProjectile&& other) noexcept = default;
		PlayerProjectile& operator=(PlayerProjectile&& other) noexcept = default;

		// Overrides
		gsl::owner<FieaGameEngine::GameObject*> Clone() const override;
		std::string ToString() const override;
		virtual void Update(const FieaGameEngine::GameTime& gameTime) override;

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

		void SetIsActive(bool isActive);

	protected:
		glm::vec4 _projectileSpeed = glm::vec4(0, -80, 0, 0);
		bool _isActive = false;
	};
	ConcreteFactory(PlayerProjectile, Scope);
}