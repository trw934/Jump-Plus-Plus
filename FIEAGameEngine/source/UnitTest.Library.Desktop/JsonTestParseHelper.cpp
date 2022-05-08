#include "pch.h"
#include "JsonTestParseHelper.h"

using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(JsonTestParseHelper);

	RTTI_DEFINITIONS(JsonTestParseHelper::Wrapper);

	void JsonTestParseHelper::Wrapper::Initialize()
	{
		JsonParseCoordinator::Wrapper::Initialize();
	}

	gsl::owner<JsonTestParseHelper::Wrapper*> JsonTestParseHelper::Wrapper::Create() const
	{
		return new Wrapper();
	}

	gsl::owner<IJsonParseHelper*> JsonTestParseHelper::Create() const
	{
		return new JsonTestParseHelper(*this);
	}

	void JsonTestParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
		InitializeCalled = true;
		StartHandlerCount = 0;
		EndHandlerCount = 0;
	}

	bool JsonTestParseHelper::StartHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string&, const Json::Value& value, size_t)
	{
		JsonTestParseHelper::Wrapper* customWrapper = wrapper.As<JsonTestParseHelper::Wrapper>();
		if (customWrapper == nullptr)
		{
			return false;
		}

		++StartHandlerCount;
		
		if (value.isArray())
		{
			++ArrayElementCount;
		}
		
		if (customWrapper->Depth() > customWrapper->MaxDepth)
		{
			customWrapper->MaxDepth = customWrapper->Depth();
		}

		return true;
	}

	bool JsonTestParseHelper::EndHandler(JsonParseCoordinator::Wrapper& wrapper, const std::string&, const Json::Value&)
	{
		JsonTestParseHelper::Wrapper* customWrapper = wrapper.As<JsonTestParseHelper::Wrapper>();
		if (customWrapper == nullptr)
		{
			return false;
		}
		++EndHandlerCount;
		return true;
	}
}