#pragma once
#include "Datum.h"
#include "Hashmap.h"

namespace FieaGameEngine
{
	struct Signature
	{
		Signature(std::string name, DatumTypes type, std::size_t size, std::size_t offset) :
			_name{ name }, _type{ type }, _size{ size }, _offset{ offset }
		{
		}
		Signature(const Signature& other) = default;
		Signature& operator=(const Signature& other) = default;
		Signature(Signature&& other) = default;
		Signature& operator=(Signature&& other) = default;
		~Signature() = default;

		std::string _name;
		DatumTypes _type;
		std::size_t _size;
		std::size_t _offset;
	};

	class TypeRegistry
	{
		using RegistryMap = Hashmap<size_t, Vector<Signature>>;
	public:
		/// <summary>
		/// Retrieves the pointer to the instance of the type registry, instantiating it if it hasn't already been instantiated.
		/// </summary>
		/// <returns>Pointer to the TypeRegistry instance</returns>
		static TypeRegistry* GetInstance();

		/// <summary>
		/// Registers the templated type into the registry by adding its signatures to the key of the typeID
		/// </summary>
		/// <typeparam name="TChild">Type to register</typeparam>
		template <typename TChild>
		void RegisterType();

		/// <summary>
		/// Registers the child type into the registry, adding the parent's registered signatures to the child's signatures first
		/// </summary>
		/// <typeparam name="TChild">Child to be registered</typeparam>
		/// <typeparam name="TParent">Parent of the child to be registered</typeparam>
		/// <exception cref="std::runtime_error">
		/// Throws when parent has not been registered in type registry
		/// </exception>
		template <typename TChild, typename TParent>
		void RegisterType();

		/// <summary>
		/// Returns the prescribed attributes of the type entered
		/// </summary>
		/// <param name="typeID">The typeID to get the attributes for</param>
		/// <returns>Reference to the vector of signatures of the registered type</returns>
		/// <exception cref="std::runtime_error">
		/// Throws when type has not been registered into the registry
		/// </exception>
		Vector<Signature>& GetPrescribedAttributes(size_t typeID);

		/// <summary>
		/// Clears the type registry
		/// </summary>
		void Clear();

	private:
		TypeRegistry() = default;
		~TypeRegistry() = default;
		TypeRegistry(const TypeRegistry& other) = delete;
		TypeRegistry(TypeRegistry&& other) = delete;
		TypeRegistry& operator=(const TypeRegistry& other) = delete;
		TypeRegistry& operator=(TypeRegistry&& other) = delete;
		static TypeRegistry* _instance;
		RegistryMap _registry;
	};
}

#include "TypeRegistry.inl"