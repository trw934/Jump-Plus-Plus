#include "pch.h"
#include "Reaction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Reaction);

	Reaction::Reaction(RTTI::IdType typeId) :
		ActionList(typeId)
	{
	}

	void Reaction::Update(const GameTime&)
	{
		// No implementation (in case Reaction is placed alongside action objects)
	}

	const Vector<Signature> Reaction::Signatures()
	{
		return Vector<Signature>();
	}
}