#include "pch.h"
#include "ToStringSpecializations.h"
#include "GameTime.h"
#include "ActionIncrement.h"
#include "ActionListIf.h"
#include "GameState.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTestLibraryDesktop;
using namespace std;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionTests)
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
			ActionList a;

			RTTI* rtti = &a;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("Action"s));
			Assert::IsTrue(rtti->Is("ActionList"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsTrue(rtti->Is("Scope"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(ActionList::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Action::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Scope::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(ActionList::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			ActionList* af = rtti->As<ActionList>();
			Assert::IsNotNull(af);
			Assert::AreEqual(&a, af);

			Attributed* fAsAttributed = rtti->As<Attributed>();
			Assert::IsNotNull(fAsAttributed);
			Assert::AreEqual(static_cast<Attributed*>(&a), fAsAttributed);

		}

		TEST_METHOD(Constructor)
		{
			ActionList testActionList;
			Assert::AreEqual(size_t(3), testActionList.Size());
		}

		TEST_METHOD(Update)
		{
			ActionList action;
			ActionIncrementFactory incrementFactory;

			const string aKey = "A"s;
			Datum& aDatum = action.AppendAuxillaryAttribute(aKey) = 0;

			auto nestedAction = action.CreateAction(ActionIncrement::TypeName(), "Increment Action"s);
			Assert::IsNotNull(nestedAction);
			auto actionIncrement = nestedAction->As<ActionIncrement>();
			Assert::IsNotNull(actionIncrement);

			actionIncrement->SetTarget(aKey);
			Assert::AreEqual(aKey, actionIncrement->Target());
			Assert::AreEqual(1.0f, actionIncrement->Step());
			Assert::AreEqual(0, aDatum.GetInt());

			GameTime gameTime;
			action.Update(gameTime);
			Assert::AreEqual(1, aDatum.GetInt());
			action.Update(gameTime);
			Assert::AreEqual(2, aDatum.GetInt());
			action.Update(gameTime);
			Assert::AreEqual(3, aDatum.GetInt());

			// Target does not exist
			actionIncrement->SetTarget("B"s);
			Assert::ExpectException<exception>([actionIncrement, &gameTime] {actionIncrement->Update(gameTime); });

			// Target exists but cannot be incremented
			Datum& b = actionIncrement->AppendAuxillaryAttribute("B"s);
			b = "This cannot be incremented"s;
			Assert::ExpectException<exception>([actionIncrement, &gameTime] {actionIncrement->Update(gameTime); });
		}

		TEST_METHOD(Clone)
		{
			ActionList action;

			auto clone = action.Clone();
			Assert::AreNotSame(action, *clone);
			Assert::AreEqual(action, *clone);

			delete clone;
		}

		TEST_METHOD(JsonDeserialization)
		{
			ActionList actionList;
			const std::string filename = "..\\..\\..\\..\\..\\source\\UnitTest.Library.Desktop\\TestAction.json"s;

			ActionListFactory listFactory;
			ActionIncrementFactory incrementFactory;
			ActionListIfFactory listIfFactory;
			JsonTableParseHelper::Wrapper wrapper(actionList);
			JsonTableParseHelper tableParseHelper;
			JsonParseCoordinator parseCoordinator(wrapper);
			parseCoordinator.AddHelper(tableParseHelper);
			parseCoordinator.Initialize();
			parseCoordinator.ParseFromFile(filename);

			Assert::AreEqual("Test Action"s, actionList.Name());

			auto& actionsDatum = actionList.Actions();
			Assert::AreEqual(size_t(3), actionsDatum.Size());
			for (size_t i = 0; i < actionsDatum.Size(); ++i)
			{
				Action* action = actionsDatum[i].As<Action>();
				Assert::IsNotNull(action);
				stringstream name;
				name << "Nested Action " << (i + 1);
				Assert::AreEqual(name.str(), action->Name());
			}

			ActionList& nestedActionList = *(actionsDatum[1].As<ActionList>());
			auto& nestedActionListActionsDatum = nestedActionList.Actions();
			Assert::AreEqual(size_t(3), nestedActionListActionsDatum.Size());
			for (size_t i = 0; i < nestedActionListActionsDatum.Size(); ++i)
			{
				Action* action = nestedActionListActionsDatum[i].As<Action>();
				Assert::IsNotNull(action);
				stringstream name;
				name << "Nested Nested Action " << (i + 1);
				Assert::AreEqual(name.str(), action->Name());
			}

			{
				auto action = nestedActionListActionsDatum[2].As<ActionListIf>();
				Assert::IsNotNull(action);
				ActionListIf& actionListIf = *action;
				Assert::AreEqual(1, actionListIf.Condition());
			}
		}

		TEST_METHOD(TestActionListIf)
		{
			GameTime gameTime;
			ActionListIf actionListIf;
			Datum& a = actionListIf.AppendAuxillaryAttribute("A"s);
			a = 0;
			Datum& b = actionListIf.AppendAuxillaryAttribute("B"s);
			b = 0;

			ActionIncrementFactory incrementFactory;
			ActionListIfFactory listIfFactory;
			auto trueClause = actionListIf.CreateAction(ActionIncrement::TypeName(), "Then"s);
			Assert::IsNotNull(trueClause);
			auto aIncrement = trueClause->As<ActionIncrement>();
			Assert::IsNotNull(aIncrement);
			aIncrement->SetTarget("A"s);

			auto falseClause = actionListIf.CreateAction(ActionIncrement::TypeName(), "Else"s);
			Assert::IsNotNull(falseClause);
			auto bIncrement = falseClause->As<ActionIncrement>();
			Assert::IsNotNull(bIncrement);
			bIncrement->SetTarget("B"s);

			{
				//True clause
				actionListIf.SetCondition(true);
				actionListIf.Update(gameTime);
				Assert::AreEqual(1, a.GetInt());
				Assert::AreEqual(0, b.GetInt());

				actionListIf.Update(gameTime);
				Assert::AreEqual(2, a.GetInt());
				Assert::AreEqual(0, b.GetInt());

				actionListIf.Update(gameTime);
				Assert::AreEqual(3, a.GetInt());
				Assert::AreEqual(0, b.GetInt());
			}

			{
				//False clause
				actionListIf.SetCondition(false);
				actionListIf.Update(gameTime);
				Assert::AreEqual(3, a.GetInt());
				Assert::AreEqual(1, b.GetInt());

				actionListIf.Update(gameTime);
				Assert::AreEqual(3, a.GetInt());
				Assert::AreEqual(2, b.GetInt());

				actionListIf.Update(gameTime);
				Assert::AreEqual(3, a.GetInt());
				Assert::AreEqual(3, b.GetInt());
			}

			bIncrement->SetTarget("C"s);
			Assert::ExpectException<exception>([&actionListIf, &gameTime] {actionListIf.Update(gameTime); });
		}

		TEST_METHOD(TestCreateAction)
		{
			GameObject rootObject;
			GameState gameState(&rootObject);
			GameTime gameTime;

			ActionList actionList;
			ActionIncrementFactory incrementFactory;
			ActionCreateActionFactory createFactory;

			auto nestedAction = actionList.CreateAction(ActionCreateAction::TypeName(), "Create Action"s);
			Assert::IsNotNull(nestedAction);
			auto actionCreate = nestedAction->As<ActionCreateAction>();
			Assert::IsNotNull(actionCreate);

			actionCreate->SetPrototype("ActionIncrement");
			Assert::AreEqual("ActionIncrement"s, actionCreate->Prototype());
			actionCreate->SetActionName("Created Action");
			Assert::AreEqual("Created Action"s, actionCreate->ActionName());

			auto& actionListActions = actionList.Actions();

			Assert::AreEqual(size_t(1), actionListActions.Size());
			gameState.Add();
			Assert::AreEqual(size_t(1), actionListActions.Size());

			actionList.Update(gameTime);
			Assert::AreEqual(size_t(1), actionListActions.Size());

			gameState.Add();
			Assert::AreEqual(size_t(2), actionListActions.Size());
			ActionIncrement* createdIncrementAction = actionListActions[1].As<ActionIncrement>();
			Assert::IsNotNull(createdIncrementAction);
			Assert::AreEqual("Created Action"s, createdIncrementAction->Name());

			gameState.Add();
			Assert::AreEqual(size_t(2), actionListActions.Size());
		}

		TEST_METHOD(TestDestroyAction)
		{
			GameObject rootObject;
			GameState gameState(&rootObject);
			GameTime gameTime;

			ActionList actionList;
			ActionIncrementFactory incrementFactory;
			ActionDestroyActionFactory destroyFactory;

			Datum& a = actionList.AppendAuxillaryAttribute("A"s);
			a = 0;

			auto nestedAction = actionList.CreateAction(ActionDestroyAction::TypeName(), "Destroy Action"s);
			Assert::IsNotNull(nestedAction);
			auto actionDestroy = nestedAction->As<ActionDestroyAction>();
			Assert::IsNotNull(actionDestroy);

			nestedAction = actionList.CreateAction(ActionIncrement::TypeName(), "Increment A"s);
			Assert::IsNotNull(nestedAction);
			auto aIncrement = nestedAction->As<ActionIncrement>();
			Assert::IsNotNull(aIncrement);
			aIncrement->SetTarget("A"s);

			actionDestroy->SetDestroyAction("Increment A");
			Assert::AreEqual("Increment A"s, actionDestroy->GetDestroyAction());

			auto& actionListActions = actionList.Actions();

			Assert::AreEqual(size_t(2), actionListActions.Size());
			gameState.Resolve();
			Assert::AreEqual(size_t(2), actionListActions.Size());

			actionList.Update(gameTime);
			Assert::AreEqual(size_t(2), actionListActions.Size());
			Assert::AreEqual(1, a.GetInt());

			gameState.Resolve();
			Assert::AreEqual(size_t(1), actionListActions.Size());

			gameState.Resolve();
			Assert::AreEqual(size_t(1), actionListActions.Size());

			actionList.Update(gameTime);
			Assert::AreEqual(size_t(1), actionListActions.Size());
			Assert::AreEqual(1, a.GetInt());

			gameState.Resolve();
		}

	private:
		static _CrtMemState _startMemState;
	};
	_CrtMemState ActionTests::_startMemState;
}