#include "pch.h"
#include "TypeManager.h"
#include "AttributedFoo.h"
#include "GameObject.h"
#include "Avatar.h"
#include "AttributedBrilliantFoo.h"
#include "Action.h"
#include "ActionIncrement.h"
#include "ActionList.h"
#include "ActionListIf.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTests 
{
	TEST_MODULE_INITIALIZE(TestModuleInitialize)
	{
		RegisterType(AttributedFoo, Attributed);
		RegisterType(AttributedBrilliantFoo, AttributedFoo);
		RegisterType(GameObject, Attributed);
		RegisterType(Avatar, GameObject);
		RegisterType(Action, Attributed);
		RegisterType(ActionIncrement, Action);
		RegisterType(ActionList, Action);
		RegisterType(ActionListIf, ActionList);
		RegisterType(ActionCreateAction, Action);
		RegisterType(ActionDestroyAction, Action);
	}
}