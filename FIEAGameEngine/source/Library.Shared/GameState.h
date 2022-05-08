#pragma once
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "GameObject.h"
#include "EventQueue.h"

namespace FieaGameEngine 
{
	class GameState
	{
	public:

		/// <summary>
		/// Defaulted special members
		/// </summary>
		GameState(GameObject* rootObject);
		~GameState() = default;
		GameState(const GameState& other) = default;
		GameState& operator=(const GameState& other) = default;
		GameState(GameState&& other) = default;
		GameState& operator=(GameState&& other) = default;

		const GameTime& GetGameTime() const;
		void SetClockEnabled();
		void SetEventQueue(EventQueue* eventQueue);
		EventQueue* GetEventQueue() const;
		void Update();

		/// <summary>
		/// Adds any actions that need to be created to the scope specified in the createdActions vector
		/// </summary>
		void Add();

		/// <summary>
		/// Destroys any actions specified in the destroyedActions vector
		/// </summary>
		void Resolve();

	private:
		GameTime _gameTime;
		GameClock _gameClock;
		GameObject* _rootObject;
		EventQueue* _eventQueue;
		bool _isGameClockEnabled;
	};
}