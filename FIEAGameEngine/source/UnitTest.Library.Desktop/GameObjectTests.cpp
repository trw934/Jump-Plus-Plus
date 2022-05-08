#include "pch.h"
#include "ToStringSpecializations.h"
#include "Avatar.h"
#include "GameTime.h"
#include "json/json.h"
#include "fstream"

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(GameObjectTests)
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
			GameObject a;

			RTTI* rtti = &a;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("GameObject"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(GameObject::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Scope::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(GameObject::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			GameObject* af = rtti->As<GameObject>();
			Assert::IsNotNull(af);
			Assert::AreEqual(&a, af);

			Attributed* fAsAttributed = rtti->As<Attributed>();
			Assert::IsNotNull(fAsAttributed);
			Assert::AreEqual(static_cast<Attributed*>(&a), fAsAttributed);

		}

		TEST_METHOD(Constructor)
		{
			GameObject testObject;
			Assert::AreEqual(size_t(7), testObject.Size());
		}

		TEST_METHOD(GameObjectParsing)
		{
			const std::string filename = "..\\..\\..\\..\\..\\source\\UnitTest.Library.Desktop\\TestGameObject.json"s;

			GameObjectFactory objectFactory;
			AvatarFactory avatarFactory;
			ActionIncrementFactory incrementFactory;
			GameObject rootObject;
			JsonTableParseHelper::Wrapper wrapper(rootObject);
			JsonTableParseHelper tableParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(tableParseHelper);

			parseCoordinator.Initialize();

			Datum* objectDatum = rootObject.Find("A");
			Assert::IsNull(objectDatum);

			parseCoordinator.ParseFromFile(filename);

			Assert::AreEqual("Root"s, rootObject.ToString());
			Assert::AreEqual("Root"s, rootObject.Name);
			Assert::IsTrue(rootObject.Equals(&rootObject));

			objectDatum = rootObject.Find("Name");
			Assert::IsNotNull(objectDatum);
			Assert::AreEqual(DatumTypes::String, objectDatum->Type());
			Assert::AreEqual(size_t(1), objectDatum->Size());
			Assert::AreEqual("Root"s, objectDatum->GetString());

			objectDatum = rootObject.Find("A");
			Assert::IsNotNull(objectDatum);
			Assert::AreEqual(DatumTypes::Integer, objectDatum->Type());
			Assert::AreEqual(size_t(1), objectDatum->Size());
			Assert::AreEqual(23, objectDatum->GetInt());

			objectDatum = rootObject.Find("Tag");
			Assert::IsNotNull(objectDatum);
			Assert::AreEqual(DatumTypes::String, objectDatum->Type());
			Assert::AreEqual(size_t(1), objectDatum->Size());
			Assert::AreEqual("TestTag"s, objectDatum->GetString());
			Assert::AreEqual("TestTag"s, rootObject.Tag);

			objectDatum = rootObject.Find("Transform");
			Assert::IsNotNull(objectDatum);
			Assert::AreEqual(DatumTypes::Matrix, objectDatum->Type());
			Assert::AreEqual(size_t(1), objectDatum->Size());
			Assert::AreEqual(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, objectDatum->GetMatrix());
			Assert::AreEqual(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, rootObject.Transform);

			Assert::AreEqual(glm::vec4{ 1,2,3,4 }, rootObject.GetPosition());
			Assert::AreEqual(glm::vec4{ 5,6,7,8 }, rootObject.GetRotation());
			Assert::AreEqual(glm::vec4{ 9,10,11,12 }, rootObject.GetScale());

			objectDatum = rootObject.Find("Children");
			Assert::IsNotNull(objectDatum);
			Assert::AreEqual(DatumTypes::Table, objectDatum->Type());
			Assert::AreEqual(size_t(2), objectDatum->Size());

			Avatar& childObject = *objectDatum->GetTable(0)->As<Avatar>();
			Assert::AreEqual(size_t(8), childObject.Size());
			Assert::AreEqual(rootObject, childObject.GetParent<GameObject>());

			Datum* childDatum = childObject.Find("Name");
			Assert::IsNotNull(childDatum);
			Assert::AreEqual(DatumTypes::String, childDatum->Type());
			Assert::AreEqual(size_t(1), childDatum->Size());
			Assert::AreEqual("Bingus"s, childDatum->GetString());
			Assert::AreEqual("Bingus"s, childObject.Name);

			childDatum = childObject.Find("Tag");
			Assert::IsNotNull(childDatum);
			Assert::AreEqual(DatumTypes::String, childDatum->Type());
			Assert::AreEqual(size_t(1), childDatum->Size());
			Assert::AreEqual("Player"s, childDatum->GetString());
			Assert::AreEqual("Player"s, childObject.Tag);

			childDatum = childObject.Find("HitPoints");
			Assert::IsNotNull(childDatum);
			Assert::AreEqual(DatumTypes::Integer, childDatum->Type());
			Assert::AreEqual(size_t(1), childDatum->Size());
			Assert::AreEqual(100, childDatum->GetInt());
			Assert::AreEqual(100, childObject.HitPoints);

			childObject = *objectDatum->GetTable(1)->As<Avatar>();
			Assert::AreEqual(size_t(8), childObject.Size());
			Assert::AreEqual(rootObject, childObject.GetParent<GameObject>());

			childDatum = childObject.Find("Name");
			Assert::IsNotNull(childDatum);
			Assert::AreEqual(DatumTypes::String, childDatum->Type());
			Assert::AreEqual(size_t(1), childDatum->Size());
			Assert::AreEqual("Lord Bungaloid"s, childDatum->GetString());
			Assert::AreEqual("Lord Bungaloid"s, childObject.Name);

			childDatum = childObject.Find("Tag");
			Assert::IsNotNull(childDatum);
			Assert::AreEqual(DatumTypes::String, childDatum->Type());
			Assert::AreEqual(size_t(1), childDatum->Size());
			Assert::AreEqual("Enemy"s, childDatum->GetString());
			Assert::AreEqual("Enemy"s, childObject.Tag);

			childDatum = childObject.Find("HitPoints");
			Assert::IsNotNull(childDatum);
			Assert::AreEqual(DatumTypes::Integer, childDatum->Type());
			Assert::AreEqual(size_t(1), childDatum->Size());
			Assert::AreEqual(5000, childDatum->GetInt());
			Assert::AreEqual(5000, childObject.HitPoints);

		}

		TEST_METHOD(Clone)
		{
			const std::string filename = "..\\..\\..\\..\\..\\source\\UnitTest.Library.Desktop\\TestGameObject.json"s;

			GameObjectFactory objectFactory;
			AvatarFactory avatarFactory;
			ActionIncrementFactory incrementFactory;
			GameObject testObject;
			JsonTableParseHelper::Wrapper wrapper(testObject);
			JsonTableParseHelper tableParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(tableParseHelper);

			parseCoordinator.Initialize();

			Datum* objectDatum = testObject.Find("A");
			Assert::IsNull(objectDatum);

			parseCoordinator.ParseFromFile(filename);

			Assert::AreEqual("Root"s, testObject.ToString());
			Assert::AreEqual("Root"s, testObject.Name);
			Assert::IsTrue(testObject.Equals(&testObject));

			auto clone = testObject.Clone();
			
			GameObject& rootObject = *clone->As<GameObject>();

			objectDatum = rootObject.Find("Name");
			Assert::IsNotNull(objectDatum);
			Assert::AreEqual(DatumTypes::String, objectDatum->Type());
			Assert::AreEqual(size_t(1), objectDatum->Size());
			Assert::AreEqual("Root"s, objectDatum->GetString());

			objectDatum = rootObject.Find("A");
			Assert::IsNotNull(objectDatum);
			Assert::AreEqual(DatumTypes::Integer, objectDatum->Type());
			Assert::AreEqual(size_t(1), objectDatum->Size());
			Assert::AreEqual(23, objectDatum->GetInt());

			objectDatum = rootObject.Find("Tag");
			Assert::IsNotNull(objectDatum);
			Assert::AreEqual(DatumTypes::String, objectDatum->Type());
			Assert::AreEqual(size_t(1), objectDatum->Size());
			Assert::AreEqual("TestTag"s, objectDatum->GetString());
			Assert::AreEqual("TestTag"s, rootObject.Tag);

			objectDatum = rootObject.Find("Sprite");
			Assert::IsNotNull(objectDatum);
			Assert::AreEqual(DatumTypes::String, objectDatum->Type());
			Assert::AreEqual(size_t(1), objectDatum->Size());
			Assert::AreEqual("TestSpriteFileLocation.png"s, objectDatum->GetString());
			Assert::AreEqual("TestSpriteFileLocation.png"s, rootObject.Sprite);

			objectDatum = rootObject.Find("Transform");
			Assert::IsNotNull(objectDatum);
			Assert::AreEqual(DatumTypes::Matrix, objectDatum->Type());
			Assert::AreEqual(size_t(1), objectDatum->Size());
			Assert::AreEqual(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, objectDatum->GetMatrix());
			Assert::AreEqual(glm::mat4{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 }, rootObject.Transform);

			Assert::AreEqual(glm::vec4{ 1,2,3,4 }, rootObject.GetPosition());
			Assert::AreEqual(glm::vec4{ 5,6,7,8 }, rootObject.GetRotation());
			Assert::AreEqual(glm::vec4{ 9,10,11,12 }, rootObject.GetScale());

			objectDatum = rootObject.Find("Children");
			Assert::IsNotNull(objectDatum);
			Assert::AreEqual(DatumTypes::Table, objectDatum->Type());
			Assert::AreEqual(size_t(2), objectDatum->Size());

			Avatar& childObject = *objectDatum->GetTable(0)->As<Avatar>();
			Assert::AreEqual(size_t(8), childObject.Size());
			Assert::AreEqual(rootObject, childObject.GetParent<GameObject>());

			Datum* childDatum = childObject.Find("Name");
			Assert::IsNotNull(childDatum);
			Assert::AreEqual(DatumTypes::String, childDatum->Type());
			Assert::AreEqual(size_t(1), childDatum->Size());
			Assert::AreEqual("Bingus"s, childDatum->GetString());
			Assert::AreEqual("Bingus"s, childObject.Name);

			childDatum = childObject.Find("Tag");
			Assert::IsNotNull(childDatum);
			Assert::AreEqual(DatumTypes::String, childDatum->Type());
			Assert::AreEqual(size_t(1), childDatum->Size());
			Assert::AreEqual("Player"s, childDatum->GetString());
			Assert::AreEqual("Player"s, childObject.Tag);

			childDatum = childObject.Find("HitPoints");
			Assert::IsNotNull(childDatum);
			Assert::AreEqual(DatumTypes::Integer, childDatum->Type());
			Assert::AreEqual(size_t(1), childDatum->Size());
			Assert::AreEqual(100, childDatum->GetInt());
			Assert::AreEqual(100, childObject.HitPoints);

			childObject = *objectDatum->GetTable(1)->As<Avatar>();
			Assert::AreEqual(size_t(8), childObject.Size());
			Assert::AreEqual(rootObject, childObject.GetParent<GameObject>());

			childDatum = childObject.Find("Name");
			Assert::IsNotNull(childDatum);
			Assert::AreEqual(DatumTypes::String, childDatum->Type());
			Assert::AreEqual(size_t(1), childDatum->Size());
			Assert::AreEqual("Lord Bungaloid"s, childDatum->GetString());
			Assert::AreEqual("Lord Bungaloid"s, childObject.Name);

			childDatum = childObject.Find("Tag");
			Assert::IsNotNull(childDatum);
			Assert::AreEqual(DatumTypes::String, childDatum->Type());
			Assert::AreEqual(size_t(1), childDatum->Size());
			Assert::AreEqual("Enemy"s, childDatum->GetString());
			Assert::AreEqual("Enemy"s, childObject.Tag);

			childDatum = childObject.Find("HitPoints");
			Assert::IsNotNull(childDatum);
			Assert::AreEqual(DatumTypes::Integer, childDatum->Type());
			Assert::AreEqual(size_t(1), childDatum->Size());
			Assert::AreEqual(5000, childDatum->GetInt());
			Assert::AreEqual(5000, childObject.HitPoints);

			delete clone;
		}

		TEST_METHOD(Update)
		{
			const std::string filename = "..\\..\\..\\..\\..\\source\\UnitTest.Library.Desktop\\TestGameObject.json"s;

			GameObjectFactory objectFactory;
			AvatarFactory avatarFactory;
			ActionIncrementFactory incrementFactory;
			GameObject rootObject;
			GameTime gameTime;
			JsonTableParseHelper::Wrapper wrapper(rootObject);
			JsonTableParseHelper tableParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(tableParseHelper);

			parseCoordinator.Initialize();

			parseCoordinator.ParseFromFile(filename);

			Datum* ADatum = rootObject.Find("A");
			Assert::IsNotNull(ADatum);
			Assert::AreEqual(DatumTypes::Integer, ADatum->Type());
			Assert::AreEqual(size_t(1), ADatum->Size());

			Datum* objectDatum = rootObject.Find("Children");
			Assert::IsNotNull(objectDatum);
			Assert::AreEqual(DatumTypes::Table, objectDatum->Type());
			Assert::AreEqual(size_t(2), objectDatum->Size());

			Avatar& playerObject = *objectDatum->GetTable(0)->As<Avatar>();
			Assert::AreEqual(100, playerObject.HitPoints);

			Avatar& enemyObject = *objectDatum->GetTable(1)->As<Avatar>();
			Assert::AreEqual(5000, enemyObject.HitPoints);

			rootObject.Update(gameTime);
			Assert::AreEqual(99, playerObject.HitPoints);
			Assert::AreEqual(4999, enemyObject.HitPoints);
			Assert::AreEqual(25, ADatum->GetInt());
		}

		TEST_METHOD(CreateAction)
		{
			const std::string filename = "..\\..\\..\\..\\..\\source\\UnitTest.Library.Desktop\\TestGameObject.json"s;

			GameObjectFactory objectFactory;
			AvatarFactory avatarFactory;
			ActionIncrementFactory incrementFactory;
			GameObject rootObject;
			GameTime gameTime;
			JsonTableParseHelper::Wrapper wrapper(rootObject);
			JsonTableParseHelper tableParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(tableParseHelper);

			parseCoordinator.Initialize();

			parseCoordinator.ParseFromFile(filename);

			Datum& b = rootObject.AppendAuxillaryAttribute("B"s);
			b = 0;

			auto newAction = rootObject.CreateAction(ActionIncrement::TypeName(), "Increment B"s);
			Assert::IsNotNull(newAction);
			auto bIncrement = newAction->As<ActionIncrement>();
			Assert::IsNotNull(bIncrement);
			bIncrement->SetTarget("B"s);
			bIncrement->SetStep(2);

			rootObject.Update(gameTime);
			Assert::AreEqual(2, b.GetInt());
			rootObject.Update(gameTime);
			Assert::AreEqual(4, b.GetInt());
			rootObject.Update(gameTime);
			Assert::AreEqual(6, b.GetInt());
		}

	private:
		static _CrtMemState _startMemState;
	};
	_CrtMemState GameObjectTests::_startMemState;
}