#include "pch.h"
#include "CppUnitTest.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(ScopeTests)
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
			Scope scope;
			Assert::IsNull(scope.GetParent());
		}

		TEST_METHOD(CopySemantics)
		{
			Scope scope;
			Datum& datumA = scope["A"] = 1;
			Datum& datumB = scope["B"] = 2;
			Datum& datumC = scope["C"] = 3;

			Scope& child = scope.AppendScope("child");
			child["D"] = 4;

			Scope& otherChild = scope.AppendScope("child");
			otherChild["E"] = 5;

			// Testing copy constructor
			Scope copyScope{ scope };

			Datum* foundDatum = copyScope.Find("A");
			Assert::IsTrue(datumA == *foundDatum);

			foundDatum = copyScope.Find("B");
			Assert::IsTrue(datumB == *foundDatum);

			foundDatum = copyScope.Find("C");
			Assert::IsTrue(datumC == *foundDatum);

			Assert::IsTrue(scope == copyScope);

			// Testing copy assignment operator
			Scope copyScope2;
			copyScope2 = scope;

			foundDatum = copyScope2.Find("A");
			Assert::IsTrue(datumA == *foundDatum);

			foundDatum = copyScope2.Find("B");
			Assert::IsTrue(datumB == *foundDatum);

			foundDatum = copyScope2.Find("C");
			Assert::IsTrue(datumC == *foundDatum);

			Assert::IsTrue(scope == copyScope2);
		}

		TEST_METHOD(MoveSemantics)
		{
			{
				//Testing move constructor
				Scope scope;
				Datum& datumA = scope["A"] = 1;
				Datum& datumB = scope["B"] = 2;
				Datum& datumC = scope["C"] = 3;

				Scope& child = scope.AppendScope("child");
				child["D"] = 4;

				Scope& otherChild = scope.AppendScope("child");
				otherChild["E"] = 5;

				Scope moveScope = std::move(scope);
				Assert::AreEqual(&moveScope, child.GetParent());
				Assert::AreEqual(&moveScope, otherChild.GetParent());

				Datum* foundDatum = moveScope.Find("A");
				Assert::AreEqual(&datumA, foundDatum);

				foundDatum = moveScope.Find("B");
				Assert::AreEqual(&datumB, foundDatum);

				foundDatum = moveScope.Find("C");
				Assert::AreEqual(&datumC, foundDatum);

				Scope* reparentedScope = new Scope(std::move(child));
				Assert::IsNull(child.GetParent());
				Assert::AreEqual(&moveScope, reparentedScope->GetParent());
				delete &child;
			}

			{
				//Testing move assignment operator
				Scope scope;
				Datum& datumA = scope["A"] = 1;
				Datum& datumB = scope["B"] = 2;
				Datum& datumC = scope["C"] = 3;

				Scope& child = scope.AppendScope("child");
				child["D"] = 4;

				Scope& otherChild = scope.AppendScope("child");
				otherChild["E"] = 5;

				Scope moveScope;
				moveScope = std::move(scope);
				Assert::AreEqual(&moveScope, child.GetParent());
				Assert::AreEqual(&moveScope, otherChild.GetParent());

				Datum* foundDatum = moveScope.Find("A");
				Assert::AreEqual(&datumA, foundDatum);

				foundDatum = moveScope.Find("B");
				Assert::AreEqual(&datumB, foundDatum);

				foundDatum = moveScope.Find("C");
				Assert::AreEqual(&datumC, foundDatum);

				Scope* reparentedScope = new Scope();
				*reparentedScope = std::move(child);
				Assert::IsNull(child.GetParent());
				Assert::AreEqual(&moveScope, reparentedScope->GetParent());
				*reparentedScope = std::move(otherChild);
				Assert::IsNull(otherChild.GetParent());
				Assert::AreEqual(&moveScope, reparentedScope->GetParent());
				delete &child;
				delete &otherChild;
			}

		}

		TEST_METHOD(ComparisonOperators)
		{
			Scope scope;
			scope["A"] = 1;
			scope["B"] = 2;
			scope["C"] = 3;

			Scope otherScope;
			otherScope["B"] = 2;
			otherScope["A"] = 1;
			otherScope["D"] = 4;

			Assert::IsFalse(scope == otherScope);
			Assert::IsTrue(scope != otherScope);

			otherScope["C"] = 3;
			Assert::IsFalse(scope == otherScope);
			Assert::IsTrue(scope != otherScope);

			scope["D"] = 4;
			Assert::IsTrue(scope == otherScope);
			Assert::IsFalse(scope != otherScope);

			otherScope["A"] = 5;
			Assert::IsFalse(scope == otherScope);
			Assert::IsTrue(scope != otherScope);

			scope["A"] = 5;
			Scope child;
			child["D"] = 4;
			scope.Adopt(child, "child");

			Scope otherChild;
			otherChild["E"] = 5;
			otherScope.Adopt(otherChild, "child");

			Assert::IsFalse(scope == otherScope);
			Assert::IsTrue(scope != otherScope);
		}

		TEST_METHOD(Append)
		{
			Scope scope;
			Datum& datum = scope.Append("A");
			Assert::AreEqual(DatumTypes::Unknown, datum.Type());
			datum.PushBack(1);

			Datum otherDatum = scope.Append("A");
			Assert::IsTrue(datum == otherDatum);

			auto expression = [&scope]() { scope.Append(""); };
			Assert::ExpectException<std::invalid_argument>(expression);
		}

		TEST_METHOD(AppendScope)
		{
			Scope scope;
			scope["A"] = 1;
			scope["B"] = 2;
			scope["C"] = 3;

			auto expression = [&scope]() { scope.AppendScope(""); };
			Assert::ExpectException<std::invalid_argument>(expression);

			auto expression2 = [&scope]() { scope.AppendScope("A"); };
			Assert::ExpectException<std::runtime_error>(expression2);

			Scope& child = scope.AppendScope("child");
			Assert::AreEqual(&scope, child.GetParent());
			child["D"] = 4;

			Scope& otherChild = scope.AppendScope("child");
			otherChild["E"] = 5;
			Assert::AreEqual(&scope, otherChild.GetParent());

		}

		TEST_METHOD(Adopt)
		{
			Scope scope;
			scope["A"] = 1;
			scope["B"] = 2;
			scope["C"] = 3;

			Scope otherScope;
			scope["A"] = 1;
			scope["B"] = 2;
			scope["C"] = 3;

			Scope child;
			child["D"] = 4;

			auto expression = [&scope, &child]() { scope.Adopt(child,""); };
			Assert::ExpectException<std::invalid_argument>(expression);

			scope.Adopt(child, "child");
			Assert::AreEqual(&scope, child.GetParent());

			Scope otherChild;
			otherChild["E"] = 5;

			otherScope.Adopt(otherChild, "child");
			Assert::AreEqual(&otherScope, otherChild.GetParent());

			scope.Adopt(otherChild, "child");
			Assert::IsFalse(otherScope.IsAncestorOf(otherChild));
			Assert::AreEqual(&scope, otherChild.GetParent());
		}

		TEST_METHOD(Search)
		{
			Scope scope;
			scope["A"] = 1;
			scope["B"] = 2;
			scope["C"] = 3;

			Scope& child = scope.AppendScope("child");
			Assert::AreEqual(&scope, child.GetParent());
			child["D"] = 4;

			Scope& otherChild = scope.AppendScope("child");
			otherChild["E"] = 5;
			Assert::AreEqual(&scope, otherChild.GetParent());

			Datum* datum = child.Search("D");
			Assert::AreEqual(4, datum->FrontInt());

			Scope* foundScope = nullptr;

			datum = child.Search("D", foundScope);
			Assert::AreEqual(4, datum->FrontInt());
			Assert::AreEqual(&child, foundScope);

			datum = child.Search("A");
			Assert::AreEqual(1, datum->FrontInt());

			datum = child.Search("A", foundScope);
			Assert::AreEqual(1, datum->FrontInt());
			Assert::AreEqual(&scope, foundScope);

			datum = child.Search("E");
			Assert::IsNull(datum);

			datum = child.Search("E", foundScope);
			Assert::IsNull(datum);
			Assert::IsNull(foundScope);
		}

		TEST_METHOD(ConstSearch)
		{
			Scope scope;
			scope["A"] = 1;
			scope["B"] = 2;
			scope["C"] = 3;

			Scope& child = scope.AppendScope("child");
			Assert::AreEqual(&scope, child.GetParent());
			child["D"] = 4;

			Scope& otherChild = scope.AppendScope("child");
			otherChild["E"] = 5;
			Assert::AreEqual(&scope, otherChild.GetParent());

			const Scope copyScope{ scope };
			const Scope& copyChild = (copyScope.Find("child"))->operator[](0);

			const Datum* datum = copyChild.Search("D");
			Assert::AreEqual(4, datum->FrontInt());

			const Scope* foundScope = nullptr;

			datum = copyChild.Search("D", foundScope);
			Assert::AreEqual(4, datum->FrontInt());
			Assert::IsTrue(*foundScope == copyChild);

			datum = copyChild.Search("A");
			Assert::AreEqual(1, datum->FrontInt());

			datum = copyChild.Search("A", foundScope);
			Assert::AreEqual(1, datum->FrontInt());
			Assert::IsTrue(*foundScope == copyScope);

			datum = copyChild.Search("E");
			Assert::IsNull(datum);

			datum = copyChild.Search("E", foundScope);
			Assert::IsNull(datum);
			Assert::IsNull(foundScope);
		}

		TEST_METHOD(Find)
		{
			Scope scope;
			Datum& a = scope.Append("A");
			a.PushBack(1);
			Datum& b = scope.Append("B");
			b.PushBack(2);
			Datum& c = scope.Append("C");
			c.PushBack(3);

			Datum* d = scope.Find("D");
			Assert::IsNull(d);

			Datum* otherA = scope.Find("A");
			Assert::IsTrue(*otherA == a);

			Datum* otherB = scope.Find("B");
			Assert::IsTrue(*otherB == b);

			Datum* otherC = scope.Find("C");
			Assert::IsTrue(*otherC == c);
		}

		TEST_METHOD(ConstFind)
		{
			Scope scope;
			Datum& a = scope.Append("A");
			a.PushBack(1);
			Datum& b = scope.Append("B");
			b.PushBack(2);
			Datum& c = scope.Append("C");
			c.PushBack(3);

			const Scope copyScope{ scope };

			const Datum* d = copyScope.Find("D");
			Assert::IsNull(d);

			const Datum* otherA = copyScope.Find("A");
			Assert::AreEqual(1, otherA->FrontInt());

			const Datum* otherB = copyScope.Find("B");
			Assert::AreEqual(2, otherB->FrontInt());

			const Datum* otherC = copyScope.Find("C");
			Assert::AreEqual(3, otherC->FrontInt());
		}

		TEST_METHOD(BracketOperator)
		{
			Scope scope(3,3);
			scope["A"] = 1;
			scope["B"] = 2;
			scope["C"] = 3;

			auto expression = [&scope]() { scope[5]; };
			Assert::ExpectException<std::out_of_range>(expression);

			Assert::AreEqual(1, scope[0].FrontInt());
			Assert::AreEqual(2, scope[1].FrontInt());
			Assert::AreEqual(3, scope[2].FrontInt());
			
		}

		TEST_METHOD(FindNestedScope)
		{
			Scope scope;
			scope["A"] = 1;
			scope["B"] = 2;
			scope["C"] = 3;

			Scope& child = scope.AppendScope("child");
			Assert::AreEqual(&scope, child.GetParent());
			child["D"] = 4;

			Scope& otherChild = scope.AppendScope("child");
			otherChild["E"] = 5;
			Assert::AreEqual(&scope, otherChild.GetParent());

			auto result = scope.FindNestedScope(&child);
			Assert::AreEqual(size_t(0), result.second);

			result = scope.FindNestedScope(&otherChild);
			Assert::AreEqual(size_t(1), result.second);

			Scope* orphanScope = new Scope();
			result = scope.FindNestedScope(orphanScope);
			Assert::IsNull(result.first);
			Assert::AreEqual(size_t(0), result.second);
			delete orphanScope;
		}

		TEST_METHOD(AncestorDescendant)
		{
			Scope scope;
			scope["A"] = 1;
			scope["B"] = 2;
			scope["C"] = 3;

			Scope& child = scope.AppendScope("child");
			Assert::IsTrue(scope.IsAncestorOf(child));
			Assert::IsTrue(child.IsDescendantOf(scope));
			child["D"] = 4;

			Scope& grandchild = child.AppendScope("child");
			grandchild["F"] = 6;
			Assert::IsTrue(scope.IsAncestorOf(grandchild));
			Assert::IsTrue(grandchild.IsDescendantOf(scope));

			Scope& otherChild = scope.AppendScope("child");
			otherChild["E"] = 5;
			Assert::IsTrue(scope.IsAncestorOf(child));
			Assert::IsTrue(child.IsDescendantOf(scope));

			Scope* orphanScope = new Scope();
			Assert::IsFalse(scope.IsAncestorOf(*orphanScope));
			Assert::IsFalse(orphanScope->IsDescendantOf(scope));
			delete orphanScope;
		}

	private:
		static _CrtMemState _startMemState;
	};
	_CrtMemState ScopeTests::_startMemState;
}