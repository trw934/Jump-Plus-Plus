#include "Vector.h"

namespace FieaGameEngine
{
#pragma region Vector Iterator
	template <typename T>
	Vector<T>::Iterator::Iterator(const Vector& owner, size_t index) :
		_owner{ &owner }, _index{ index }
	{
	}

	template <typename T>
	Vector<T>::ConstIterator::ConstIterator(const Vector& owner, size_t index) :
		_owner{ &owner }, _index{ index }
	{
	}

	template <typename T>
	Vector<T>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner{ other._owner }, _index{ other._index }
	{
	}

	template <typename T>
	bool Vector<T>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}

	template <typename T>
	bool Vector<T>::Iterator::operator!=(const Iterator& other) const
	{
		if (_owner == other._owner)
		{
			return (_index != other._index);
		}
		return true;
	}

	template <typename T>
	bool Vector<T>::Iterator::operator<(const Iterator& other) const
	{
		return (_index < other._index);
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container.");
		}
		if (_index < _owner->_size)
		{
			++_index;
		}
		return *this;
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		++(*this);
		return temp;
	}

	template <typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container.");
		}
		if (_index > 0)
		{
			--_index;
		}
		return *this;
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
	{
		Iterator temp = *this;
		--(*this);
		return temp;
	}

	template <typename T>
	T& Vector<T>::Iterator::operator*() const
	{
		if (_owner == nullptr || _index >= _owner->_size)
		{
			throw std::runtime_error("Cannot dereference iterator that points to nullptr.");
		}
		return _owner->_head[_index];
	}

	template <typename T>
	bool Vector<T>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));
	}

	template <typename T>
	bool Vector<T>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		if (_owner == other._owner)
		{
			return (_index != other._index);
		}
		return true;
	}

	template <typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container.");
		}
		if (_index < _owner->_size)
		{
			++_index;
		}
		return *this;
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		++(*this);
		return temp;
	}

	template <typename T>
	const T& Vector<T>::ConstIterator::operator*() const
	{
		if (_owner == nullptr || _index >= _owner->_size)
		{
			throw std::runtime_error("Cannot dereference iterator that points to nullptr.");
		}
		return _owner->_head[_index];
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator{ *this, 0 };
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::begin() const
	{
		return ConstIterator{ *this, 0 };
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator{ *this, 0 };
	}

	template <typename T>
	inline typename Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator{ *this, _size };
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::end() const
	{
		return ConstIterator{ *this, _size };
	}

	template <typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator{ *this, _size };
	}

	template <typename T>
	typename Vector<T>::Iterator Vector<T>::Find(const T& data, std::function<bool(const T&, const T&)> equalityFunc)
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
	typename Vector<T>::ConstIterator Vector<T>::Find(const T& data, std::function<bool(const T&, const T&)> equalityFunc) const
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
	bool Vector<T>::RemoveAt(size_t index)
	{
		if (index < _size && _size > 0)
		{
			if (index == _size - 1)
			{
				PopBack();
			}
			else
			{
				for (size_t i = index; i < _size - 1; ++i)
				{
					_head[i] = _head[i + 1];
				}
				std::destroy_at(_head + _size - 1);
				--_size;
			}
			return true;
		}
		return false;
	}

	template <typename T>
	bool Vector<T>::Remove(const T& data, std::function<bool(const T&, const T&)> equalityFunc)
	{
		Iterator iter = Find(data, equalityFunc);
		if (iter != end())
		{
			return RemoveAt(iter._index);
		}
		return false;
	}

	template <typename T>
	bool Vector<T>::Remove(const Iterator& iter)
	{
		if (iter != end())
		{
			return RemoveAt(iter._index);
		}
		return false;
	}

	template<typename T>
	void Vector<T>::Remove(const Iterator& first, const Iterator& last)
	{
		if ((first._owner != last._owner) || (first._owner != this))
		{
			throw std::runtime_error("Error, Iterators do not belong to the same container!");
		}

		if (last._index < first._index)
		{
			throw std::runtime_error("Error, iterators in invalid order!");
		}

		const auto elementsToRemove = last._index - first._index;
		if (elementsToRemove > 0)
		{
			for (size_t i = first._index; i < last._index; ++i)
			{
				_head[i].~T();
			}

			const auto elementsToMove = _size - last._index;
			if (elementsToMove > 0)
			{
				const auto size = elementsToMove * sizeof(T);
#if defined(_WIN64) || defined(_WIN32)
				memmove_s(&_head[first._index], size, &_head[last._index], size);
#else
				memmove(&_head[first._index], &_head[last._index], size);
#endif
			}

			_size -= elementsToRemove;
		}
	}

#pragma endregion

