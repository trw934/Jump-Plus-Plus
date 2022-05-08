#pragma once

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <functional>

namespace FieaGameEngine
{
	/// <summary>
	/// A singly-linked list
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	class SList final
	{
	private:
		struct Node final
		{
			T Data;
			Node* Next;

			Node(const T& data, Node* next = nullptr);
		};
		
	public:
		/// <summary>
		/// An iterator implementation for linked lists
		/// </summary>
		class Iterator
		{
			friend SList;
			friend class ConstIterator;
		public:
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
			/// Increments the iterator to point to the next node in the list
			/// </summary>
			/// <returns> A reference to the iterator after it increments</returns>
			Iterator& operator++();

			/// <summary>
			/// Increments the iterator to point to the next node in the list
			/// </summary>
			/// <returns> A reference to the iterator before it increments</returns>
			Iterator operator++(int);

			/// <summary>
			/// Returns a reference to the data contained in the node of the iterator
			/// </summary>
			/// <returns>A reference to the data contained in the node of the iterator</returns>
			T& operator*() const;

		private:
			Iterator(const SList& owner, Node* node = nullptr);
			Node* _node{ nullptr };
			const SList* _owner{ nullptr };
		};

		/// <summary>
		/// An iterator implementation for constant linked lists
		/// </summary>
		class ConstIterator
		{
			friend SList;
		public:
			/// <summary>
			/// Constructs a ConstIterator based on an existing iterator
			/// </summary>
			/// <param name="other">reference to a const iterator</param>
			ConstIterator(const Iterator& other);
			ConstIterator() = default;
			ConstIterator(const ConstIterator& other) = default;
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
			bool operator!=(const ConstIterator& other) const;

			/// <summary>
			/// Increments the iterator to point to the next node in the list
			/// </summary>
			/// <returns> A reference to the iterator after it increments</returns>
			ConstIterator& operator++();

			/// <summary>
			/// Increments the iterator to point to the next node in the list
			/// </summary>
			/// <returns> A reference to the iterator before it increments</returns>
			ConstIterator operator++(int);

			/// <summary>
			/// Returns a reference to the data contained in the node of the iterator
			/// </summary>
			/// <returns>A reference to the data contained in the node of the iterator</returns>
			const T& operator*() const;

		private:
			ConstIterator(const SList& owner, Node* node = nullptr);
			Node* _node{ nullptr };
			const SList* _owner{ nullptr };
		};

		/// <summary>
		/// Returns an iterator that points to the head of the list
		/// </summary>
		/// <returns>An iterator that points to the head of the list</returns>
		Iterator begin();

		/// <summary>
		/// Returns a ConstIterator that points to the head of the list
		/// </summary>
		/// <returns>A ConstIterator that points to the head of the list</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Returns a ConstIterator that points to the head of the list
		/// </summary>
		/// <returns>A ConstIterator that points to the head of the list</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Returns an iterator that points past the end of the list
		/// </summary>
		/// <returns>An iterator that points past the end of the list</returns>
		Iterator end();

		/// <summary>
		/// Returns a ConstIterator that points past the end of the list
		/// </summary>
		/// <returns>A ConstIterator that points past the end of the list</returns>
		ConstIterator end() const;

		/// <summary>
		/// Returns a ConstIterator that points past the end of the list
		/// </summary>
		/// <returns>A ConstIterator that points past the end of the list</returns>
		ConstIterator cend() const;

		/// <summary>
		/// Returns an Iterator that points to a given item in the list. If the item is not in the list, the Iterator will point past the end of the list.
		/// </summary>
		/// <param name="data">A reference to the data to search for</param>
		/// <param name="equalityFunc">A function used to compare the data used to determine when the desired node is found</param>
		/// <returns>An Iterator that points to a given item in the list</returns>
		Iterator Find(const T& data, std::function<bool(const T&, const T&)> equalityFunc = std::equal_to<T>());

		/// <summary>
		/// Returns an ConstIterator that points to a given item in the list. If the item is not in the list, the ConstIterator will point past the end of the list.
		/// </summary>
		/// <param name="data">A reference to the data to search for</param>
		/// <param name="equalityFunc">A function used to compare the data used to determine when the desired node is found</param>
		/// <returns>An ConstIterator that points to a given item in the list</returns>
		ConstIterator Find(const T& data, std::function<bool(const T&, const T&)> equalityFunc = std::equal_to<T>()) const;

		/// <summary>
		/// Appends the given data after the item the given iterator points to.
		/// </summary>
		/// <param name="iter">The iterator that the data is to be placed after</param>
		/// <param name="data">The data to be placed in the list</param>
		void InsertAfter(Iterator iter, const T& data);

		/// <summary>
		/// Removes the first occurrence of the item associated with the given data and maintains list integrity. If the item does not exist in the list, this method does nothing.
		/// </summary>
		/// <param name="data">The data to be removed</param>
		/// <param name="equalityFunc">A function used to compare the data used to determine when the desired node is found</param>
		bool Remove(const T& data, std::function<bool(const T&, const T&)> equalityFunc = std::equal_to<T>());

		/// <summary>
		/// Removes the item associated with the iterator.
		/// </summary>
		/// <param name="iter">The iterator to the data to be removed</param>
		bool Remove(Iterator iter);


		/// <summary>
		/// Instantiates an empty linked list
		/// </summary>
		SList() = default;

		/// <summary>
		/// Instantiates an SList from a list of data
		/// </summary>
		/// <param name="list">List of data to create the SList from</param>
		SList(std::initializer_list<T> list);

		/// <summary>
		/// Instantiates an SList from an existing instance
		/// </summary>
		/// <typeparam name="T"></typeparam>
		SList(const SList& other);

		/// <summary>
		/// Instantiates an SList from an existing instance
		/// </summary>
		/// <returns> A reference to the instantiates SList</returns>
		SList& operator=(const SList& other);

		/// <summary>
		/// Destroys a linked list
		/// </summary>
		~SList();

		/// <summary>
		/// Appends data to front of list
		/// </summary>
		/// <param name="data">reference to const T</param>
		/// <returns>Iterator to inserted element</returns>
		Iterator PushFront(const T& data);

		/// <summary>
		/// Appends data to front of list
		/// </summary>
		/// <param name="data">reference to T</param>
		/// <returns>Iterator to inserted element</returns>
		Iterator PushFront(T&& data);

		/// <summary>
		/// Appends data to back of list
		/// </summary>
		/// <param name="data">reference to const T</param>
		/// <returns>Iterator to inserted element</returns>
		Iterator PushBack(const T& data);

		/// <summary>
		/// Appends data to back of list
		/// </summary>
		/// <param name="data">reference to const T</param>
		/// <returns>Iterator to inserted element</returns>
		Iterator PushBack(T&& data);

		/// <summary>
		/// Removes the first item of the list.
		/// </summary>
		/// <returns>True if the operation is successful, false otherwise</returns>
		bool PopFront();

		/// <summary>
		/// Removes the last item of the list.
		/// </summary>
		/// <returns>True if the operation is successful, false otherwise</returns>
		bool PopBack();

		/// <summary>
		/// Returns the item at the front of the list
		/// </summary>
		/// <exception cref="std::runtime_error"> Throws when the list is empty.</exception>
		/// <returns>A reference to the data stored at the front of the list.</returns>
		T& Front();

		/// <summary>
		/// Returns the item at the front of the list
		/// </summary>
		/// <exception cref="std::runtime_error"> Throws when the list is empty.</exception>
		/// <returns>A const reference to the data stored at the front of the list.</returns>
		const T& Front() const;

		/// <summary>
		/// Returns the item at the back of the list
		/// </summary>
		/// <exception cref="std::runtime_error"> Throws when the list is empty.</exception>
		/// <returns>A reference to the data stored at the front of the list.</returns>
		T& Back();

		/// <summary>
		/// Returns the item at the back of the list
		/// </summary>
		/// <exception cref="std::runtime_error"> Throws when the list is empty.</exception>
		/// <returns>A const reference to the data stored at the front of the list.</returns>
		const T& Back() const;

		/// <summary>
		/// Returns the number of items in the list
		/// </summary>
		/// <returns> A size_t representing the number of items in the list</returns>
		std::size_t Size() const;

		/// <summary>
		/// Returns whether or not the list is empty
		/// </summary>
		/// <returns> A boolean which is true if the list is empty</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Removes all items in the list
		/// </summary>
		void Clear();

	private:
		std::size_t _size{ 0 };
		Node* _front{ nullptr };
		Node* _back{ nullptr };
	};
}

#include "SList.inl"