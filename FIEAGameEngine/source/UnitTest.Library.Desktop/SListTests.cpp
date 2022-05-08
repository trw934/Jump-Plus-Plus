#include "pch.h"
#include "CppUnitTest.h"
#include "SList.h"
#include "ToStringSpecializations.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(SListTests)
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
			SList<Foo> list;
			Assert::AreEqual(size_t(0), list.Size());
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(CopyConstructor)
		{
			// Testing copy constructor
			SList<Foo> list;
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			list.PushFront(a);
			list.PushFront(b);
			list.PushFront(c);
			SList<Foo> list2{ list };
			Assert::AreEqual(list.Size(), list2.Size());
			while (!list.IsEmpty())
			{
				Assert::AreEqual(list.Front(), list2.Front());
				list.PopFront();
				list2.PopFront();
			}

			// Testing copy assignment operator
			list.PushFront(a);
			list.PushFront(b);
			list.PushFront(c);
			list2 = list;
			Assert::AreEqual(list.Size(), list2.Size());
			while (!list.IsEmpty())
			{
				Assert::AreEqual(list.Front(), list2.Front());
				list.PopFront();
				list2.PopFront();
			}
		}

		TEST_METHOD(Front)
		{
			SList<Foo> list;
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };

			auto expression = [&list]() { list.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushFront(a);
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());

			list.PushFront(b);
			Assert::AreEqual(b, list.Front());
			Assert::AreEqual(a, list.Back());

			list.PushFront(c);
			Assert::AreEqual(c, list.Front());
			Assert::AreEqual(a, list.Back());
		}

		TEST_METHOD(ConstFront)
		{
			SList<Foo> list;
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };

			list.PushFront(a);
			list.PushFront(b);
			list.PushFront(c);

			const SList<Foo> emptyConstList;

			auto expression = [&emptyConstList]() { emptyConstList.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);

			const SList<Foo> constList{ list };
			Assert::AreEqual(size_t(3), constList.Size());
			Assert::IsFalse(constList.IsEmpty());
			Assert::AreEqual(c, constList.Front());
		}

		TEST_METHOD(Back)
		{
			SList<Foo> list;
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };

			auto expression = [&list]() { list.Back(); };
			Assert::ExpectException<std::runtime_error>(expression);

			list.PushBack(a);
			Assert::AreEqual(a, list.Back());
			Assert::AreEqual(a, list.Front());

			list.PushBack(b);
			Assert::AreEqual(b, list.Back());
			Assert::AreEqual(a, list.Front());

			list.PushBack(c);
			Assert::AreEqual(c, list.Back());
			Assert::AreEqual(a, list.Front());
		}

		TEST_METHOD(ConstBack)
		{
			SList<Foo> list;
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };

			list.PushFront(a);
			list.PushFront(b);
			list.PushFront(c);

			const SList<Foo> emptyConstList;

			auto expression = [&emptyConstList]() { emptyConstList.Back(); };
			Assert::ExpectException<std::runtime_error>(expression);

			const SList<Foo> constList{ list };
			Assert::AreEqual(size_t(3), constList.Size());
			Assert::IsFalse(constList.IsEmpty());
			Assert::AreEqual(a, constList.Back());
		}

		TEST_METHOD(PushFront)
		{
			SList<Foo> list;
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };

			Assert::IsTrue(list.IsEmpty());
			list.PushFront(a);
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());
			Assert::AreSame(list.Front(), list.Back());

			list.PushFront(b);
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(b, list.Front());
			Assert::AreEqual(a, list.Back());

			list.PushFront(c);
			Assert::AreEqual(size_t(3), list.Size());
			Assert::AreEqual(c, list.Front());
			Assert::AreEqual(a, list.Back());
		}

		TEST_METHOD(PushBack)
		{
			SList<Foo> list;
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };

			Assert::IsTrue(list.IsEmpty());
			list.PushBack(a);
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());
			Assert::AreSame(list.Front(), list.Back());

			list.PushBack(b);
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(b, list.Back());

			list.PushBack(c);
			Assert::AreEqual(size_t(3), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(c, list.Back());
		}

		TEST_METHOD(PopFront)
		{
			SList<Foo> list;
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };

			Assert::IsFalse(list.PopFront());

			list.PushFront(a);
			list.PushFront(b);
			list.PushFront(c);

			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(size_t(3), list.Size());
			Assert::AreEqual(c, list.Front());
			Assert::AreEqual(a, list.Back());

			Assert::IsTrue(list.PopFront());
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(b, list.Front());
			Assert::AreEqual(a, list.Back());

			Assert::IsTrue(list.PopFront());
			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());

			Assert::IsTrue(list.PopFront());
			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual(size_t(0), list.Size());
			
		}
		
		TEST_METHOD(PopBack)
		{
			SList<Foo> list;
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };

			Assert::IsFalse(list.PopBack());

			list.PushFront(a);
			list.PushFront(b);
			list.PushFront(c);

			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(size_t(3), list.Size());
			Assert::AreEqual(c, list.Front());
			Assert::AreEqual(a, list.Back());

			Assert::IsTrue(list.PopBack());
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(c, list.Front());
			Assert::AreEqual(b, list.Back());

			Assert::IsTrue(list.PopBack());
			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(c, list.Front());
			Assert::AreEqual(c, list.Back());

			Assert::IsTrue(list.PopBack());
			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual(size_t(0), list.Size());
		}

		TEST_METHOD(Clear)
		{
			SList<Foo> list;
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			list.PushFront(a);
			list.PushFront(b);
			list.PushFront(c);

			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(size_t(3), list.Size());
			
			list.Clear();
			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual(size_t(0), list.Size());
		}

		TEST_METHOD(Iterator)
		{
			{
				SList<Foo> list;
				Assert::AreEqual(list.begin(), list.end());

				const Foo a{ 10 };
				list.PushBack(a);
				Assert::IsTrue(list.begin() != list.end());
			}

			{
				const Foo a = { 15 };
				const Foo b = { 3 };
				const Foo c = { 7 };
				SList<Foo> list = { a,b,c };

				auto iter1 = list.begin();
				SList<Foo>::Iterator iter2;
				Assert::IsFalse(iter1 == iter2);

				size_t count = 0;
				for (auto iter = list.begin(); iter != list.end(); ++iter, ++count)
				{
					;
				}
				Assert::AreEqual(list.Size(), count);

				count = 0;
				for (const Foo& value : list)
				{
					value;
					count++;
				}
				Assert::AreEqual(list.Size(), count);
			}
		}
		
		TEST_METHOD(ConstIterator)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			const SList<Foo> list = { a,b,c };

			auto iter1 = list.begin();
			SList<Foo>::ConstIterator iter2;
			Assert::IsFalse(iter1 == iter2);

			size_t count = 0;
			for (auto iter = list.begin(); iter != list.end(); ++iter, ++count)
			{
				;
			}
			Assert::AreEqual(list.Size(), count);

			count = 0;
			for (const Foo& value : list)
			{
				value;
				count++;
			}
			Assert::AreEqual(list.Size(), count);
		}

		TEST_METHOD(IteratorCBeginCEnd)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			SList<Foo> list = { a,b,c };

			auto iter1 = list.begin();
			SList<Foo>::ConstIterator iter2{ iter1 };
			Assert::AreEqual(list.cbegin(), iter2);

			Assert::AreEqual(a, *list.cbegin());
			size_t count = 0;
			for (auto iter = list.cbegin(); iter != list.cend(); ++iter, ++count)
			{
				;
			}
			Assert::AreEqual(list.Size(), count);
		}

		TEST_METHOD(Find)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			const Foo d = { 45 };
			SList<Foo> list = { a,b,c };

			std::function<bool(const Foo&, const Foo&)> func = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };

			auto iter = list.begin();
			Assert::AreEqual(iter++, list.Find(a, func));
			Assert::AreEqual(iter, list.Find(b, func));
			Assert::AreEqual(list.end(), list.Find(d, func));
		}

		TEST_METHOD(ConstFind)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			const Foo d = { 45 };
			const SList<Foo> list = { a,b,c };

			std::function<bool(const Foo&, const Foo&)> func = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };

			auto iter = list.begin();
			Assert::AreEqual(iter++, list.Find(a, func));
			Assert::AreEqual(iter, list.Find(b, func));
			Assert::AreEqual(list.end(), list.Find(d, func));
		}

		TEST_METHOD(InsertAfter)
		{
			SList<Foo> list;
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };

			Assert::ExpectException<runtime_error>([] { SList<Foo>::Iterator iter; SList<Foo> list; const Foo a = { 15 }; list.InsertAfter(iter,a); });
			Assert::ExpectException<runtime_error>([] { SList<Foo> list;  auto iter = list.begin(); SList<Foo> list2; const Foo a = { 15 }; list2.InsertAfter(iter, a); });

			auto iter = list.begin();
			list.InsertAfter(iter, a);
			Assert::AreEqual(*list.begin(), a);
			Assert::AreEqual(list.Size(), size_t(1));
			Assert::AreEqual(list.Front(), a);
			Assert::AreEqual(list.Back(), a);

			iter = list.begin();
			Assert::AreEqual(*iter, a);
			list.InsertAfter(iter, c);
			Assert::AreEqual(*list.begin(), a);
			Assert::AreEqual(list.Size(), size_t(2));
			Assert::AreEqual(list.Front(), a);
			Assert::AreEqual(list.Back(), c);

			list.InsertAfter(iter, b);
			Assert::AreEqual(*list.begin(), a);
			Assert::AreEqual(list.Size(), size_t(3));
			Assert::AreEqual(list.Front(), a);
			Assert::AreEqual(*(++iter), b);
			Assert::AreEqual(list.Back(), c);
		}

		TEST_METHOD(Remove)
		{
			SList<Foo> list;
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			const Foo d = { 45 };
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			std::function<bool(const Foo&, const Foo&)> func = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };

			list.Remove(d, func);
			Assert::AreEqual(list.Size(), size_t(3));

			list.Remove(b, func);
			Assert::AreEqual(list.Size(), size_t(2));
			Assert::AreEqual(list.Front(), a);
			Assert::AreEqual(list.Back(), c);

			list.Remove(c, func);
			Assert::AreEqual(list.Size(), size_t(1));
			Assert::AreEqual(list.Front(), a);
			Assert::AreEqual(list.Back(), a);

			list.Remove(a, func);
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(IteratorIncrementFail)
		{
			Assert::ExpectException<runtime_error>([] { SList<Foo>::Iterator iter; ++iter; });
		}

		TEST_METHOD(IteratorDereferenceFail)
		{
			Assert::ExpectException<runtime_error>([] { SList<Foo>::Iterator iter; *iter; });
		}

		TEST_METHOD(ConstIteratorIncrementFail)
		{
			Assert::ExpectException<runtime_error>([] { SList<Foo>::ConstIterator iter; ++iter; });
		}

		TEST_METHOD(ConstIteratorDereferenceFail)
		{
			Assert::ExpectException<runtime_error>([] { SList<Foo>::ConstIterator iter; *iter; });
		}

		private:
			static _CrtMemState _startMemState;
	};
	_CrtMemState SListTests::_startMemState;
}
