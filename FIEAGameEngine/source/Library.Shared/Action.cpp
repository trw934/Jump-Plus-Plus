#include "pch.h"
#include "Action.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action);

	Action::Action(size_t childID) :
		Attributed(childID)
	{
	}

	const Vector<Signature> Action::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", DatumTypes::String, 1, offsetof(Action, _name) }
		};
	}

	void Action::SetName(const std::string& value)
	{
		_name = value;
	}

	std::string& Action::Name()
	{
		return _name;
	}
}