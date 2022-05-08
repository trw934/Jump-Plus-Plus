#include "pch.h"
#include "CppUnitTest.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(DatumTests)
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
			Datum datum;
			Assert::AreEqual(size_t(0), datum.Capacity());
			Assert::AreEqual(size_t(0), datum.Size());
			Assert::AreEqual(DatumTypes::Unknown, datum.Type());
		}

		TEST_METHOD(AssignmentConstructor)
		{
			{
				Datum datum;
				datum = 3;
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
				Assert::AreEqual(3, datum.FrontInt());
				Assert::AreEqual(3, datum.BackInt());
				Assert::AreEqual(3, datum.GetInt());
			}

			{
				Datum datum;
				datum = 3.0f;
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(DatumTypes::Float, datum.Type());
				Assert::AreEqual(3.0f, datum.FrontFloat());
				Assert::AreEqual(3.0f, datum.BackFloat());
				Assert::AreEqual(3.0f, datum.GetFloat());
			}

			{
				glm::vec4 value = { 1,2,3,4 };
				Datum datum;
				datum = value;
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(DatumTypes::Vector, datum.Type());
				Assert::AreEqual(value, datum.FrontVector());
				Assert::AreEqual(value, datum.BackVector());
				Assert::AreEqual(value, datum.GetVector());
			}

			{
				glm::mat4 value = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
				Datum datum;
				datum = value;
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(DatumTypes::Matrix, datum.Type());
				Assert::AreEqual(value, datum.FrontMatrix());
				Assert::AreEqual(value, datum.BackMatrix());
				Assert::AreEqual(value, datum.GetMatrix());
			}

			{
				std::string value = "Hello";
				Datum datum;
				datum = value;
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(DatumTypes::String, datum.Type());
				Assert::AreEqual(value, datum.FrontString());
				Assert::AreEqual(value, datum.BackString());
				Assert::AreEqual(value, datum.GetString());
			}

			{
				std::string value = "Hello";
				Datum datum;
				datum = value;
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(DatumTypes::String, datum.Type());
				Assert::AreEqual(value, datum.FrontString());
				Assert::AreEqual(value, datum.BackString());
				Assert::AreEqual(value, datum.GetString());
			}

			{
				Foo f{ 10 };
				RTTI* value = &f;
				Datum datum;
				datum = value;
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(DatumTypes::Pointer, datum.Type());
				Assert::AreEqual(value, datum.FrontPointer());
				Assert::AreEqual(value, datum.BackPointer());
				Assert::AreEqual(value, datum.GetPointer());
			}
		}

		TEST_METHOD(CopyConstructor)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				// Testing copy constructor
				Datum copyDatum{ datum };
				Assert::AreEqual(datum.Size(), copyDatum.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum.Capacity());
				Assert::AreEqual(datum.Type(), copyDatum.Type());
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Assert::AreEqual(datum.GetInt(i), copyDatum.GetInt(i));
				}

				// Testing copy assignment operator
				Datum copyDatum2;
				copyDatum2 = datum;
				Assert::AreEqual(datum.Size(), copyDatum2.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum2.Capacity());
				Assert::AreEqual(datum.Type(), copyDatum2.Type());
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Assert::AreEqual(datum.GetInt(i), copyDatum2.GetInt(i));
				}
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				// Testing copy constructor
				Datum copyDatum{ datum };
				Assert::AreEqual(datum.Size(), copyDatum.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum.Capacity());
				Assert::AreEqual(datum.Type(), copyDatum.Type());
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Assert::AreEqual(datum.GetFloat(i), copyDatum.GetFloat(i));
				}

				// Testing copy assignment operator
				Datum copyDatum2;
				copyDatum2 = datum;
				Assert::AreEqual(datum.Size(), copyDatum2.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum2.Capacity());
				Assert::AreEqual(datum.Type(), copyDatum2.Type());
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Assert::AreEqual(datum.GetFloat(i), copyDatum2.GetFloat(i));
				}
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				// Testing copy constructor
				Datum copyDatum{ datum };
				Assert::AreEqual(datum.Size(), copyDatum.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum.Capacity());
				Assert::AreEqual(datum.Type(), copyDatum.Type());
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Assert::AreEqual(datum.GetVector(i), copyDatum.GetVector(i));
				}

				// Testing copy assignment operator
				Datum copyDatum2;
				copyDatum2 = datum;
				Assert::AreEqual(datum.Size(), copyDatum2.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum2.Capacity());
				Assert::AreEqual(datum.Type(), copyDatum2.Type());
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Assert::AreEqual(datum.GetVector(i), copyDatum2.GetVector(i));
				}
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				// Testing copy constructor
				Datum copyDatum{ datum };
				Assert::AreEqual(datum.Size(), copyDatum.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum.Capacity());
				Assert::AreEqual(datum.Type(), copyDatum.Type());
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Assert::AreEqual(datum.GetMatrix(i), copyDatum.GetMatrix(i));
				}

				// Testing copy assignment operator
				Datum copyDatum2;
				copyDatum2 = datum;
				Assert::AreEqual(datum.Size(), copyDatum2.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum2.Capacity());
				Assert::AreEqual(datum.Type(), copyDatum2.Type());
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Assert::AreEqual(datum.GetMatrix(i), copyDatum2.GetMatrix(i));
				}
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				// Testing copy constructor
				Datum copyDatum{ datum };
				Assert::AreEqual(datum.Size(), copyDatum.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum.Capacity());
				Assert::AreEqual(datum.Type(), copyDatum.Type());
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Assert::AreEqual(datum.GetString(i), copyDatum.GetString(i));
				}

				// Testing copy assignment operator
				Datum copyDatum2;
				copyDatum2 = datum;
				Assert::AreEqual(datum.Size(), copyDatum2.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum2.Capacity());
				Assert::AreEqual(datum.Type(), copyDatum2.Type());
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Assert::AreEqual(datum.GetString(i), copyDatum2.GetString(i));
				}
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				// Testing copy constructor
				Datum copyDatum{ datum };
				Assert::AreEqual(datum.Size(), copyDatum.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum.Capacity());
				Assert::AreEqual(datum.Type(), copyDatum.Type());
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Assert::AreEqual(datum.GetPointer(i), copyDatum.GetPointer(i));
				}

				// Testing copy assignment operator
				Datum copyDatum2;
				copyDatum2 = datum;
				Assert::AreEqual(datum.Size(), copyDatum2.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum2.Capacity());
				Assert::AreEqual(datum.Type(), copyDatum2.Type());
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Assert::AreEqual(datum.GetPointer(i), copyDatum2.GetPointer(i));
				}
			}
		}

		TEST_METHOD(ExternalCopyConstructor)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;

				int arr[3]{ a,b,c };

				Datum externalDatum;
				externalDatum.SetStorage(arr, 3);

				Datum internalDatum;
				internalDatum.PushBack(a);
				internalDatum.PushBack(b);
				internalDatum.PushBack(c);

				//Testing copy constructor
				Datum copyDatum{ externalDatum };
				Assert::AreEqual(externalDatum.Size(), copyDatum.Size());
				Assert::AreEqual(externalDatum.Capacity(), copyDatum.Capacity());
				Assert::AreEqual(externalDatum.Type(), copyDatum.Type());
				for (size_t i = 0; i < externalDatum.Size(); ++i)
				{
					Assert::AreEqual(externalDatum.GetInt(i), copyDatum.GetInt(i));
				}
				auto expression = [&copyDatum]() {copyDatum.Clear(); };
				Assert::ExpectException<std::runtime_error>(expression);

				//Testing copy assignment operator
				Datum copyExternal; // External to External Copy
				copyExternal.SetStorage(arr, 3);
				copyExternal = externalDatum;
				Assert::AreEqual(externalDatum.Size(), copyExternal.Size());
				Assert::AreEqual(externalDatum.Capacity(), copyExternal.Capacity());
				Assert::AreEqual(externalDatum.Type(), copyExternal.Type());
				for (size_t i = 0; i < externalDatum.Size(); ++i)
				{
					Assert::AreEqual(externalDatum.GetInt(i), copyExternal.GetInt(i));
				}
				auto expression2 = [&copyExternal]() {copyExternal.Clear(); };
				Assert::ExpectException<std::runtime_error>(expression2);

				Datum copyInternal; // External to Internal Copy
				copyInternal = externalDatum;
				Assert::AreEqual(externalDatum.Size(), copyInternal.Size());
				Assert::AreEqual(externalDatum.Capacity(), copyInternal.Capacity());
				Assert::AreEqual(externalDatum.Type(), copyInternal.Type());
				for (size_t i = 0; i < externalDatum.Size(); ++i)
				{
					Assert::AreEqual(externalDatum.GetInt(i), copyInternal.GetInt(i));
				}
				auto expression3 = [&copyInternal]() {copyInternal.Clear(); };
				Assert::ExpectException<std::runtime_error>(expression3);

				Datum externalCopy; // Internal to External Copy
				externalCopy.SetStorage(arr, 3);
				externalCopy = internalDatum;
				Assert::AreEqual(internalDatum.Size(), externalCopy.Size());
				Assert::AreEqual(internalDatum.Capacity(), externalCopy.Capacity());
				Assert::AreEqual(internalDatum.Type(), externalCopy.Type());
				for (size_t i = 0; i < internalDatum.Size(); ++i)
				{
					Assert::AreEqual(internalDatum.GetInt(i), externalCopy.GetInt(i));
				}
				externalCopy.Clear();
			}
		}

		TEST_METHOD(MoveConstructor)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				// Testing move constructor
				Datum moveDatum = std::move(datum);
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(3), moveDatum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
				Assert::AreEqual(size_t(4), moveDatum.Capacity());
				
				Assert::AreEqual(a, moveDatum.GetInt(0));
				Assert::AreEqual(b, moveDatum.GetInt(1));
				Assert::AreEqual(c, moveDatum.GetInt(2));

				// Testing move assignment operator
				Datum moveDatum2;
				moveDatum2 = std::move(moveDatum);
				Assert::AreEqual(size_t(0), moveDatum.Size());
				Assert::AreEqual(size_t(3), moveDatum2.Size());
				Assert::AreEqual(size_t(0), moveDatum.Capacity());
				Assert::AreEqual(size_t(4), moveDatum2.Capacity());

				Assert::AreEqual(a, moveDatum2.GetInt(0));
				Assert::AreEqual(b, moveDatum2.GetInt(1));
				Assert::AreEqual(c, moveDatum2.GetInt(2));
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				// Testing move constructor
				Datum moveDatum = std::move(datum);
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(3), moveDatum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
				Assert::AreEqual(size_t(4), moveDatum.Capacity());

				Assert::AreEqual(a, moveDatum.GetFloat(0));
				Assert::AreEqual(b, moveDatum.GetFloat(1));
				Assert::AreEqual(c, moveDatum.GetFloat(2));

				// Testing move assignment operator
				Datum moveDatum2;
				moveDatum2 = std::move(moveDatum);
				Assert::AreEqual(size_t(0), moveDatum.Size());
				Assert::AreEqual(size_t(3), moveDatum2.Size());
				Assert::AreEqual(size_t(0), moveDatum.Capacity());
				Assert::AreEqual(size_t(4), moveDatum2.Capacity());

				Assert::AreEqual(a, moveDatum2.GetFloat(0));
				Assert::AreEqual(b, moveDatum2.GetFloat(1));
				Assert::AreEqual(c, moveDatum2.GetFloat(2));
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				// Testing move constructor
				Datum moveDatum = std::move(datum);
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(3), moveDatum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
				Assert::AreEqual(size_t(4), moveDatum.Capacity());

				Assert::AreEqual(a, moveDatum.GetVector(0));
				Assert::AreEqual(b, moveDatum.GetVector(1));
				Assert::AreEqual(c, moveDatum.GetVector(2));

				// Testing move assignment operator
				Datum moveDatum2;
				moveDatum2 = std::move(moveDatum);
				Assert::AreEqual(size_t(0), moveDatum.Size());
				Assert::AreEqual(size_t(3), moveDatum2.Size());
				Assert::AreEqual(size_t(0), moveDatum.Capacity());
				Assert::AreEqual(size_t(4), moveDatum2.Capacity());

				Assert::AreEqual(a, moveDatum2.GetVector(0));
				Assert::AreEqual(b, moveDatum2.GetVector(1));
				Assert::AreEqual(c, moveDatum2.GetVector(2));
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				// Testing move constructor
				Datum moveDatum = std::move(datum);
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(3), moveDatum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
				Assert::AreEqual(size_t(4), moveDatum.Capacity());

				Assert::AreEqual(a, moveDatum.GetMatrix(0));
				Assert::AreEqual(b, moveDatum.GetMatrix(1));
				Assert::AreEqual(c, moveDatum.GetMatrix(2));

				// Testing move assignment operator
				Datum moveDatum2;
				moveDatum2 = std::move(moveDatum);
				Assert::AreEqual(size_t(0), moveDatum.Size());
				Assert::AreEqual(size_t(3), moveDatum2.Size());
				Assert::AreEqual(size_t(0), moveDatum.Capacity());
				Assert::AreEqual(size_t(4), moveDatum2.Capacity());

				Assert::AreEqual(a, moveDatum2.GetMatrix(0));
				Assert::AreEqual(b, moveDatum2.GetMatrix(1));
				Assert::AreEqual(c, moveDatum2.GetMatrix(2));
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				// Testing move constructor
				Datum moveDatum = std::move(datum);
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(3), moveDatum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
				Assert::AreEqual(size_t(4), moveDatum.Capacity());

				Assert::AreEqual(a, moveDatum.GetString(0));
				Assert::AreEqual(b, moveDatum.GetString(1));
				Assert::AreEqual(c, moveDatum.GetString(2));

				// Testing move assignment operator
				Datum moveDatum2;
				moveDatum2 = std::move(moveDatum);
				Assert::AreEqual(size_t(0), moveDatum.Size());
				Assert::AreEqual(size_t(3), moveDatum2.Size());
				Assert::AreEqual(size_t(0), moveDatum.Capacity());
				Assert::AreEqual(size_t(4), moveDatum2.Capacity());

				Assert::AreEqual(a, moveDatum2.GetString(0));
				Assert::AreEqual(b, moveDatum2.GetString(1));
				Assert::AreEqual(c, moveDatum2.GetString(2));
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				// Testing move constructor
				Datum moveDatum = std::move(datum);
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(3), moveDatum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
				Assert::AreEqual(size_t(4), moveDatum.Capacity());

				Assert::AreEqual(a, moveDatum.GetPointer(0));
				Assert::AreEqual(b, moveDatum.GetPointer(1));
				Assert::AreEqual(c, moveDatum.GetPointer(2));

				// Testing move assignment operator
				Datum moveDatum2;
				moveDatum2 = std::move(moveDatum);
				Assert::AreEqual(size_t(0), moveDatum.Size());
				Assert::AreEqual(size_t(3), moveDatum2.Size());
				Assert::AreEqual(size_t(0), moveDatum.Capacity());
				Assert::AreEqual(size_t(4), moveDatum2.Capacity());

				Assert::AreEqual(a, moveDatum2.GetPointer(0));
				Assert::AreEqual(b, moveDatum2.GetPointer(1));
				Assert::AreEqual(c, moveDatum2.GetPointer(2));
			}
		}

		TEST_METHOD(DatumComparisonOperators)
		{
			{
				Datum datum;
				Datum other;
				Assert::IsTrue(datum == other);
				Assert::IsFalse(datum != other);
			}
			{
				int a = 1;
				int b = 2;
				int c = 3;
				int d = 4;

				Datum datum;
				Datum other;

				datum.PushBack(a);
				Assert::IsFalse(datum == other);
				other.PushBack(a);
				Assert::IsTrue(datum == other);
				Assert::IsFalse(datum != other);

				datum.PushBack(b);
				other.PushBack(b);
				Assert::IsTrue(datum == other);
				Assert::IsFalse(datum != other);

				datum.PushBack(c);
				other.PushBack(d);
				Assert::IsTrue(datum != other);
				Assert::IsFalse(datum == other);
				
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;
				float d = 4.0f;

				Datum datum;
				Datum other;

				datum.PushBack(a);
				other.PushBack(a);
				Assert::IsTrue(datum == other);
				Assert::IsFalse(datum != other);

				datum.PushBack(b);
				other.PushBack(b);
				Assert::IsTrue(datum == other);
				Assert::IsFalse(datum != other);

				datum.PushBack(c);
				other.PushBack(d);
				Assert::IsTrue(datum != other);
				Assert::IsFalse(datum == other);
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };
				glm::vec4 d = { 1,4,3,2 };

				Datum datum;
				Datum other;

				datum.PushBack(a);
				other.PushBack(a);
				Assert::IsTrue(datum == other);
				Assert::IsFalse(datum != other);

				datum.PushBack(b);
				other.PushBack(b);
				Assert::IsTrue(datum == other);
				Assert::IsFalse(datum != other);

				datum.PushBack(c);
				other.PushBack(d);
				Assert::IsTrue(datum != other);
				Assert::IsFalse(datum == other);
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };
				glm::mat4 d = { 1,2,3,4,5,6,7,8,9,10,68,12,13,14,15,16 };

				Datum datum;
				Datum other;

				datum.PushBack(a);
				other.PushBack(a);
				Assert::IsTrue(datum == other);
				Assert::IsFalse(datum != other);

				datum.PushBack(b);
				other.PushBack(b);
				Assert::IsTrue(datum == other);
				Assert::IsFalse(datum != other);

				datum.PushBack(c);
				other.PushBack(d);
				Assert::IsTrue(datum != other);
				Assert::IsFalse(datum == other);
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";
				std::string d = "Error";

				Datum datum;
				Datum other;

				datum.PushBack(a);
				other.PushBack(a);
				Assert::IsTrue(datum == other);
				Assert::IsFalse(datum != other);

				datum.PushBack(b);
				other.PushBack(b);
				Assert::IsTrue(datum == other);
				Assert::IsFalse(datum != other);

				datum.PushBack(c);
				other.PushBack(d);
				Assert::IsTrue(datum != other);
				Assert::IsFalse(datum == other);
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;
				Foo k{ 40 }; 
				RTTI* d = &k;

				Datum datum;
				Datum other;

				datum.PushBack(a);
				other.PushBack(a);
				Assert::IsTrue(datum == other);
				Assert::IsFalse(datum != other);

				datum.PushBack(b);
				other.PushBack(b);
				Assert::IsTrue(datum == other);
				Assert::IsFalse(datum != other);

				datum.PushBack(c);
				other.PushBack(d);
				Assert::IsTrue(datum != other);
				Assert::IsFalse(datum == other);
			}
		}

		TEST_METHOD(DatumTypeComparisonOperators)
		{
			{
				int value = 10;
				int value2 = 20;
				Datum datum;
				datum = value;
				Assert::IsTrue(datum == value);
				Assert::IsFalse(datum != value);
				Assert::IsFalse(datum == value2);
				Assert::IsTrue(datum != value2);
			}

			{
				float value = 10.0f;
				float value2 = 20.0f;
				Datum datum;
				datum = value;
				Assert::IsTrue(datum == value);
				Assert::IsFalse(datum != value);
				Assert::IsFalse(datum == value2);
				Assert::IsTrue(datum != value2);
			}

			{
				glm::vec4 value = { 1,2,3,4 };
				glm::vec4 value2 = {5,6,7,8};
				Datum datum;
				datum = value;
				Assert::IsTrue(datum == value);
				Assert::IsFalse(datum != value);
				Assert::IsFalse(datum == value2);
				Assert::IsTrue(datum != value2);
			}

			{
				glm::mat4 value = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 value2 = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
				Datum datum;
				datum = value;
				Assert::IsTrue(datum == value);
				Assert::IsFalse(datum != value);
				Assert::IsFalse(datum == value2);
				Assert::IsTrue(datum != value2);
			}

			{
				std::string value = "Hello";
				std::string value2 = "Goodbye";
				Datum datum;
				datum = value;
				Assert::IsTrue(datum == value);
				Assert::IsFalse(datum != value);
				Assert::IsFalse(datum == value2);
				Assert::IsTrue(datum != value2);
			}

			{
				Foo f{ 10 };
				Foo g{ 20 };
				RTTI* value = &f;
				RTTI* value2 = &g;
				Datum datum;
				datum = value;
				Assert::IsTrue(datum == value);
				Assert::IsFalse(datum != value);
				Assert::IsFalse(datum == value2);
				Assert::IsTrue(datum != value2);
			}
		}

		TEST_METHOD(SetType)
		{
			Datum datum;
			Assert::AreEqual(DatumTypes::Unknown, datum.Type());

			datum.SetType(DatumTypes::Integer);
			Assert::AreEqual(DatumTypes::Integer, datum.Type());

			auto expression = [&datum]() { datum.SetType(DatumTypes::Float); };
			Assert::ExpectException<std::runtime_error>(expression);
		}

		TEST_METHOD(Reserve)
		{
			Datum d;
			auto expression = [&d]() { d.Reserve(3); };
			Assert::ExpectException<std::runtime_error>(expression);
			Assert::AreEqual(size_t(0), d.Capacity());
			
			d = 10;

			d.Reserve(3);
			Assert::AreEqual(size_t(3), d.Capacity());

			d.Reserve(2);
			Assert::AreEqual(size_t(3), d.Capacity());

			d.Reserve(5);
			Assert::AreEqual(size_t(5), d.Capacity());
		}

		TEST_METHOD(Resize)
		{
			{
				Datum datum;
				auto expression = [&datum]() { datum.Resize(3); };
				Assert::ExpectException<std::runtime_error>(expression);
			}

			{
				int a = 1;
				int b = 2;
				int c = 3;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.Resize(5);
				Assert::AreEqual(size_t(5), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());

				datum.Resize(2);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				Assert::AreEqual(b, datum.BackInt());

			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.Resize(5);
				Assert::AreEqual(size_t(5), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());

				datum.Resize(2);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				Assert::AreEqual(b, datum.BackFloat());
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.Resize(5);
				Assert::AreEqual(size_t(5), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());

				datum.Resize(2);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				Assert::AreEqual(b, datum.BackVector());
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.Resize(5);
				Assert::AreEqual(size_t(5), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());

				datum.Resize(2);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				Assert::AreEqual(b, datum.BackMatrix());
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.Resize(5);
				Assert::AreEqual(size_t(5), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());

				datum.Resize(2);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				Assert::AreEqual(b, datum.BackString());
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.Resize(5);
				Assert::AreEqual(size_t(5), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());

				datum.Resize(2);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				Assert::AreEqual(b, datum.BackPointer());
			}

		}

		TEST_METHOD(ShrinkToFit)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.ShrinkToFit();
				Assert::AreEqual(datum.Size(), datum.Capacity());

				auto expression = []() { Datum badDatum; badDatum.ShrinkToFit(); };
				Assert::ExpectException<std::runtime_error>(expression);
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.ShrinkToFit();
				Assert::AreEqual(datum.Size(), datum.Capacity());
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.ShrinkToFit();
				Assert::AreEqual(datum.Size(), datum.Capacity());
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.ShrinkToFit();
				Assert::AreEqual(datum.Size(), datum.Capacity());
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.ShrinkToFit();
				Assert::AreEqual(datum.Size(), datum.Capacity());
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.ShrinkToFit();
				Assert::AreEqual(datum.Size(), datum.Capacity());
			}

		}

		TEST_METHOD(Clear)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());

				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
			}
		}

		TEST_METHOD(SetStorage)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;
				int d = 4;
				
				int arr[3]{ a,b,c };

				Datum datum;

				auto expression = [&d, &arr]() { Datum badDatum; badDatum.PushBack(d); badDatum.SetStorage(arr, 3); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum]() { int* badPtr = nullptr; datum.SetStorage(badPtr, 3); };
				Assert::ExpectException<std::runtime_error>(expression2);

				datum.SetStorage(arr, 3);

				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(a, datum.GetInt(0));
				Assert::AreEqual(b, datum.GetInt(1));
				Assert::AreEqual(c, datum.GetInt(2));
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;
				float d = 4.0f;

				float arr[3]{ a,b,c };

				Datum datum;

				auto expression = [&d, &arr]() { Datum badDatum; badDatum.PushBack(d); badDatum.SetStorage(arr, 3); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum]() { float* badPtr = nullptr; datum.SetStorage(badPtr, 3); };
				Assert::ExpectException<std::runtime_error>(expression2);

				datum.SetStorage(arr, 3);

				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(a, datum.GetFloat(0));
				Assert::AreEqual(b, datum.GetFloat(1));
				Assert::AreEqual(c, datum.GetFloat(2));
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };
				glm::vec4 d = { 1,7,3,4 };

				glm::vec4 arr[3]{ a,b,c };

				Datum datum;

				auto expression = [&d, &arr]() { Datum badDatum; badDatum.PushBack(d); badDatum.SetStorage(arr, 3); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum]() { glm::vec4* badPtr = nullptr; datum.SetStorage(badPtr, 3); };
				Assert::ExpectException<std::runtime_error>(expression2);

				datum.SetStorage(arr, 3);

				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(a, datum.GetVector(0));
				Assert::AreEqual(b, datum.GetVector(1));
				Assert::AreEqual(c, datum.GetVector(2));
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };
				glm::mat4 d = { 1,2,3,4,5,6,7,8,9,10,11,12,23,14,15,16 };

				glm::mat4 arr[3]{ a,b,c };

				Datum datum;

				auto expression = [&d, &arr]() { Datum badDatum; badDatum.PushBack(d); badDatum.SetStorage(arr, 3); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum]() { glm::mat4* badPtr = nullptr; datum.SetStorage(badPtr, 3); };
				Assert::ExpectException<std::runtime_error>(expression2);

				datum.SetStorage(arr, 3);

				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(a, datum.GetMatrix(0));
				Assert::AreEqual(b, datum.GetMatrix(1));
				Assert::AreEqual(c, datum.GetMatrix(2));
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";
				std::string d = "Error";

				std::string arr[3]{ a,b,c };

				Datum datum;

				auto expression = [&d, &arr]() { Datum badDatum; badDatum.PushBack(d); badDatum.SetStorage(arr, 3); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum]() { std::string* badPtr = nullptr; datum.SetStorage(badPtr, 3); };
				Assert::ExpectException<std::runtime_error>(expression2);

				datum.SetStorage(arr, 3);

				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(a, datum.GetString(0));
				Assert::AreEqual(b, datum.GetString(1));
				Assert::AreEqual(c, datum.GetString(2));
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;
				Foo k{ 10 };
				RTTI* d = &k;

				RTTI* arr[3]{ a,b,c };

				Datum datum;

				auto expression = [&d, &arr]() { Datum badDatum; badDatum.PushBack(d); badDatum.SetStorage(arr, 3); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum]() { RTTI** badPtr = nullptr; datum.SetStorage(badPtr, 3); };
				Assert::ExpectException<std::runtime_error>(expression2);

				datum.SetStorage(arr, 3);

				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(a, datum.GetPointer(0));
				Assert::AreEqual(b, datum.GetPointer(1));
				Assert::AreEqual(c, datum.GetPointer(2));
			}
		}

		TEST_METHOD(ExternalStorageFails)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;
				int d = 4;

				int arr[3]{ a,b,c };

				Datum datum;
				datum.SetStorage(arr, 3);

				auto expression = [&datum]() { datum.SetType(DatumTypes::Float); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum]() { datum.Reserve(5); };
				Assert::ExpectException<std::runtime_error>(expression2);

				auto expression3 = [&datum]() { datum.Resize(5); };
				Assert::ExpectException<std::runtime_error>(expression3);

				auto expression4 = [&datum]() { datum.ShrinkToFit(); };
				Assert::ExpectException<std::runtime_error>(expression4);

				auto expression5 = [&datum]() { datum.Clear(); };
				Assert::ExpectException<std::runtime_error>(expression5);

				auto expression6 = [&datum]() { datum.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expression6);

				auto expression7 = [&datum, &d]() { datum.PushBack(d); };
				Assert::ExpectException<std::runtime_error>(expression7);

				auto expression8 = [&datum, &a]() { datum.Remove(a); };
				Assert::ExpectException<std::runtime_error>(expression8);

				auto expression9 = [&datum]() { datum.RemoveAt(1); };
				Assert::ExpectException<std::runtime_error>(expression9);
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;
				float d = 4.0f;

				float arr[3]{ a,b,c };

				Datum datum;
				datum.SetStorage(arr, 3);

				auto expression = [&datum, &d]() { datum.PushBack(d); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum, &a]() { datum.Remove(a); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };
				glm::vec4 d = { 1,7,3,4 };

				glm::vec4 arr[3]{ a,b,c };

				Datum datum;
				datum.SetStorage(arr, 3);

				auto expression = [&datum, &d]() { datum.PushBack(d); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum, &a]() { datum.Remove(a); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };
				glm::mat4 d = { 1,2,3,4,5,6,7,8,9,10,11,12,23,14,15,16 };

				glm::mat4 arr[3]{ a,b,c };

				Datum datum;
				datum.SetStorage(arr, 3);

				auto expression = [&datum, &d]() { datum.PushBack(d); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum, &a]() { datum.Remove(a); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";
				std::string d = "Error";

				std::string arr[3]{ a,b,c };

				Datum datum;
				datum.SetStorage(arr, 3);

				auto expression = [&datum, &d]() { datum.PushBack(d); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum, &a]() { datum.Remove(a); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;
				Foo k{ 10 };
				RTTI* d = &k;

				RTTI* arr[3]{ a,b,c };

				Datum datum;
				datum.SetStorage(arr, 3);

				auto expression = [&datum, &d]() { datum.PushBack(d); };
				Assert::ExpectException<std::runtime_error>(expression);

				auto expression2 = [&datum, &a]() { datum.Remove(a); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}
		}

		TEST_METHOD(SetGet)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;
				int d = 4;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.Set(d, 2);
				Assert::AreEqual(d, datum.GetInt(2));

				datum.Set(b);
				Assert::AreEqual(b, datum.FrontInt());

				auto expression = [&datum, &d]() { datum.Set(d, 5); };
				Assert::ExpectException<std::out_of_range>(expression);

				auto expression2 = [&datum]() { datum.GetInt(5); };
				Assert::ExpectException<std::out_of_range>(expression2);

				auto expression3 = [&datum]() { float e = 4.0f; datum.Set(e); };
				Assert::ExpectException<std::runtime_error>(expression3);

				auto expression4 = [&datum]() { datum.GetFloat(1); };
				Assert::ExpectException<std::runtime_error>(expression4);
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;
				float d = 4.0f;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.Set(d, 2);
				Assert::AreEqual(d, datum.GetFloat(2));

				datum.Set(b);
				Assert::AreEqual(b, datum.FrontFloat());

				auto expression = [&datum, &d]() { datum.Set(d, 5); };
				Assert::ExpectException<std::out_of_range>(expression);

				auto expression2 = [&datum]() { datum.GetFloat(5); };
				Assert::ExpectException<std::out_of_range>(expression2);

				auto expression3 = [&datum]() { glm::vec4 e = { 1,2,3,4 }; datum.Set(e); };
				Assert::ExpectException<std::runtime_error>(expression3);

				auto expression4 = [&datum]() { datum.GetVector(1); };
				Assert::ExpectException<std::runtime_error>(expression4);
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };
				glm::vec4 d = { 1,7,3,4 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.Set(d, 2);
				Assert::AreEqual(d, datum.GetVector(2));

				datum.Set(b);
				Assert::AreEqual(b, datum.FrontVector());

				auto expression = [&datum, &d]() { datum.Set(d, 5); };
				Assert::ExpectException<std::out_of_range>(expression);

				auto expression2 = [&datum]() { datum.GetVector(5); };
				Assert::ExpectException<std::out_of_range>(expression2);

				auto expression3 = [&datum]() { glm::mat4 e = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }; datum.Set(e); };
				Assert::ExpectException<std::runtime_error>(expression3);

				auto expression4 = [&datum]() { datum.GetMatrix(1); };
				Assert::ExpectException<std::runtime_error>(expression4);
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };
				glm::mat4 d = { 1,2,3,4,5,6,7,8,9,10,11,12,23,14,15,16 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.Set(d, 2);
				Assert::AreEqual(d, datum.GetMatrix(2));

				datum.Set(b);
				Assert::AreEqual(b, datum.FrontMatrix());

				auto expression = [&datum, &d]() { datum.Set(d, 5); };
				Assert::ExpectException<std::out_of_range>(expression);

				auto expression2 = [&datum]() { datum.GetMatrix(5); };
				Assert::ExpectException<std::out_of_range>(expression2);

				auto expression3 = [&datum]() { std::string e = "Error"; datum.Set(e); };
				Assert::ExpectException<std::runtime_error>(expression3);

				auto expression4 = [&datum]() { datum.GetString(1); };
				Assert::ExpectException<std::runtime_error>(expression4);
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";
				std::string d = "Error";

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.Set(d, 2);
				Assert::AreEqual(d, datum.GetString(2));

				datum.Set(b);
				Assert::AreEqual(b, datum.FrontString());

				auto expression = [&datum, &d]() { datum.Set(d, 5); };
				Assert::ExpectException<std::out_of_range>(expression);

				auto expression2 = [&datum]() { datum.GetString(5); };
				Assert::ExpectException<std::out_of_range>(expression2);

				auto expression3 = [&datum]() { Foo f{ 10 }; RTTI* e = &f; datum.Set(e); };
				Assert::ExpectException<std::runtime_error>(expression3);

				auto expression4 = [&datum]() { datum.GetPointer(1); };
				Assert::ExpectException<std::runtime_error>(expression4);
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;
				Foo k{ 10 }; 
				RTTI* d = &k;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.Set(d, 2);
				Assert::AreEqual(d, datum.GetPointer(2));

				datum.Set(b);
				Assert::AreEqual(b, datum.FrontPointer());

				auto expression = [&datum, &d]() { datum.Set(d, 5); };
				Assert::ExpectException<std::out_of_range>(expression);

				auto expression2 = [&datum]() { datum.GetPointer(5); };
				Assert::ExpectException<std::out_of_range>(expression2);

				auto expression3 = [&datum]() { int e = 4; datum.Set(e); };
				Assert::ExpectException<std::runtime_error>(expression3);

				auto expression4 = [&datum]() { datum.GetInt(1); };
				Assert::ExpectException<std::runtime_error>(expression4);
			}
		}

		TEST_METHOD(ConstGet)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(a, constDatum.GetInt(0));
				Assert::AreEqual(b, constDatum.GetInt(1));
				Assert::AreEqual(c, constDatum.GetInt(2));

				auto expression = [&constDatum]() { constDatum.GetInt(5); };
				Assert::ExpectException<std::out_of_range>(expression);

				auto expression2 = [&constDatum]() { constDatum.GetFloat(1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(a, constDatum.GetFloat(0));
				Assert::AreEqual(b, constDatum.GetFloat(1));
				Assert::AreEqual(c, constDatum.GetFloat(2));

				auto expression = [&constDatum]() { constDatum.GetFloat(5); };
				Assert::ExpectException<std::out_of_range>(expression);

				auto expression2 = [&constDatum]() { constDatum.GetVector(1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(a, constDatum.GetVector(0));
				Assert::AreEqual(b, constDatum.GetVector(1));
				Assert::AreEqual(c, constDatum.GetVector(2));

				auto expression = [&constDatum]() { constDatum.GetVector(5); };
				Assert::ExpectException<std::out_of_range>(expression);

				auto expression2 = [&constDatum]() { constDatum.GetMatrix(1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(a, constDatum.GetMatrix(0));
				Assert::AreEqual(b, constDatum.GetMatrix(1));
				Assert::AreEqual(c, constDatum.GetMatrix(2));

				auto expression = [&constDatum]() { constDatum.GetMatrix(5); };
				Assert::ExpectException<std::out_of_range>(expression);

				auto expression2 = [&constDatum]() { constDatum.GetString(1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(a, constDatum.GetString(0));
				Assert::AreEqual(b, constDatum.GetString(1));
				Assert::AreEqual(c, constDatum.GetString(2));

				auto expression = [&constDatum]() { constDatum.GetString(5); };
				Assert::ExpectException<std::out_of_range>(expression);

				auto expression2 = [&constDatum]() { constDatum.GetPointer(1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(a, constDatum.GetPointer(0));
				Assert::AreEqual(b, constDatum.GetPointer(1));
				Assert::AreEqual(c, constDatum.GetPointer(2));

				auto expression = [&constDatum]() { constDatum.GetPointer(5); };
				Assert::ExpectException<std::out_of_range>(expression);

				auto expression2 = [&constDatum]() { constDatum.GetInt(1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}
		}

		TEST_METHOD(SetFromString)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;
				int d = 4;
				std::string string = "4";

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.SetFromString(string, 2);
				Assert::AreEqual(d, datum.GetInt(2));

				auto expression = [&datum, &string]() { datum.SetFromString(string, 5); };
				Assert::ExpectException<std::out_of_range>(expression);

				auto expression2 = [&string]() { Datum badDatum; badDatum.SetFromString(string, 1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;
				float d = 4.0f;
				std::string string = "4.0f";

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.SetFromString(string, 2);
				Assert::AreEqual(d, datum.GetFloat(2));
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };
				glm::vec4 d = { 1,7,3,4 };
				std::string string = "vec4(1, 7, 3, 4)";

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.SetFromString(string, 2);
				Assert::AreEqual(d, datum.GetVector(2));
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };
				glm::mat4 d = { 1,2,3,4,5,6,7,8,9,10,11,12,23,14,15,16 };
				std::string string = "mat4x4((1,2,3,4),(5,6,7,8),(9,10,11,12),(23,14,15,16))";

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.SetFromString(string, 2);
				Assert::AreEqual(d, datum.GetMatrix(2));
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";
				std::string d = "Error";

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				datum.SetFromString(d, 2);
				Assert::AreEqual(d, datum.GetString(2));
			}
		}

		TEST_METHOD(ToString)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(std::string("2"), datum.ToString(1));

				auto expression = [&datum]() { datum.ToString(5); };
				Assert::ExpectException<std::out_of_range>(expression);

				auto expression2 = []() { Datum badDatum; badDatum.ToString(1); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(std::string("2.000000"), datum.ToString(1));
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(std::string("vec4(1.000000, 2.000000, 1.000000, 4.000000)"), datum.ToString(1));
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(std::string("mat4x4((1.000000, 2.000000, 3.000000, 4.000000), (5.000000, 6.000000, 7.000000, 8.000000), (9.000000, 10.000000, 11.000000, 12.000000), (3.000000, 14.000000, 15.000000, 16.000000))"), datum.ToString(1));
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::AreEqual(b, datum.ToString(1));
			}
		}

		TEST_METHOD(PushBack)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;

				Datum datum;
				Assert::AreEqual(size_t(0), datum.Capacity());
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());

				datum.PushBack(a);
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(DatumTypes::Integer, datum.Type());

				datum.PushBack(b);
				Assert::AreEqual(size_t(2), datum.Capacity());
				Assert::AreEqual(size_t(2), datum.Size());

				datum.PushBack(c);
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(size_t(3), datum.Size());

				auto expression = [&datum]() { float d = 4.0f; datum.PushBack(d); };
				Assert::ExpectException<std::runtime_error>(expression);
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;

				Datum datum;
				Assert::AreEqual(size_t(0), datum.Capacity());
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());

				datum.PushBack(a);
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(DatumTypes::Float, datum.Type());

				datum.PushBack(b);
				Assert::AreEqual(size_t(2), datum.Capacity());
				Assert::AreEqual(size_t(2), datum.Size());

				datum.PushBack(c);
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(size_t(3), datum.Size());

				auto expression = [&datum]() { glm::vec4 d = { 1,2,3,4 }; datum.PushBack(d); };
				Assert::ExpectException<std::runtime_error>(expression);
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };

				Datum datum;
				Assert::AreEqual(size_t(0), datum.Capacity());
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());

				datum.PushBack(a);
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(DatumTypes::Vector, datum.Type());

				datum.PushBack(b);
				Assert::AreEqual(size_t(2), datum.Capacity());
				Assert::AreEqual(size_t(2), datum.Size());

				datum.PushBack(c);
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(size_t(3), datum.Size());

				auto expression = [&datum]() { glm::mat4 d = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }; datum.PushBack(d); };
				Assert::ExpectException<std::runtime_error>(expression);
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };

				Datum datum;
				Assert::AreEqual(size_t(0), datum.Capacity());
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());

				datum.PushBack(a);
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(DatumTypes::Matrix, datum.Type());

				datum.PushBack(b);
				Assert::AreEqual(size_t(2), datum.Capacity());
				Assert::AreEqual(size_t(2), datum.Size());

				datum.PushBack(c);
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(size_t(3), datum.Size());

				auto expression = [&datum]() { std::string d = "Error"; datum.PushBack(d); };
				Assert::ExpectException<std::runtime_error>(expression);
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";

				Datum datum;
				Assert::AreEqual(size_t(0), datum.Capacity());
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());

				datum.PushBack(a);
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(DatumTypes::String, datum.Type());

				datum.PushBack(b);
				Assert::AreEqual(size_t(2), datum.Capacity());
				Assert::AreEqual(size_t(2), datum.Size());

				datum.PushBack(c);
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(size_t(3), datum.Size());

				auto expression = [&datum]() { Foo f{ 10 }; RTTI* d = &f; datum.PushBack(d); };
				Assert::ExpectException<std::runtime_error>(expression);
			}

			{
				Foo f{ 10 }; 
				RTTI* a = &f;
				Foo g{ 20 }; 
				RTTI* b = &g;
				Foo h{ 30 }; 
				RTTI* c = &h;

				Datum datum;
				Assert::AreEqual(size_t(0), datum.Capacity());
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());

				datum.PushBack(a);
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(DatumTypes::Pointer, datum.Type());

				datum.PushBack(b);
				Assert::AreEqual(size_t(2), datum.Capacity());
				Assert::AreEqual(size_t(2), datum.Size());

				datum.PushBack(c);
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(size_t(3), datum.Size());

				auto expression = [&datum]() { int d = 4; datum.PushBack(d); };
				Assert::ExpectException<std::runtime_error>(expression);
			}
		}

		TEST_METHOD(PopBack)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(size_t(3), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(2), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(1), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(0), datum.Size());

				auto expression = [&datum]() { datum.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expression);
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(size_t(3), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(2), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(1), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(0), datum.Size());

				auto expression = [&datum]() { datum.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expression);
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(size_t(3), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(2), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(1), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(0), datum.Size());

				auto expression = [&datum]() { datum.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expression);
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(size_t(3), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(2), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(1), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(0), datum.Size());

				auto expression = [&datum]() { datum.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expression);
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(size_t(3), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(2), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(1), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(0), datum.Size());

				auto expression = [&datum]() { datum.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expression);
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(size_t(3), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(2), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(1), datum.Size());

				datum.PopBack();
				Assert::AreEqual(size_t(0), datum.Size());

				auto expression = [&datum]() { datum.PopBack(); };
				Assert::ExpectException<std::runtime_error>(expression);
			}
		}

		TEST_METHOD(Front)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;

				Datum datum;
				auto expression = [&datum]() { datum.FrontInt(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				Assert::AreEqual(a, datum.FrontInt());
				datum.PushBack(b);
				Assert::AreEqual(a, datum.FrontInt());
				datum.PushBack(c);
				Assert::AreEqual(a, datum.FrontInt());

				auto expression2 = [&datum]() { datum.FrontFloat(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;

				Datum datum;
				auto expression = [&datum]() { datum.FrontFloat(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				Assert::AreEqual(a, datum.FrontFloat());
				datum.PushBack(b);
				Assert::AreEqual(a, datum.FrontFloat());
				datum.PushBack(c);
				Assert::AreEqual(a, datum.FrontFloat());

				auto expression2 = [&datum]() { datum.FrontVector(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };

				Datum datum;
				auto expression = [&datum]() { datum.FrontVector(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				Assert::AreEqual(a, datum.FrontVector());
				datum.PushBack(b);
				Assert::AreEqual(a, datum.FrontVector());
				datum.PushBack(c);
				Assert::AreEqual(a, datum.FrontVector());

				auto expression2 = [&datum]() { datum.FrontMatrix(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };

				Datum datum;
				auto expression = [&datum]() { datum.FrontMatrix(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				Assert::AreEqual(a, datum.FrontMatrix());
				datum.PushBack(b);
				Assert::AreEqual(a, datum.FrontMatrix());
				datum.PushBack(c);
				Assert::AreEqual(a, datum.FrontMatrix());

				auto expression2 = [&datum]() { datum.FrontString(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";

				Datum datum;
				auto expression = [&datum]() { datum.FrontString(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				Assert::AreEqual(a, datum.FrontString());
				datum.PushBack(b);
				Assert::AreEqual(a, datum.FrontString());
				datum.PushBack(c);
				Assert::AreEqual(a, datum.FrontString());

				auto expression2 = [&datum]() { datum.FrontPointer(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;

				Datum datum;
				auto expression = [&datum]() { datum.FrontPointer(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				Assert::AreEqual(a, datum.FrontPointer());
				datum.PushBack(b);
				Assert::AreEqual(a, datum.FrontPointer());
				datum.PushBack(c);
				Assert::AreEqual(a, datum.FrontPointer());

				auto expression2 = [&datum]() { datum.FrontInt(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}
		}

		TEST_METHOD(ConstFront)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;

				auto expression = []() { const Datum badDatum; badDatum.FrontInt(); };
				Assert::ExpectException<std::runtime_error>(expression);

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(a, constDatum.FrontInt());

				auto expression2 = [&constDatum]() { constDatum.FrontFloat(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;

				auto expression = []() { const Datum badDatum; badDatum.FrontFloat(); };
				Assert::ExpectException<std::runtime_error>(expression);

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(a, constDatum.FrontFloat());

				auto expression2 = [&constDatum]() { constDatum.FrontVector(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };

				auto expression = []() { const Datum badDatum; badDatum.FrontVector(); };
				Assert::ExpectException<std::runtime_error>(expression);

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(a, constDatum.FrontVector());

				auto expression2 = [&constDatum]() { constDatum.FrontMatrix(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };

				auto expression = []() { const Datum badDatum; badDatum.FrontMatrix(); };
				Assert::ExpectException<std::runtime_error>(expression);

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(a, constDatum.FrontMatrix());

				auto expression2 = [&constDatum]() { constDatum.FrontString(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";

				auto expression = []() { const Datum badDatum; badDatum.FrontString(); };
				Assert::ExpectException<std::runtime_error>(expression);

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(a, constDatum.FrontString());

				auto expression2 = [&constDatum]() { constDatum.FrontPointer(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;

				auto expression = []() { const Datum badDatum; badDatum.FrontPointer(); };
				Assert::ExpectException<std::runtime_error>(expression);

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(a, constDatum.FrontPointer());

				auto expression2 = [&constDatum]() { constDatum.FrontInt(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}
		}

		TEST_METHOD(Back)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;

				Datum datum;
				auto expression = [&datum]() { datum.BackInt(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				Assert::AreEqual(a, datum.BackInt());
				datum.PushBack(b);
				Assert::AreEqual(b, datum.BackInt());
				datum.PushBack(c);
				Assert::AreEqual(c, datum.BackInt());

				auto expression2 = [&datum]() { datum.BackFloat(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;

				Datum datum;
				auto expression = [&datum]() { datum.BackFloat(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				Assert::AreEqual(a, datum.BackFloat());
				datum.PushBack(b);
				Assert::AreEqual(b, datum.BackFloat());
				datum.PushBack(c);
				Assert::AreEqual(c, datum.BackFloat());

				auto expression2 = [&datum]() { datum.BackVector(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };

				Datum datum;
				auto expression = [&datum]() { datum.BackVector(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				Assert::AreEqual(a, datum.BackVector());
				datum.PushBack(b);
				Assert::AreEqual(b, datum.BackVector());
				datum.PushBack(c);
				Assert::AreEqual(c, datum.BackVector());

				auto expression2 = [&datum]() { datum.BackMatrix(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };

				Datum datum;
				auto expression = [&datum]() { datum.BackMatrix(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				Assert::AreEqual(a, datum.BackMatrix());
				datum.PushBack(b);
				Assert::AreEqual(b, datum.BackMatrix());
				datum.PushBack(c);
				Assert::AreEqual(c, datum.BackMatrix());

				auto expression2 = [&datum]() { datum.BackString(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";

				Datum datum;
				auto expression = [&datum]() { datum.BackString(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				Assert::AreEqual(a, datum.BackString());
				datum.PushBack(b);
				Assert::AreEqual(b, datum.BackString());
				datum.PushBack(c);
				Assert::AreEqual(c, datum.BackString());

				auto expression2 = [&datum]() { datum.BackPointer(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;

				Datum datum;
				auto expression = [&datum]() { datum.BackPointer(); };
				Assert::ExpectException<std::runtime_error>(expression);

				datum.PushBack(a);
				Assert::AreEqual(a, datum.BackPointer());
				datum.PushBack(b);
				Assert::AreEqual(b, datum.BackPointer());
				datum.PushBack(c);
				Assert::AreEqual(c, datum.BackPointer());

				auto expression2 = [&datum]() { datum.BackInt(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}
		}

		TEST_METHOD(ConstBack)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;

				auto expression = []() { const Datum badDatum; badDatum.BackInt(); };
				Assert::ExpectException<std::runtime_error>(expression);

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(c, constDatum.BackInt());

				auto expression2 = [&constDatum]() { constDatum.BackFloat(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;

				auto expression = []() { const Datum badDatum; badDatum.BackFloat(); };
				Assert::ExpectException<std::runtime_error>(expression);

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(c, constDatum.BackFloat());

				auto expression2 = [&constDatum]() { constDatum.BackVector(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };

				auto expression = []() { const Datum badDatum; badDatum.BackVector(); };
				Assert::ExpectException<std::runtime_error>(expression);

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(c, constDatum.BackVector());

				auto expression2 = [&constDatum]() { constDatum.BackMatrix(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };

				auto expression = []() { const Datum badDatum; badDatum.BackMatrix(); };
				Assert::ExpectException<std::runtime_error>(expression);

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(c, constDatum.BackMatrix());

				auto expression2 = [&constDatum]() { constDatum.BackString(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";

				auto expression = []() { const Datum badDatum; badDatum.BackString(); };
				Assert::ExpectException<std::runtime_error>(expression);

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(c, constDatum.BackString());

				auto expression2 = [&constDatum]() { constDatum.BackPointer(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;

				auto expression = []() { const Datum badDatum; badDatum.BackPointer(); };
				Assert::ExpectException<std::runtime_error>(expression);

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				const Datum constDatum{ datum };
				Assert::AreEqual(c, constDatum.BackPointer());

				auto expression2 = [&constDatum]() { constDatum.BackInt(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}
		}

		TEST_METHOD(Find)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;
				int d = 4;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				
				auto result = datum.Find(a);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(0), result.second);

				result = datum.Find(b);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(1), result.second);

				result = datum.Find(c);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(2), result.second);

				result = datum.Find(d);
				Assert::IsFalse(result.first);
				Assert::AreEqual(size_t(3), result.second);

				float value = 10.f;
				result = datum.Find(value);
				Assert::IsFalse(result.first);
				Assert::AreEqual(size_t(3), result.second);
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;
				float d = 4.0f;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				auto result = datum.Find(a);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(0), result.second);

				result = datum.Find(b);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(1), result.second);

				result = datum.Find(c);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(2), result.second);

				result = datum.Find(d);
				Assert::IsFalse(result.first);
				Assert::AreEqual(size_t(3), result.second);

				glm::vec4 value = { 1,4,3,2 };
				result = datum.Find(value);
				Assert::IsFalse(result.first);
				Assert::AreEqual(size_t(3), result.second);
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };
				glm::vec4 d = { 1,4,3,2 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				auto result = datum.Find(a);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(0), result.second);

				result = datum.Find(b);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(1), result.second);

				result = datum.Find(c);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(2), result.second);

				result = datum.Find(d);
				Assert::IsFalse(result.first);
				Assert::AreEqual(size_t(3), result.second);

				glm::mat4 value = { 1,2,3,4,5,6,7,8,9,10,68,12,13,14,15,16 };
				result = datum.Find(value);
				Assert::IsFalse(result.first);
				Assert::AreEqual(size_t(3), result.second);
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };
				glm::mat4 d = { 1,2,3,4,5,6,7,8,9,10,68,12,13,14,15,16 };

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				auto result = datum.Find(a);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(0), result.second);

				result = datum.Find(b);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(1), result.second);

				result = datum.Find(c);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(2), result.second);

				result = datum.Find(d);
				Assert::IsFalse(result.first);
				Assert::AreEqual(size_t(3), result.second);

				std::string value = "Error";
				result = datum.Find(value);
				Assert::IsFalse(result.first);
				Assert::AreEqual(size_t(3), result.second);
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";
				std::string d = "Error";

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				auto result = datum.Find(a);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(0), result.second);

				result = datum.Find(b);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(1), result.second);

				result = datum.Find(c);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(2), result.second);

				result = datum.Find(d);
				Assert::IsFalse(result.first);
				Assert::AreEqual(size_t(3), result.second);

				Foo k{ 40 };
				RTTI* value = &k;
				result = datum.Find(value);
				Assert::IsFalse(result.first);
				Assert::AreEqual(size_t(3), result.second);
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;
				Foo k{ 40 };
				RTTI* d = &k;

				Datum datum;
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				auto result = datum.Find(a);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(0), result.second);

				result = datum.Find(b);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(1), result.second);

				result = datum.Find(c);
				Assert::IsTrue(result.first);
				Assert::AreEqual(size_t(2), result.second);

				result = datum.Find(d);
				Assert::IsFalse(result.first);
				Assert::AreEqual(size_t(3), result.second);

				int value = 10;
				result = datum.Find(value);
				Assert::IsFalse(result.first);
				Assert::AreEqual(size_t(3), result.second);
			}
		}

		TEST_METHOD(Remove)
		{
			{
				int a = 1;
				int b = 2;
				int c = 3;
				int d = 4;

				Datum datum;
				Assert::IsFalse(datum.RemoveAt(0));

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::IsFalse(datum.RemoveAt(10));
				Assert::IsFalse(datum.Remove(d));

				Assert::IsTrue(datum.Remove(a));
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(b, datum.FrontInt());
				Assert::AreEqual(c, datum.BackInt());

				Assert::IsTrue(datum.Remove(c));
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(b, datum.FrontInt());
				Assert::AreEqual(b, datum.BackInt());

				Assert::IsTrue(datum.Remove(b));
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
			}

			{
				float a = 1.0f;
				float b = 2.0f;
				float c = 3.0f;
				float d = 4.0f;

				Datum datum;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::IsFalse(datum.Remove(d));

				Assert::IsTrue(datum.Remove(a));
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(b, datum.FrontFloat());
				Assert::AreEqual(c, datum.BackFloat());

				Assert::IsTrue(datum.Remove(c));
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(b, datum.FrontFloat());
				Assert::AreEqual(b, datum.BackFloat());

				Assert::IsTrue(datum.Remove(b));
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
			}

			{
				glm::vec4 a = { 1,2,3,4 };
				glm::vec4 b = { 1,2,1,4 };
				glm::vec4 c = { 1,2,5,4 };
				glm::vec4 d = { 1,4,3,2 };

				Datum datum;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::IsFalse(datum.Remove(d));

				Assert::IsTrue(datum.Remove(a));
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(b, datum.FrontVector());
				Assert::AreEqual(c, datum.BackVector());

				Assert::IsTrue(datum.Remove(c));
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(b, datum.FrontVector());
				Assert::AreEqual(b, datum.BackVector());

				Assert::IsTrue(datum.Remove(b));
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
			}

			{
				glm::mat4 a = { 1,2,3,4,5,2,7,8,9,10,11,12,13,14,15,16 };
				glm::mat4 b = { 1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16 };
				glm::mat4 c = { 1,2,3,4,5,6,7,8,9,7,11,12,13,14,15,16 };
				glm::mat4 d = { 1,2,3,4,5,6,7,8,9,10,68,12,13,14,15,16 };

				Datum datum;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::IsFalse(datum.Remove(d));

				Assert::IsTrue(datum.Remove(a));
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(b, datum.FrontMatrix());
				Assert::AreEqual(c, datum.BackMatrix());

				Assert::IsTrue(datum.Remove(c));
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(b, datum.FrontMatrix());
				Assert::AreEqual(b, datum.BackMatrix());

				Assert::IsTrue(datum.Remove(b));
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
			}

			{
				std::string a = "Hello";
				std::string b = "Goodbye";
				std::string c = "Bruh";
				std::string d = "Error";

				Datum datum;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::IsFalse(datum.Remove(d));

				Assert::IsTrue(datum.Remove(a));
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(b, datum.FrontString());
				Assert::AreEqual(c, datum.BackString());

				Assert::IsTrue(datum.Remove(c));
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(b, datum.FrontString());
				Assert::AreEqual(b, datum.BackString());

				Assert::IsTrue(datum.Remove(b));
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
			}

			{
				Foo f{ 10 };
				RTTI* a = &f;
				Foo g{ 20 };
				RTTI* b = &g;
				Foo h{ 30 };
				RTTI* c = &h;
				Foo k{ 40 };
				RTTI* d = &k;

				Datum datum;

				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);

				Assert::IsFalse(datum.Remove(d));

				Assert::IsTrue(datum.Remove(a));
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(b, datum.FrontPointer());
				Assert::AreEqual(c, datum.BackPointer());

				Assert::IsTrue(datum.Remove(c));
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				Assert::AreEqual(b, datum.FrontPointer());
				Assert::AreEqual(b, datum.BackPointer());

				Assert::IsTrue(datum.Remove(b));
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
			}
		}
		

	private:
		static _CrtMemState _startMemState;
	};
	_CrtMemState DatumTests::_startMemState;
}