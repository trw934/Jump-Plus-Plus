#include "Factory.h"

namespace FieaGameEngine
{
	template <typename AbstractTypeT>
	typename const Factory<AbstractTypeT>* Factory<AbstractTypeT>::Find(const std::string& className)
	{
		auto iter = _factoryMap.Find(className);
		if (iter == _factoryMap.end())
		{
			return nullptr;
		}
		return iter->second;
	}

	template <typename AbstractTypeT>
	typename AbstractTypeT* Factory<AbstractTypeT>::Create(const std::string& className)
	{
		const Factory<AbstractTypeT>* const factory = Find(className);
		if (factory == nullptr)
		{
			return nullptr;
		}
		return factory->Create();
	}

	template <typename AbstractTypeT>
	void Factory<AbstractTypeT>::Clear()
	{
		_factoryMap.Clear();
	}

	template <typename AbstractTypeT>
	std::size_t Factory<AbstractTypeT>::Size()
	{
		return _factoryMap.Size();
	}

	template <typename AbstractTypeT>
	void Factory<AbstractTypeT>::Add(Factory<AbstractTypeT>& factory)
	{
		_factoryMap.Insert(std::make_pair(factory.ClassName(), &factory));
	}

	template <typename AbstractTypeT>
	void Factory<AbstractTypeT>::Remove(Factory<AbstractTypeT>& factory)
	{
		_factoryMap.Remove(factory.ClassName());
	}
}