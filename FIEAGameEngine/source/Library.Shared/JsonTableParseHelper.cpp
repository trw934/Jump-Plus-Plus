#include "pch.h"
#include "JsonTableParseHelper.h"

using namespace std;
using namespace std::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonTableParseHelper);

	RTTI_DEFINITIONS(JsonTableParseHelper::Wrapper);

	JsonTableParseHelper::Wrapper::Wrapper(Scope& scope) :
		_scope{ &scope }
	{
	}

	void JsonTableParseHelper::Wrapper::Initialize()
	{
		JsonParseCoordinator::Wrapper::Initialize();
	}

	gsl::owner<JsonTableParseHelper::Wrapper*> JsonTableParseHelper::Wrapper::Create() const
	{
		return new Wrapper(*_scope);
	}

	gsl::owner<IJsonParseHelper*> JsonTableParseHelper::Create() const
	{
		return new JsonTableParseHelper(*this);
	}

	void JsonTableParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
		_typeMap.Insert(std::make_pair("integer", DatumTypes::Integer));
		_typeMap.Insert(std::make_pair("float", DatumTypes::Float));
		_typeMap.Insert(std::make_pair("vector", DatumTypes::Vector));
		_typeMap.Insert(std::make_pair("matrix", DatumTypes::Matrix));
		_typeMap.Insert(std::make_pair("string", DatumTypes::String));
		_typeMap.Insert(std::make_pair("table", DatumTypes::Table));
	}

	void JsonTableParseHelper::Cleanup()
	{
		IJsonParseHelper::Cleanup();
		_typeMap.Clear();
		_context.Clear();
	}

	bool JsonTableParseHelper::StartHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value, size_t arrIndex)
	{
		JsonTableParseHelper::Wrapper* customWrapper = wrapper.As<JsonTableParseHelper::Wrapper>();
		if (customWrapper == nullptr)
		{
			return false;
		}
		
		if (key == ClassKey)
		{
			_context.Top()._className = value.asString();
		}
		else if (key == TypeKey)
		{
			auto iter = _typeMap.Find(value.asString());
			if (iter == _typeMap.end())
			{
				throw std::runtime_error("Invalid type encountered while parsing.");
			}
			DatumTypes datumType = iter->second;
			
			Datum& datum = _context.Top()._scope->Append(_context.Top()._key);
			datum.SetType(datumType);
		}
		else if (key == ValueKey)
		{
			Datum& datum = _context.Top()._scope->operator[](_context.Top()._key);
			if (datum.Type() != DatumTypes::Table)
			{
				if (datum.IsExternal())
				{
					datum.SetFromString(value.asString(), arrIndex);
				}
				else
				{
					datum.PushBackFromString(value.asString());
				}
			}
			else
			{
				Scope* newScope = Factory<Scope>::Create(_context.Top()._className);
				_context.Top()._scope->Adopt(*newScope, _context.Top()._key);
				_context.Push(ContextFrame(_context.Top()._key, _context.Top()._className, *newScope));
			}
		} 
		else
		{
			if (_context.IsEmpty())
			{
				_context.Push(ContextFrame(key, "", *customWrapper->_scope));
			}
			else
			{
				_context.Push(ContextFrame(key, "", *_context.Top()._scope));
			}
		}
		return true;
	}

	bool JsonTableParseHelper::EndHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string&, const Json::Value& value)
	{
		JsonTableParseHelper::Wrapper* customWrapper = wrapper.As<JsonTableParseHelper::Wrapper>();
		if (customWrapper == nullptr)
		{
			return false;
		}
		if (value.isObject())
		{
			_context.Pop();
		}
		return true;
	}
}