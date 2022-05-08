#include "TypeRegistry.h"

namespace FieaGameEngine
{
	template <typename TChild>
	void TypeRegistry::RegisterType()
	{
		_registry.Insert(std::make_pair(TChild::TypeIdClass(), TChild::Signatures()));
	}

	template <typename TChild, typename TParent>
	void TypeRegistry::RegisterType()
	{
		size_t parentID = TParent::TypeIdClass();
		size_t childID = TChild::TypeIdClass();

		Vector<Signature> attributes;

		auto parentIter = _registry.Find(parentID);
		if (parentIter == _registry.end())
		{
			throw std::runtime_error("Parent class not in registry.");
		}
		attributes = parentIter->second;

		for (auto& childIter : TChild::Signatures())
		{
			attributes.PushBack(childIter);
		}

		_registry.Insert(std::make_pair(childID, attributes));
	}
}