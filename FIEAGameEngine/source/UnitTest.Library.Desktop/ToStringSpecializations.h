#pragma once

#include "CppUnitTest.h"
#include "CppUnitTestAssert.h"
#include "Foo.h"
#include "SList.h"
#include "Vector.h"
#include "Hashmap.h"
#include "Datum.h"
#include "Scope.h"
#include "AttributedBar.h"
#include "Avatar.h"
#include "JsonParseCoordinator.h"
#include "JsonIntegerParseHelper.h"
#include "JsonTestParseHelper.h"
#include "JsonTableParseHelper.h"
#include "ActionIncrement.h"
#include "ActionListIf.h"
#include "EventQueue.h"
#include "Event.h"
#include "EventSubscribers.h"

using namespace UnitTestLibraryDesktop;
using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<EventPublisher>(EventPublisher* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Event<Foo>>(Event<Foo>* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Action>(Action* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<ActionList>(ActionList* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<ActionList>(const ActionList& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<GameObject>(GameObject* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<GameObject>(const GameObject* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<GameObject>(const GameObject& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<JsonParseCoordinator>(JsonParseCoordinator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonParseCoordinator>(const JsonParseCoordinator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonParseCoordinator::Wrapper>(JsonParseCoordinator::Wrapper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonParseCoordinator::Wrapper>(const JsonParseCoordinator::Wrapper& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<JsonParseCoordinator::Wrapper>(const JsonParseCoordinator::Wrapper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<IJsonParseHelper>(IJsonParseHelper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonTestParseHelper>(JsonTestParseHelper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonTestParseHelper::Wrapper>(JsonTestParseHelper::Wrapper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonTestParseHelper::Wrapper>(const JsonTestParseHelper::Wrapper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonTableParseHelper>(JsonTableParseHelper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonTableParseHelper::Wrapper>(JsonTableParseHelper::Wrapper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<JsonTableParseHelper::Wrapper>(const JsonTableParseHelper::Wrapper* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Foo>(const Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<Foo>(const Foo* t)
	{
		RETURN_WIDE_STRING(t->Data());
	}

	template<>
	inline std::wstring ToString<Foo>(Foo* t)
	{
		RETURN_WIDE_STRING(t->Data());
	}

	template<>
	inline std::wstring ToString<SList<Foo>::Iterator>(const SList<Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<SList<Foo>::ConstIterator>(const SList<Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Hashmap<Foo, int>::PairType>(const Hashmap<Foo, int>::PairType& t)
	{
		try
		{
			return ToString(t.first) + L","s + ToString(t.second);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Hashmap<Foo, int>::Iterator>(const Hashmap<Foo, int>::Iterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Hashmap<Foo, int>::ConstIterator>(const Hashmap<Foo, int>::ConstIterator& t)
	{
		try
		{
			return ToString(*t);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString <enum DatumTypes>(const enum DatumTypes& t)
	{
		t;
		return L"end()"s;
	}

	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
	{
		try
		{
			return ToString(t[0]) + L","s + ToString(t[1]) + L","s + ToString(t[2]) + L","s + ToString(t[3]);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& t)
	{
		try
		{
			return ToString(t[0][0]) + L","s + ToString(t[0][1]) + L","s + ToString(t[0][2]) + L","s + ToString(t[0][3]) + L","s +
				ToString(t[1][0]) + L","s + ToString(t[1][1]) + L","s + ToString(t[1][2]) + L","s + ToString(t[1][3]) + L","s +
				ToString(t[2][0]) + L","s + ToString(t[2][1]) + L","s + ToString(t[2][2]) + L","s + ToString(t[2][3]) + L","s +
				ToString(t[3][0]) + L","s + ToString(t[3][1]) + L","s + ToString(t[3][2]) + L","s + ToString(t[3][3]);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::RTTI>(FieaGameEngine::RTTI* t)
	{
		try
		{
			t;
			return L"end()"s;
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}

	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Scope>(FieaGameEngine::Scope* t)
	{
		try
		{
			t;
			return L"end()"s;
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::Datum>(FieaGameEngine::Datum* t)
	{
		try
		{
			t;
			return L"end()"s;
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(AttributedFoo* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(const AttributedFoo* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<AttributedBar>(AttributedBar* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<AttributedBar>(const AttributedBar* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(const AttributedFoo& t)
	{
		try
		{
			RETURN_WIDE_STRING(t.ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	inline std::wstring ToString<Attributed>(Attributed* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}
}