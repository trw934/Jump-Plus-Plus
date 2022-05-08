#include "pch.h"
#include "Datum.h"
#pragma warning (push)
#pragma warning (disable : 4201)
#include "glm/gtx/string_cast.hpp"
#pragma warning (pop)



namespace FieaGameEngine
{

	const std::size_t Datum::DatumTypeSizes[] =
	{
		0, //Unknown
		sizeof(std::int32_t),
		sizeof(float),
		sizeof(glm::vec4),
		sizeof(glm::mat4),
		sizeof(std::string),
		sizeof(FieaGameEngine::RTTI),
		sizeof(FieaGameEngine::Scope*)
	};

#pragma region Datum operator=
	Datum& Datum::operator=(const std::int32_t& value)
	{
		if (_size == 0)
		{
			PushBack(value);
		}
		else
		{
			Set(value);
		}
		return *this;
	}

	Datum& Datum::operator=(const float& value)
	{
		if (_size == 0)
		{
			PushBack(value);
		}
		else
		{
			Set(value);
		}
		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& value)
	{
		if (_size == 0)
		{
			PushBack(value);
		}
		else
		{
			Set(value);
		}
		return *this;
	}

	Datum& Datum::operator=(const glm::mat4& value)
	{
		if (_size == 0)
		{
			PushBack(value);
		}
		else
		{
			Set(value);
		}
		return *this;
	}

	Datum& Datum::operator=(const std::string& value)
	{
		if (_size == 0)
		{
			PushBack(value);
		}
		else
		{
			Set(value);
		}
		return *this;
	}

	Datum& Datum::operator=(FieaGameEngine::RTTI* const & value)
	{
		if (_size == 0)
		{
			PushBack(value);
		}
		else
		{
			Set(value);
		}
		return *this;
	}

	Datum& Datum::operator=(FieaGameEngine::Scope& value)
	{
		if (_size == 0)
		{
			PushBack(value);
		}
		else
		{
			Set(value);
		}
		return *this;
	}
#pragma endregion

	Datum::Datum(const Datum& other) :
		_capacity{ other._capacity }, _type{ other._type }, _isExternal{ other._isExternal }
	{
		if (other._isExternal)
		{
			_data = other._data;
			_size = other._size;
		}
		else if (_type != DatumTypes::Unknown)
		{
			std::size_t size = DatumTypeSizes[static_cast<std::size_t>(_type)];
			_data.vp = malloc(size * _capacity);
			if (_type == DatumTypes::String)
			{
				for (std::size_t i = 0; i < other._size; ++i)
				{
					PushBack(other.GetString(i));
				}
			}
			else
			{
				auto dataArr = reinterpret_cast<uint8_t*>(_data.vp);
				auto otherDataArr = reinterpret_cast<uint8_t*>(other._data.vp);
				memcpy(dataArr, otherDataArr, other._size * size);
				_size = other._size;
			}
		}
	}

	Datum& Datum::operator=(const Datum& other)
	{
		if (this != &other)
		{
			if (other._isExternal)
			{
				if (!_isExternal)
				{
					Clear();
					free(_data.vp);
				}
				_data = other._data;
				_capacity = other._capacity;
				_type = other._type;
				_size = other._size;
				_isExternal = other._isExternal;
			}
			else
			{
				if (_isExternal)
				{
					_data.vp = nullptr;
				}
				else
				{
					Clear();
				}
				_capacity = other._capacity;
				_type = other._type;
				_isExternal = other._isExternal;
				if (_type != DatumTypes::Unknown)
				{
					std::size_t size = DatumTypeSizes[static_cast<std::size_t>(_type)];
					_data.vp = realloc(_data.vp, other._capacity * size);
					if (_type == DatumTypes::String)
					{
						for (std::size_t i = 0; i < other._size; ++i)
						{
							PushBack(other.GetString(i));
						}
					}
					else
					{
						memcpy(_data.vp, other._data.vp, size * other._size);
						_size = other._size;
					}
				}
				
			}
		}
		return *this;
	}

	Datum::Datum(Datum&& other) :
		_capacity{other._capacity}, _size{other._size}, _type{other._type},_isExternal{other._isExternal}, _data{other._data}
	{
		other._size = 0;
		other._capacity = 0;
		other._data.vp = nullptr;
	}

