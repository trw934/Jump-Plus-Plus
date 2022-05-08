#include "pch.h"
#include "PlayerManager.h"
#include <iostream>
namespace FieaGameEngine
{
	RTTI_DEFINITIONS(PlayerManager)



		PlayerManager::PlayerManager() : GameObject(PlayerManager::TypeIdClass())
	{
		for (size_t i = 0; i < _projectiles.Capacity(); ++i)
		{
			_projectiles.PushBack(PlayerProjectile());
		}
		_projectiles.ShrinkToFit();
	}
	void PlayerManager::Update(const GameTime& gameTime)
	{
		GameObject::Update(gameTime);
		if (_health <= 0 || GetPosition().y>501.f)
		{
			 AudioManager::Play(3);
			_isGameOver = true;
		}

		
			if (_timer < _initialTimer)
			{
				_timer += 0.01f;
				// 0,0 is top right
				if (_canMove)
				{
					EnqueueJumpEvent(gameTime);
					
					//GetPosition().y -= _speed * 0.01f;// multiply Time.deltaTime, using 0.01 as 1/60 is roughly 0.01 so assuming 60fps
				}
			}
			else
			{
				_speed += _gravity * 0.01f;

				//Clamp jump speed
				if (_speed <= -60.f)
				{
					_speed = -60.f;
				}
				if (_canMove)
				{
					GetPosition().y += _speed * 0.01f;
				}
			}
		


		// Update bullets
		for (auto it = _projectiles.begin(); it != _projectiles.end(); ++it)
		{
			(*it).Update(gameTime);
		}
		if (_bulletCooldown >= 0.01f)
		{
			_bulletCooldown -= 0.01f;
		}

		if (_isMoving)
		{
			
			if (_isMovingRight)
			{
				
				if (GetPosition().x > 0.f)
				{
					GetPosition().x -= _horizontalSpeed * 0.01f;
				}
				
			}
			else 
			{
				if(GetPosition().x+SpriteWidth<318.f)
				GetPosition().x += _horizontalSpeed * 0.01f;
			}
		}
		
		if (GetPosition().y < lastHeight && _canMove)
		{
			_score = _score + (lastHeight - static_cast<int>(GetPosition().y));
			lastHeight = static_cast<int>(GetPosition().y);
		}

		//CODE FOR PLATFORM COLLISION DETECTION
		
		Datum* activeLevels = _parent->Find("Children")->GetTable(1)->Find("ActiveLevels");
		assert(activeLevels != nullptr);

		Datum* Platforms = activeLevels->GetTable(0)->Find("Platforms");
		for (size_t i = 0; i < Platforms->Size(); ++i)
		{
			Platform& currentPlatform = *Platforms->GetTable(i)->As<Platform>();

			// Y AXIS CHECK
			// Check if bottom of Doodle Paul is between the top and bottom of the Platform
			if (static_cast<int>(GetPosition().y) + SpriteHeight >= static_cast<int>(currentPlatform.GetPosition().y) && static_cast<int>(GetPosition().y) + SpriteHeight <= static_cast<int>(currentPlatform.GetPosition().y) + currentPlatform.GetSpriteHeight())
			{
				// MinX  = The Left most x coord of the Platform
				// MaxX = The Right most X coord of the Platform 
				auto minX = static_cast<int>(currentPlatform.GetPosition().x);
				auto maxX = static_cast<int>(currentPlatform.GetPosition().x) + currentPlatform.GetSpriteWidth();

				// X AXIS CHECK
				// Check if right of Doodle Paul is greater than minX, and left of Doodle Paul is less than maxX
				if (static_cast<int>(GetPosition().x) + SpriteWidth >= minX && static_cast<int>(GetPosition().x) <= maxX)
				{
					DestructablePlatform* p = currentPlatform.As<DestructablePlatform>();
					if (p == nullptr)
					{
						PlayerManager payload = *this;
						std::shared_ptr<Event<PlayerManager>> playerJumpEvent = std::make_shared<Event<PlayerManager>>(payload);

						_eventQueue->Enqueue(playerJumpEvent, const_cast<GameTime&>(gameTime));
					}
					else
					{
						p->wasLandedOn = true;
					}
				}
			}
		}

		// Collision detection for enemies
		Datum* Enemies = activeLevels->GetTable(0)->Find("Enemies");
		assert(Enemies != nullptr);

		for (size_t i = 0; i < Enemies->Size(); ++i)
		{
			EnemyBase& currentEnemy = *Enemies->GetTable(i)->As<EnemyBase>();

			// X AXIS CHECK
			// Check if bottom of Doodle Paul is between the top and bottom of the Platform
			// MinX  = The Left most x coord of the Platform
			// MaxX = The Right most X coord of the Platform 
			auto minX = static_cast<int>(currentEnemy.GetPosition().x);
			auto maxX = static_cast<int>(currentEnemy.GetPosition().x) + currentEnemy.SpriteWidth;
			if (static_cast<int>(GetPosition().x) + SpriteWidth >= minX && static_cast<int>(GetPosition().x) <= maxX)
			{
				// Y AXIS CHECK
			// Check if right of Doodle Paul is greater than minX, and left of Doodle Paul is less than maxX
				if (static_cast<int>(GetPosition().y) + SpriteHeight >= static_cast<int>(currentEnemy.GetPosition().y) - 1 && static_cast<int>(GetPosition().y) + SpriteHeight <= static_cast<int>(currentEnemy.GetPosition().y) + 1)
				{
					currentEnemy.~EnemyBase();

					PlayerManager payload = *this;
					std::shared_ptr<Event<PlayerManager>> playerJumpEvent = std::make_shared<Event<PlayerManager>>(payload);
					_eventQueue->Enqueue(playerJumpEvent, const_cast<GameTime&>(gameTime));
				}
				else if (static_cast<int>(GetPosition().y) + SpriteHeight >= static_cast<int>(currentEnemy.GetPosition().y) && static_cast<int>(GetPosition().y) <= static_cast<int>(currentEnemy.GetPosition().y) + currentEnemy.SpriteHeight)
				{
					EnemyCollision(currentEnemy);
				}
			}
		}

		// Collision detection for bullets
		for (size_t i = 0; i < Enemies->Size(); ++i)
		{
			EnemyBase& currentEnemy = *Enemies->GetTable(i)->As<EnemyBase>();

			// Y AXIS CHECK
			// Check if bottom of Doodle Paul is between the top and bottom of the Platform
			for (auto it = _projectiles.begin(); it != _projectiles.end(); ++it)
			{
				if (static_cast<int>((*it).GetPosition().y) + (*it).SpriteHeight >= static_cast<int>(currentEnemy.GetPosition().y) && static_cast<int>((*it).GetPosition().y) + (*it).SpriteHeight <= static_cast<int>(currentEnemy.GetPosition().y) + currentEnemy.SpriteHeight)
				{
					// MinX  = The Left most x coord of the Platform
					// MaxX = The Right most X coord of the Platform 
					auto minX = static_cast<int>(currentEnemy.GetPosition().x);
					auto maxX = static_cast<int>(currentEnemy.GetPosition().x) + currentEnemy.SpriteWidth;

					// X AXIS CHECK
					// Check if right of Doodle Paul is greater than minX, and left of Doodle Paul is less than maxX
					if (static_cast<int>((*it).GetPosition().x) + (*it).SpriteWidth >= minX && static_cast<int>((*it).GetPosition().x) <= maxX)
					{
						BulletCollision(currentEnemy, it);
					}
				}
			}

		}

		Renderer::GetInstance()->Draw(Sprite, static_cast<int>(GetPosition().x), static_cast<int>(GetPosition().y), SpriteWidth, SpriteHeight);
		Renderer::GetInstance()->renderScore(std::to_string(_score), 250, 0);
	}
	gsl::owner<PlayerManager*> PlayerManager::Clone() const
	{
		return new PlayerManager(*this);
	}
	Vector<Signature> FieaGameEngine::PlayerManager::Signatures()
	{
		return Vector<Signature>
		{
			{"Speed", DatumTypes::Float, 1, offsetof(PlayerManager, _speed)},
			{ "Horizontal Speed", DatumTypes::Float, 1, offsetof(PlayerManager, _horizontalSpeed) },
			{ "Health",DatumTypes::Float,1,offsetof(PlayerManager,_health) },
			{ "Score",DatumTypes::Integer,1,offsetof(PlayerManager, _score) }
		};
	}

