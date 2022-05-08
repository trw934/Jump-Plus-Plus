#pragma once

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <cassert>
#include <functional>

#include "DefaultHash.inl"
#include "SList.h"
#include "Vector.h"

namespace FieaGameEngine
{
	/// <summary>
	/// A hashmap implementation
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename TKey, typename TData>
	class Hashmap final
	{
	public:
		using PairType = std::pair<const TKey,TData>;
		using ChainType = SList<PairType>;
		using BucketType = Vector<ChainType>;
		using ChainIterator = typename ChainType::Iterator;
		using ChainConstIterator = typename ChainType::ConstIterator;
		using HashFunctor = std::function<size_t(const TKey&)>;
		using EqualityFunctor = std::function<bool(const TKey&, const TKey&)>;


		/// <summary>
		/// An iterator implementation for hashmaps
		/// </summary>
		class Iterator
		{
			friend Hashmap;
		public:
			Iterator() = default;
			Iterator(const Iterator& other) = default;
			Iterator& operator=(const Iterator& other) = default;
			~Iterator() = default;

			/// <summary>
			/// Compares the iterator to the iterator passed in
			/// </summary>
			/// <param name="other"> Iterator to be compared to</param>
			/// <returns> true if the iterators reference the same node, owner, and chainIter, false otherwise</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// Compares the iterator to the iterator passed in
			/// </summary>
			/// <param name="other"> Iterator to be compared to</param>
			/// <returns> false if the iterators reference the same node, owner, and chainIter, true otherwise</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Points the iterator to the next occupied element of the hashmap
			/// </summary>
			/// <returns> A reference to the iterator after it increments</returns>
			Iterator& operator++();

			/// <summary>
			/// Points the iterator to the next occupied element of the hashmap
			/// </summary>
			/// <returns> A reference to the iterator before it increments</returns>
			Iterator operator++(int);

			/// <summary>
			/// Returns a reference to the data pointed to by the iterator
			/// </summary>
			/// <returns>A reference to the data pointed to by the iteratorr</returns>
			PairType& operator*() const;

			/// <summary>
			/// Returns a pointer to the data pointed to by the iterator
			/// </summary>
			/// <returns>A pointer to the data pointed to by the iterator</returns>
			PairType* operator->() const;

		private:
			Iterator(Hashmap& owner, std::size_t index);
			Iterator(Hashmap& owner, std::size_t index, ChainIterator chainIter);
			std::size_t _index{ 0 };
			Hashmap* _owner{ nullptr };
			ChainIterator _chainIter;
		};

		/// <summary>
		/// An iterator implementation for constant vectors
		/// </summary>
		class ConstIterator
		{
			friend Hashmap;
		public:
			/// <summary>
			/// Constructs a ConstIterator based on an existing iterator
			/// </summary>
			/// <param name="other">reference to a const iterator</param>
			ConstIterator(const Iterator& other);
			ConstIterator() = default;
			ConstIterator(const ConstIterator& other) = default;
			ConstIterator& operator=(const ConstIterator& other) = default;
			~ConstIterator() = default;

			/// <summary>
			/// Compares the iterator to the iterator passed in
			/// </summary>
			/// <param name="other"> Iterator to be compared to</param>
			/// <returns> true if the iterators reference the same node, owner, and chainIter, false otherwise</returns>
			bool operator==(const ConstIterator& other) const;

			/// <summary>
			/// Compares the iterator to the iterator passed in
			/// </summary>
			/// <param name="other"> Iterator to be compared to</param>
			/// <returns> false if the iterators reference the same node, owner, and chainIter, true otherwise</returns>
			bool operator!=(const ConstIterator& other) const;

			/// <summary>
			/// Points the iterator to the next occupied element of the hashmap
			/// </summary>
			/// <returns> A reference to the iterator after it increments</returns>
			ConstIterator& operator++();

			/// <summary>
			/// Points the iterator to the next occupied element of the hashmap
			/// </summary>
			/// <returns> A reference to the iterator before it increments</returns>
			ConstIterator operator++(int);

			/// <summary>
			/// Returns a reference to the data pointed to by the iterator
			/// </summary>
			/// <returns>A reference to the data pointed to by the iterator</returns>
			const PairType& operator*() const;

			/// <summary>
			/// Returns a pointer to the data pointed to by the iterator
			/// </summary>
			/// <returns>A pointer to the data pointed to by the iterator</returns>
			const PairType* operator->() const;

