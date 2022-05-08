#include "pch.h"
#include "CppUnitTest.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(VectorTests)
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
			Vector<Foo> vector;
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(3), vector.Capacity());
			Assert::IsTrue(vector.IsEmpty());
		}

		TEST_METHOD(CopyConstructor)
		{
			// Testing copy constructor
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			Vector<Foo> vector = { a, b, c };

			Vector<Foo> copyVector{ vector };
			Assert::AreEqual(vector.Size(), copyVector.Size());
			Assert::AreEqual(vector.Capacity(), copyVector.Capacity());
			for (size_t i = 0; i < vector.Size(); ++i)
			{
				Assert::AreEqual(vector[i], copyVector[i]);
			}
			
			// Testing copy assignment operator
			Vector<Foo> copyVector2;
			copyVector2 = vector;
			Assert::AreEqual(vector.Size(), copyVector2.Size());
			Assert::AreEqual(vector.Capacity(), copyVector2.Capacity());
			for (size_t i = 0; i < vector.Size(); ++i)
			{
				Assert::AreEqual(vector[i], copyVector2[i]);
			}

		}

		TEST_METHOD(MoveConstructor)
		{
			// Test move constructor
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			Vector<Foo> vector = { a, b, c };

			Vector<Foo> moveVector = std::move(vector);
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(3), moveVector.Size());
			Assert::AreEqual(size_t(0), vector.Capacity());
			Assert::AreEqual(size_t(3), moveVector.Capacity());

			Assert::AreEqual(a, moveVector.Front());
			Assert::AreEqual(b, moveVector.At(1));
			Assert::AreEqual(c, moveVector.Back());

			// Test move assignment operator
			Vector<Foo> moveVector2;
			moveVector2 = std::move(moveVector);
			Assert::AreEqual(size_t(0), moveVector.Size());
			Assert::AreEqual(size_t(3), moveVector2.Size());
			Assert::AreEqual(size_t(0), moveVector.Capacity());
			Assert::AreEqual(size_t(3), moveVector2.Capacity());

			Assert::AreEqual(a, moveVector2.Front());
			Assert::AreEqual(b, moveVector2.At(1));
			Assert::AreEqual(c, moveVector2.Back());
		}

		TEST_METHOD(At)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			Vector<Foo> vector = { a, b, c };

			auto expression1 = [&vector]() { vector.At(10); };
			Assert::ExpectException<std::out_of_range>(expression1);

			Assert::AreEqual(a, vector.At(0));
			Assert::AreEqual(b, vector.At(1));
			Assert::AreEqual(c, vector.At(2));

			auto expression2 = [&vector]() { vector[10]; };
			Assert::ExpectException<std::out_of_range>(expression2);

			Assert::AreEqual(a, vector[0]);
			Assert::AreEqual(b, vector[1]);
			Assert::AreEqual(c, vector[2]);
		}

		TEST_METHOD(ConstAt)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			const Vector<Foo> vector = { a, b, c };

			auto expression1 = [&vector]() { vector.At(10); };
			Assert::ExpectException<std::out_of_range>(expression1);

			Assert::AreEqual(a, vector.At(0));
			Assert::AreEqual(b, vector.At(1));
			Assert::AreEqual(c, vector.At(2));

			auto expression2 = [&vector]() { vector[10]; };
			Assert::ExpectException<std::out_of_range>(expression2);

			Assert::AreEqual(a, vector[0]);
			Assert::AreEqual(b, vector[1]);
			Assert::AreEqual(c, vector[2]);
		}

		TEST_METHOD(Reserve)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			Vector<Foo> vector = { a, b, c };

			Assert::AreEqual(size_t(3), vector.Capacity());

			vector.Reserve(2);
			Assert::AreEqual(size_t(3), vector.Capacity());

			vector.Reserve(5);
			Assert::AreEqual(size_t(5), vector.Capacity());
		}

		TEST_METHOD(Front)
		{
			Vector<Foo> vector;
			const Foo a = { 15 };
			const Foo b = { 3 };

			auto expression = [&vector]() { vector.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);

			vector.PushBack(a);
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(a, vector.Back());

			vector.PushBack(b);
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(b, vector.Back());
		}

		TEST_METHOD(ConstFront)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			const Vector<Foo> vector = { a, b, c };

			const Vector<Foo> emptyVector;
			auto expression = [&emptyVector]() { emptyVector.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);

			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(c, vector.Back());
		}

		TEST_METHOD(Back)
		{
			Vector<Foo> vector;
			const Foo a = { 15 };
			const Foo b = { 3 };

			auto expression = [&vector]() { vector.Back(); };
			Assert::ExpectException<std::runtime_error>(expression);

			vector.PushBack(a);
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(a, vector.Back());

			vector.PushBack(b);
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(b, vector.Back());
		}

		TEST_METHOD(ConstBack)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			const Vector<Foo> vector = { a, b, c };

			const Vector<Foo> emptyVector;
			auto expression = [&emptyVector]() { emptyVector.Back(); };
			Assert::ExpectException<std::runtime_error>(expression);

			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(c, vector.Back());
		}

		TEST_METHOD(PushBack)
		{
			Vector<Foo> vector;
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			const Foo d = { 45 };

			Assert::IsTrue(vector.IsEmpty());
			vector.PushBack(a);
			Assert::IsFalse(vector.IsEmpty());
			Assert::AreEqual(size_t(1), vector.Size());
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(a, vector.Back());
			Assert::AreSame(vector.Front(), vector.Back());

			vector.PushBack(b);
			Assert::AreEqual(size_t(2), vector.Size());
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(b, vector.Back());

			vector.PushBack(c);
			Assert::AreEqual(size_t(3), vector.Size());
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(c, vector.Back());
			Assert::AreEqual(size_t(3), vector.Capacity());

			vector.PushBack(d);
			Assert::AreEqual(size_t(4), vector.Size());
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(d, vector.Back());
			Assert::AreEqual(size_t(6), vector.Capacity());
		}

		TEST_METHOD(PopBack)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			Vector<Foo> vector = { a,b,c };

			Vector<Foo> emptyVector;
			Assert::IsFalse(emptyVector.PopBack());

			Assert::IsFalse(vector.IsEmpty());
			Assert::AreEqual(size_t(3), vector.Size());
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(c, vector.Back());

			Assert::IsTrue(vector.PopBack());
			Assert::AreEqual(size_t(2), vector.Size());
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(b, vector.Back());

			Assert::IsTrue(vector.PopBack());
			Assert::AreEqual(size_t(1), vector.Size());
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(a, vector.Back());

			Assert::IsTrue(vector.PopBack());
			Assert::IsTrue(vector.IsEmpty());
			Assert::AreEqual(size_t(0), vector.Size());
		}

		TEST_METHOD(Clear)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			Vector<Foo> vector = { a,b,c };

			Assert::IsFalse(vector.IsEmpty());
			Assert::AreEqual(size_t(3), vector.Size());
			Assert::AreEqual(size_t(3), vector.Capacity());

			vector.Clear();
			Assert::IsTrue(vector.IsEmpty());
			Assert::AreEqual(size_t(3), vector.Capacity());

			vector.PushBack(a);
			Assert::IsFalse(vector.IsEmpty());
			Assert::AreEqual(size_t(1), vector.Size());
		}

		TEST_METHOD(Iterator)
		{
			{
				Vector<Foo> vector;
				Assert::AreEqual(vector.begin(), vector.end());

				const Foo a{ 10 };
				vector.PushBack(a);
				Assert::IsTrue(vector.begin() != vector.end());
			}

			{
				const Foo a = { 15 };
				const Foo b = { 3 };
				const Foo c = { 7 };
				Vector<Foo> vector = { a,b,c };

				auto iter1 = vector.begin();
				Vector<Foo>::Iterator iter2;
				Assert::IsFalse(iter1 == iter2);

				size_t count = 0;
				for (auto iter = vector.begin(); iter != vector.end(); ++iter, ++count)
				{
					;
				}
				Assert::AreEqual(vector.Size(), count);

				count = 0;
				for (const Foo& value : vector)
				{
					value;
					count++;
				}
				Assert::AreEqual(vector.Size(), count);
			}
		}

		TEST_METHOD(ConstIterator)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			const Vector<Foo> vector = { a,b,c };

			auto iter1 = vector.begin();
			Vector<Foo>::ConstIterator iter2;
			Assert::IsFalse(iter1 == iter2);

			size_t count = 0;
			for (auto iter = vector.begin(); iter != vector.end(); ++iter, ++count)
			{
				;
			}
			Assert::AreEqual(vector.Size(), count);

			count = 0;
			for (const Foo& value : vector)
			{
				value;
				count++;
			}
			Assert::AreEqual(vector.Size(), count);
		}

		TEST_METHOD(IteratorCBeginCEnd)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			Vector<Foo> vector = { a,b,c };

			auto iter1 = vector.begin();
			Vector<Foo>::ConstIterator iter2{ iter1 };
			Assert::AreEqual(vector.cbegin(), iter2);

			Assert::AreEqual(a, *vector.cbegin());
			size_t count = 0;
			for (auto iter = vector.cbegin(); iter != vector.cend(); ++iter, ++count)
			{
				;
			}
			Assert::AreEqual(vector.Size(), count);
		}

		TEST_METHOD(Find)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			const Foo d = { 45 };
			Vector<Foo> vector = { a,b,c };

			std::function<bool(const Foo&, const Foo&)> func = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };

			auto iter = vector.begin();
			Assert::AreEqual(iter++, vector.Find(a, func));
			Assert::AreEqual(iter, vector.Find(b, func));
			Assert::AreEqual(vector.end(), vector.Find(d, func));
		}

		TEST_METHOD(ConstFind)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			const Foo d = { 45 };
			const Vector<Foo> vector = { a,b,c };

			std::function<bool(const Foo&, const Foo&)> func = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };

			auto iter = vector.begin();
			Assert::AreEqual(iter++, vector.Find(a, func));
			Assert::AreEqual(iter, vector.Find(b, func));
			Assert::AreEqual(vector.end(), vector.Find(d, func));
		}

		TEST_METHOD(RemoveAt)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			Vector<Foo> vector = { a,b,c };
			Vector<Foo> vector2 = { a, b, c };
			Vector<Foo> vector3 = { a, b, c };

			Assert::IsFalse(vector.RemoveAt(15));

			Assert::IsTrue(vector.RemoveAt(0));
			Assert::AreEqual(b, vector.Front());
			Assert::AreEqual(c, vector.Back());
			Assert::AreEqual(size_t(2), vector.Size());
			Assert::AreEqual(size_t(3), vector.Capacity());

			Assert::IsTrue(vector.RemoveAt(0));
			Assert::AreEqual(c, vector.Front());
			Assert::AreEqual(c, vector.Back());
			Assert::AreEqual(size_t(1), vector.Size());
			Assert::AreEqual(size_t(3), vector.Capacity());

			Assert::IsTrue(vector.RemoveAt(0));
			Assert::AreEqual(size_t(0), vector.Size());
			Assert::AreEqual(size_t(3), vector.Capacity());

			Assert::IsTrue(vector2.RemoveAt(1));
			Assert::AreEqual(a, vector2.Front());
			Assert::AreEqual(c, vector2.Back());
			Assert::AreEqual(size_t(2), vector2.Size());
			Assert::AreEqual(size_t(3), vector2.Capacity());

			Assert::IsTrue(vector3.RemoveAt(2));
			Assert::AreEqual(a, vector3.Front());
			Assert::AreEqual(b, vector3.Back());
			Assert::AreEqual(size_t(2), vector3.Size());
			Assert::AreEqual(size_t(3), vector3.Capacity());
		}

		TEST_METHOD(Remove)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			const Foo d = { 45 };
			Vector<Foo> vector = { a,b,c };

			std::function<bool(const Foo&, const Foo&)> func = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };

			Assert::IsFalse(vector.Remove(d, func));

			Assert::IsTrue(vector.Remove(b, func));
			Assert::AreEqual(vector.Size(), size_t(2));
			Assert::AreEqual(vector.Front(), a);
			Assert::AreEqual(vector.Back(), c);

			auto iter = vector.begin();
			auto badIter = vector.end();
			++iter;
			Assert::IsFalse(vector.Remove(badIter));
			Assert::IsTrue(vector.Remove(iter));
			Assert::AreEqual(vector.Size(), size_t(1));
			Assert::AreEqual(vector.Front(), a);
			Assert::AreEqual(vector.Back(), a);

			Assert::IsTrue(vector.Remove(a, func));
			Assert::IsTrue(vector.IsEmpty());
		}

		TEST_METHOD(RemoveRange)
		{
			const Foo a = { 15 };
			const Foo b = { 3 };
			const Foo c = { 7 };
			const Foo d = { 45 };
			const Foo e = { 63 };
			Vector<Foo> vector = { a,b,c,d,e };

			auto startIter = vector.begin();
			auto endIter = vector.begin();
			++startIter;
			++endIter;
			++endIter;
			++endIter;

			vector.Remove(startIter, endIter);
			Assert::AreEqual(size_t(3), vector.Size());
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(d, vector[1]);
			Assert::AreEqual(e, vector.Back());

			startIter = vector.begin();
			endIter = vector.end();
			vector.Remove(startIter, endIter);
			Assert::IsTrue(vector.IsEmpty());
		}

		TEST_METHOD(IteratorIncrementFail)
		{
			Assert::ExpectException<runtime_error>([] { Vector<Foo>::Iterator iter; ++iter; });
		}

		TEST_METHOD(IteratorDereferenceFail)
		{
			Assert::ExpectException<runtime_error>([] { Vector<Foo>::Iterator iter; *iter; });
		}

		TEST_METHOD(ConstIteratorIncrementFail)
		{
			Assert::ExpectException<runtime_error>([] { Vector<Foo>::ConstIterator iter; ++iter; });
		}

		TEST_METHOD(ConstIteratorDereferenceFail)
		{
			Assert::ExpectException<runtime_error>([] { Vector<Foo>::ConstIterator iter; *iter; });
		}

	private:
		static _CrtMemState _startMemState;
	};
	_CrtMemState VectorTests::_startMemState;
}