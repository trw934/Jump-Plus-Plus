#include "SList.h"

namespace FieaGameEngine
{
#pragma region SList Iterator
	template <typename T>
	SList<T>::Iterator::Iterator(const SList& owner, Node* node) :
		_owner{ &owner }, _node{ node }
	{
	}

	template <typename T>
	SList<T>::ConstIterator::ConstIterator(const SList& owner, Node* node) :
		_owner{ &owner }, _node{ node }
	{
	}

	template<typename T>
	SList<T>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner{ other._owner }, _node{ other._node }
	{
	}

	template <typename T>
	bool SList<T>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}

	template <typename T>
	bool SList<T>::Iterator::operator!=(const Iterator& other) const
	{
		if (_owner == other._owner)
		{
			return (_node != other._node);
		}
		return true;
	}

	template <typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container.");
		}

		if (_node != nullptr)
		{
			_node = _node->Next;
		}

		return *this;
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		++(*this);
		return temp;
	}

	template <typename T>
	T& SList<T>::Iterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Cannot dereference iterator that points to nullptr.");
		}
		return _node->Data;
	}

	template <typename T>
	bool SList<T>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));
	}

	template <typename T>
	bool SList<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		if (_owner == other._owner)
		{
			return (_node != other._node);
		}
		return true;
	}

	template <typename T>
	typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container.");
		}

		if (_node != nullptr)
		{
			_node = _node->Next;
		}

		return *this;
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		++(*this);
		return temp;
	}

	template <typename T>
	const T& SList<T>::ConstIterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Cannot dereference iterator that points to nullptr.");
		}
		return _node->Data;
	}


	template <typename T>
	inline typename SList<T>::Iterator SList<T>::begin()
	{
		return Iterator{ *this, _front };
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::begin() const
	{
		return ConstIterator{ *this, _front };
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return ConstIterator{ *this, _front };
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::end()
	{
		return Iterator{ *this };
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::end() const
	{
		return ConstIterator{ *this };
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return ConstIterator{ *this };
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& data, std::function<bool(const T&, const T&)> equalityFunc)
	{
		Iterator iter;
		for (iter = begin(); iter != end(); ++iter)
		{
			if (equalityFunc(*iter, data))
			{
				break;
			}
		}

		return iter;
	}

	template <typename T>
	typename SList<T>::ConstIterator SList<T>::Find(const T& data, std::function<bool(const T&, const T&)> equalityFunc) const
	{
		ConstIterator iter;
		for (iter = begin(); iter != end(); ++iter)
		{
			if (equalityFunc(*iter, data))
			{
				break;
			}
		}

		return iter;
	}

	template <typename T>
	void SList<T>::InsertAfter(Iterator iter, const T& data)
	{
		if (this != iter._owner) 
		{
			throw std::runtime_error("Iterator is not associated with this container.");
		}
		if (iter._node == nullptr)
		{
			PushBack(data);
		}
		else
		{
			Node* toInsert = new Node{ data, iter._node->Next };
			iter._node->Next = toInsert;
			if (iter._node == _back)
			{
				_back = toInsert;
			}
			_size++;
		}
	}

	template <typename T>
	bool SList<T>::Remove(const T& data, std::function<bool(const T&, const T&)> equalityFunc)
	{
		return Remove(Find(data, equalityFunc));
	}

	template <typename T>
	bool SList<T>::Remove(Iterator iter)
	{
		if (iter._owner != this)
		{
			throw std::runtime_error("Iterator doesn't belong to this list");
		}

		bool didRemove = false;

		if (iter != end())
		{
			if (iter._node == _back)
			{
				PopBack();
			}
			else
			{
				Node* next = iter._node->Next;
				if (next == _back)
				{
					_back = iter._node;
				}
				iter._node->Data.~T();
				new (&iter._node->Data)T(std::move(next->Data));
				iter._node->Next = next->Next;

				delete next;
				_size--;
			}
			didRemove = true;
		}

		return didRemove;
	}

#pragma endregion

#pragma region SList

	template <typename T>
	SList<T>::Node::Node(const T& data, Node* next) :
		Data{ data }, Next{ next }
	{
	}

	template <typename T>
	SList<T>::SList(std::initializer_list<T> list)
	{
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template <typename T>
	SList<T>::SList(const SList& other)
	{
		Node* current = other._front;
		while (current != nullptr)
		{
			PushBack(current->Data);
			current = current->Next;
		}
	}

	template <typename T>
	SList<T>& SList<T>::operator=(const SList& other)
	{
		if (this != &other)
		{
			Clear();
			Node* current = other._front;
			while (current != nullptr)
			{
				PushBack(current->Data);
				current = current->Next;
			}
		}
		return *this;
	}

	template <typename T>
	SList<T>::~SList()
	{
		Node* current = _front;
		while (current != nullptr)
		{
			Node* next = current->Next;
			delete current;
			current = next;
		}
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushFront(const T& data)
	{
		_front = new Node{ data,_front };
		if (IsEmpty())
		{
			_back = _front;
		}
		_size++;
		return Iterator{ *this, _front };
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushFront(T&& data)
	{
		_front = new Node{ std::move(data),_front };
		if (IsEmpty())
		{
			_back = _front;
		}
		_size++;
		return Iterator{ *this, _front };
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& data)
	{
		Node* temp = new Node{ data,nullptr };
		if (IsEmpty())
		{
			_front = temp;
		}
		else
		{
			_back->Next = temp;
		}
		_back = temp;

		_size++;
		return Iterator{ *this, _back };
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushBack(T&& data)
	{
		Node* temp = new Node{ std::forward(data),nullptr };
		if (IsEmpty())
		{
			_front = temp;
		}
		else
		{
			_back->Next = temp;
		}
		_back = temp;

		_size++;
		return Iterator{ *this, _back };
	}

	template <typename T>
	bool SList<T>::PopFront()
	{
		if (_front != nullptr)
		{
			Node* temp = _front;
			_front = _front->Next;
			delete temp;
			_size--;
			if (IsEmpty())
			{
				_back = nullptr;
			}
			return true;
		}
		return false;
	}

	template <typename T>
	bool SList<T>::PopBack()
	{
		if (_front != nullptr)
		{
			Node* temp = _front;
			if (_size == 1)
			{
				_back = nullptr;
			}
			else
			{
				for (int iter = 1; iter < (int)_size - 1; iter++)
				{
					temp = temp->Next;
				}
				_back = temp;
				temp = temp->Next;
				_back->Next = nullptr;
			}
			delete temp;
			_size--;
			if (IsEmpty())
			{
				_front = nullptr;
			}
			return true;
		}
		return false;
	}

	template <typename T>
	T& SList<T>::Front()
	{
		if (_front == nullptr)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return _front->Data;
	}

	template <typename T>
	const T& SList<T>::Front() const
	{
		if (_front == nullptr)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return _front->Data;
	}

	template <typename T>
	T& SList<T>::Back()
	{
		if (_back == nullptr)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return _back->Data;
	}

	template <typename T>
	const T& SList<T>::Back() const
	{
		if (_back == nullptr)
		{
			throw std::runtime_error("Cannot access an empty list.");
		}

		return _back->Data;
	}

	template <typename T>
	inline std::size_t SList<T>::Size() const
	{
		return _size;
	}

	template <typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return _size == 0;
	}

	template <typename T>
	void SList<T>::Clear() 
	{
		Node* current = _front;
		while (current != nullptr)
		{
			Node* next = current->Next;
			delete current;
			current = next;
		}

		_front = _back = nullptr;
		_size = 0;
	}

#pragma endregion

}