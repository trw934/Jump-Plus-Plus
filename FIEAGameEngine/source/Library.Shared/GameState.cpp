#include "pch.h"
#include "GameState.h"

namespace FieaGameEngine
{

	void GameState::Add()
	{
		for (auto& [scope, action] : ActionCreateAction::createdActions)
		{
			scope->Adopt(*action, "Actions");
		}
		ActionCreateAction::createdActions.Clear();
	}

	void GameState::Resolve()
	{
		for (auto& [action, index] : ActionDestroyAction::destroyedActions)
		{
			delete action->GetTable(index);
			action->RemoveAt(index);
		}
		ActionDestroyAction::destroyedActions.Clear();
	}

	GameState::GameState(GameObject* rootObject) : 
		_rootObject(rootObject)
	{
	}

	const GameTime& GameState::GetGameTime() const
	{
		return _gameTime;
	}

	void GameState::SetClockEnabled()
	{
		_isGameClockEnabled = true;
	}

	void GameState::SetEventQueue(EventQueue* eventQueue)
	{
		_eventQueue = eventQueue;
	}

	EventQueue* GameState::GetEventQueue() const
	{
		assert(_eventQueue != nullptr);
		return _eventQueue;
	}

	void GameState::Update()
	{
		if (_isGameClockEnabled)
		{
			_gameClock.UpdateGameTime(_gameTime);
		}

		if (_rootObject != nullptr)
		{
			_rootObject->Update(_gameTime);
		}

		_eventQueue->Update(_gameTime);
	}
}