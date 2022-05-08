#include "pch.h"
#include "TypeRegistry.h"

namespace FieaGameEngine
{
	TypeRegistry *TypeRegistry::_instance = nullptr;

	TypeRegistry* TypeRegistry::GetInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new TypeRegistry;
		}
		return _instance;
	}

	Vector<Signature>& TypeRegistry::GetPrescribedAttributes(size_t typeID)
	{
		auto result = _registry.Find(typeID);
		if (result == _registry.end())
		{
			throw std::runtime_error("Class has not been registered in type array");
		}
		return result->second;
	}

	void TypeRegistry::Clear()
	{
		_registry.Clear();
	}
}