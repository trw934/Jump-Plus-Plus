#include "Hashmap.h"

namespace FieaGameEngine
{

#pragma region Hashmap Iterator
	template <typename TKey, typename TData>
	Hashmap<TKey, TData>::Iterator::Iterator(Hashmap& owner, size_t index, ChainIterator chainIter) :
		_owner{ &owner }, _index{ index }, _chainIter{chainIter}
	{
	}

	template <typename TKey, typename TData>
	Hashmap<TKey, TData>::Iterator::Iterator(Hashmap& owner, size_t index) :
		_owner{ &owner }, _index{ index }
	{
	}

	template <typename TKey, typename TData>
	Hashmap<TKey, TData>::ConstIterator::ConstIterator(const Hashmap& owner, size_t index, ChainConstIterator chainIter) :
		_owner{ &owner }, _index{ index }, _chainIter{ chainIter }
	{
	}

	template <typename TKey, typename TData>
	Hashmap<TKey, TData>::ConstIterator::ConstIterator(const Hashmap& owner, size_t index) :
		_owner{ &owner }, _index{ index }
	{
	}

	template <typename TKey, typename TData>
	Hashmap<TKey, TData>::ConstIterator::ConstIterator(const Iterator& other) :
		_owner{ other._owner }, _index{ other._index }, _chainIter{ other._chainIter }
	{
	}

	template <typename TKey, typename TData>
	bool Hashmap<TKey, TData>::Iterator::operator==(const Iterator& other) const
	{
		return !(operator!=(other));
	}

	template <typename TKey, typename TData>
	bool Hashmap<TKey, TData>::Iterator::operator!=(const Iterator& other) const
	{
		if (_owner == other._owner)
		{
			if (_index == other._index)
			{
				if (_index < _owner->_bucketSize)
				{
					return (_chainIter != other._chainIter);
				}
				return false;
			}
		}
		return true;
	}

