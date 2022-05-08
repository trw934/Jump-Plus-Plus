#include "pch.h"
#include "CppUnitTest.h"
#include "ToStringSpecializations.h"
#include "Foo.h"
#include <crtdbg.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FooTests)
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
			const int data = 10;

			Foo f{ data };
			Assert::AreEqual(data, f.Data());
		}

		TEST_METHOD(CopyConstructor)
		{
			// Testing copy constructor
			const int data = 10;
			Foo f{ data };
			Foo g{ f };
			Assert::AreEqual(f, g);

			// Testing assignment operator
			Foo h = f;
			Assert::AreEqual(f, h);
		}

		TEST_METHOD(Data)
		{
			Foo f{ 10 };
			Assert::AreEqual(f.Data(), 10);
			int& data = f.Data();
			data = 20;
			Assert::AreEqual(f.Data(), 20);

			const Foo& g = f;
			Assert::AreEqual(g.Data(), 20);
		}

		TEST_METHOD(EqualOperator)
		{
			int data = 10;
			Foo f{ data };
			Foo g{ data };
			Assert::AreNotSame(f, g);

			Assert::IsTrue(f == f);
			Assert::IsTrue(f == g);

			g.Data() = 20;
			Assert::IsTrue(f != g);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState FooTests::_startMemState;
}