#pragma region Vector

	template <typename T>
	Vector<T>::Vector(size_t capacity) :
		_capacity{ capacity }, _head{ capacity > 0 ? reinterpret_cast<T*>(malloc(sizeof(T) * capacity)) : nullptr }
	{
	}

	template <typename T>
	Vector<T>::Vector(std::initializer_list<T> vector)
	{
		_capacity = vector.size();
		_head = reinterpret_cast<T*>(malloc(sizeof(T) * _capacity));
		for (const auto& value : vector)
		{
			PushBack(value);
		}
	}

	template <typename T>
	Vector<T>::Vector(const Vector& other) :
		_capacity{ other._capacity }, _head{ other._capacity > 0 ? reinterpret_cast<T*>(malloc(sizeof(T) * other._capacity)) : nullptr }
	{
		for (size_t i = 0; i < other._size; ++i)
		{
			PushBack(other[i]);
		}
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(const Vector& other)
	{
		if (this != &other)
		{
			Clear();
			_capacity = other._capacity;
			_head = reinterpret_cast<T*>(realloc(_head, sizeof(T) * _capacity));
			for (size_t i = 0; i < other._size; ++i)
			{
				PushBack(other[i]);
			}
		}
		return *this;
	}

	template <typename T>
	Vector<T>::Vector(Vector&& other) :
		_size{other._size}, _capacity{other._capacity}, _head{other._head}
	{
		other._size = 0;
		other._capacity = 0;
		other._head = nullptr;
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(Vector&& other)
	{
		if (this != &other)
		{
			Clear();
			free(_head);
			_size = other._size;
			_capacity = other._capacity;
			_head = other._head;

			other._size = 0;
			other._capacity = 0;
			other._head = nullptr;
		}
		return *this;
	}

	template <typename T>
	Vector<T>::~Vector()
	{
		for (size_t i = 0; i < _size; ++i)
		{
			std::destroy_at(_head + i);
		}
		free(_head);
	}

	template <typename T>
	void Vector<T>::Reserve(size_t capacity)
	{
		if (capacity > _capacity)
		{
			T* data = reinterpret_cast<T*>(realloc(_head, sizeof(T) * capacity));
			assert(data != nullptr);
			_head = data;
			_capacity = capacity;
		}
	}

	template <typename T>
	void Vector<T>::Resize(size_t size)
	{
		if (size < _size)
		{
			for (size_t i = size; i < _size; ++i)
			{
				_head[i].~T();
			}
		}
		else
		{
			Reserve(size);
			for (size_t i = _size; i < size; ++i)
			{
				new (_head + i)T();
			}
		}
		_size = size;
		_capacity = size;
	}

	template <typename T>
	void Vector<T>::ShrinkToFit()
	{
		T* data = reinterpret_cast<T*>(realloc(_head, sizeof(T) * _size));
		assert(data != nullptr);
		_head = data;
		_capacity = _size;
	}

	template <typename T>
	T& Vector<T>::operator[](size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid.");
		}
		return _head[index];
	}

	template <typename T>
	const T& Vector<T>::operator[](size_t index) const
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid.");
		}
		return _head[index];
	}

	template <typename T>
	T& Vector<T>::At(size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid.");
		}
		return _head[index];
	}

	template <typename T>
	const T& Vector<T>::At(size_t index) const
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid.");
		}
		return _head[index];
	}

	template <typename T>
	void Vector<T>::PushBack(const T& data)
	{
		if (_size + 1 > _capacity)
		{
			if (_capacity == 0)
			{
				_capacity = 3;
			}
			else
			{
				_capacity *= 2;
			}
			_head = reinterpret_cast<T*>(realloc(_head, _capacity * sizeof(T)));
			assert(_head != nullptr);
		}
		new (_head + _size) T(data);
		++_size;
	}

	template <typename T>
	bool Vector<T>::PopBack()
	{
		if (_size > 0)
		{
			std::destroy_at(_head + (_size - 1));
			--_size;
			return true;
		}
		return false;
	}

	template <typename T>
	T& Vector<T>::Front()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty vector.");
		}
		return _head[0];
	}

	template <typename T>
	const T& Vector<T>::Front() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty vector.");
		}
		return _head[0];
	}

	template <typename T>
	T& Vector<T>::Back()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty vector.");
		}
		return _head[_size - 1];
	}

	template <typename T>
	const T& Vector<T>::Back() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty vector.");
		}
		return _head[_size - 1];
	}

	template <typename T>
	inline std::size_t Vector<T>::Size() const
	{
		return _size;
	}

	template <typename T>
	inline std::size_t Vector<T>::Capacity() const
	{
		return _capacity;
	}

	template <typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return _size == 0;
	}

	template <typename T>
	void Vector<T>::Clear()
	{
		for (size_t i = 0; i < _size; ++i)
		{
			std::destroy_at(_head + i);
		}

		_size = 0;
	}

#pragma endregion

}