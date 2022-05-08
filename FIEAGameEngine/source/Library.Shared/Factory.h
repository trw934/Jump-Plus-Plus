#pragma once
#include "gsl/gsl"
#include "Hashmap.h"
namespace FieaGameEngine
{
	/// <summary>
	/// Design pattern for instantiating multiple types that don't exist at compile time
	/// </summary>
	/// <typeparam name="AbstractTypeT">Type of class the factory will produce</typeparam>
	template <typename AbstractTypeT>
	class Factory
	{
	public:

		/// <summary>
		/// Defaulted special members
		/// </summary>
		Factory() = default;
		virtual ~Factory() = default;
		Factory(const Factory& other) = default;
		Factory& operator=(const Factory& other) = default;
		Factory(Factory&& other) = default;
		Factory& operator=(Factory&& other) = default;

		/// <summary>
		/// Finds the factory associated with the entered class name
		/// </summary>
		/// <param name="className">Class name of the factory</param>
		/// <returns>Pointer to the factory, or nullptr if the factory hasn't been instantiated</returns>
		static const Factory* Find(const std::string& className);

		/// <summary>
		/// Returns a new object of the type entered with the class name
		/// </summary>
		/// <param name="className">Class name of the object to create</param>
		/// <returns>Pointer to the created object, or nullptr if the factory hasn't been instantiated</returns>
		static AbstractTypeT* Create(const std::string& className);

		/// <summary>
		/// Clears the entries of the list of factories.
		/// </summary>
		static void Clear();

		/// <summary>
		/// Returns the number of entries in the list of factories
		/// </summary>
		/// <returns>Number of entries in the list of factories</returns>
		static std::size_t Size();

		/// <summary>
		/// Returns the address of a concrete product associated with the factory class
		/// </summary>
		/// <returns>Address of a concrete product associated with the factory class</returns>
		virtual gsl::owner<AbstractTypeT*> Create() const = 0;

		/// <summary>
		/// Returns a string associated with the concrete factory class
		/// </summary>
		/// <returns>String representing the name of the class the factory instantiates</returns>
		virtual const std::string& ClassName() const = 0;
	protected:
		static void Add(Factory& factory);
		static void Remove(Factory& factory);
	private:
		inline static Hashmap<std::string, const Factory* const> _factoryMap;
	};
}

#include "Factory.inl"

#define ConcreteFactory(ConcreteProductType, AbstractProductType)			\
class ConcreteProductType ## Factory : public FieaGameEngine::Factory<AbstractProductType>	\
{																			\
public:																		\
	ConcreteProductType ## Factory() :										\
	_className{#ConcreteProductType}										\
	{																		\
		Add(*this);															\
	}																		\
	virtual ~ConcreteProductType ## Factory()								\
	{																		\
		Remove(*this);														\
	}																		\
	const std::string& ClassName() const override							\
	{																		\
		return _className;													\
	}																		\
	gsl::owner<AbstractProductType*> Create() const override				\
	{																		\
		return new ConcreteProductType;										\
	}																		\
private:																	\
	std::string _className;													\
};																			\


