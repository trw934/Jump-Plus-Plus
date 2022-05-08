#include "pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "Foo.h"
#include "RTTI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTestLibraryDesktop;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<UnitTestLibraryDesktop::FooFactory>(const UnitTestLibraryDesktop::FooFactory* t)
	{
		RETURN_WIDE_STRING(t);
	}
}

namespace UnitTestLibraryDesktop 
{
	TEST_CLASS(FactoryTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(Constructor)
		{
			Assert::AreEqual(size_t(0), Factory<RTTI>::Size());
		}

		TEST_METHOD(AddRemoveFactory)
		{
			const Factory<RTTI>* foundFactory = Factory<RTTI>::Find("Foo"s);
			Assert::IsNull(foundFactory);
			{
				const FooFactory fooFactory;
				Assert::AreEqual(size_t(1), Factory<RTTI>::Size());
				foundFactory = Factory<RTTI>::Find("Foo"s);
				Assert::AreEqual(&fooFactory, static_cast<const FooFactory*>(foundFactory));
				Factory<RTTI>::Clear();
				Assert::AreEqual(size_t(0), Factory<RTTI>::Size());
			}

			{
				const FooFactory fooFactory;
				Assert::AreEqual(size_t(1), Factory<RTTI>::Size());
				foundFactory = Factory<RTTI>::Find("Foo"s);
				Assert::AreEqual(&fooFactory, static_cast<const FooFactory*>(foundFactory));
			}

			Assert::AreEqual(size_t(0), Factory<RTTI>::Size());
			foundFactory = Factory<RTTI>::Find("Foo"s);
			Assert::IsNull(foundFactory);
		}

		TEST_METHOD(ProductCreation)
		{
			RTTI* rtti = Factory<RTTI>::Create("Foo"s);
			Assert::IsNull(rtti);

			{
				FooFactory fooFactory;

				rtti = Factory<RTTI>::Create("Foo"s);
				Assert::IsNotNull(rtti);

				Foo* foo = rtti->As<Foo>();
				Assert::IsNotNull(foo);

				delete foo;
			}

			rtti = Factory<RTTI>::Create("Foo"s);
			Assert::IsNull(rtti);
		}

	private:
		static _CrtMemState _startMemState;
	};
	_CrtMemState FactoryTests::_startMemState;
}