	Datum& Datum::operator=(Datum&& other)
	{
		if (this != &other)
		{
			Clear();
			free(_data.vp);
			_size = other._size;
			_capacity = other._capacity;
			_data = other._data;
			_type = other._type;

			other._size = 0;
			other._capacity = 0;
			other._data.vp = nullptr;
		}
		return *this;
	}

	Datum::~Datum()
	{
		if (!_isExternal)
		{
			if (_type == DatumTypes::String)
			{
				for (size_t i = 0; i < _size; ++i)
				{
					std::destroy_at(_data.s + i);
				}
			}
			free(_data.vp);
		}
	}

#pragma region Datum ==operators

	bool Datum::operator==(const Datum& other) const
	{
		if (_type == other._type && _size == other._size)
		{
			if (_type == DatumTypes::String)
			{
				for (std::size_t i = 0; i < _size; ++i)
				{
					if (_data.s[i].compare(other._data.s[i]) != 0)
					{
						return false;
					}
				}
				return true;
			}
			else if (_type == DatumTypes::Pointer || _type == DatumTypes::Table)
			{
				for (std::size_t i = 0; i < _size; ++i)
				{
					if (!(_data.r[i]->Equals(other._data.r[i])))
					{
						return false;
					}
				}
				return true;
			}
			else
			{
				std::size_t size = DatumTypeSizes[static_cast<std::size_t>(_type)];
				auto dataArr = reinterpret_cast<uint8_t*>(_data.vp);
				auto otherDataArr = reinterpret_cast<uint8_t*>(other._data.vp);
				int diff = memcmp(dataArr, otherDataArr, _size * size);
				return diff == 0;
			}
		}
		return false;
	}

	bool Datum::operator!=(const Datum& other) const
	{
		return !(operator==(other));
	}

	bool Datum::operator==(std::int32_t value)  const
	{
		return (value == FrontInt());
	}

	bool Datum::operator!=(std::int32_t value) const
	{
		return !(operator==(value));
	}

	bool Datum::operator==(float value) const
	{
		return (value == FrontFloat());
	}

	bool Datum::operator!=(float value) const
	{
		return !(operator==(value));
	}

	bool Datum::operator==(const glm::vec4& value) const
	{
		return (value == FrontVector());
	}

	bool Datum::operator!=(const glm::vec4& value) const
	{
		return !(operator==(value));
	}

	bool Datum::operator==(const glm::mat4& value) const
	{
		return (value == FrontMatrix());
	}

	bool Datum::operator!=(const glm::mat4& value) const
	{
		return !(operator==(value));
	}

	bool Datum::operator==(const std::string& value) const
	{
		return (value == FrontString());
	}

	bool Datum::operator!=(const std::string& value) const
	{
		return !(operator==(value));
	}

	bool Datum::operator==(FieaGameEngine::RTTI* const& value) const
	{
		return (FrontPointer()->Equals(value));
	}

	bool Datum::operator!=(FieaGameEngine::RTTI* const& value) const
	{
		return !(operator==(value));
	}

	bool Datum::operator==(FieaGameEngine::Scope* const& value) const
	{
		return (value == FrontTable());
	}

	bool Datum::operator!=(FieaGameEngine::Scope* const& value) const
	{
		return !(operator==(value));
	}

#pragma endregion

	DatumTypes Datum::Type() const
	{
		return _type;
	}

	void Datum::SetType(DatumTypes type)
	{
		if (_type == type)
		{
			return;
		}
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		if (_type != DatumTypes::Unknown)
		{
			throw std::runtime_error("This Datum already has a type set.");
		}
		_type = type;
	}

	std::size_t Datum::Size() const
	{
		return _size;
	}

	std::size_t Datum::Capacity() const
	{
		return _capacity;
	}

	void Datum::Reserve(std::size_t capacity)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		if (_type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Cannot modify capacity of Unknown Datum");
		}

