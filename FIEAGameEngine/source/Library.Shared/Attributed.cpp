#include "pch.h"
#include "Attributed.h"

using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed);

	/*Attributed::SliceIterator::SliceIterator(Attributed& owner, IteratorModes mode) :
		_vector{ &owner._orderVector }, _mode{ mode }
	{
		if (_mode == IteratorModes::Prescribed)
		{
			_end = owner._auxStart;
		}
		else if (_mode == IteratorModes::Auxillary)
		{
			_begin = owner._auxStart;
		}
	}

	Attributed::SliceIterator& Attributed::SliceIterator::operator++()
	{
		if (_vector == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container.");
		}
		if (_index < _end)
		{
			++_index;
		}
		return *this;
	}

	Attributed::SliceIterator Attributed::SliceIterator::operator++(int)
	{
		SliceIterator temp = *this;
		++(*this);
		return temp;
	}

	Scope::HashmapEntry& Attributed::SliceIterator::operator*() const
	{
		if (_vector == nullptr || _index >= _end)
		{
			throw std::runtime_error("Cannot dereference iterator that is at the end.");
		}
		return _vector->At(_index);
	}*/


	Attributed::Attributed(RTTI::IdType typeID)
	{
		(*this)["this"] = this;
		Populate(TypeRegistry::GetInstance()->GetPrescribedAttributes(typeID));
	}

	Attributed::Attributed(const Attributed& other) :
		Scope(other), _auxStart{other._auxStart}
	{
		UpdateExternalStorage(TypeRegistry::GetInstance()->GetPrescribedAttributes(other.TypeIdInstance()));
	}

	Attributed& Attributed::operator=(const Attributed& other)
	{
		Scope::operator=(other);
		UpdateExternalStorage(TypeRegistry::GetInstance()->GetPrescribedAttributes(other.TypeIdInstance()));
		return *this;
	}

	Attributed::Attributed(Attributed&& other) noexcept :
		Scope(std::forward<Scope>(other)), _auxStart{ other._auxStart }
	{
		UpdateExternalStorage(TypeRegistry::GetInstance()->GetPrescribedAttributes(other.TypeIdInstance()));
	}

	Attributed& Attributed::operator=(Attributed&& other) noexcept
	{
		Scope::operator=(std::forward<Scope>(other));
		UpdateExternalStorage(TypeRegistry::GetInstance()->GetPrescribedAttributes(other.TypeIdInstance()));
		return *this;
	}

	void Attributed::Populate(const Vector<Signature>& signatures)
	{
		uint8_t* thisptr = reinterpret_cast<uint8_t*>(this);
		for (auto& iter : signatures)
		{
			Datum& newDatum = Append(iter._name);
			newDatum.SetType(iter._type);
			if (newDatum.Type() != DatumTypes::Table)
			{
				newDatum.SetStorage(iter._type, (thisptr + iter._offset), iter._size);
			}
		}
		_auxStart = _orderVector.Size();
	}

	void Attributed::UpdateExternalStorage(const Vector<Signature>& signatures)
	{
		(*this)["this"] = this;
		uint8_t* thisptr = reinterpret_cast<uint8_t*>(this);
		for (auto& iter : signatures)
		{
			if (iter._type != DatumTypes::Table)
			{
				Datum* newDatum = Find(iter._name);
				assert(newDatum != nullptr);
				newDatum->SetStorage(iter._type, (thisptr + iter._offset), iter._size);
			}
		}
	}

	Datum& Attributed::AppendAuxillaryAttribute(const std::string& key)
	{
		if (IsPrescribedAttribute(key))
		{
			throw std::runtime_error("Entry is already a prescribed attribute");
		}
		return Append(key);
	}

	bool Attributed::IsAttribute(const std::string& key) const
	{
		return Find(key) != nullptr;
	}

	bool Attributed::IsPrescribedAttribute(const std::string& key) const
	{
		auto iter = _orderVector.cbegin();
		size_t count = 0;
		while (count < _auxStart)
		{
			if (key == (*iter++)->first)
			{
				return true;
			}
			++count;
		}
		return false;
	}

	bool Attributed::IsAuxillaryAttribute(const std::string& key) const
	{
		return IsAttribute(key) && !IsPrescribedAttribute(key);
	}

	/*typename Attributed::SliceIterator Attributed::Attributes()
	{
		return SliceIterator(*this, SliceIterator::IteratorModes::All);
	}

	typename Attributed::SliceIterator Attributed::PrescribedAttributes()
	{
		return SliceIterator(*this, SliceIterator::IteratorModes::Prescribed);
	}

	typename Attributed::SliceIterator Attributed::AuxillaryAttributes()
	{
		return SliceIterator(*this, SliceIterator::IteratorModes::Auxillary);
	}*/
}