	template <typename TKey, typename TData>
	typename Hashmap<TKey, TData>::Iterator& Hashmap<TKey, TData>::Iterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container.");
		}
		if (_index < _owner->_bucketSize)
		{
			++_chainIter;
			if (_chainIter == _owner->_hashmap.At(_index).end())
			{
				++_index;
				while (_index < _owner->_bucketSize)
				{
					if (!_owner->_hashmap[_index].IsEmpty())
					{
						_chainIter = _owner->_hashmap[_index].begin();
						return *this;
					}
					++_index;
				}
			}
		}
		return *this;
	}

	template <typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::Iterator Hashmap<TKey, TData>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		++(*this);
		return temp;
	}

	template <typename TKey, typename TData>
	typename Hashmap<TKey, TData>::PairType& Hashmap<TKey, TData>::Iterator::operator*() const
	{
		if (_owner == nullptr || _index >= _owner->_bucketSize)
		{
			throw std::runtime_error("Cannot derefernce iterator that points to nullptr.");
		}
		return *_chainIter;
	}

	template <typename TKey, typename TData>
	typename Hashmap<TKey, TData>::PairType* Hashmap<TKey, TData>::Iterator::operator->() const
	{
		if (_owner == nullptr || _index >= _owner->_bucketSize)
		{
			throw std::runtime_error("Cannot derefernce iterator that points to nullptr.");
		}
		return &(*_chainIter);
	}

	template <typename TKey, typename TData>
	bool Hashmap<TKey, TData>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !(operator!=(other));
	}

	template <typename TKey, typename TData>
	bool Hashmap<TKey, TData>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		if (_owner == other._owner)
		{
			if (_index == other._index)
			{
				if (_index < _owner->_bucketSize)
				{
					return (_chainIter != other._chainIter);
				}
				return false;
			}
		}
		return true;
	}

	template <typename TKey, typename TData>
	typename Hashmap<TKey, TData>::ConstIterator& Hashmap<TKey, TData>::ConstIterator::operator++()
	{
		if (_owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container.");
		}
		if (_index < _owner->_bucketSize)
		{
			++_chainIter;
			if (_chainIter == _owner->_hashmap.At(_index).end())
			{
				++_index;
				while (_index < _owner->_bucketSize)
				{
					if (!_owner->_hashmap[_index].IsEmpty())
					{
						_chainIter = _owner->_hashmap[_index].begin();
						return *this;
					}
					++_index;
				}
			}
		}
		return *this;
	}

	template <typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		++(*this);
		return temp;
	}

	template <typename TKey, typename TData>
	const typename Hashmap<TKey, TData>::PairType& Hashmap<TKey, TData>::ConstIterator::operator*() const
	{
		if (_owner == nullptr || _index >= _owner->_bucketSize)
		{
			throw std::runtime_error("Cannot derefernce iterator that points to nullptr.");
		}
		return *_chainIter;
	}

	template <typename TKey, typename TData>
	const typename Hashmap<TKey, TData>::PairType* Hashmap<TKey, TData>::ConstIterator::operator->() const
	{
		if (_owner == nullptr || _index >= _owner->_bucketSize)
		{
			throw std::runtime_error("Cannot derefernce iterator that points to nullptr.");
		}
		return &(*_chainIter);
	}

	template <typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::Iterator Hashmap<TKey, TData>::begin()
	{
		if (_size != 0)
		{
			for (size_t index = 0; index < _bucketSize; ++index)
			{
				if (!_hashmap[index].IsEmpty())
				{
					return Iterator{ *this,index,_hashmap[index].begin() };
				}
			}
		}
		return end();
	}

	template <typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::begin() const
	{
		if (_size != 0)
		{
			for (size_t index = 0; index < _bucketSize; ++index)
			{
				if (!_hashmap[index].IsEmpty())
				{
					return ConstIterator{ *this,index,_hashmap[index].begin() };
				}
			}
		}
		return end();
	}

	template <typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::cbegin() const
	{
		if (_size != 0)
		{
			for (size_t index = 0; index < _bucketSize; ++index)
			{
				if (!_hashmap[index].IsEmpty())
				{
					return ConstIterator{ *this,index,_hashmap[index].begin() };
				}
			}
		}
		return end();
	}

	template <typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::Iterator Hashmap<TKey, TData>::end()
	{
		return Iterator{ *this, _bucketSize };
	}

	template <typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::end() const
	{
		return ConstIterator{ *this, _bucketSize };
	}

	template <typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::cend() const
	{
		return ConstIterator{ *this, _bucketSize };
	}

	template <typename TKey, typename TData>
	typename Hashmap<TKey, TData>::Iterator Hashmap<TKey, TData>::Find(const TKey& key)
	{
		size_t index = (_hashFunc(key) % _bucketSize);
		ChainIterator chainIter;
		for (chainIter = _hashmap[index].begin(); chainIter != _hashmap[index].end(); ++chainIter)
		{
			if (_equalityFunc((*chainIter).first, key))
			{
				return Iterator{ *this, index, chainIter };
			}
		}
		return end();
	}

	template <typename TKey, typename TData>
	std::pair<typename Hashmap<TKey, TData>::Iterator, size_t> Hashmap<TKey, TData>::FindReturnHash(const TKey& key)
	{
		size_t index = (_hashFunc(key) % _bucketSize);
		ChainIterator chainIter;
		for (chainIter = _hashmap[index].begin(); chainIter != _hashmap[index].end(); ++chainIter)
		{
			if (_equalityFunc((*chainIter).first, key))
			{
				return std::pair<typename Hashmap<TKey, TData>::Iterator, size_t>(Iterator{ *this, index, chainIter }, index);
			}
		}
		return std::pair<typename Hashmap<TKey, TData>::Iterator, size_t>(end(), index);
	}

	template <typename TKey, typename TData>
	typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::Find(const TKey& key) const
	{
		size_t index = (_hashFunc(key) % _bucketSize);
		ChainConstIterator chainIter;
		for (chainIter = _hashmap[index].cbegin(); chainIter != _hashmap[index].cend(); ++chainIter)
		{
			if (_equalityFunc((*chainIter).first, key))
			{
				return ConstIterator{ *this, index, chainIter };
			}
		}
		return end();
	}

	template <typename TKey, typename TData>
	bool Hashmap<TKey, TData>::Remove(const TKey& key)
	{
		auto [iter, index] = FindReturnHash(key);
		if (iter != end())
		{
			bool result = _hashmap[index].Remove(*iter);
			if (result)
			{
				--_size;
				return true;
			}
		}
		return false;
	}


#pragma endregion