		if (capacity > _capacity)
		{
			std::size_t size = DatumTypeSizes[static_cast<std::size_t>(_type)];
			void* data = realloc(_data.vp, capacity * size);
			assert(data != nullptr);
			_data.vp = data;
			_capacity = capacity;
		}
	}

	void Datum::Resize(std::size_t size)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		if (_type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Cannot modify capacity of Unknown Datum");
		}
		if (_type == DatumTypes::String)
		{
			if (size < _size)
			{
				while (size < _size)
				{
					PopBack();
				}
			}
			else 
			{
				Reserve(size);
				while (_size < size)
				{
					PushBack("");
				}
			}
		}
		else
		{
			if (size > _size)
			{
				Reserve(size);
			}
			_size = size;
		}
		_capacity = size;
	}

	void Datum::ShrinkToFit()
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		if (_type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Cannot modify capacity of Unknown Datum");
		}

		if (_size < _capacity)
		{
			std::size_t size = DatumTypeSizes[static_cast<std::size_t>(_type)];
			void* data = realloc(_data.vp, _size * size);
			assert(data != nullptr);
			_data.vp = data;
			_capacity = _size;
		}
	}

	void Datum::Clear()
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		if (_type == DatumTypes::String)
		{
			for (size_t i = 0; i < _size; ++i)
			{
				std::destroy_at(_data.s + i);
			}
		}
		_size = 0;
	}
#pragma region Datum SetStorage

	void Datum::SetStorage(DatumTypes type, uint8_t* values, std::size_t numElements)
	{
		if (numElements == 0 || values == nullptr)
		{
			throw std::runtime_error("Storage array cannot be empty");
		}
		if (type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Type cannot be unknown");
		}
		if (_type != DatumTypes::Unknown && _type != type)
		{
			throw std::runtime_error("Type mismatch.");
		}
		_type = type;
		_data.b = values;
		_size = numElements;
		_capacity = numElements;
		_isExternal = true;
	}

	void Datum::SetStorage(std::int32_t* values, std::size_t numElements)
	{
		if (_capacity != 0 && _isExternal == false)
		{
			throw std::runtime_error("Can't set to external storage after allocation");
		}
		SetStorage(DatumTypes::Integer, reinterpret_cast<uint8_t*>(values), numElements);
	}

	void Datum::SetStorage(float* values, std::size_t numElements)
	{
		if (_capacity != 0 && _isExternal == false)
		{
			throw std::runtime_error("Can't set to external storage after allocation");
		}
		SetStorage(DatumTypes::Float, reinterpret_cast<uint8_t*>(values), numElements);
	}

	void Datum::SetStorage(glm::vec4* values, std::size_t numElements)
	{
		if (_capacity != 0 && _isExternal == false)
		{
			throw std::runtime_error("Can't set to external storage after allocation");
		}
		SetStorage(DatumTypes::Vector, reinterpret_cast<uint8_t*>(values), numElements);
	}

	void Datum::SetStorage(glm::mat4* values, std::size_t numElements)
	{
		if (_capacity != 0 && _isExternal == false)
		{
			throw std::runtime_error("Can't set to external storage after allocation");
		}
		SetStorage(DatumTypes::Matrix, reinterpret_cast<uint8_t*>(values), numElements);
	}

	void Datum::SetStorage(std::string* values, std::size_t numElements)
	{
		if (_capacity != 0 && _isExternal == false)
		{
			throw std::runtime_error("Can't set to external storage after allocation");
		}
		SetStorage(DatumTypes::String, reinterpret_cast<uint8_t*>(values), numElements);
	}

	void Datum::SetStorage(FieaGameEngine::RTTI** values, std::size_t numElements)
	{
		if (_capacity != 0 && _isExternal == false)
		{
			throw std::runtime_error("Can't set to external storage after allocation");
		}
		SetStorage(DatumTypes::Pointer, reinterpret_cast<uint8_t*>(values), numElements);
	}

#pragma endregion

#pragma region Datum Set
	void Datum::Set(const std::int32_t value, std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		_data.i[index] = value;
	}

	void Datum::Set(const float value, std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		_data.f[index] = value;
	}

	void Datum::Set(const glm::vec4& value, std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		_data.v[index] = value;
	}

	void Datum::Set(const glm::mat4& value, std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		_data.m[index] = value;
	}

	void Datum::Set(const std::string& value, std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		_data.s[index] = value;
	}

	void Datum::Set(FieaGameEngine::RTTI* const& value, std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		_data.r[index] = value;
	}

	void Datum::Set(FieaGameEngine::Scope& value, std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		_data.t[index] = &value;
	}

#pragma endregion

