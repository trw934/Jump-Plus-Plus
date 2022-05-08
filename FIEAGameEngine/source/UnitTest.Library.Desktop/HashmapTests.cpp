#include "pch.h"
#include "CppUnitTest.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;


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

	TEST_CLASS(HashmapTests)
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
			{
				Hashmap<Foo, int> hashmap;
				Assert::AreEqual(size_t(0), hashmap.Size());
				Assert::AreEqual(size_t(11), hashmap.BucketSize());
			}

			{
				std::function<bool(const Foo&, const Foo&)> equalityFunc = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };
				std::function<size_t(const Foo&)> hashFunc = [](const Foo& key) { return abs(key.Data()); };

				auto expression1 = [&hashFunc, &equalityFunc]() { Hashmap<Foo, int> badHashmap(size_t(1), hashFunc, equalityFunc); };
				Assert::ExpectException<std::runtime_error>(expression1);

				Hashmap<Foo, int> hashmap(size_t(15),hashFunc, equalityFunc);
				Assert::AreEqual(size_t(0), hashmap.Size());
				Assert::AreEqual(size_t(15), hashmap.BucketSize());
			}
		}

		TEST_METHOD(CopyConstructor)
		{
			std::function<bool(const Foo&, const Foo&)> equalityFunc = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };
			std::function<size_t(const Foo&)> hashFunc = [](const Foo& key) { return abs(key.Data()); };

			const Foo a(10);
			const Foo b(10);
			const Foo c(28);
			std::pair<Foo, int> entryA(a, 100);
			std::pair<Foo, int> entryB(b, 200);
			std::pair<Foo, int> entryC(c, 300);

			Hashmap<Foo, int> hashmap{ {entryA, entryB, entryC}, size_t(11), hashFunc, equalityFunc };

			Hashmap<Foo, int> hashCopy(hashmap);
			auto iter = hashCopy.begin();
			Assert::AreEqual(size_t(2), hashCopy.Size());
			Assert::AreEqual(iter++, hashCopy.Find(c));
			Assert::AreEqual(iter++, hashCopy.Find(a));


		}

		TEST_METHOD(Find)
		{
			std::function<bool(const Foo&, const Foo&)> equalityFunc = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };
			std::function<size_t(const Foo&)> hashFunc = [](const Foo& key) { return abs(key.Data()); };

			const Foo a(10);
			const Foo b(21);
			const Foo c(28);
			const Foo d(3);
			std::pair<Foo, int> entryA(a, 100);
			std::pair<Foo, int> entryB(b, 200);
			std::pair<Foo, int> entryC(c, 300);
			std::pair<Foo, int> entryD(d, 400);

			auto expression1 = [&entryA, &entryB, &entryC, &hashFunc, &equalityFunc]() { Hashmap<Foo, int> badHashmap({ entryA, entryB, entryC }, size_t(1), hashFunc, equalityFunc); };
			Assert::ExpectException<std::runtime_error>(expression1);

			Hashmap<Foo, int> hashmap{ {entryA, entryB, entryC}, size_t(11), hashFunc, equalityFunc };

			auto iter = hashmap.begin();
			Assert::AreEqual(iter++, hashmap.Find(c));
			Assert::AreEqual(iter++, hashmap.Find(a));
			Assert::AreEqual(iter, hashmap.Find(b));
			Assert::AreEqual(hashmap.end(), hashmap.Find(d));
		}

		TEST_METHOD(ConstFind)
		{
			std::function<bool(const Foo&, const Foo&)> equalityFunc = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };
			std::function<size_t(const Foo&)> hashFunc = [](const Foo& key) { return abs(key.Data()); };

			const Foo a(10);
			const Foo b(21);
			const Foo c(28);
			const Foo d(3);
			std::pair<Foo, int> entryA(a, 100);
			std::pair<Foo, int> entryB(b, 200);
			std::pair<Foo, int> entryC(c, 300);
			std::pair<Foo, int> entryD(d, 400);

			const Hashmap<Foo, int> hashmap{ {entryA, entryB, entryC}, size_t(11), hashFunc, equalityFunc };

			auto iter = hashmap.begin();
			Assert::AreEqual(iter++, hashmap.Find(c));
			Assert::AreEqual(iter++, hashmap.Find(a));
			Assert::AreEqual(iter, hashmap.Find(b));
			Assert::AreEqual(hashmap.end(), hashmap.Find(d));
		}

		TEST_METHOD(Insert)
		{
			std::function<bool(const Foo&, const Foo&)> equalityFunc = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };
			std::function<size_t(const Foo&)> hashFunc = [](const Foo& key) { return abs(key.Data()); };

			const Foo a(10);
			const Foo b(10);
			const Foo c(28);
			const Foo d(2);
			std::pair<Foo, int> entryA(a, 100);
			std::pair<Foo, int> entryB(b, 200);
			std::pair<Foo, int> entryC(c, 300);
			std::pair<Foo, int> entryD(d, 400);

			Hashmap<Foo, int> hashmap(size_t(15), hashFunc, equalityFunc);
			auto result = hashmap.Insert(entryA);

			Assert::AreEqual(size_t(1), hashmap.Size());
			Assert::AreEqual(a, (*(result.first)).first);
			Assert::AreEqual(100, (*(result.first)).second);
			Assert::IsTrue(result.second);

			Assert::IsTrue(hashmap.ContainsKey(a));

			result = hashmap.Insert(entryB);

			Assert::AreEqual(size_t(1), hashmap.Size());
			Assert::AreEqual(a, (*(result.first)).first);
			Assert::AreEqual(100, (*(result.first)).second);
			Assert::AreEqual(a, (result.first)->first);
			Assert::AreEqual(100, (result.first)->second);
			Assert::IsFalse(result.second);
			Assert::IsTrue(hashmap.ContainsKey(a));

			result = hashmap.Insert(entryC);

			Assert::AreEqual(size_t(2), hashmap.Size());
			Assert::AreEqual(c, (*(result.first)).first);
			Assert::AreEqual(300, (*(result.first)).second);
			Assert::IsTrue(result.second);

			Assert::IsTrue(hashmap.ContainsKey(c));

			Assert::IsFalse(hashmap.ContainsKey(d));
			
		}

		TEST_METHOD(At)
		{
			std::function<bool(const Foo&, const Foo&)> equalityFunc = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };

			const Foo a(10);
			const Foo b(21);
			const Foo c(28);
			const Foo d(4);
			std::pair<Foo, int> entryA(a, 100);
			std::pair<Foo, int> entryB(b, 200);
			std::pair<Foo, int> entryC(c, 300);
			std::pair<Foo, int> entryD(d, 400);

			Hashmap<Foo, int> hashmap;
			hashmap.Insert(entryA);
			hashmap.Insert(entryB);
			hashmap.Insert(entryC);

			auto expression1 = [&hashmap]() { const Foo e(3);  hashmap.At(e); };
			Assert::ExpectException<std::runtime_error>(expression1);

			Assert::AreEqual(100, hashmap.At(a));
			Assert::AreEqual(200, hashmap.At(b));
			Assert::AreEqual(300, hashmap.At(c));

			Assert::AreEqual(100, hashmap[a]);
			Assert::AreEqual(200, hashmap[b]);
			Assert::AreEqual(300, hashmap[c]);

			hashmap[d] = 400;
			Assert::AreEqual(400, hashmap[d]);
		}

		TEST_METHOD(ConstAt)
		{
			std::function<bool(const Foo&, const Foo&)> equalityFunc = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };
			std::function<size_t(const Foo&)> hashFunc = [](const Foo& key) { return abs(key.Data()); };

			const Foo a(10);
			const Foo b(21);
			const Foo c(28);
			const Foo d(4);
			std::pair<Foo, int> entryA(a, 100);
			std::pair<Foo, int> entryB(b, 200);
			std::pair<Foo, int> entryC(c, 300);
			std::pair<Foo, int> entryD(d, 400);

			const Hashmap<Foo, int> hashmap{ {entryA, entryB, entryC}, size_t(11), hashFunc, equalityFunc };

			auto expression1 = [&hashmap]() { const Foo e(3);  hashmap.At(e); };
			Assert::ExpectException<std::runtime_error>(expression1);

			Assert::AreEqual(100, hashmap.At(a));
			Assert::AreEqual(200, hashmap.At(b));
			Assert::AreEqual(300, hashmap.At(c));
		}

		TEST_METHOD(Remove)
		{
			std::function<bool(const Foo&, const Foo&)> equalityFunc = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };
			std::function<size_t(const Foo&)> hashFunc = [](const Foo& key) { return abs(key.Data()); };

			const Foo a(10);
			const Foo b(21);
			const Foo c(28);
			const Foo d(4);
			std::pair<Foo, int> entryA(a, 100);
			std::pair<Foo, int> entryB(b, 200);
			std::pair<Foo, int> entryC(c, 300);
			std::pair<Foo, int> entryD(d, 400);

			Hashmap<Foo, int> hashmap{ {entryA, entryB, entryC}, size_t(11), hashFunc, equalityFunc };

			Assert::IsFalse(hashmap.Remove(d));

			Assert::IsTrue(hashmap.Remove(b));
			Assert::AreEqual(size_t(2), hashmap.Size());
			Assert::IsFalse(hashmap.Remove(b));

			Assert::IsTrue(hashmap.Remove(a));
			Assert::AreEqual(size_t(1), hashmap.Size());

			Assert::IsTrue(hashmap.Remove(c));
			Assert::AreEqual(size_t(0), hashmap.Size());
		}

		TEST_METHOD(Clear)
		{
			const Foo a(10);
			const Foo b(21);
			const Foo c(28);
			std::pair<Foo, int> entryA(a, 100);
			std::pair<Foo, int> entryB(b, 200);
			std::pair<Foo, int> entryC(c, 300);

			Hashmap<Foo, int> hashmap{entryA, entryB, entryC};

			Assert::AreEqual(size_t(3), hashmap.Size());
			Assert::AreEqual(size_t(11), hashmap.BucketSize());

			hashmap.Clear();
			Assert::AreEqual(size_t(0), hashmap.Size());
			Assert::AreEqual(size_t(11), hashmap.BucketSize());
		}

		TEST_METHOD(IteratorBeginEnd)
		{
			{
				Hashmap<Foo, int> hashmap;
				Assert::AreEqual(hashmap.begin(), hashmap.end());
				Assert::AreEqual(hashmap.cbegin(), hashmap.cend());
			}
			{
				const Hashmap<Foo, int> hashmap;
				Assert::AreEqual(hashmap.begin(), hashmap.end());
			}
			const Foo a(10);
			const Foo b(21);
			const Foo c(28);
			std::pair<Foo, int> entryA(a, 100);
			std::pair<Foo, int> entryB(b, 200);
			std::pair<Foo, int> entryC(c, 300);

			Hashmap<Foo, int> hashmap{ entryA, entryB, entryC };

			auto iter1 = hashmap.begin();
			Hashmap<Foo,int>::ConstIterator iter2{ iter1 };
			Assert::AreEqual(hashmap.cbegin(), iter2);

			size_t count = 0;
			for (auto iter = hashmap.cbegin(); iter != hashmap.cend(); ++iter, ++count)
			{
				;
			}
			Assert::AreEqual(hashmap.Size(), count);
		}

		TEST_METHOD(IteratorArrowOperator)
		{
			
			std::function<bool(const Foo&, const Foo&)> equalityFunc = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };
			std::function<size_t(const Foo&)> hashFunc = [](const Foo& key) { return abs(key.Data()); };

			const Foo a(1);
			const Foo b(3);
			const Foo c(5);
			std::pair<Foo, int> entryA(a, 100);
			std::pair<Foo, int> entryB(b, 200);
			std::pair<Foo, int> entryC(c, 300);

			Hashmap<Foo, int> hashmap{ {entryA, entryB, entryC}, size_t(11), hashFunc, equalityFunc };

			auto iter = hashmap.begin();
			Assert::AreEqual(a, (iter++)->first);
			Assert::AreEqual(b, (iter++)->first);
			Assert::AreEqual(c, (iter++)->first);

			auto cIter = hashmap.cbegin();
			Assert::AreEqual(a, (cIter++)->first);
			Assert::AreEqual(b, (cIter++)->first);
			Assert::AreEqual(c, (cIter++)->first);

		}

		TEST_METHOD(ResizeHash)
		{
			std::function<bool(const Foo&, const Foo&)> equalityFunc = [](const Foo& lhs, const Foo& rhs) { return lhs.Data() == rhs.Data(); };
			std::function<size_t(const Foo&)> hashFunc = [](const Foo& key) { return abs(key.Data()); };

			const Foo a(1);
			const Foo b(2);
			const Foo c(3);
			const Foo d(4);
			std::pair<Foo, int> entryA(a, 100);
			std::pair<Foo, int> entryB(b, 200);
			std::pair<Foo, int> entryC(c, 300);
			std::pair<Foo, int> entryD(d, 400);

			Hashmap<Foo, int> hashmap{ {entryA, entryB, entryC, entryD}, size_t(4), hashFunc, equalityFunc };

			Assert::AreEqual(1.0f, hashmap.GetLoadFactor());

			auto expression1 = [&hashmap]() { hashmap.ResizeHash(1); };
			Assert::ExpectException<std::runtime_error>(expression1);

			hashmap.ResizeHash(8);

			Assert::AreEqual(size_t(4), hashmap.Size());
			Assert::AreEqual(size_t(8), hashmap.BucketSize());
			Assert::IsTrue(hashmap.ContainsKey(a));
			Assert::IsTrue(hashmap.ContainsKey(b));
			Assert::IsTrue(hashmap.ContainsKey(c));
			Assert::IsTrue(hashmap.ContainsKey(d));
			Assert::AreEqual(0.5f, hashmap.GetLoadFactor());

		}

		TEST_METHOD(IteratorIncrementFail)
		{
			Assert::ExpectException<runtime_error>([] { Hashmap<Foo, int>::Iterator iter; ++iter; });
		}

		TEST_METHOD(IteratorDereferenceFail)
		{
			Assert::ExpectException<runtime_error>([] { Hashmap<Foo, int>::Iterator iter; *iter; });
		}

		TEST_METHOD(IteratorArrowFail)
		{
			Assert::ExpectException<runtime_error>([] { Hashmap<Foo, int>::Iterator iter; iter.operator->(); });
		}

		TEST_METHOD(ConstIteratorIncrementFail)
		{
			Assert::ExpectException<runtime_error>([] { Hashmap<Foo, int>::ConstIterator iter; ++iter; });
		}

		TEST_METHOD(ConstIteratorDereferenceFail)
		{
			Assert::ExpectException<runtime_error>([] { Hashmap<Foo, int>::ConstIterator iter; *iter; });
		}

		TEST_METHOD(ConstIteratorArrowFail)
		{
			Assert::ExpectException<runtime_error>([] { Hashmap<Foo, int>::ConstIterator iter; iter.operator->(); });
		}

	private:
		static _CrtMemState _startMemState;
	};
	_CrtMemState HashmapTests::_startMemState;
}