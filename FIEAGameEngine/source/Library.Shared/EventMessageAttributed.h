#pragma once
#include "Attributed.h"
#include "TypeRegistry.h"
#include "GameState.h"
#include <string>
#include <functional>

namespace FieaGameEngine
{
	class EventMessageAttributed final : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed);
	public:
		EventMessageAttributed(const GameState& gameState, const std::string& subType);
		EventMessageAttributed(const EventMessageAttributed&) = default;
		EventMessageAttributed(EventMessageAttributed&&) noexcept = default;
		EventMessageAttributed& operator=(const EventMessageAttributed&) = default;
		EventMessageAttributed& operator=(EventMessageAttributed&&) noexcept = default;
		~EventMessageAttributed() = default;

		const GameState& GetGameState() const;
		void SetGameState(GameState& gameState);

		const std::string& SubType() const;
		void SetSubType(const std::string& subType);

		gsl::owner<EventMessageAttributed*> Clone() const override;

		static const Vector<Signature> Signatures();

	private:
		std::reference_wrapper<const GameState> _gameState;
		std::string _subType;
	};
}