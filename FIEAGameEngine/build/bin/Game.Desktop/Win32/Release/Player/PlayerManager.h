#pragma once
#include "GameObject.h"
#include "EventSubscriber.h"
#include "Event.h"
#include "EventQueue.h"
#include "..\Input\InputEventPayload.h"
#include "..\\Rendering\Renderer.h"
#include "..\Game.Desktop\LevelStuff\Platform.h"
#include "../Library.Desktop/EnemyBase.h"
#include "PlayerProjectile.h"
#include <iostream>
#include "../LevelStuff/DestructablePlatform.h"
#include "../Output/AudioManager.h"

namespace FieaGameEngine
{
	// make singleton maybe?
	class PlayerManager : public GameObject
	{
		RTTI_DECLARATIONS(PlayerManager, GameObject)
	public:
		PlayerManager();
		PlayerManager(const PlayerManager& rhs) = default;
		PlayerManager& operator=(const PlayerManager& rhs) = default;
		PlayerManager(PlayerManager&& rhs) = default;
		PlayerManager& operator=(PlayerManager&& rhs) = default;
		virtual ~PlayerManager() = default;

		virtual void Update(const GameTime& gameTime);

		gsl::owner<PlayerManager*> Clone() const override;
		static Vector<Signature> Signatures();


		void IsMovingRight(bool isMovingRight);

		void IsMoving(bool isMoving);

		void Jump();

		void EnemyCollision(EnemyBase& enemy);

		void BulletCollision(EnemyBase& enemy, Vector<PlayerProjectile>::Iterator& it);

		void SetEventQueue(EventQueue& queue);

		void SetCanMove(bool canMove);

		void EnqueueJumpEvent(const GameTime& gameTime);
		bool GetCanMove();

		float _speed;
		float _health;
		float _horizontalSpeed;

		int _score;
		int lastHeight = 500;
		Vector<PlayerProjectile> _projectiles{ size_t(5) };
		size_t _projectileIndex{ 0 };
		float _bulletCooldown{ 0 };
		float _bulletMaxCooldown{ 5.0f };
		bool _isGameOver{ false };

	private:
		EventQueue* _eventQueue{ nullptr };
		bool _isMovingRight{ false };
		bool _isMoving{ false };
		bool _canMove{ true };
		float _gravity{ 9.8f };
		float _initialTimer{ 1.f };
		float _timer{ 0.0f };


	};
	struct KeyPress final : public EventSubscriber
	{
		KeyPress(PlayerManager* player) {
			_player = player;
		}
		void Notify(const FieaGameEngine::EventPublisher& event) override
		{
			if (_player->GetCanMove())
			{
				const Event<InputDownEventPayload>* keyPress = event.As<Event<InputDownEventPayload>>();
				if (keyPress != nullptr)
				{
					const InputDownEventPayload keyDownEvent = keyPress->Message();
					if (keyDownEvent.A)
					{
						_player->IsMoving(true);
						_player->IsMovingRight(true);

					}
					else if (keyDownEvent.D)
					{
						_player->IsMoving(true);
						_player->IsMovingRight(false);

					}
					else if (keyDownEvent.F)
					{
						if (_player->_bulletCooldown <= 0.01f)
						{
							AudioManager::Play(2);
							PlayerProjectile& projectile = _player->_projectiles[_player->_projectileIndex];
							projectile.GetPosition() = _player->GetPosition() + glm::vec4((_player->SpriteWidth / 4), 0, 0, 0);
							projectile.SetIsActive(true);
							_player->_projectileIndex = ++_player->_projectileIndex % _player->_projectiles.Capacity();
							_player->_bulletCooldown = _player->_bulletMaxCooldown;
						}
					}
				}
			}
		}

	private: PlayerManager* _player;
	};

	struct  KeyRelease final : public EventSubscriber
	{
		KeyRelease(PlayerManager* player) {
			_player = player;
		}
		void Notify(const FieaGameEngine::EventPublisher& event) override
		{

			const Event<InputUpEventPayload>* keyRelease = event.As<Event<InputUpEventPayload>>();
			if (keyRelease != nullptr)
			{
				const InputUpEventPayload keyDownEvent = keyRelease->Message();
				if (keyDownEvent.A)
				{
					_player->IsMoving(false);
					_player->IsMovingRight(false);

				}
				else if (keyDownEvent.D)
				{
					_player->IsMoving(false);
					_player->IsMovingRight(false);

				}
				else if (keyDownEvent.F)
				{

				}
			}
		}

	private: PlayerManager* _player;

	};

	struct PlayerJump final : public EventSubscriber
	{
		PlayerJump(PlayerManager* player) :
			_player(player)
		{
		}

		void Notify(const FieaGameEngine::EventPublisher& event)
		{
			const Event<PlayerManager>* e = event.As<Event<PlayerManager>>();

			if (e != nullptr)
			{
				//Get player from the message message
				_player->Jump();
			}
		}

	private:
		PlayerManager* _player;
	};

	//struct PlayerKeyRelease final : public EventSubscriber
	//{
	//	void Notify(const FieaGameEngine::EventPublisher& event) override
	//	{
	//		const Event<PlayerManager>* e = event.As<Event<PlayerManager>>();
	//		if (e != nullptr)
	//		{
	//			const PlayerManager& player = e->Message();
	//			player.ToggleMovement();

	//		}
	//	}


	//};
	ConcreteFactory(PlayerManager, Scope)


}