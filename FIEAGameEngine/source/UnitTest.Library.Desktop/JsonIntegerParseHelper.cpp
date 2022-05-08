#include "pch.h"
#include "JsonIntegerParseHelper.h"

using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(JsonIntegerParseHelper);

	RTTI_DEFINITIONS(JsonIntegerParseHelper::Wrapper);

	void JsonIntegerParseHelper::Wrapper::Initialize()
	{
		JsonParseCoordinator::Wrapper::Initialize();
	}

	gsl::owner<JsonIntegerParseHelper::Wrapper*> JsonIntegerParseHelper::Wrapper::Create() const
	{
		return new Wrapper();
	}

	gsl::owner<IJsonParseHelper*> JsonIntegerParseHelper::Create() const
	{
		return new JsonIntegerParseHelper(*this);
	}

	void JsonIntegerParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
		_parsingData = false;
	}

	bool JsonIntegerParseHelper::StartHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value& value, size_t)
	{
		JsonIntegerParseHelper::Wrapper* customWrapper = wrapper.As<JsonIntegerParseHelper::Wrapper>();
		if (customWrapper == nullptr)
		{
			return false;
		}

		if (key != IntegerKey || value.isArray())
		{
			return false;
		}

		if (_parsingData)
		{
			throw runtime_error("Received another start element while already parsing an integer");
		}

		_parsingData = true;

		customWrapper->_data.PushBack(value.asInt());

		return true;
	}

	bool JsonIntegerParseHelper::EndHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string& key, const Json::Value&)
	{
		JsonIntegerParseHelper::Wrapper* customWrapper = wrapper.As<JsonIntegerParseHelper::Wrapper>();
		if (customWrapper == nullptr || key != IntegerKey || _parsingData == false)
		{
			return false;
		}

		_parsingData = false;
		return true;
	}
}