#include "pch.h"
#include "CppUnitTest.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace glm;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(AttributedTests)
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

		TEST_METHOD(RTTITest)
		{
			AttributedFoo a;

			RTTI* rtti = &a;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("AttributedFoo"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Scope::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(AttributedFoo::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			AttributedFoo* af = rtti->As<AttributedFoo>();
			Assert::IsNotNull(af);
			Assert::AreEqual(&a, af);

			Attributed* fAsAttributed = rtti->As<Attributed>();
			Assert::IsNotNull(fAsAttributed);
			Assert::AreEqual(static_cast<Attributed*>(&a), fAsAttributed);

			AttributedFoo otherAFoo;
			Assert::IsTrue(rtti->Equals(&otherAFoo));

			Foo otherFoo(10);
			Assert::IsFalse(rtti->Equals(&otherFoo));

			Assert::AreEqual("AttributedFoo"s, rtti->ToString());
		}

		TEST_METHOD(Attributes)
		{
			AttributedFoo foo;

			Assert::IsTrue(foo.IsAttribute("this"));
			Assert::IsTrue(foo.IsAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsAttribute("ExternalString"));
			Assert::IsTrue(foo.IsAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsAttribute("NestedScope"));
			Assert::IsTrue(foo.IsAttribute("NestedScopeArray"));

			Assert::IsTrue(foo.IsPrescribedAttribute("this"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScopeArray"));

			Assert::IsFalse(foo.IsAuxillaryAttribute("this"));
			Assert::IsFalse(foo.IsAuxillaryAttribute("ExternalInteger"));
			Assert::IsFalse(foo.IsAuxillaryAttribute("ExternalFloat"));
			Assert::IsFalse(foo.IsAuxillaryAttribute("ExternalString"));
			Assert::IsFalse(foo.IsAuxillaryAttribute("ExternalVector"));
			Assert::IsFalse(foo.IsAuxillaryAttribute("ExternalMatrix"));
			Assert::IsFalse(foo.IsAuxillaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(foo.IsAuxillaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(foo.IsAuxillaryAttribute("ExternalStringArray"));
			Assert::IsFalse(foo.IsAuxillaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(foo.IsAuxillaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(foo.IsAuxillaryAttribute("NestedScope"));
			Assert::IsFalse(foo.IsAuxillaryAttribute("NestedScopeArray"));

			Assert::AreEqual(&foo, foo["this"].GetPointer()->As<AttributedFoo>());
		}

		TEST_METHOD(AuxillaryAttributes)
		{
			AttributedFoo foo;
			
			foo.AppendAuxillaryAttribute("AuxillaryInteger");
			Assert::IsTrue(foo.IsAttribute("AuxillaryInteger"));
			Assert::IsTrue(foo.IsAuxillaryAttribute("AuxillaryInteger"));
			Assert::IsFalse(foo.IsPrescribedAttribute("AuxillaryInteger"));

			Assert::ExpectException<exception>([&foo] {foo.AppendAuxillaryAttribute("ExternalInteger"); });
		}

		TEST_METHOD(ExternalAssignment)
		{
			AttributedFoo foo;

			Datum* datum = foo.Find("ExternalInteger");
			Assert::AreEqual(size_t(1), datum->Size());
			Assert::AreSame(datum->GetInt(), foo.ExternalInteger);

			int a = 5;
			datum->Set(a);
			Assert::AreEqual(a, datum->GetInt());
			Assert::AreEqual(a, foo.ExternalInteger);

			float b = 6.0f;
			foo["ExternalFloat"] = b;
			Assert::AreEqual(b, foo.ExternalFloat);
		}

		TEST_METHOD(CopyConstructor)
		{
			AttributedFoo foo;
			AttributedFoo copyFoo = foo;
			
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("this"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("NestedScopeArray"));

			Assert::AreEqual(&copyFoo, copyFoo["this"].GetPointer()->As<AttributedFoo>());

			int& fooData = foo["ExternalInteger"].GetInt();
			int& copyFooData = copyFoo["ExternalInteger"].GetInt();
			Assert::AreNotSame(copyFooData, fooData);
			Assert::AreEqual(copyFoo, foo);
		}

		TEST_METHOD(CopyAssignmentOperator)
		{
			AttributedFoo foo;
			AttributedFoo copyFoo;
			copyFoo = foo;

			Assert::IsTrue(copyFoo.IsPrescribedAttribute("this"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(copyFoo.IsPrescribedAttribute("NestedScopeArray"));

			Assert::AreEqual(&copyFoo, copyFoo["this"].GetPointer()->As<AttributedFoo>());

			int& fooData = foo["ExternalInteger"].GetInt();
			int& copyFooData = copyFoo["ExternalInteger"].GetInt();
			Assert::AreNotSame(copyFooData, fooData);
		}

		TEST_METHOD(MoveConstructor)
		{
			AttributedFoo foo;

			size_t originalSize = foo.Size();
			Assert::AreNotEqual(size_t(0), originalSize);
			Assert::AreNotEqual(size_t(1), originalSize);

			foo.ExternalInteger = 1;
			foo.ExternalFloat = 2.0f;
			foo.ExternalString = "Hello, World!"s;
			foo.ExternalVector = vec4(1, 2, 3, 4);
			foo.ExternalMatrix = mat4(-1);

			char helloWorld[14];
			strcpy_s(helloWorld, foo.ExternalString.c_str());
			string helloWorldString = helloWorld;
			Assert::AreEqual(helloWorldString, foo.ExternalString);

			int integerArray[] = { 1,2,3,4,5 };
			std::copy(std::begin(integerArray), std::end(integerArray), foo.ExternalIntegerArray);

			AttributedFoo moveFoo = std::move(foo);
			Assert::AreEqual(originalSize, moveFoo.Size());
			Assert::AreEqual(size_t(0), foo.Size());
			Assert::AreEqual(1, moveFoo.ExternalInteger);
			Assert::AreEqual(2.0f, moveFoo.ExternalFloat);
			Assert::AreEqual(helloWorldString, moveFoo.ExternalString);
			Assert::AreNotSame(helloWorldString, moveFoo.ExternalString);
			Assert::IsTrue(foo.ExternalString.empty());
			Assert::AreEqual(moveFoo.ExternalVector, vec4(1, 2, 3, 4));
			Assert::AreEqual(moveFoo.ExternalMatrix, mat4(-1));
			Assert::IsTrue(std::equal(std::begin(moveFoo.ExternalIntegerArray), std::end(moveFoo.ExternalIntegerArray), std::begin(integerArray)));
			Assert::AreEqual(&moveFoo, moveFoo["this"].GetPointer()->As<AttributedFoo>());
		}

		TEST_METHOD(MoveAssignmentOperator)
		{
			AttributedFoo foo;

			size_t originalSize = foo.Size();
			Assert::AreNotEqual(size_t(0), originalSize);
			Assert::AreNotEqual(size_t(1), originalSize);

			foo.ExternalInteger = 1;
			foo.ExternalFloat = 2.0f;
			foo.ExternalString = "Hello, World!"s;
			foo.ExternalVector = vec4(1, 2, 3, 4);
			foo.ExternalMatrix = mat4(-1);

			char helloWorld[14];
			strcpy_s(helloWorld, foo.ExternalString.c_str());
			string helloWorldString = helloWorld;
			Assert::AreEqual(helloWorldString, foo.ExternalString);

			int integerArray[] = { 1,2,3,4,5 };
			std::copy(std::begin(integerArray), std::end(integerArray), foo.ExternalIntegerArray);

			AttributedFoo moveFoo;
			moveFoo = std::move(foo);
			Assert::AreEqual(originalSize, moveFoo.Size());
			Assert::AreEqual(size_t(0), foo.Size());
			Assert::AreEqual(1, moveFoo.ExternalInteger);
			Assert::AreEqual(2.0f, moveFoo.ExternalFloat);
			Assert::AreEqual(helloWorldString, moveFoo.ExternalString);
			Assert::AreNotSame(helloWorldString, moveFoo.ExternalString);
			Assert::IsTrue(foo.ExternalString.empty());
			Assert::AreEqual(moveFoo.ExternalVector, vec4(1, 2, 3, 4));
			Assert::AreEqual(moveFoo.ExternalMatrix, mat4(-1));
			Assert::IsTrue(std::equal(std::begin(moveFoo.ExternalIntegerArray), std::end(moveFoo.ExternalIntegerArray), std::begin(integerArray)));
			Assert::AreEqual(&moveFoo, moveFoo["this"].GetPointer()->As<AttributedFoo>());
		}

		TEST_METHOD(NestedScopeAttributes)
		{
			AttributedFoo foo;
			Datum& nestedScopeDatum = *foo.Find("NestedScope"s);
			Assert::AreEqual(size_t(0), nestedScopeDatum.Size());

			/*Datum& nestedScopeArrayDatum = *foo.Find("NestedScopeArray"s);
			Assert::AreEqual(AttributedFoo::ArraySize, nestedScopeArrayDatum.Size());*/
		}

		TEST_METHOD(Clone)
		{
			AttributedFoo foo;
			auto clone = foo.Clone();
			auto cloneFoo = clone->As<AttributedFoo>();
			Assert::IsNotNull(cloneFoo);

			Assert::IsTrue(cloneFoo->IsPrescribedAttribute("this"));
			Assert::IsTrue(cloneFoo->IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(cloneFoo->IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(cloneFoo->IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(cloneFoo->IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(cloneFoo->IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(cloneFoo->IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(cloneFoo->IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(cloneFoo->IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(cloneFoo->IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(cloneFoo->IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(cloneFoo->IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(cloneFoo->IsPrescribedAttribute("NestedScopeArray"));

			Assert::AreEqual(cloneFoo, (*cloneFoo)["this"].GetPointer()->As<AttributedFoo>());

			int& fooData = foo["ExternalInteger"].GetInt();
			int& cloneFooData = (*cloneFoo)["ExternalInteger"].GetInt();
			Assert::AreNotSame(cloneFooData, fooData);

			delete clone;
		}

		TEST_METHOD(DerivedTest)
		{
			AttributedBar bar;

			Assert::IsTrue(bar.IsAttribute("this"));
			Assert::IsTrue(bar.IsAttribute("ExternalInteger"));
			Assert::IsTrue(bar.IsAttribute("ExternalFloat"));
			Assert::IsTrue(bar.IsAttribute("ExternalString"));
			Assert::IsTrue(bar.IsAttribute("ExternalVector"));
			Assert::IsTrue(bar.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(bar.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(bar.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(bar.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(bar.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(bar.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(bar.IsAttribute("NestedScope"));
			Assert::IsTrue(bar.IsAttribute("NestedScopeArray"));
			Assert::IsTrue(bar.IsAttribute("BarExternalInteger"));

			Assert::IsTrue(bar.IsPrescribedAttribute("this"));
			Assert::IsTrue(bar.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(bar.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(bar.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(bar.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(bar.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(bar.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(bar.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(bar.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(bar.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(bar.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(bar.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(bar.IsPrescribedAttribute("NestedScopeArray"));
			Assert::IsTrue(bar.IsPrescribedAttribute("BarExternalInteger"));

			Assert::IsFalse(bar.IsAuxillaryAttribute("this"));
			Assert::IsFalse(bar.IsAuxillaryAttribute("ExternalInteger"));
			Assert::IsFalse(bar.IsAuxillaryAttribute("ExternalFloat"));
			Assert::IsFalse(bar.IsAuxillaryAttribute("ExternalString"));
			Assert::IsFalse(bar.IsAuxillaryAttribute("ExternalVector"));
			Assert::IsFalse(bar.IsAuxillaryAttribute("ExternalMatrix"));
			Assert::IsFalse(bar.IsAuxillaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(bar.IsAuxillaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(bar.IsAuxillaryAttribute("ExternalStringArray"));
			Assert::IsFalse(bar.IsAuxillaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(bar.IsAuxillaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(bar.IsAuxillaryAttribute("NestedScope"));
			Assert::IsFalse(bar.IsAuxillaryAttribute("NestedScopeArray"));
			Assert::IsFalse(bar.IsAuxillaryAttribute("BarExternalInteger"));

			Assert::AreEqual(&bar, bar["this"].GetPointer()->As<AttributedBar>());
		}


	private:
		static _CrtMemState _startMemState;
	};
	_CrtMemState AttributedTests::_startMemState;
}