		private:
			ConstIterator(const Hashmap& owner, std::size_t index);
			ConstIterator(const Hashmap& owner, std::size_t index, ChainConstIterator chainIter);
			std::size_t _index{ 0 };
			const Hashmap* _owner{ nullptr };
			ChainConstIterator _chainIter;
		};

		/// <summary>
		/// Returns an iterator that points to the first occupied element of the hashmap. If the hashmap is empty, iterator will point to end of hashmap
		/// </summary>
		/// <returns>An iterator that points to the first occupied element of the hashmap.</returns>
		Iterator begin();

		/// <summary>
		/// Returns a ConstIterator that points to the first occupied element of the hashmap. If the hashmap is empty, iterator will point to end of hashmap
		/// </summary>
		/// <returns>A ConstIterator that points to the first occupied element of the hashmap.</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Returns a ConstIterator that points to the first occupied element of the hashmap. If the hashmap is empty, iterator will point to end of hashmap
		/// </summary>
		/// <returns>A ConstIterator that points to the first occupied element of the hashmap.</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Returns an iterator that points past the end of the hashmap
		/// </summary>
		/// <returns>An iterator that points past the end of the hashmap</returns>
		Iterator end();

		/// <summary>
		/// Returns a ConstIterator that points past the end of the hashmap
		/// </summary>
		/// <returns>A ConstIterator that points past the end of the hashmap</returns>
		ConstIterator end() const;

		/// <summary>
		/// Returns a ConstIterator that points past the end of the hashmap
		/// </summary>
		/// <returns>A ConstIterator that points past the end of the hashmap</returns>
		ConstIterator cend() const;


		/// <summary>
		/// Returns an Iterator that points to a given item in the hashmap. If the item is not in the list, the Iterator will point past the end of the vector.
		/// </summary>
		/// <param name="data">A reference to the key to search for</param>
		/// <param name="equalityFunc">A function used to compare the data used to determine when the desired data is found</param>
		/// <returns>An Iterator that points to a given item in the hashmap</returns>
		Iterator Find(const TKey& key);

		/// <summary>
		/// Returns an ConstIterator that points to a given item in the hashmap. If the item is not in the list, the ConstIterator will point past the end of the vector.
		/// </summary>
		/// <param name="data">A reference to the key to search for</param>
		/// <param name="equalityFunc">A function used to compare the data used to determine when the desired data is found</param>
		/// <returns>An ConstIterator that points to a given item in the hashmap</returns>
		ConstIterator Find(const TKey& key) const;

	
		/// <summary>
		/// Removes the pair associated with the key from the hashmap. If the key does not exist, the hashmap is not modified.
		/// </summary>
		/// <param name="key">Key of pair to be removed</param>
		/// <returns>true if removal is successful, false otherwise</returns>
		bool Remove(const TKey& key);

		/// <summary>
		/// Constructs a new hashmap with a specified number of buckets, hash function, and equality function
		/// </summary>
		/// <param name="bucketSize">number of buckets to include in hashmap</param>
		explicit Hashmap(size_t bucketSize = 11);

		/// <summary>
		/// Constructs a new hashmap with given number of buckets, hash function, and equality function
		/// </summary>
		/// <param name="bucketSize">Desired number of buckets. Mush be greater than 1</param>
		/// <param name="hashFunc">Desired hash function</param>
		/// <param name="equalityFunc">Desired equality function</param>
		/// <exception cref="std::runtime_error"> Throws if bucketSize is less than 2</exception>
		Hashmap(size_t bucketSize, HashFunctor hashFunc, EqualityFunctor equalityFunc);

		/// <summary>
		/// Constructs a new hashmap with a default number of buckets, hash function, and equality function, then inserts the provided pairs
		/// </summary>
		/// <param name="list">List of pairs to insert</param>
		Hashmap(std::initializer_list<PairType> list);

		/// <summary>
		/// Constructs a new hashmap with given number of buckets, hash function, and equality function, then inserts the provided pairs
		/// </summary>
		/// <param name="list">List of pairs to insert</param>
		/// <param name="bucketSize">Desired number of buckets. Mush be greater than 1</param>
		/// <param name="hashFunc">Desired hash function</param>
		/// <param name="equalityFunc">Desired equality function</param>
		/// <exception cref="std::runtime_error"> Throws if bucketSize is less than 2</exception>
		Hashmap(std::initializer_list<PairType> list, size_t bucketSize, HashFunctor hashFunc, EqualityFunctor equalityFunc);