	void PlayerManager::IsMovingRight(bool isMovingRight)
	{
		_isMovingRight = isMovingRight;
	}

	void PlayerManager::IsMoving(bool isMoving)
	{
		AudioManager::Play(0);
		_isMoving = isMoving;
	}

	void PlayerManager::Jump()
	{
		
		_speed = -50.f;
	}

	void PlayerManager::EnemyCollision(EnemyBase& enemy)
	{
			_health = 0;
		enemy.~EnemyBase();
	}

	void PlayerManager::BulletCollision(EnemyBase& enemy, Vector<PlayerProjectile>::Iterator& it)
	{
		enemy.TakeDamage(1);
		(*it).GetPosition() = glm::vec4(0);
		(*it).SetIsActive(false);
	}

	void PlayerManager::SetEventQueue(EventQueue& queue)
	{
		_eventQueue = &queue;
	}

	void PlayerManager::SetCanMove(bool canMove)
	{
		_canMove = canMove;
	}
	void PlayerManager::EnqueueJumpEvent(const GameTime& gameTime)
	{
		PlayerManager payload = *this;
		std::shared_ptr<Event<PlayerManager>> playerJumpEvent = std::make_shared<Event<PlayerManager>>(payload);
		_eventQueue->Enqueue(playerJumpEvent, const_cast<GameTime&>(gameTime));
	}

	bool PlayerManager::GetCanMove()
	{
		return _canMove;
	}




}