#pragma once
#include "ActionList.h"
#include "TypeRegistry.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "GameState.h"

namespace FieaGameEngine
{
	class Reaction : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList);

	public:
		Reaction(const Reaction&) = default;
		Reaction(Reaction&&) noexcept = default;
		Reaction& operator=(const Reaction&) = default;
		Reaction& operator=(Reaction&&) noexcept = default;
		virtual ~Reaction() = default;

		void Update(const GameTime& gameTime) override;

		static const Vector<Signature> Signatures();

	protected:
		Reaction(RTTI::IdType typeId);
	};
}