		/// <summary>
		/// Copy constructor for hashmap
		/// </summary>
		/// <param name="other">Hashmap to be copied</param>
		Hashmap(const Hashmap& other) = default;

		/// <summary>
		/// Copy assignment operator for hashmap
		/// </summary>
		/// <param name="other">Hashmap to be copied</param>
		/// <returns>Reference to copied hashmap</returns>
		Hashmap& operator=(const Hashmap& other) = default;

		/// <summary>
		/// Move constructor for hashmap. Moved objects shouldn't be reused
		/// </summary>
		/// <param name="other">Hashmap to move</param>
		Hashmap(Hashmap&& other) = default;

		/// <summary>
		/// Move assignment operator for hashmap. Moved objects shouldn't be reused.
		/// </summary>
		/// <param name="other">Hashmap to move</param>
		/// <returns>Reference to moved hashmap</returns>
		Hashmap& operator=(Hashmap&& other) = default;

		/// <summary>
		/// Destroys a hashmap
		/// </summary>
		~Hashmap() = default;

		/// <summary>
		/// Checks if the hashmap contains the desired key
		/// </summary>
		/// <param name="key">Key to search for</param>
		/// <returns>true if hashmap contains the key, false otherwise</returns>
		bool ContainsKey(const TKey& key) const;

		/// <summary>
		/// Inserts the desired pair into the hashmap and returns an iterator to the inserted pair. If the key already exists, the iterator will point to that pair without modifying it.
		/// </summary>
		/// <param name="pair">Pair to be inserted</param>
		/// <returns>pair of <Iterator, bool>. Iterator will point to inserted pair or preexisting pair. bool is true if insertion was successful, false if key already existed in hashmap.</returns>
		std::pair<Iterator, bool> Insert(const PairType& pair);


		/// <summary>
		/// Returns a reference to the data paired with the entered key. If the key does not exist, inserts a new pair with the entered key and a default constructed data.
		/// </summary>
		/// <param name="key">Key to search for</param>
		/// <returns>A reference to the data associated with the key</returns>
		TData& operator[](const TKey& key);

		/// <summary>
		/// Returns a reference to the data paired with the entered key
		/// </summary>
		/// <param name="key">Key to search for</param>
		/// <exception cref="std::runtime_error"> Throws if key is not present in hashmap</exception>
		/// <returns>A reference to the data associated with the key</returns>
		TData& At(const TKey& key);

		/// <summary>
		/// Returns a reference to the data paired with the entered key
		/// </summary>
		/// <param name="key">Key to search for</param>
		/// <exception cref="std::runtime_error"> Throws if key is not present in hashmap</exception>
		/// <returns>A reference to the data associated with the key</returns>
		const TData& At(const TKey& key) const;

		/// <summary>
		/// Returns the number of items in the hashmap
		/// </summary>
		/// <returns> A size_t representing the number of items in the hashmap</returns>
		std::size_t Size() const;

		/// <summary>
		/// Returns the number of buckets in the hashmap
		/// </summary>
		/// <returns>A size_t representing the number of buckets in the hashmap</returns>
		std::size_t BucketSize() const;

		/// <summary>
		/// Removes all items in the hashmap, deleting any memory allocated as well.
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns a ratio of the size of the hashmap to the number of buckets.
		/// </summary>
		/// <returns>a ratio of the size of the hashmap to the number of buckets.</returns>
		float GetLoadFactor() const;

		/// <summary>
		/// Rehashes the current hashmap with a different number of buckets.
		/// </summary>
		/// <param name="capacity">New number of buckets</param>
		/// <exception cref="std::runtime_error"> Throws if capacity is less than 2</exception>
		void ResizeHash(size_t capacity);

	private:
		std::pair<bool, size_t> ContainsKeyReturnHash(const TKey & key) const;
		std::pair<Iterator, size_t> FindReturnHash(const TKey & key);
		std::size_t _size{ 0 };
		std::size_t _bucketSize{ 11 };
		HashFunctor _hashFunc{ DefaultHash<TKey>() };
		EqualityFunctor _equalityFunc{ std::equal_to<>() };
		BucketType _hashmap;
	};

}

#include "Hashmap.inl"