#pragma region Datum Get
	std::int32_t& Datum::GetInt(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return _data.i[index];
	}

	const std::int32_t Datum::GetInt(std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return _data.i[index];
	}

	float& Datum::GetFloat(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return _data.f[index];
	}

	const float Datum::GetFloat(std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return _data.f[index];
	}

	glm::vec4& Datum::GetVector(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return _data.v[index];
	}

	const glm::vec4& Datum::GetVector(std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return _data.v[index];
	}

	glm::mat4& Datum::GetMatrix(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return _data.m[index];
	}

	const glm::mat4& Datum::GetMatrix(std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return _data.m[index];
	}

	std::string& Datum::GetString(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return _data.s[index];
	}

	const std::string& Datum::GetString(std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return _data.s[index];
	}

	FieaGameEngine::RTTI*& Datum::GetPointer(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return _data.r[index];
	}

	FieaGameEngine::RTTI* Datum::GetPointer(std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return _data.r[index];
	}

	FieaGameEngine::Scope*& Datum::GetTable(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return _data.t[index];
	}

	FieaGameEngine::Scope* Datum::GetTable(std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return _data.t[index];
	}

#pragma endregion

	void Datum::PushBackFromString(const std::string& value)
	{
		int result;
		if (_type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Cannot set an Unknown datum");
		}
		switch (_type)
		{
		case DatumTypes::Integer:
			PushBack(std::stoi(value));
			break;
		case DatumTypes::Float:
			PushBack(std::stof(value));
			break;
		case DatumTypes::Vector:
			float a, b, c, d;
			result = sscanf_s(value.c_str(), "vec4(%f, %f, %f, %f)", &a, &b, &c, &d);
			assert(result == 4);
			glm::vec4 vector = { a,b,c,d };
			PushBack(vector);
			break;
		case DatumTypes::Matrix:
			glm::mat4 matrix;
			result = sscanf_s(value.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
				&matrix[0][0], &matrix[0][1], &matrix[0][2], &matrix[0][3],
				&matrix[1][0], &matrix[1][1], &matrix[1][2], &matrix[1][3],
				&matrix[2][0], &matrix[2][1], &matrix[2][2], &matrix[2][3],
				&matrix[3][0], &matrix[3][1], &matrix[3][2], &matrix[3][3]);
			assert(result == 16);
			PushBack(matrix);
			break;
		case DatumTypes::String:
			PushBack(value);
			break;
		}
	}

	void Datum::SetFromString(const std::string& value, std::size_t index)
	{
		int result;
		if (_type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Cannot set an Unknown datum");
		}
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		switch (_type)
		{
		case DatumTypes::Integer:
			Set(std::stoi(value), index);
			break;
		case DatumTypes::Float:
			Set(std::stof(value), index);
			break;
		case DatumTypes::Vector:
			float a, b, c, d;
			result = sscanf_s(value.c_str(), "vec4(%f, %f, %f, %f)", &a, &b, &c, &d);
			assert(result == 4);
			glm::vec4 vector = { a,b,c,d };
			Set(vector, index);
			break;
		case DatumTypes::Matrix:
			glm::mat4 matrix;
			result = sscanf_s(value.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
				&matrix[0][0], &matrix[0][1], &matrix[0][2], &matrix[0][3],
				&matrix[1][0], &matrix[1][1], &matrix[1][2], &matrix[1][3], 
				&matrix[2][0], &matrix[2][1], &matrix[2][2], &matrix[2][3], 
				&matrix[3][0], &matrix[3][1], &matrix[3][2], &matrix[3][3]);
			assert(result == 16);
			Set(matrix, index);
			break;
		case DatumTypes::String:
			Set(value, index);
			break;
		}
	}

	std::string Datum::ToString(std::size_t index)
	{
		if (_type == DatumTypes::Unknown)
		{
			throw std::runtime_error("Cannot get from an Unknown datum");
		}
		if (index >= _size)
		{
			throw std::out_of_range("Index is invalid");
		}
		std::string string;
		switch (_type)
		{
		case DatumTypes::Integer:
			string = std::to_string(GetInt(index));
			break;
		case DatumTypes::Float:
			string = std::to_string(GetFloat(index));
			break;
		case DatumTypes::Vector:
			glm::vec4 vector = GetVector(index);
			string = glm::to_string(vector);
			break;
		case DatumTypes::Matrix:
			glm::mat4 matrix = GetMatrix(index);
			string = glm::to_string(matrix);
			break;
		case DatumTypes::String:
			string = GetString(index);
			break;
		}
		return string;
	}

