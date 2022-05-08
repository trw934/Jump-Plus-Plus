#include "pch.h"
#include "ToStringSpecializations.h"
#include "json/json.h"
#include "fstream"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTestLibraryDesktop;
using namespace std;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonParseCoordinatorTests)
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

		TEST_METHOD(JsonParseTestHelperRTTI)
		{
			JsonTestParseHelper helper;

			RTTI* rtti = &helper;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("IJsonParseHelper"s));
			Assert::IsTrue(rtti->Is("JsonTestParseHelper"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(JsonTestParseHelper::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(JsonTestParseHelper::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			JsonTestParseHelper* h = rtti->As<JsonTestParseHelper>();
			Assert::IsNotNull(h);
			Assert::AreEqual(&helper, h);
			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(JsonTestParseHelper::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(JsonParseTestHelperWrapperRTTI)
		{
			JsonTestParseHelper::Wrapper wrapper;

			RTTI* rtti = &wrapper;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("Wrapper"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(JsonTestParseHelper::Wrapper::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(JsonTestParseHelper::Wrapper::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			JsonTestParseHelper::Wrapper* h = rtti->As<JsonTestParseHelper::Wrapper>();
			Assert::IsNotNull(h);
			Assert::AreEqual(&wrapper, h);
			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(JsonTestParseHelper::Wrapper::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(Constructor)
		{
			JsonTestParseHelper::Wrapper wrapper;
			JsonParseCoordinator parseCoordinator(wrapper);
			Assert::IsFalse(parseCoordinator.IsClone());
			Assert::AreEqual(size_t(0), parseCoordinator.Helpers().Size());
			Assert::AreEqual(&wrapper, parseCoordinator.GetWrapper().As<JsonTestParseHelper::Wrapper>());

			JsonTestParseHelper testParseHelper;
			parseCoordinator.AddHelper(testParseHelper);
			Assert::AreEqual(size_t(1), parseCoordinator.Helpers().Size());

			Assert::AreEqual(&wrapper, parseCoordinator.GetWrapper().As<JsonTestParseHelper::Wrapper>());
			Assert::AreEqual(&parseCoordinator, wrapper.GetJsonParseCoordinator());

			const auto& constWrapper = wrapper;
			Assert::AreEqual(const_cast<const JsonParseCoordinator*>(&parseCoordinator), constWrapper.GetJsonParseCoordinator());

			const auto& constParseCoordinator = parseCoordinator;
			Assert::AreEqual(const_cast<const JsonTestParseHelper::Wrapper*>(&wrapper), constParseCoordinator.GetWrapper().As<JsonTestParseHelper::Wrapper>());

			JsonTestParseHelper::Wrapper anotherWrapper;
			parseCoordinator.SetWrapper(anotherWrapper);
			Assert::AreEqual(&anotherWrapper, parseCoordinator.GetWrapper().As<JsonTestParseHelper::Wrapper>());
		}

		TEST_METHOD(MoveSemantics)
		{
			JsonTestParseHelper::Wrapper wrapper;
			JsonTestParseHelper testParseHelper;

			{
				JsonParseCoordinator parseCoordinator(wrapper);
				parseCoordinator.AddHelper(testParseHelper);

				// Move constructor
				JsonParseCoordinator otherParseCoordinator = std::move(parseCoordinator);
				Assert::IsFalse(otherParseCoordinator.IsClone());
				Assert::AreEqual(size_t(1), otherParseCoordinator.Helpers().Size());
				Assert::AreEqual(&wrapper, otherParseCoordinator.GetWrapper().As<JsonTestParseHelper::Wrapper>());
				Assert::AreEqual(&otherParseCoordinator, wrapper.GetJsonParseCoordinator());
			}

			{
				JsonParseCoordinator parseCoordinator(wrapper);
				parseCoordinator.AddHelper(testParseHelper);

				// Move assignment operator
				JsonTestParseHelper::Wrapper otherWrapper;
				JsonParseCoordinator otherParseCoordinator(otherWrapper);
				otherParseCoordinator = std::move(parseCoordinator);
				Assert::IsFalse(otherParseCoordinator.IsClone());
				Assert::AreEqual(size_t(1), otherParseCoordinator.Helpers().Size());
				Assert::AreEqual(&wrapper, otherParseCoordinator.GetWrapper().As<JsonTestParseHelper::Wrapper>());
				Assert::AreEqual(&otherParseCoordinator, wrapper.GetJsonParseCoordinator());
			}
		}

		TEST_METHOD(ParseFromString)
		{
			const string inputString = R"(
			{
				"Address":
				{
					"type": "table",
					"value":
					{
						"Street":
						{
							"type": "string",
							"value": "123 Anystreet St."
						}
					}
				}
			})"s;

			JsonTestParseHelper::Wrapper wrapper;
			JsonTestParseHelper testParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(testParseHelper);

			Assert::IsFalse(testParseHelper.InitializeCalled);
			parseCoordinator.Initialize();
			Assert::IsTrue(testParseHelper.InitializeCalled);

			Assert::AreEqual(0U, testParseHelper.StartHandlerCount);
			Assert::AreEqual(0U, testParseHelper.EndHandlerCount);
			Assert::AreEqual(0U, wrapper.Depth());
			Assert::AreEqual(0U, wrapper.MaxDepth);

			parseCoordinator.Parse(inputString);

			Assert::AreEqual(6U, testParseHelper.StartHandlerCount);
			Assert::AreEqual(6U, testParseHelper.EndHandlerCount);
			Assert::AreEqual(0U, wrapper.Depth());
			Assert::AreEqual(4U, wrapper.MaxDepth);
		}

		TEST_METHOD(ParseFromStream)
		{
			stringstream inputStream;
			inputStream << R"(
			{
				"Health":
				{
					"type": "integer",
					"value": 100
				}
			})"s;

			JsonTestParseHelper::Wrapper wrapper;
			JsonTestParseHelper testParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(testParseHelper);

			Assert::IsFalse(testParseHelper.InitializeCalled);
			parseCoordinator.Initialize();
			Assert::IsTrue(testParseHelper.InitializeCalled);

			parseCoordinator.Parse(inputStream);

			Assert::AreEqual(3U, testParseHelper.StartHandlerCount);
			Assert::AreEqual(3U, testParseHelper.EndHandlerCount);
			Assert::AreEqual(0U, wrapper.Depth());
			Assert::AreEqual(2U, wrapper.MaxDepth);
		}

		TEST_METHOD(FileParsing)
		{
			const std::string filename = "TestFile.json";
			const string inputString = R"({ "Health": { "type": "integer", "value": 100} })";

			std::ofstream outputFile(filename);
			Assert::IsTrue(outputFile.good());

			outputFile << inputString;
			outputFile.close();

			JsonTestParseHelper::Wrapper wrapper;
			JsonTestParseHelper testParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(testParseHelper);

			Assert::IsFalse(testParseHelper.InitializeCalled);
			parseCoordinator.Initialize();
			Assert::IsTrue(testParseHelper.InitializeCalled);

			parseCoordinator.ParseFromFile(filename);

			Assert::AreEqual(3U, testParseHelper.StartHandlerCount);
			Assert::AreEqual(3U, testParseHelper.EndHandlerCount);
			Assert::AreEqual(0U, wrapper.Depth());
			Assert::AreEqual(2U, wrapper.MaxDepth);
			Assert::AreEqual(filename, parseCoordinator.GetFileName());

			Assert::ExpectException<exception>([&parseCoordinator] { parseCoordinator.ParseFromFile("NonExistentFile.json"); });
		}

		TEST_METHOD(Clone)
		{
			{
				JsonTestParseHelper::Wrapper wrapper;
				JsonParseCoordinator parseCoordinator(wrapper);
				Assert::IsFalse(parseCoordinator.IsClone());

				JsonTestParseHelper testParseHelper;
				parseCoordinator.AddHelper(testParseHelper);

				JsonIntegerParseHelper integerParseHelper;
				parseCoordinator.AddHelper(integerParseHelper);

				JsonTableParseHelper tableParseHelper;
				parseCoordinator.AddHelper(tableParseHelper);

				auto clone = parseCoordinator.Clone();
				Assert::AreNotEqual(clone, &parseCoordinator);
				Assert::IsTrue(clone->IsClone());
				Assert::AreEqual(size_t(3), clone->Helpers().Size());

				auto clonedHelper = clone->Helpers()[0]->As<JsonTestParseHelper>();
				Assert::IsNotNull(clonedHelper);
				Assert::AreNotEqual(clonedHelper, parseCoordinator.Helpers()[0]->As<JsonTestParseHelper>());
				Assert::AreNotSame(clone->GetWrapper(), parseCoordinator.GetWrapper());

				delete clone;
			}

			{
				Scope scope;
				JsonTableParseHelper::Wrapper wrapper(scope);
				JsonParseCoordinator parseCoordinator(wrapper);
				Assert::IsFalse(parseCoordinator.IsClone());

				JsonTableParseHelper tableParseHelper;
				parseCoordinator.AddHelper(tableParseHelper);

				auto clone = parseCoordinator.Clone();
				Assert::AreNotEqual(clone, &parseCoordinator);
				Assert::IsTrue(clone->IsClone());
				Assert::AreEqual(size_t(1), clone->Helpers().Size());

				auto clonedHelper = clone->Helpers()[0]->As<JsonTableParseHelper>();
				Assert::IsNotNull(clonedHelper);
				Assert::AreNotEqual(clonedHelper, parseCoordinator.Helpers()[0]->As<JsonTableParseHelper>());
				Assert::AreNotSame(clone->GetWrapper(), parseCoordinator.GetWrapper());

				delete clone;
			}
			
		}

		TEST_METHOD(ParsingInvalidJson)
		{
			{
				JsonTestParseHelper::Wrapper wrapper;
				JsonTestParseHelper testParseHelper;
				JsonParseCoordinator parseCoordinator(wrapper);
				parseCoordinator.AddHelper(testParseHelper);
				Assert::ExpectException<exception>([&parseCoordinator]
					{
						const string inputString = R"({ "integer": {100} })";
						parseCoordinator.Parse(inputString);
					});
			}

			{
				Scope scope;
				JsonTableParseHelper::Wrapper wrapper(scope);
				JsonTableParseHelper tableParseHelper;
				JsonParseCoordinator parseCoordinator(wrapper);
				parseCoordinator.AddHelper(tableParseHelper);

				Assert::ExpectException<exception>([&parseCoordinator]
					{
						const string inputString = R"(
			{
				"Health":
				{
					"type": "bungus",
					"value": 100
				}
			})"s;
						parseCoordinator.Parse(inputString);
					});
			}
		}

		TEST_METHOD(IntegerParsing)
		{
			string inputString = R"({ "integer": 100 })";

			JsonIntegerParseHelper::Wrapper wrapper;
			JsonIntegerParseHelper integerParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(integerParseHelper);

			parseCoordinator.Parse(inputString);
			Assert::AreEqual(size_t(1), wrapper._data.Size());
			Assert::AreEqual(100, wrapper._data.Front());
		}

		TEST_METHOD(IntegerArrayParsing)
		{
			string inputString = R"({ "integer": [ 10, 20, 30, 40 ] })";

			JsonIntegerParseHelper::Wrapper wrapper;
			JsonIntegerParseHelper integerParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(integerParseHelper);

			parseCoordinator.Parse(inputString);
			Assert::AreEqual(size_t(4), wrapper._data.Size());
			Assert::AreEqual(10, wrapper._data[0]);
			Assert::AreEqual(20, wrapper._data[1]);
			Assert::AreEqual(30, wrapper._data[2]);
			Assert::AreEqual(40, wrapper._data[3]);
		}

		TEST_METHOD(ValueArrayParsing)
		{
			const string inputString = R"(
			{
				"Numbers": [ 1, 2, 3 ]
			})"s;

			JsonTestParseHelper::Wrapper wrapper;
			JsonTestParseHelper testParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(testParseHelper);

			Assert::IsFalse(testParseHelper.InitializeCalled);
			parseCoordinator.Initialize();
			Assert::IsTrue(testParseHelper.InitializeCalled);

			Assert::AreEqual(0U, testParseHelper.StartHandlerCount);
			Assert::AreEqual(0U, testParseHelper.EndHandlerCount);
			Assert::AreEqual(0U, wrapper.Depth());
			Assert::AreEqual(0U, wrapper.MaxDepth);

			parseCoordinator.Parse(inputString);

			Assert::AreEqual(3U, testParseHelper.StartHandlerCount);
			Assert::AreEqual(3U, testParseHelper.EndHandlerCount);
			Assert::AreEqual(0U, wrapper.Depth());
			Assert::AreEqual(1U, wrapper.MaxDepth);
		}

		TEST_METHOD(ObjectArrayParsing)
		{
			const string inputString = R"(
			{
				"Objects":
				[
					{
						"1": 1,
						"2": 1,
						"3": 1,
						"4": 1
					},
					{ "2": 2 },
					{ "3": 3 }
				]
			})"s;

			JsonTestParseHelper::Wrapper wrapper;
			JsonTestParseHelper testParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(testParseHelper);

			Assert::IsFalse(testParseHelper.InitializeCalled);
			parseCoordinator.Initialize();
			Assert::IsTrue(testParseHelper.InitializeCalled);

			Assert::AreEqual(0U, testParseHelper.StartHandlerCount);
			Assert::AreEqual(0U, testParseHelper.EndHandlerCount);
			Assert::AreEqual(0U, wrapper.Depth());
			Assert::AreEqual(0U, wrapper.MaxDepth);

			parseCoordinator.Parse(inputString);

			Assert::AreEqual(6U, testParseHelper.StartHandlerCount);
			Assert::AreEqual(6U, testParseHelper.EndHandlerCount);
			Assert::AreEqual(0U, wrapper.Depth());
			Assert::AreEqual(2U, wrapper.MaxDepth);
		}

		TEST_METHOD(AddRemoveHelpers)
		{
			JsonTestParseHelper::Wrapper wrapper;
			JsonParseCoordinator parseCoordinator(wrapper);

			JsonTestParseHelper testParseHelper;
			Assert::AreEqual(size_t(0), parseCoordinator.Helpers().Size());
			parseCoordinator.AddHelper(testParseHelper);
			Assert::AreEqual(size_t(1), parseCoordinator.Helpers().Size());

			Assert::ExpectException<exception>([&parseCoordinator, &testParseHelper] {parseCoordinator.AddHelper(testParseHelper); });
			Assert::ExpectException<exception>([&parseCoordinator] {JsonTestParseHelper helper; parseCoordinator.AddHelper(helper); });

			JsonIntegerParseHelper integerParseHelper;
			parseCoordinator.AddHelper(integerParseHelper);
			Assert::AreEqual(size_t(2), parseCoordinator.Helpers().Size());

			parseCoordinator.RemoveHelper(testParseHelper);
			Assert::AreEqual(size_t(1), parseCoordinator.Helpers().Size());

			parseCoordinator.RemoveHelper(integerParseHelper);
			Assert::AreEqual(size_t(0), parseCoordinator.Helpers().Size());
		}

		TEST_METHOD(FallingThroughTheChain)
		{
			const string inputString = R"(
			{
				"Test": 100
			})"s;

			JsonTestParseHelper::Wrapper wrapper;
			JsonIntegerParseHelper integerParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(integerParseHelper);

			parseCoordinator.Parse(inputString);

			Assert::AreEqual(0U, wrapper.Depth());
			Assert::AreEqual(0U, wrapper.MaxDepth);
		}

		TEST_METHOD(SmallScopeParsing)
		{
			stringstream inputStream;
			inputStream << R"(
			{
				"Health":
				{
					"type": "integer",
					"value": 100
				}
			})"s;

			Scope scope;
			JsonTableParseHelper::Wrapper wrapper(scope);
			JsonTableParseHelper tableParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(tableParseHelper);

			parseCoordinator.Initialize();

			Assert::AreEqual(0U, wrapper.Depth());
			parseCoordinator.Parse(inputStream);

			Assert::AreEqual(0U, wrapper.Depth());
			Datum* datum = scope.Find("Health");
			Assert::IsNotNull(datum);
			Assert::AreEqual(DatumTypes::Integer, datum->Type());
			Assert::AreEqual(100, datum->GetInt());
		}

		TEST_METHOD(NestedScopeParsing)
		{
			stringstream inputStream;
			inputStream << R"(
			{
				"Address": {
					"type": "table",
					"class": "AttributedFoo",
					"value": {
						"ExternalInteger": {
							"type": "integer",
							"value": 100
						},
						"Street": {
							"type": "string",
							"value": "123 Anystreet St."
						},
						"City": {
							"type": "string",
							"value": "Orlando"
						},
						"Postal Code": {
							"type": "table",
							"class": "AttributedFoo",
							"value": {
								"Zip Code": {
									"type": "integer",
									"value": 12345
								},
								"Zip Code+4": {
									"type": "integer",
									"value": 6789
								}
							}
						}
					}
				}
			})"s;

			AttributedFooFactory fooFactory;
			size_t attributeCount = 13;
			Scope scope;
			JsonTableParseHelper::Wrapper wrapper(scope);
			JsonTableParseHelper tableParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(tableParseHelper);

			parseCoordinator.Initialize();

			Assert::AreEqual(0U, wrapper.Depth());
			parseCoordinator.Parse(inputStream);
			Assert::AreEqual(0U, wrapper.Depth());
			
			Datum* datum = scope.Find("Address");
			Assert::IsNotNull(datum);
			Assert::AreEqual(DatumTypes::Table, datum->Type());
			Assert::AreEqual(size_t(1), datum->Size());

			Scope& addressScope = datum->operator[](0);
			Assert::AreEqual(size_t(3 + attributeCount), addressScope.Size());
			Assert::AreEqual(&scope, addressScope.GetParent());

			Datum& attrFooDatum = addressScope["ExternalInteger"s];
			Assert::AreEqual(size_t(1), attrFooDatum.Size());
			Assert::AreEqual(100, attrFooDatum.GetInt());

			Datum& streetDatum = addressScope["Street"];
			Assert::AreEqual(DatumTypes::String, streetDatum.Type());
			Assert::AreEqual(size_t(1), streetDatum.Size());
			Assert::AreEqual("123 Anystreet St."s, streetDatum.GetString());

			Datum& cityDatum = addressScope["City"];
			Assert::AreEqual(DatumTypes::String, cityDatum.Type());
			Assert::AreEqual(size_t(1), cityDatum.Size());
			Assert::AreEqual("Orlando"s, cityDatum.GetString());

			Datum& postalCodeDatum = addressScope["Postal Code"];
			Assert::AreEqual(DatumTypes::Table, postalCodeDatum.Type());
			Assert::AreEqual(size_t(1), postalCodeDatum.Size());

			Scope& postalCodeScope = postalCodeDatum[0];
			Assert::AreEqual(size_t(2 + attributeCount), postalCodeScope.Size());
			Assert::AreEqual(&addressScope, postalCodeScope.GetParent());

			Datum& zipCodeDatum = postalCodeScope["Zip Code"];
			Assert::AreEqual(DatumTypes::Integer, zipCodeDatum.Type());
			Assert::AreEqual(size_t(1), zipCodeDatum.Size());
			Assert::AreEqual(12345, zipCodeDatum.GetInt());

			Datum& zipCodeMoreDatum = postalCodeScope["Zip Code+4"];
			Assert::AreEqual(DatumTypes::Integer, zipCodeMoreDatum.Type());
			Assert::AreEqual(size_t(1), zipCodeMoreDatum.Size());
			Assert::AreEqual(6789, zipCodeMoreDatum.GetInt());

		}

		TEST_METHOD(TestScopeFileParsing)
		{
			const std::string filename = "..\\..\\..\\..\\..\\source\\UnitTest.Library.Desktop\\TestScope.json"s;
			//const std::string filename = R"(Content\TestScope.json)"s;

			AttributedFooFactory fooFactory;
			size_t attributeCount = 13;
			Scope scope;
			JsonTableParseHelper::Wrapper wrapper(scope);
			JsonTableParseHelper tableParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(tableParseHelper);

			parseCoordinator.Initialize();

			parseCoordinator.ParseFromFile(filename);

			Datum* scopeDatum = scope.Find("Name");
			Assert::IsNotNull(scopeDatum);
			Assert::AreEqual(DatumTypes::String, scopeDatum->Type());
			Assert::AreEqual(size_t(1), scopeDatum->Size());
			Assert::AreEqual("Test Name"s, scopeDatum->GetString());

			scopeDatum = scope.Find("Health");
			Assert::IsNotNull(scopeDatum);
			Assert::AreEqual(DatumTypes::Integer, scopeDatum->Type());
			Assert::AreEqual(size_t(1), scopeDatum->Size());
			Assert::AreEqual(100, scopeDatum->GetInt());

			scopeDatum = scope.Find("Direction");
			Assert::IsNotNull(scopeDatum);
			Assert::AreEqual(DatumTypes::Vector, scopeDatum->Type());
			Assert::AreEqual(size_t(1), scopeDatum->Size());
			Assert::AreEqual(glm::vec4(1, 0, 0, 1), scopeDatum->GetVector());

			scopeDatum = scope.Find("Transform");
			Assert::IsNotNull(scopeDatum);
			Assert::AreEqual(DatumTypes::Matrix, scopeDatum->Type());
			Assert::AreEqual(size_t(1), scopeDatum->Size());
			Assert::AreEqual(glm::mat4{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}, scopeDatum->GetMatrix());

			scopeDatum = scope.Find("Aliases");
			Assert::IsNotNull(scopeDatum);
			Assert::AreEqual(DatumTypes::String, scopeDatum->Type());
			Assert::AreEqual(size_t(3), scopeDatum->Size());
			Assert::AreEqual("Bob"s, scopeDatum->GetString(0));
			Assert::AreEqual("Jim"s, scopeDatum->GetString(1));
			Assert::AreEqual("Fred"s, scopeDatum->GetString(2));

			scopeDatum = scope.Find("Address");
			Assert::IsNotNull(scopeDatum);
			Assert::AreEqual(DatumTypes::Table, scopeDatum->Type());
			Assert::AreEqual(size_t(1), scopeDatum->Size());

			Scope& addressScope = scopeDatum->operator[](0);
			Assert::AreEqual(size_t(3 + attributeCount), addressScope.Size());
			Assert::AreEqual(&scope, addressScope.GetParent());

			Datum& streetDatum = addressScope["Street"];
			Assert::AreEqual(DatumTypes::String, streetDatum.Type());
			Assert::AreEqual(size_t(1), streetDatum.Size());
			Assert::AreEqual("123 Anystreet St."s, streetDatum.GetString());

			Datum& cityDatum = addressScope["City"];
			Assert::AreEqual(DatumTypes::String, cityDatum.Type());
			Assert::AreEqual(size_t(1), cityDatum.Size());
			Assert::AreEqual("Orlando"s, cityDatum.GetString());

			Datum& postalCodeDatum = addressScope["Postal Code"];
			Assert::AreEqual(DatumTypes::Table, postalCodeDatum.Type());
			Assert::AreEqual(size_t(1), postalCodeDatum.Size());

			Scope& postalCodeScope = postalCodeDatum[0];
			Assert::AreEqual(size_t(2 + attributeCount), postalCodeScope.Size());
			Assert::AreEqual(&addressScope, postalCodeScope.GetParent());

			Datum& zipCodeDatum = postalCodeScope["Zip Code"];
			Assert::AreEqual(DatumTypes::Integer, zipCodeDatum.Type());
			Assert::AreEqual(size_t(1), zipCodeDatum.Size());
			Assert::AreEqual(12345, zipCodeDatum.GetInt());

			Datum& zipCodeMoreDatum = postalCodeScope["Zip Code+4"];
			Assert::AreEqual(DatumTypes::Integer, zipCodeMoreDatum.Type());
			Assert::AreEqual(size_t(1), zipCodeMoreDatum.Size());
			Assert::AreEqual(6789, zipCodeMoreDatum.GetInt());

			scopeDatum = scope.Find("EmptyNestedScopeArray");
			Assert::IsNotNull(scopeDatum);
			Assert::AreEqual(DatumTypes::Table, scopeDatum->Type());
			Assert::AreEqual(size_t(0), scopeDatum->Size());

			scopeDatum = scope.Find("Powers");
			Assert::IsNotNull(scopeDatum);
			Assert::AreEqual(DatumTypes::Table, scopeDatum->Type());
			Assert::AreEqual(size_t(2), scopeDatum->Size());

			Scope& powerScope = scopeDatum->operator[](0);
			Assert::AreEqual(size_t(2 + attributeCount), powerScope.Size());
			Assert::AreEqual(&scope, powerScope.GetParent());
			
			Datum& powerDatum = powerScope["Dps"];
			Assert::AreEqual(DatumTypes::Float, powerDatum.Type());
			Assert::AreEqual(1.5f, powerDatum.GetFloat());

			powerDatum = powerScope["Name"];
			Assert::AreEqual(DatumTypes::String, powerDatum.Type());
			Assert::AreEqual("Laser Vision"s, powerDatum.GetString());

			Scope& punchScope = scopeDatum->operator[](1);
			Assert::AreEqual(size_t(2 + attributeCount), punchScope.Size());
			Assert::AreEqual(&scope, punchScope.GetParent());

			Datum& punchDatum = punchScope["Dps"];
			Assert::AreEqual(DatumTypes::Float, punchDatum.Type());
			Assert::AreEqual(5.0f, punchDatum.GetFloat());

			punchDatum = punchScope["Name"];
			Assert::AreEqual(DatumTypes::String, punchDatum.Type());
			Assert::AreEqual("Megaton Punch"s, punchDatum.GetString());


		}

	private:
		static _CrtMemState _startMemState;
	};
	_CrtMemState JsonParseCoordinatorTests::_startMemState;
}