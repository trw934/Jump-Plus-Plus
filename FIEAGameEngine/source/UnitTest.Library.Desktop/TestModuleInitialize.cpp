#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedBar.h"
#include "Avatar.h"
#include "ActionListIf.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "ActionIncrement.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_MODULE_INITIALIZE(TestModuleInitialize)
	{
		TypeRegistry::GetInstance()->RegisterType<AttributedFoo>();
		TypeRegistry::GetInstance()->RegisterType<AttributedBar, AttributedFoo>();
		TypeRegistry::GetInstance()->RegisterType<GameObject>();
		TypeRegistry::GetInstance()->RegisterType<Avatar, GameObject>();
		TypeRegistry::GetInstance()->RegisterType<Action>();
		TypeRegistry::GetInstance()->RegisterType<ActionList, Action>();
		TypeRegistry::GetInstance()->RegisterType<ActionListIf, ActionList>();
		TypeRegistry::GetInstance()->RegisterType<ActionIncrement, Action>();
		TypeRegistry::GetInstance()->RegisterType<ActionCreateAction, Action>();
		TypeRegistry::GetInstance()->RegisterType<ActionDestroyAction, Action>();
	}
	
	TEST_MODULE_CLEANUP(TestModuleCleanup)
	{
		TypeRegistry::GetInstance()->Clear();
	}
}