#pragma region Datum Pushback
	void Datum::PushBack(std::int32_t value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		if (_type == DatumTypes::Unknown)
		{
			SetType(DatumTypes::Integer);
		}
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("PushBack type doesn't match");
		}
		if (_size + 1 > _capacity)
		{
			_capacity = _capacity == 0 ? 1 : _capacity * 2;
			_data.i = reinterpret_cast<int32_t*>(realloc(_data.i, _capacity * sizeof(int32_t)));
		}
		new (_data.i + _size) int32_t(value);
		++_size;
	}

	void Datum::PushBack(float value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		if (_type == DatumTypes::Unknown)
		{
			SetType(DatumTypes::Float);
		}
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("PushBack type doesn't match");
		}
		if (_size + 1 > _capacity)
		{
			_capacity = _capacity == 0 ? 1 : _capacity * 2;
			_data.f = reinterpret_cast<float*>(realloc(_data.f, _capacity * sizeof(float)));
		}
		new (_data.f + _size) float(value);
		++_size;
	}

	void Datum::PushBack(const glm::vec4& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		if (_type == DatumTypes::Unknown)
		{
			SetType(DatumTypes::Vector);
		}
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("PushBack type doesn't match");
		}
		if (_size + 1 > _capacity)
		{
			_capacity = _capacity == 0 ? 1 : _capacity * 2;
			_data.v = reinterpret_cast<glm::vec4*>(realloc(_data.v, _capacity * sizeof(glm::vec4)));
		}
		new (_data.v + _size) glm::vec4(value);
		++_size;
	}

	void Datum::PushBack(const glm::mat4& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		if (_type == DatumTypes::Unknown)
		{
			SetType(DatumTypes::Matrix);
		}
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("PushBack type doesn't match");
		}
		if (_size + 1 > _capacity)
		{
			_capacity = _capacity == 0 ? 1 : _capacity * 2;
			_data.m = reinterpret_cast<glm::mat4*>(realloc(_data.m, _capacity * sizeof(glm::mat4)));
		}
		new (_data.m + _size) glm::mat4(value);
		++_size;
	}

	void Datum::PushBack(const std::string& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		if (_type == DatumTypes::Unknown)
		{
			SetType(DatumTypes::String);
		}
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("PushBack type doesn't match");
		}
		if (_size + 1 > _capacity)
		{
			_capacity = _capacity == 0 ? 1 : _capacity * 2;
			_data.s = reinterpret_cast<std::string*>(realloc(_data.s, _capacity * sizeof(std::string)));
		}
		new (_data.s + _size) std::string(value);
		++_size;
	}

	void Datum::PushBack(FieaGameEngine::RTTI* const& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		if (_type == DatumTypes::Unknown)
		{
			SetType(DatumTypes::Pointer);
		}
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("PushBack type doesn't match");
		}
		if (_size + 1 > _capacity)
		{
			_capacity = _capacity == 0 ? 1 : _capacity * 2;
			_data.r = reinterpret_cast<FieaGameEngine::RTTI**>(realloc(_data.r, _capacity * sizeof(FieaGameEngine::RTTI)));
		}
		new (_data.r + _size) FieaGameEngine::RTTI*(value);
		++_size;
	}

	void Datum::PushBack(FieaGameEngine::Scope& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		if (_type == DatumTypes::Unknown)
		{
			SetType(DatumTypes::Table);
		}
		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("PushBack type doesn't match");
		}
		if (_size + 1 > _capacity)
		{
			_capacity = _capacity == 0 ? 1 : _capacity * 2;
			_data.t = reinterpret_cast<FieaGameEngine::Scope**>(realloc(_data.t, _capacity * sizeof(FieaGameEngine::Scope*)));
		}
		new (_data.t + _size) FieaGameEngine::Scope* (&value);
		++_size;
	}
#pragma endregion

	void Datum::PopBack()
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		if (_size == 0)
		{
			throw std::runtime_error("Can't Pop empty Datum");
		}
		if (_type == DatumTypes::String)
		{
			std::destroy_at(_data.s + _size - 1);
		}
		--_size;
	}

