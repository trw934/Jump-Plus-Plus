#include "pch.h"
#include "ReactionAttributed.h"
#include <cassert>
using namespace std::string_literals;
using namespace std;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ReactionAttributed);

	ReactionAttributed::ReactionAttributed(const string&, const string& subType) :
		Reaction(ReactionAttributed::TypeIdClass()), _subType(subType)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(const ReactionAttributed& rhs) :
		Reaction(rhs), _subType(rhs._subType)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(ReactionAttributed&& rhs) noexcept :
		Reaction(move(rhs)), _subType(move(rhs._subType))
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	const string& ReactionAttributed::SubType() const
	{
		return _subType;
	}

	void ReactionAttributed::SetSubType(const std::string& subType)
	{
		_subType = subType;
	}

	gsl::owner<ReactionAttributed*> ReactionAttributed::Clone() const
	{
		return new ReactionAttributed(*this);
	}

	void ReactionAttributed::Notify(const EventPublisher& event)
	{
		assert(event.Is(Event<EventMessageAttributed>::TypeIdClass()));
		const Event<EventMessageAttributed>& typedEvent = static_cast<const Event<EventMessageAttributed>&>(event);
		const EventMessageAttributed& eventMessageAttributed = typedEvent.Message();
		if (_subType == eventMessageAttributed.SubType())
		{
			const auto& signatures = TypeRegistry::GetInstance()->GetPrescribedAttributes(EventMessageAttributed::TypeIdClass());
			const auto auxillaryAttributeCount = eventMessageAttributed.Size() - signatures.Size();
			Vector<Scope> parameters(auxillaryAttributeCount);
			
		}
	}

	const Vector<Signature> ReactionAttributed::Signatures()
	{
		return Vector<Signature>
		{
			{"SubType"s, DatumTypes::String, 1, offsetof(ReactionAttributed, _subType)}
		};
	}
}