#include "pch.h"
#include "EventMessageAttributed.h"

namespace FieaGameEngine
{
	EventMessageAttributed::EventMessageAttributed(const GameState& gameState, const std::string& subType) :
		_gameState{ gameState }, _subType{subType}, Attributed(EventMessageAttributed::TypeIdClass())
	{
	}

	const GameState& EventMessageAttributed::GetGameState() const
	{
		return _gameState.get();
	}

	void EventMessageAttributed::SetGameState(GameState& gameState)
	{
		_gameState = gameState;
	}

	const std::string& EventMessageAttributed::SubType() const
	{
		return _subType;
	}

	void EventMessageAttributed::SetSubType(const std::string& subType)
	{
		_subType = subType;
	}

	gsl::owner<EventMessageAttributed*> EventMessageAttributed::Clone() const
	{
		return new EventMessageAttributed(*this);
	}

	const Vector<Signature> EventMessageAttributed::Signatures()
	{
		return Vector<Signature>();
	}
}