#pragma region Datum Front
	std::int32_t& Datum::FrontInt()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.i[0];
	}

	const std::int32_t& Datum::FrontInt() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.i[0];
	}

	float& Datum::FrontFloat()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.f[0];
	}

	const float& Datum::FrontFloat() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.f[0];
	}

	glm::vec4& Datum::FrontVector()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.v[0];
	}

	const glm::vec4& Datum::FrontVector() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.v[0];
	}

	glm::mat4& Datum::FrontMatrix()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.m[0];
	}

	const glm::mat4& Datum::FrontMatrix() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.m[0];
	}

	std::string& Datum::FrontString()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.s[0];
	}

	const std::string& Datum::FrontString() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.s[0];
	}

	FieaGameEngine::RTTI*& Datum::FrontPointer() 
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.r[0];
	}

	FieaGameEngine::RTTI* const& Datum::FrontPointer() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.r[0];
	}

	FieaGameEngine::Scope*& Datum::FrontTable()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.t[0];
	}

	FieaGameEngine::Scope* const& Datum::FrontTable() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.t[0];
	}

#pragma endregion

#pragma region Datum Back

	std::int32_t& Datum::BackInt()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.i[_size - 1];
	}

	const std::int32_t& Datum::BackInt() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Integer)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.i[_size - 1];
	}

	float& Datum::BackFloat()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.f[_size - 1];
	}

	const float& Datum::BackFloat() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Float)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.f[_size - 1];
	}

	glm::vec4& Datum::BackVector()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.v[_size - 1];
	}

	const glm::vec4& Datum::BackVector() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Vector)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.v[_size - 1];
	}

	glm::mat4& Datum::BackMatrix()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.m[_size - 1];
	}

	const glm::mat4& Datum::BackMatrix() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Matrix)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.m[_size - 1];
	}

	std::string& Datum::BackString()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.s[_size - 1];
	}

	const std::string& Datum::BackString() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::String)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.s[_size - 1];
	}

	FieaGameEngine::RTTI*& Datum::BackPointer()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.r[_size - 1];
	}

	FieaGameEngine::RTTI* const& Datum::BackPointer() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Pointer)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.r[_size - 1];
	}

	FieaGameEngine::Scope*& Datum::BackTable()
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.t[_size - 1];
	}

	FieaGameEngine::Scope* const& Datum::BackTable() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("Cannot access an empty datum");
		}
		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Type does not match datum type");
		}
		return _data.t[_size - 1];
	}

#pragma endregion

