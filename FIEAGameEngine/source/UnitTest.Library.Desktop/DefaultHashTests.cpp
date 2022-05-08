#include "pch.h"
#include "CppUnitTest.h"
#include "ToStringSpecializations.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTestLibraryDesktop;
using namespace std;

namespace FieaGameEngine
{
	template <>
	class DefaultHash<Foo>
	{
	public:
		std::size_t operator()(const Foo& key) const
		{
			return abs(key.Data());
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DefaultHashTests)
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

		TEST_METHOD(IntegerHash)
		{
			int a = 10;
			int b = 20;
			int c = 10;
			DefaultHash<int> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

		TEST_METHOD(StringHash)
		{
			string a = "Hello"s;
			string b = "Goodbye"s;
			string c(a);
			DefaultHash<string> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

		TEST_METHOD(CStringHash)
		{
			const char* a = "Hello";
			const char* b = "Goodbye";
			char c[6];
			strcpy_s(c, a);
			DefaultHash<char*> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

		TEST_METHOD(FooHash)
		{
			Foo a(10);
			Foo b(20);
			Foo c(10);
			DefaultHash<Foo> hashFunc;

			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

	private:
		static _CrtMemState _startMemState;
	};
	_CrtMemState DefaultHashTests::_startMemState;
}