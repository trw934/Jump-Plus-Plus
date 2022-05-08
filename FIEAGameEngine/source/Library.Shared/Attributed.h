#pragma once
#include "Scope.h"
#include "TypeRegistry.h"

namespace FieaGameEngine
{
	class Datum;
	class Attributed : public Scope
	{
		/*class SliceIterator 
		{
			friend Attributed;
			enum class IteratorModes
			{
				All,
				Prescribed,
				Auxillary
			};
		public:
			SliceIterator() = default;
			~SliceIterator() = default;
			SliceIterator(const SliceIterator& other) = default;
			SliceIterator& operator=(const SliceIterator& other) = default;

			SliceIterator& operator++();

			SliceIterator operator++(int);

			Scope::HashmapEntry& operator*() const;
		private:
			SliceIterator(Attributed& owner, IteratorModes mode);
			IteratorModes _mode{ IteratorModes::All };
			Vector<Scope::HashmapEntry>* _vector{ nullptr };
			std::size_t _index{ 0 };
			std::size_t _begin{ 0 };
			std::size_t _end{ _vector->Size() };
		};*/

		RTTI_DECLARATIONS(Attributed, Scope);
	public:

		/// <summary>
		/// Checks if the key belongs to an attribute of the object
		/// </summary>
		/// <param name="key">Attribute key to search for</param>
		/// <returns>True if key is present in the scope, false otherwise</returns>
		bool IsAttribute(const std::string& key) const;

		/// <summary>
		/// Checks if the key belongs to a prescribed attribute of the object
		/// </summary>
		/// <param name="key">Attribute key to search for</param>
		/// <returns>True if key belongs to a prescribed attribute, false otherwise</returns>
		bool IsPrescribedAttribute(const std::string& key) const;

		/// <summary>
		/// Checks if the key belongs to a auxillary attribute of the object
		/// </summary>
		/// <param name="key">Attribute key to search for</param>
		/// <returns>True if key belongs to a auxillary attribute, false otherwise</returns>
		bool IsAuxillaryAttribute(const std::string& key) const;

		/// <summary>
		/// Appends an auxillary attribute to the object with the given key
		/// </summary>
		/// <param name="key">Key to use for the attribute</param>
		/// <returns>Reference to the datum where the auxillary attribute is located</returns>
		/// <exception cref="std::invalid_argument">
		/// Throws when key is an empty string
		/// </exception>
		Datum& AppendAuxillaryAttribute(const std::string& key);

		/*typename Attributed::SliceIterator Attributes();
		typename Attributed::SliceIterator PrescribedAttributes();
		typename Attributed::SliceIterator AuxillaryAttributes();*/

	protected:
		explicit Attributed(RTTI::IdType typeID);
		Attributed(const Attributed& other);
		Attributed& operator=(const Attributed& other);
		Attributed(Attributed&& other) noexcept;
		Attributed& operator=(Attributed&& other) noexcept;
		virtual ~Attributed() = default;

	private:
		void Populate(const Vector<Signature>& signatures);
		void UpdateExternalStorage(const Vector<Signature>& signatures);
		std::size_t _auxStart;
	};

}
