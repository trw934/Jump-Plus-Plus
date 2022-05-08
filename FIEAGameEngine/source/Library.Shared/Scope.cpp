#include "pch.h"
#include "Scope.h"
#include <sstream>


namespace FieaGameEngine 
{
	RTTI_DEFINITIONS(Scope);

	Scope::Scope(std::size_t capacity, std::size_t bucketSize) :
		_map{bucketSize > 1 ? bucketSize : 11, DefaultHash<std::string>(), std::equal_to<std::string>() }
	{
		if (capacity > 0)
		{
			_orderVector.Reserve(capacity);
		}
	}

	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}

	void Scope::CopyHelper(const Scope& other)
	{
		for (const auto value : other._orderVector)
		{
			Datum newDatum;
			Datum& existingDatum = value->second;
			if (existingDatum.Type() == DatumTypes::Table)
			{
				newDatum.SetType(DatumTypes::Table);
				newDatum.Reserve(existingDatum.Size());
				for (size_t index = 0; index < existingDatum.Size(); ++index)
				{
					Scope& nestedScope = existingDatum[index];
					Scope* childCopy = nestedScope.Clone();
					childCopy->_parent = this;
					newDatum.PushBack(*childCopy);
				}
			}
			else
			{
				newDatum = existingDatum;
			}
			Append(value->first) = newDatum;
		}
	}

	Scope::Scope(const Scope& other) :
		_map{other._map.BucketSize()}, _orderVector{other._orderVector.Capacity()}
	{
		CopyHelper(other);
	}

	Scope& Scope::operator=(const Scope& other)
	{
		if (*this != other)
		{
			Clear();
			_map.ResizeHash(other._map.BucketSize());
			_orderVector.Reserve(other._orderVector.Capacity());
			CopyHelper(other);
		}
		return *this;
	}

	void Scope::MoveHelper() noexcept
	{
		for (const auto value : _orderVector)
		{
			Datum& existingDatum = value->second;
			if (existingDatum.Type() == DatumTypes::Table)
			{
				for (size_t index = 0; index < existingDatum.Size(); ++index)
				{
					Scope& nestedScope = existingDatum[index];
					nestedScope._parent = this;
				}
			}
		}
	}

	Scope::Scope(Scope&& other) noexcept :
		_map{ std::move(other._map) }, _orderVector{ std::move(other._orderVector) }, _parent{ other._parent }
	{
		if (_parent != nullptr)
		{
			auto [scopeDatum, index] = _parent->FindNestedScope(&other);
			scopeDatum->Set(*this, index);
			other._parent = nullptr;
		}
		MoveHelper();
	}

	Scope& Scope::operator=(Scope&& other) noexcept
	{
		if (*this != other)
		{
			if (_parent != nullptr)
			{
				_parent->Orphan(*this);
			}
			Clear();
			_map = std::move(other._map);
			_orderVector = std::move(other._orderVector);
			_parent = other._parent;
			if (_parent != nullptr)
			{
				auto [scopeDatum, index] = _parent->FindNestedScope(&other);
				scopeDatum->Set(*this, index);
				other._parent = nullptr;
			}
			MoveHelper();
		}
		
		return *this;
	}

	Scope::~Scope()
	{
		if (_parent != nullptr)
		{
			_parent->Orphan(*this);
		}
		Clear();
	}

	Datum* Scope::Find(const std::string& key)
	{
		Datum* result = nullptr;
		auto iter = _map.Find(key);
		if (iter != _map.end())
		{
			result = &(iter->second);
		}
		return result;
	}

	const Datum* Scope::Find(const std::string& key) const
	{
		const Datum* result = nullptr;
		auto iter = _map.Find(key);
		if (iter != _map.cend())
		{
			result = &(iter->second);
		}
		return result;
	}

	Datum* Scope::Search(const std::string& key)
	{
		Datum* result = nullptr;
		result = Find(key);
		if (result != nullptr)
		{
			return result;
		}
		if (_parent != nullptr)
		{
			result = _parent->Search(key);
		}
		return result;
	}

	const Datum* Scope::Search(const std::string& key) const
	{
		const Datum* result = nullptr;
		result = Find(key);
		if (result != nullptr)
		{
			return result;
		}
		if (_parent != nullptr)
		{
			result = _parent->Search(key);
		}
		return result;
	}

	Datum* Scope::Search(const std::string& key, Scope*& foundScope)
	{
		Datum* result = Find(key);
		if (result != nullptr)
		{
			foundScope = this;
			return result;
		}
		if (_parent != nullptr)
		{
			return _parent->Search(key, foundScope);
		}
		else
		{
			foundScope = nullptr;
			return nullptr;
		}
	}

	const Datum* Scope::Search(const std::string& key, const Scope*& foundScope) const
	{
		const Datum* result = Find(key);
		if (result != nullptr)
		{
			foundScope = this;
			return result;
		}
		if (_parent != nullptr)
		{
			return _parent->Search(key, foundScope);
		}
		else
		{
			foundScope = nullptr;
			return nullptr;
		}
	}

	Datum& Scope::Append(const std::string& key)
	{
		bool wasInserted;
		return Append(key, wasInserted);
	}

	Datum& Scope::Append(const std::string& key, bool& wasInserted)
	{
		if (key.empty())
		{
			throw std::invalid_argument("Name cannot be empty.");
		}
		auto result = _map.Insert({ key, Datum() });
		wasInserted = result.second;
		if (wasInserted)
		{
			_orderVector.PushBack(&(*result.first));
		}
		return result.first->second;
	}

	Scope& Scope::AppendScope(const std::string& key)
	{
		if (key.empty())
		{
			throw std::invalid_argument("Name cannot be empty.");
		}
		bool wasInserted;
		Datum& datum = Append(key, wasInserted);
		if (wasInserted == false)
		{
			if (datum.Type() != DatumTypes::Unknown && datum.Type() != DatumTypes::Table)
			{
				throw std::runtime_error("Datum is not of type table");
			}
		}

		Scope* insertScope = new Scope();
		datum.PushBack(*insertScope);
		insertScope->_parent = this;
		return *insertScope;
	}

	void Scope::Adopt(Scope& scope, const std::string& key)
	{
		if (key.empty())
		{
			throw std::invalid_argument("Name cannot be empty.");
		}

		if (scope != *this && !scope.IsAncestorOf(*this))
		{
			Datum* datum = Find(key);
			if (datum != nullptr)
			{
				datum->PushBack(scope);
			}
			else
			{
				Datum newDatum;
				newDatum = scope;
				Append(key) = newDatum;
			}
			if (scope._parent != nullptr)
			{
				scope._parent->Orphan(scope);
			}
			scope._parent = this;
		}
	}

	Scope* Scope::GetParent() const
	{
		return _parent;
	}

	Datum& Scope::operator[](const std::string& key)
	{
		return Append(key);
	}

	Datum& Scope::operator[](std::size_t index)
	{
		auto entry = _orderVector[index];
		return entry->second;
	}

	bool Scope::operator==(const Scope& other) const
	{
		if (_orderVector.Size() != other._orderVector.Size())
		{
			return false;
		}

		for (auto iter : _orderVector)
		{
			auto [key, datum] = *iter;
			if (key != "this")
			{
				auto foundIter = (other._map.Find(key));
				if (foundIter == other._map.cend())
				{
					return false;
				}
				if (datum != foundIter->second)
				{
					return false;
				}
			}
		}

		return true;
	}

	bool Scope::operator!=(const Scope& other) const
	{
		return !(operator==(other));
	}

	void Scope::Clear()
	{
		for (auto iter : _orderVector)
		{
			if (iter->second.Type() == DatumTypes::Table)
			{
				size_t count = iter->second.Size();
				for (size_t index = 0; index < count; ++index)
				{
					auto scope = iter->second.GetTable(index);
					scope->_parent = nullptr;
					delete scope;
				}
			}
		}
		_map.Clear();
		_orderVector.Clear();
	}

	bool Scope::IsAncestorOf(const Scope& scope) const
	{
		bool result = false;
		Scope* parent = scope._parent;
		while (parent != nullptr)
		{
			if (parent == this)
			{
				result = true;
				break;
			}
			parent = parent->_parent;
		}
		return result;
	}

	bool Scope::IsDescendantOf(const Scope& scope) const
	{
		return scope.IsAncestorOf(*this);
	}

	std::pair<Datum*, std::size_t> Scope::FindNestedScope(const Scope* scope) const
	{
		Datum* datum = nullptr;
		auto iter = _orderVector.cbegin();
		while (iter != _orderVector.cend())
		{
			datum = &((*iter)->second);
			if (datum->Type() == DatumTypes::Table)
			{
				auto [found, index] = datum->Find(*scope);
				if (found)
				{
					return std::pair<Datum*, std::size_t>{datum, index};
				}
			}
			++iter;
		}
		return std::pair<Datum*, std::size_t>{nullptr, 0};
	}

	void Scope::Orphan(Scope& child)
	{
		auto [datum, index] = FindNestedScope(&child);
		if (datum != nullptr)
		{
			child._parent = nullptr;
			datum->RemoveAt(index);
		}
	}

	std::size_t Scope::Size() const
	{
		return _orderVector.Size();
	}

	bool Scope::Equals(const RTTI* other) const
	{
		const auto otherScope = other->As<Scope>();
		if (otherScope == nullptr)
		{
			return false;
		}
		return *this == *otherScope;
	}


	Datum* Scope::GlobalSearch(const std::string& datumString)
	{
		std::stringstream datumStringStream(datumString);
		std::string token;
		bool firstToken = true;
		Datum* foundDatum = nullptr;
		Scope* foundScope = nullptr;
		while (std::getline(datumStringStream, token, '.'))
		{
			if (!token.empty())
			{
				if (firstToken)
				{
					foundDatum = Search(token, foundScope);
				}
				else
				{
					assert(foundScope != nullptr);
					if (foundDatum->Type() == DatumTypes::Table)
					{
						for (std::size_t i = 0; i < foundDatum->Size(); ++i)
						{
							auto nestedScope = foundDatum->GetTable(i);
							if (nestedScope->Find(token) != nullptr)
							{
								foundScope = nestedScope;
							}
						}
					}
					foundDatum = foundScope->Find(token);
				}
				firstToken = false;
			}
		}

		return foundDatum;
	}
}