#pragma region Hashmap

	template <typename TKey, typename TData>
	Hashmap<TKey, TData>::Hashmap(size_t bucketSize) :
		_bucketSize{bucketSize > 1 ? bucketSize : 11}
	{
		_hashmap.Resize(_bucketSize);
	}

	template <typename TKey, typename TData>
	Hashmap<TKey, TData>::Hashmap(size_t bucketSize, HashFunctor hashFunc, EqualityFunctor equalityFunc) :
		 _hashFunc{ hashFunc }, _equalityFunc{equalityFunc}
	{
		if (bucketSize <= 1)
		{
			throw std::runtime_error("Cannot create hashmap with number of buckets less than 2.");
		}
		_bucketSize = bucketSize;
		_hashmap.Resize(_bucketSize);
	}

	template <typename TKey, typename TData>
	Hashmap<TKey, TData>::Hashmap(std::initializer_list<PairType> list)
	{
		_hashmap.Resize(_bucketSize);
		for (const auto& value : list)
		{
			Insert(value);
		}
	}

	template <typename TKey, typename TData>
	Hashmap<TKey, TData>::Hashmap(std::initializer_list<PairType> list, size_t bucketSize, HashFunctor hashFunc, EqualityFunctor equalityFunc) :
		_hashFunc{ hashFunc }, _equalityFunc{ equalityFunc }
	{
		if (bucketSize <= 1)
		{
			throw std::runtime_error("Cannot create hashmap with number of buckets less than 2.");
		}
		_bucketSize = bucketSize;
		_hashmap.Resize(_bucketSize);
		for (const auto& value : list)
		{
			Insert(value);
		}
	}

	template <typename TKey, typename TData>
	bool Hashmap<TKey, TData>::ContainsKey(const TKey& key) const
	{
		ConstIterator iter = Find(key);
		return !(iter == end());
	}

	template <typename TKey, typename TData>
	std::pair<bool, size_t> Hashmap<TKey, TData>::ContainsKeyReturnHash(const TKey& key) const
	{
		ConstIterator iter = Find(key);
		return std::pair<bool, size_t>(!(iter == end()), (_hashFunc(key)) % _bucketSize);
	}

	template <typename TKey, typename TData>
	std::pair<typename Hashmap<TKey, TData>::Iterator, bool> Hashmap<TKey, TData>::Insert(const PairType& pair)
	{
		auto [iter, index] = FindReturnHash(pair.first);

		if (iter != end())
		{
			return std::pair<typename Hashmap<TKey, TData>::Iterator, bool>(iter, false);
		}
		else
		{
			Iterator result{ *this, index, _hashmap.At(index).PushBack(pair) };
			++_size;
			return std::pair<typename Hashmap<TKey, TData>::Iterator, bool>(result, true);
		}
	}

	template <typename TKey, typename TData>
	TData& Hashmap<TKey, TData>::operator[](const TKey& key)
	{
		auto [entry, wasInserted] = Insert({ key, TData{} });
		return entry->second;
	}

	template <typename TKey, typename TData>
	TData& Hashmap<TKey, TData>::At(const TKey& key)
	{
		Iterator iter = Find(key);
		if (iter == end())
		{
			throw std::runtime_error("Key was not in the hashmap.");
		}
		return iter->second;
	}

	template <typename TKey, typename TData>
	const TData& Hashmap<TKey, TData>::At(const TKey& key) const
	{
		ConstIterator iter = Find(key);
		if (iter == end())
		{
			throw std::runtime_error("Key was not in the hashmap.");
		}
		return iter->second;
	}

	template <typename TKey, typename TData>
	inline size_t Hashmap<TKey, TData>::Size() const
	{
		return _size;
	}

	template <typename TKey, typename TData>
	inline size_t Hashmap<TKey, TData>::BucketSize() const
	{
		return _bucketSize;
	}

	template <typename TKey, typename TData>
	void Hashmap<TKey, TData>::Clear()
	{
		for (auto& chain : _hashmap)
		{
			chain.Clear();
		}
		_size = 0;
	}

	template <typename TKey, typename TData>
	float Hashmap<TKey, TData>::GetLoadFactor() const
	{
		float bucketsAllocated = 0.0f;
		for (size_t i = 0; i < _bucketSize; ++i)
		{
			if (!_hashmap[i].IsEmpty())
			{
				++bucketsAllocated;
			}
		}
		return bucketsAllocated / static_cast<float>(_bucketSize);
	}

	template <typename TKey, typename TData>
	void Hashmap<TKey, TData>::ResizeHash(size_t capacity)
	{
		if (capacity <= 1)
		{
			throw std::runtime_error("Cannot resize hashmap with number of buckets less than 2.");
		}
		Hashmap temp(capacity, _hashFunc, _equalityFunc);
		for (auto& item : *this)
		{
			temp.Insert(std::move(item));
		}

		*this = std::move(temp);
	}

#pragma endregion

}