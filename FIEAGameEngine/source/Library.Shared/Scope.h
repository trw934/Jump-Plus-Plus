#pragma once
#include "Datum.h"
#include "Hashmap.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class Scope : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI);

	public:
		using ScopeHashmap = Hashmap<std::string, Datum>;
		using HashmapEntry = Hashmap<std::string, Datum>::PairType*;

		/// <summary>
		/// Constructs an empty scope using the parameters given
		/// </summary>
		/// <param name="capacity">Capacity of order vector</param>
		/// <param name="bucketSize">Number of buckets for hashmap</param>
		explicit Scope(size_t capacity = 11, size_t bucketSize = 11);

		/// <summary>
		/// Instantiates a scope using an existing scope, performing a deep copy
		/// </summary>
		/// <param name="other">Reference to the scope to be copied from</param>
		Scope(const Scope& other);

		/// <summary>
		/// Instantiates a scope using an existing scope, performing a deep copy
		/// </summary>
		/// <param name="other">Reference to the scope to be copied from</param>
		/// <returns>Reference to the instantiated scope</returns>
		Scope& operator=(const Scope& other);

		/// <summary>
		/// Instantiates a scope using an existing scope, moving the contents over from the other scope
		/// </summary>
		/// <param name="other">Reference to the scope to move over</param>
		Scope(Scope&& other) noexcept;

		/// <summary>
		/// Instantiates a scope using an existing scope, moving the contents over from the other scope
		/// </summary>
		/// <param name="other">Reference to the scope to move over</param>
		/// <returns>Reference to the instantiated scope</returns>
		Scope& operator=(Scope&& other) noexcept;

		/// <summary>
		/// Destructs a scope, orphaning it from its parent if it has one first
		/// </summary>
		virtual ~Scope();

		/// <summary>
		/// Performs a local search of the scope to find the datum associated with the key
		/// </summary>
		/// <param name="key">Key to search for</param>
		/// <returns>Pointer to the datum if found, nullptr otherwise</returns>
		Datum* Find(const std::string& key);

		/// <summary>
		/// Performs a local search of the scope to find the datum associated with the key
		/// </summary>
		/// <param name="key">Key to search for</param>
		/// <returns>Pointer to the const datum if found, nullptr otherwise</returns>
		const Datum* Find(const std::string& key) const;

		/// <summary>
		/// Performs a search in the current scope as well as parent scopes to find the datum associated with the key.
		/// </summary>
		/// <param name="key">Key to search for</param>
		/// <returns>Pointer to the datum if found, nullptr otherwise</returns>
		Datum* Search(const std::string& key);

		/// <summary>
		/// Performs a search in the current scope as well as parent scopes to find the datum associated with the key.
		/// </summary>
		/// <param name="key">Key to search for</param>
		/// <returns>Pointer to the const datum if found, nullptr otherwise</returns>
		const Datum* Search(const std::string& key) const;

		/// <summary>
		/// Performs a search in the current scope as well as parent scopes to find the datum associated with the key.
		/// </summary>
		/// <param name="key">Key to search for</param>
		/// <param name="foundScope">Scope where datum was found</param>
		/// <returns>Pointer to datum if found, nullptr otherwise</returns>
		Datum* Search(const std::string& key, Scope*& foundScope);

		/// <summary>
		/// Performs a search in the current scope as well as parent scopes to find the datum associated with the key.
		/// </summary>
		/// <param name="key">Key to search for</param>
		/// <param name="foundScope">Scope where datum was found</param>
		/// <returns>Pointer to const datum if found, nullptr otherwise</returns>
		const Datum* Search(const std::string& key, const Scope*& foundScope) const;

		Datum* GlobalSearch(const std::string& datumString);

		/// <summary>
		/// Adds a defaultly constructed datum into the scope associated with the key entered and returns a reference to that datum.
		/// </summary>
		/// <param name="key">Key to be entered into the scope</param>
		/// <returns>Reference to the datum associated with the key</returns>
		/// <exception cref="std::invalid_argument">
		/// Throws when key is an empty string
		/// </exception>
		Datum& Append(const std::string& key);

		/// <summary>
		/// Adds a defaultly constructed scope datum into the scope associated with the key entered and returns a reference to that scope
		/// </summary>
		/// <param name="key">Key to be entered</param>
		/// <returns>Reference to the created scope</returns>
		/// <exception cref="std::runtime_error">
		/// Throws if key points to a datum whose type is not Unknown or Table
		/// </exception>
		/// <exception cref="std::invalid_argument">
		/// Throws when key is an empty string
		/// </exception>
		Scope& AppendScope(const std::string& key);

		/// <summary>
		/// Takes the scope passed in and makes it a child of the current scope, at the datum specified with the key.
		/// If the scope is a ancestor of or is the current scope, this method does nothing
		/// </summary>
		/// <param name="scope">Scope to be adopted</param>
		/// <param name="key">Key to use for adopted scope</param>
		/// <exception cref="std::invalid_argument">
		/// Throws when key is an empty string
		/// </exception>
		virtual void Adopt(Scope& scope, const std::string& key);

		/// <summary>
		/// Returns a pointer to the parent of the scope
		/// </summary>
		/// <returns>Pointer to the parent of the scope, or nullptr if the scope doesn't have a parent</returns>
		Scope* GetParent() const;

		template <typename T>
		T& GetParent() const;

		/// <summary>
		/// Adds a defaultly constructed datum into the scope associated with the key entered and returns a reference to that datum.
		/// </summary>
		/// <param name="key">Key to be entered into the scope</param>
		/// <returns>Reference to the datum associated with the key</returns>
		Datum& operator[](const std::string& key);

		/// <summary>
		/// Returns the datum at the index in the order vector
		/// </summary>
		/// <param name="index">Index of the order vector to access</param>
		/// <returns>Reference to the found datum</returns>
		/// <exception cref="std::out_of_range">
		/// Throws when index is out of range of vector
		/// </exception>
		Datum& operator[](std::size_t index);

		/// <summary>
		/// Checks if the contents of the two scopes are equivalent
		/// </summary>
		/// <param name="other">Scope to be compared to this scope</param>
		/// <returns>True if contents are equivalent, false otherwise</returns>
		bool operator==(const Scope& other) const;

		/// <summary>
		/// Checks if the contents of the two scopes are equivalent
		/// </summary>
		/// <param name="other">Scope to be compared to this scope</param>
		/// <returns>False if contents are equivalent, true otherwise</returns>
		bool operator!=(const Scope& other) const;

		/// <summary>
		/// Clears the contents of the scope and any nested scopes it contains
		/// </summary>
		void Clear();

		/// <summary>
		/// Compares the contents of the scope to the object pointed to by the RTTI pointer, if it is of type Scope
		/// </summary>
		/// <param name="other">RTTI* to be compared to this scope</param>
		/// <returns>True if contents are equivalent, false otherwise or if RTTI* is not of type Scope</returns>
		bool Equals(const RTTI* other) const override;

		/// <summary>
		/// Checks to see if the entered scope is a descendant of this scope
		/// </summary>
		/// <param name="scope">Scope to be checked against</param>
		/// <returns>True if entered scope is a descendant of this child, false otherwise</returns>
		bool IsAncestorOf(const Scope& scope) const;

		/// <summary>
		/// Checks to see if the entered scope is a ancestor of this scope
		/// </summary>
		/// <param name="scope">Scope to be checked against</param>
		/// <returns>True if entered scope is a ancestor of this child, false otherwise</returns>
		bool IsDescendantOf(const Scope& scope) const;

		/// <summary>
		/// Checks the scope to see if it contains the scope entered
		/// </summary>
		/// <param name="scope">Scope to search for</param>
		/// <returns>
		/// Pair of a datum pointer and a size_t. The pointer will point to the datum of the found scope, or nullptr otherwise.
		/// The size_t will be the index into the datum that the scope is located at.
		/// </returns>
		std::pair<Datum*, std::size_t> FindNestedScope(const Scope* scope) const;

		/// <summary>
		/// Returns the number of elements in the scope
		/// </summary>
		/// <returns> Size_t representing the number of elements in the scope</returns>
		std::size_t Size() const;

	protected:
		
		virtual gsl::owner<Scope*> Clone() const;
		void Orphan(Scope& scope);

		Scope* _parent{ nullptr };
		ScopeHashmap _map;
		Vector<HashmapEntry> _orderVector;

	private:
		void CopyHelper(const Scope& other);
		void MoveHelper() noexcept;
		Datum& Append(const std::string& key, bool& wasInserted);
	};

	ConcreteFactory(Scope, Scope);
}

#include "Scope.inl"