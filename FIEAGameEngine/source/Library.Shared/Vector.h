#pragma once

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <cassert>
#include <functional>
#include <algorithm>

namespace FieaGameEngine
{
	/// <summary>
	/// A vector implementation
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	class Vector final
	{
	private:

	public:
		/// <summary>
		/// An iterator implementation for vectors
		/// </summary>
		class Iterator
		{
			friend Vector;
		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using pointer = T*;
			using iterator_category = std::bidirectional_iterator_tag;

			Iterator() = default;
			Iterator(const Iterator& other) = default;
			Iterator& operator=(const Iterator& other) = default;
			~Iterator() = default;

			/// <summary>
			/// Compares the iterator to the iterator passed in
			/// </summary>
			/// <param name="other"> Iterator to be compared to</param>
			/// <returns> true if the iterators reference the same node and owner, false otherwise</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// Compares the iterator to the iterator passed in
			/// </summary>
			/// <param name="other"> Iterator to be compared to</param>
			/// <returns> false if the iterators reference the same node and owner, true otherwise</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Compares the iterator to the iterator passed in
			/// </summary>
			/// <param name="other"> Iterator to be compared to</param>
			/// <returns> true if the current iterator's index is less than the other iterator's index, false otherwise</returns>
			bool operator<(const Iterator& other) const;

			/// <summary>
			/// Increments the iterator's index by 1
			/// </summary>
			/// <returns> A reference to the iterator after it increments</returns>
			Iterator& operator++();

			/// <summary>
			/// Increments the iterator's index by 1
			/// </summary>
			/// <returns> A reference to the iterator before it increments</returns>
			Iterator operator++(int);

			/// <summary>
			/// Decrements the iterator's index by 1
			/// </summary>
			/// <returns> A reference to the iterator after it increments</returns>
			Iterator& operator--();

			/// <summary>
			/// Decrements the iterator's index by 1
			/// </summary>
			/// <returns> A reference to the iterator before it increments</returns>
			Iterator operator--(int);

			/// <summary>
			/// Returns a reference to the data contained at that index of the vector
			/// </summary>
			/// <returns>A reference to the data contained at that index of the vector</returns>
			T& operator*() const;

		private:
			Iterator(const Vector& owner, std::size_t index);
			std::size_t _index{ 0 };
			const Vector* _owner{ nullptr };
		};

		/// <summary>
		/// An iterator implementation for constant vectors
		/// </summary>
		class ConstIterator
		{
			friend Vector;
		public:
			/// <summary>
			/// Constructs a ConstIterator based on an existing iterator
			/// </summary>
			/// <param name="other">reference to a const iterator</param>
			ConstIterator(const Iterator& other);
			ConstIterator() = default;
			ConstIterator(const ConstIterator & other) = default;
			ConstIterator& operator=(const ConstIterator & other) = default;
			~ConstIterator() = default;

			/// <summary>
			/// Compares the iterator to the iterator passed in
			/// </summary>
			/// <param name="other"> Iterator to be compared to</param>
			/// <returns> true if the iterators reference the same node and owner, false otherwise</returns>
			bool operator==(const ConstIterator& other) const;

			/// <summary>
			/// Compares the iterator to the iterator passed in
			/// </summary>
			/// <param name="other"> Iterator to be compared to</param>
			/// <returns> false if the iterators reference the same node and owner, true otherwise</returns>
			bool operator!=(const ConstIterator & other) const;

			/// <summary>
			/// Increments the iterator's index by 1
			/// </summary>
			/// <returns> A reference to the iterator after it increments</returns>
			ConstIterator& operator++();

			/// <summary>
			/// Increments the iterator's index by 1
			/// </summary>
			/// <returns> A reference to the iterator before it increments</returns>
			ConstIterator operator++(int);

			/// <summary>
			/// Returns a reference to the data contained at that index of the vector
			/// </summary>
			/// <returns>A reference to the data contained at that index of the vector</returns>
			const T& operator*() const;

		private:
			ConstIterator(const Vector& owner, std::size_t index);
			std::size_t _index{ 0 };
			const Vector* _owner{ nullptr };
		};

		/// <summary>
		/// Returns an iterator that points to the head of the vector
		/// </summary>
		/// <returns>An iterator that points to the head of the vector</returns>
		Iterator begin();

		/// <summary>
		/// Returns a ConstIterator that points to the head of the vector
		/// </summary>
		/// <returns>A ConstIterator that points to the head of the vector</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Returns a ConstIterator that points to the head of the vector
		/// </summary>
		/// <returns>A ConstIterator that points to the head of the vector</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Returns an iterator that points past the end of the vector
		/// </summary>
		/// <returns>An iterator that points past the end of the vector</returns>
		Iterator end();

		/// <summary>
		/// Returns a ConstIterator that points past the end of the vector
		/// </summary>
		/// <returns>A ConstIterator that points past the end of the vector</returns>
		ConstIterator end() const;

		/// <summary>
		/// Returns a ConstIterator that points past the end of the vector
		/// </summary>
		/// <returns>A ConstIterator that points past the end of the vector</returns>
		ConstIterator cend() const;


		/// <summary>
		/// Returns an Iterator that points to a given item in the vector. If the item is not in the list, the Iterator will point past the end of the vector.
		/// </summary>
		/// <param name="data">A reference to the data to search for</param>
		/// <param name="equalityFunc">A function used to compare the data used to determine when the desired data is found</param>
		/// <returns>An Iterator that points to a given item in the vector</returns>
		Iterator Find(const T& data, std::function<bool(const T&, const T&)> equalityFunc = std::equal_to<T>());

		/// <summary>
		/// Returns an ConstIterator that points to a given item in the vector. If the item is not in the list, the ConstIterator will point past the end of the vector.
		/// </summary>
		/// <param name="data">A reference to the data to search for</param>
		/// <param name="equalityFunc">A function used to compare the data used to determine when the desired data is found</param>
		/// <returns>An ConstIterator that points to a given item in the vector</returns>
		ConstIterator Find(const T& data, std::function<bool(const T&, const T&)> equalityFunc = std::equal_to<T>()) const;

		/// <summary>
		/// Removes the value from the vector at the given index. The contents of the array will shift down an index to fill the gap left by deleting the element
		/// </summary>
		/// <param name="index">An index to the value to be removed</param>
		/// <returns>true if removal was successful, false otherwise</returns>
		bool RemoveAt(size_t index);

		/// <summary>
		/// Removes the first occurrence of the item associated with the given data from the vector. If the item doesn't exist in the list, the method returns false.
		/// </summary>
		/// <param name="data">The data to be removed</param>
		/// <param name="equalityFunc">A function used to compare the data used to determine when the data is found</param>
		/// <returns>true if removal was successful, false otherwise</returns>
		bool Remove(const T& data, std::function<bool(const T&, const T&)> equalityFunc = std::equal_to<T>());

		/// <summary>
		/// Removes the item at the index of the iterator from the vector
		/// </summary>
		/// <param name="iter">The iterator to remove the item at</param>
		/// <returns>true if removal was successful, false otherwise</returns>
		bool Remove(const Iterator& iter);

		/// <summary>
		/// Removes the items between the indexes of the startIter and endIter, including the element startIter points to but not endIter.
		/// </summary>
		/// <param name="startIter">start of the range to be removed</param>
		/// <param name="endIter">end of the range to be removed</param>
		/// <returns>true if removal was successfule, false otherwise</returns>
		void Remove(const Iterator& startIter, const Iterator& endIter);

		/// <summary>
		/// Instantiates an empty vector with specific capacity
		/// </summary>
		/// <param name="capacity">Desired capacity</param>
		explicit Vector(std::size_t capacity = 3);

		/// <summary>
		/// Instantiates a vector from a list of data
		/// </summary>
		/// <param name="vector">list of data to create the vector from</param>
		Vector(std::initializer_list<T> vector);

		/// <summary>
		/// Instantiates a vector from an existing vector
		/// </summary>
		/// <param name="other">A reference to the vector to copy over</param>
		Vector(const Vector& other);

		/// <summary>
		/// Instantiates a vector from an existing vector
		/// </summary>
		/// <param name="other">A reference to the vector to copy over</param>
		/// <returns>A reference to the created vector</returns>
		Vector& operator=(const Vector& other);

		/// <summary>
		/// Instantiates a vector from an existing vector, moving the contents over from the other vector
		/// </summary>
		/// <param name="other"> A reference to the vector to move over</param>
		Vector(Vector&& other);

		/// <summary>
		/// Instantiates a vector from an existing vector, moving the contents over from the other vector
		/// </summary>
		/// <param name="other"> A reference to the vector to move over</param>
		/// <returns>A reference to the created vector</returns>
		Vector& operator=(Vector&& other);

		/// <summary>
		/// Destroys a vector
		/// </summary>
		~Vector();

		/// <summary>
		/// Adds more capacity to the vector. The new capacity must be larger than the current capacity
		/// </summary>
		/// <param name="capacity">The desired capacity</param>
		void Reserve(std::size_t capacity);

		/// <summary>
		/// Resizes the vector to the desired size, default constructing elements if increasing size and destructing elements if
		/// decreasing size. The capacity will be equivalent to the size after calling this method.
		/// </summary>
		/// <param name="size">The desired size</param>
		void Resize(std::size_t size);

		/// <summary>
		/// Lowers the capacity of the vector to be equivalent to the current size.
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Returns a reference to the data at the specified index of the vector
		/// </summary>
		/// <param name="index">The index to get the data from</param>
		/// <returns>A reference to the data at the specified index</returns>
		T& operator[](std::size_t index);

		/// <summary>
		/// Returns a reference to the data at the specified index of the vector
		/// </summary>
		/// <param name="index">The index to get the data from</param>
		/// <returns>A reference to the data at the specified index</returns>
		const T& operator[](std::size_t index) const;

		/// <summary>
		/// Returns a reference to the data at the specified index of the vector
		/// </summary>
		/// <param name="index">The index to get the data from</param>
		/// <returns>A reference to the data at the specified index</returns>
		T& At(std::size_t index);

		/// <summary>
		/// Returns a reference to the data at the specified index of the vector
		/// </summary>
		/// <param name="index">The index to get the data from</param>
		/// <returns>A reference to the data at the specified index</returns>
		const T& At(std::size_t index) const;

		/// <summary>
		/// Appends given data at the end of the vector. Doubles vector capacity if the new size would exceed original capacity
		/// </summary>
		/// <param name="data">const reference to the data</param>
		void PushBack(const T& data);

		/// <summary>
		/// Removes the last item of the vector
		/// </summary>
		/// <returns>True if the operation is successful, false otherwise</returns>
		bool PopBack();

		/// <summary>
		/// Returns the item at the front of the vector
		/// </summary>
		/// <exception cref="std::runtime_error"> Throws when the vector is empty.</exception>
		/// <returns>A reference to the data stored at the front of the vector.</returns>
		T& Front();

		/// <summary>
		/// Returns the item at the front of the vector
		/// </summary>
		/// <exception cref="std::runtime_error"> Throws when the vector is empty.</exception>
		/// <returns>A reference to the data stored at the front of the vector.</returns>
		const T& Front() const;

		/// <summary>
		/// Returns the item at the back of the vector
		/// </summary>
		/// <exception cref="std::runtime_error"> Throws when the vector is empty.</exception>
		/// <returns>A reference to the data stored at the back of the vector.</returns>
		T& Back();

		/// <summary>
		/// Returns the item at the back of the vector
		/// </summary>
		/// <exception cref="std::runtime_error"> Throws when the vector is empty.</exception>
		/// <returns>A reference to the data stored at the back of the vector.</returns>
		const T& Back() const;

		/// <summary>
		/// Returns the number of items in the vector
		/// </summary>
		/// <returns> A const reference to a size_t representing the number of items in the vector</returns>
		std::size_t Size() const;

		/// <summary>
		/// Returns the capacity of the vector
		/// </summary>
		/// <returns> A size_t representing the capacity of the vector</returns>
		std::size_t Capacity() const;

		/// <summary>
		/// Returns whether or not the vector is empty
		/// </summary>
		/// <returns> A boolean which is true if the list is empty</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Removes all items in the vector
		/// </summary>
		void Clear();

	private:
		std::size_t _size{ 0 };
		std::size_t _capacity{ 0 };
		T* _head{ nullptr };
	};
}

#include "Vector.inl"