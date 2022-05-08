#include "pch.h"
#include "IJsonParseHelper.h"
#include "JsonParseCoordinator.h"

using namespace std::string_literals;

namespace FieaGameEngine
{
#pragma region Wrapper

	RTTI_DEFINITIONS(JsonParseCoordinator::Wrapper);
	void JsonParseCoordinator::Wrapper::Initialize()
	{
		_depth = 0;
	}

	void JsonParseCoordinator::Wrapper::SetJsonParseCoordinator(JsonParseCoordinator& parseCoordinator)
	{
		_parseCoordinator = &parseCoordinator;
	}

	JsonParseCoordinator* JsonParseCoordinator::Wrapper::GetJsonParseCoordinator()
	{
		return _parseCoordinator;
	}

	const JsonParseCoordinator* JsonParseCoordinator::Wrapper::GetJsonParseCoordinator() const
	{
		return _parseCoordinator;
	}

	void JsonParseCoordinator::Wrapper::IncrementDepth()
	{
		++_depth;
	}

	void JsonParseCoordinator::Wrapper::DecrementDepth()
	{
		if (_depth > 0)
		{
			--_depth;
		}
	}

	std::uint32_t JsonParseCoordinator::Wrapper::Depth() const
	{
		return _depth;
	}
#pragma endregion

#pragma region JsonParseCoordinator
	JsonParseCoordinator::JsonParseCoordinator(Wrapper& wrapper) :
		_wrapper{ &wrapper }
	{
		_wrapper->Initialize();
		_wrapper->SetJsonParseCoordinator(*this);
	}

	JsonParseCoordinator::~JsonParseCoordinator()
	{
		if (_isClone)
		{
			delete _wrapper;
			for (size_t i = 0; i < _helpers.Size(); ++i)
			{
				delete _helpers[i];
			}
		}
	}

	JsonParseCoordinator* JsonParseCoordinator::Clone()
	{
		JsonParseCoordinator* clone = new JsonParseCoordinator(*_wrapper->Create());
		for (auto& helper : _helpers)
		{
			clone->_helpers.PushBack(helper->Create());
		}
		clone->_isClone = true;
		return clone;
	}

	JsonParseCoordinator::JsonParseCoordinator(JsonParseCoordinator&& other) noexcept :
		_helpers{ std::move(other._helpers) }, _wrapper{ std::move(other._wrapper) }, _isClone{ other._isClone }
	{
		_wrapper->SetJsonParseCoordinator(*this);
	}

	JsonParseCoordinator& JsonParseCoordinator::operator=(JsonParseCoordinator&& other) noexcept
	{
		_helpers = std::move(other._helpers);
		_wrapper = std::move(other._wrapper);
		_isClone = other._isClone;
		_wrapper->SetJsonParseCoordinator(*this);
		return *this;
	}

	void JsonParseCoordinator::Initialize()
	{
		_wrapper->Initialize();
		for (auto& helper : _helpers)
		{
			helper->Initialize();
		}
	}

	void JsonParseCoordinator::Cleanup()
	{
		for (auto& helper : _helpers)
		{
			helper->Cleanup();
		}
	}

	void JsonParseCoordinator::AddHelper(IJsonParseHelper& insertHelper)
	{
		assert(_isClone == false);
		for (auto& helper : _helpers)
		{
			if (helper->TypeIdInstance() == insertHelper.TypeIdInstance())
			{
				throw std::runtime_error("Can't insert duplicate helper.");
			}
		}
		_helpers.PushBack(&insertHelper);
	}

	void JsonParseCoordinator::RemoveHelper(IJsonParseHelper& helper)
	{
		assert(_isClone == false);
		_helpers.Remove(&helper);
	}
	void JsonParseCoordinator::Parse(const std::string& value)
	{
		std::stringstream stream;
		stream << value;
		Parse(stream);
	}

	void JsonParseCoordinator::ParseFromFile(std::string filename)
	{
		std::ifstream inputStream(filename);
		if (!inputStream.good())
		{
			throw std::runtime_error("Couldn't open file");
		}
		_filename = std::move(filename);
		Parse(inputStream);
	}

	void JsonParseCoordinator::Parse(std::istream& stream)
	{
		Json::Value jsonObj;
		stream >> jsonObj;
		_wrapper->IncrementDepth();
		ParseMembers(jsonObj);
		_wrapper->DecrementDepth();
		Cleanup();
	}

	const std::string& JsonParseCoordinator::GetFileName() const
	{
		return _filename;
	}

	JsonParseCoordinator::Wrapper& JsonParseCoordinator::GetWrapper()
	{
		return *_wrapper;
	}

	const JsonParseCoordinator::Wrapper& JsonParseCoordinator::GetWrapper() const
	{
		return *_wrapper;
	}

	void JsonParseCoordinator::SetWrapper(Wrapper& wrapper)
	{
		assert(_isClone == false);
		_wrapper = &wrapper;
		_wrapper->Initialize();
		_wrapper->SetJsonParseCoordinator(*this);
	}

	bool JsonParseCoordinator::IsClone() const
	{
		return _isClone;
	}

	Vector<IJsonParseHelper*>& JsonParseCoordinator::Helpers()
	{
		return _helpers;
	}

	void JsonParseCoordinator::ParseMembers(const Json::Value& value)
	{
		if (value.size() > 0)
		{
			const auto keys = value.getMemberNames();
			for (const auto& key : keys)
			{
				Parse(key, value[key], 0);
			}
		}
	}

	void JsonParseCoordinator::Parse(const std::string& key, const Json::Value& value, size_t arrIndex)
	{
		if (value.isObject())
		{
			_wrapper->IncrementDepth();
			for (size_t i = 0; i < _helpers.Size(); ++i)
			{
				if (_helpers[i]->StartHandler(*_wrapper, key, value, arrIndex))
				{
					ParseMembers(value);
					_helpers[i]->EndHandler(*_wrapper, key, value);
					break;
				}
			}
			_wrapper->DecrementDepth();
		}
		else if (value.isArray())
		{
			size_t index = 0;
			for (auto& jsonValue : value)
			{
				if (jsonValue.isObject())
				{
					_wrapper->IncrementDepth();
					ParseMembers(jsonValue);
					_wrapper->DecrementDepth();
				}
				else
				{
					Parse(key, jsonValue, index);
				}
				++index;
			}
		}
		else
		{
			for (size_t i = 0; i < _helpers.Size(); ++i)
			{
				if (_helpers[i]->StartHandler(*_wrapper, key, value, arrIndex))
				{
					_helpers[i]->EndHandler(*_wrapper, key, value);
					break;
				}
			}
		}
	}

#pragma endregion
}