#pragma region Datum Find

	std::pair<bool, std::size_t> Datum::Find(const std::int32_t& value) const
	{
		if (_type != DatumTypes::Integer)
		{
			return std::pair<bool, std::size_t>(false, _size);
		}
		size_t index = 0;
		while (index != _size)
		{
			if (_data.i[index] == value)
			{
				return std::pair<bool, std::size_t>(true, index);
			}
			++index;
		}
		return std::pair<bool, std::size_t>(false, index);
	}

	std::pair<bool, std::size_t> Datum::Find(const float& value) const
	{
		if (_type != DatumTypes::Float)
		{
			return std::pair<bool, std::size_t>(false, _size);
		}
		size_t index = 0;
		while (index != _size)
		{
			if (_data.f[index] == value)
			{
				return std::pair<bool, std::size_t>(true, index);
			}
			++index;
		}
		return std::pair<bool, std::size_t>(false, index);
	}

	std::pair<bool, std::size_t> Datum::Find(const glm::vec4& value) const
	{
		if (_type != DatumTypes::Vector)
		{
			return std::pair<bool, std::size_t>(false, _size);
		}
		size_t index = 0;
		while (index != _size)
		{
			if (_data.v[index] == value)
			{
				return std::pair<bool, std::size_t>(true, index);
			}
			++index;
		}
		return std::pair<bool, std::size_t>(false, index);
	}

	std::pair<bool, std::size_t> Datum::Find(const glm::mat4& value) const
	{
		if (_type != DatumTypes::Matrix)
		{
			return std::pair<bool, std::size_t>(false, _size);
		}
		size_t index = 0;
		while (index != _size)
		{
			if (_data.m[index] == value)
			{
				return std::pair<bool, std::size_t>(true, index);
			}
			++index;
		}
		return std::pair<bool, std::size_t>(false, index);
	}

	std::pair<bool, std::size_t> Datum::Find(const std::string& value) const
	{
		if (_type != DatumTypes::String)
		{
			return std::pair<bool, std::size_t>(false, _size);
		}
		size_t index = 0;
		while (index != _size)
		{
			if (_data.s[index] == value)
			{
				return std::pair<bool, std::size_t>(true, index);
			}
			++index;
		}
		return std::pair<bool, std::size_t>(false, index);
	}

	std::pair<bool, std::size_t> Datum::Find(FieaGameEngine::RTTI* const& value) const
	{
		if (_type != DatumTypes::Pointer)
		{
			return std::pair<bool, std::size_t>(false, _size);
		}
		size_t index = 0;
		while (index != _size)
		{
			if (_data.r[index] == value)
			{
				return std::pair<bool, std::size_t>(true, index);
			}
			++index;
		}
		return std::pair<bool, std::size_t>(false, index);
	}

	std::pair<bool, std::size_t> Datum::Find(FieaGameEngine::Scope& value) const
	{
		if (_type != DatumTypes::Table)
		{
			return std::pair<bool, std::size_t>(false, _size);
		}
		size_t index = 0;
		while (index != _size)
		{
			if (_data.t[index] == &value)
			{
				return std::pair<bool, std::size_t>(true, index);
			}
			++index;
		}
		return std::pair<bool, std::size_t>(false, index);
	}

	std::pair<bool, std::size_t> Datum::Find(const FieaGameEngine::Scope& value) const
	{
		if (_type != DatumTypes::Table)
		{
			return std::pair<bool, std::size_t>(false, _size);
		}
		size_t index = 0;
		while (index != _size)
		{
			if (_data.t[index] == &value)
			{
				return std::pair<bool, std::size_t>(true, index);
			}
			++index;
		}
		return std::pair<bool, std::size_t>(false, index);
	}

#pragma endregion

#pragma region Datum Remove

	bool Datum::RemoveAt(std::size_t index)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		if (index < _size && _size > 0)
		{
			if (_type == DatumTypes::String)
			{
				std::destroy_at(_data.s + index);
			}
			std::size_t size = DatumTypeSizes[static_cast<std::size_t>(_type)];
			auto dataArr = reinterpret_cast<uint8_t*>(_data.vp);
			memmove(dataArr + (size * index), dataArr + (size * (index + 1)), (_size - index) * size);
			--_size;
			return true;
		}
		return false;
		
	}

	bool Datum::Remove(const std::int32_t& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		std::pair<bool, std::size_t> result = Find(value);
		if (result.first)
		{
			return RemoveAt(result.second);
		}
		return false;
	}

	bool Datum::Remove(const float& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		std::pair<bool, std::size_t> result = Find(value);
		if (result.first)
		{
			return RemoveAt(result.second);
		}
		return false;
	}

	bool Datum::Remove(const glm::vec4& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		std::pair<bool, std::size_t> result = Find(value);
		if (result.first)
		{
			return RemoveAt(result.second);
		}
		return false;
	}

	bool Datum::Remove(const glm::mat4& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		std::pair<bool, std::size_t> result = Find(value);
		if (result.first)
		{
			return RemoveAt(result.second);
		}
		return false;
	}

	bool Datum::Remove(const std::string& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		std::pair<bool, std::size_t> result = Find(value);
		if (result.first)
		{
			return RemoveAt(result.second);
		}
		return false;
	}

	bool Datum::Remove(FieaGameEngine::RTTI* const& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		std::pair<bool, std::size_t> result = Find(value);
		if (result.first)
		{
			return RemoveAt(result.second);
		}
		return false;
	}

	bool Datum::Remove(FieaGameEngine::Scope& value)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot perform operation on external storage.");
		}
		std::pair<bool, std::size_t> result = Find(value);
		if (result.first)
		{
			return RemoveAt(result.second);
		}
		return false;
	}
#pragma endregion


	Scope& Datum::operator[](std::size_t index)
	{
		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return *GetTable(index);
	}

	Scope& Datum::operator[](std::size_t index) const
	{
		if (_type != DatumTypes::Table)
		{
			throw std::runtime_error("Datum type doesn't match");
		}
		return *GetTable(index);
	}
}