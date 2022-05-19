#include "pch.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionIncrement.h"
#include "ActionListIf.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "GameState.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTests
{
	TEST_CLASS(ActionTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(ActionIncrementRTTI)
		{
			ActionIncrement action;

			RTTI* rtti = &action;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("ActionIncrement"s));
			Assert::IsTrue(rtti->Is("Action"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(ActionIncrement::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Action::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(ActionIncrement::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			ActionIncrement* a = rtti->As<ActionIncrement>();
			Assert::IsNotNull(a);
			Assert::AreEqual(&action, a);
			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(ActionIncrement::TypeIdClass());
			Assert::IsNotNull(r);

			r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(ActionIncrement::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(ActionCreateActionRTTI)
		{
			ActionCreateAction action;

			RTTI* rtti = &action;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("ActionCreateAction"s));
			Assert::IsTrue(rtti->Is("Action"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(ActionCreateAction::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Action::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(ActionCreateAction::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			ActionCreateAction* a = rtti->As<ActionCreateAction>();
			Assert::IsNotNull(a);
			Assert::AreEqual(&action, a);
			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(ActionCreateAction::TypeIdClass());
			Assert::IsNotNull(r);

			r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(ActionCreateAction::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(ActionDestroyActionRTTI)
		{
			ActionDestroyAction action;

			RTTI* rtti = &action;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("ActionDestroyAction"s));
			Assert::IsTrue(rtti->Is("Action"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(ActionDestroyAction::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Action::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(ActionDestroyAction::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			ActionDestroyAction* a = rtti->As<ActionDestroyAction>();
			Assert::IsNotNull(a);
			Assert::AreEqual(&action, a);
			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(ActionDestroyAction::TypeIdClass());
			Assert::IsNotNull(r);

			r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(ActionDestroyAction::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(ActionListRTTI)
		{
			ActionList actionList;

			RTTI* rtti = &actionList;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("ActionList"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(ActionList::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(ActionList::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			ActionList* a = rtti->As<ActionList>();
			Assert::IsNotNull(a);
			Assert::AreEqual(&actionList, a);
			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(ActionList::TypeIdClass());
			Assert::IsNotNull(r);

			r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(ActionList::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(ActionListIfRTTI)
		{
			ActionListIf actionList;

			RTTI* rtti = &actionList;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("ActionListIf"s));
			Assert::IsTrue(rtti->Is("ActionList"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(ActionListIf::TypeIdClass()));
			Assert::IsTrue(rtti->Is(ActionList::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(ActionListIf::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			ActionListIf* a = rtti->As<ActionListIf>();
			Assert::IsNotNull(a);
			Assert::AreEqual(&actionList, a);
			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(ActionListIf::TypeIdClass());
			Assert::IsNotNull(r);

			r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(ActionListIf::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(Update)
		{
			ScopeFactory scopefactory;
			ActionIncrementFactory actionIncrementFactory;
			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);
			const std::string fileName = R"(Content\ActionList.json)"s;
			parseMaster.ParseFromFile(fileName);

			Datum* datum = sharedData.GetData().Find("Name");
			Assert::AreEqual(datum->Get<std::string>(), "Our World"s);
			datum = sharedData.GetData().Find("Children");
			Assert::AreEqual(DatumTypes::Table, datum->Type());
			datum = sharedData.GetData().Find("Actions");
			Assert::AreEqual(size_t(2), datum->Size());
			Scope* ascope = datum->Get<Scope*>(0);
			ActionIncrement* actionIncrement = ascope->As<ActionIncrement>();
			Assert::IsNotNull(actionIncrement);
			std::string cmrString = "A";
			Assert::AreEqual("A"s, actionIncrement->Target());


			GameTime gameTime;
			Datum* a = sharedData.GetData().Find("A"s);
			Assert::IsNotNull(a);
			Assert::AreEqual(DatumTypes::Integer, a->Type());
			Assert::AreEqual(0, a->Get<std::int32_t>());
			actionIncrement->Update(gameTime);
			Assert::AreEqual(1, a->Get<int32_t>());
			actionIncrement->Update(gameTime);
			Assert::AreEqual(2, a->Get<int32_t>());
			actionIncrement->Update(gameTime);
			Assert::AreEqual(3, a->Get<int32_t>());
		}

		TEST_METHOD(JsonDeserialization)
		{
			ScopeFactory scopefactory;
			ActionIncrementFactory actionIncrementFactory;
			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);
			const std::string fileName = R"(Content\ActionList.json)"s;
			parseMaster.ParseFromFile(fileName);
		}

		TEST_METHOD(ActionList_If)
		{
			ScopeFactory scopefactory;
			ActionIncrementFactory actionIncrementFactory;
			ActionListIfFactory actionListIfFactory;
			ActionListFactory actionListFactory;
			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);
			const std::string fileName = R"(Content\ActionList.json)"s;
			parseMaster.ParseFromFile(fileName);

			Datum* datum = sharedData.GetData().Find("Name");
			Assert::AreEqual(datum->Get<std::string>(), "Our World"s);

			datum = sharedData.GetData().Find("Children");
			Assert::AreEqual(DatumTypes::Table, datum->Type());

			datum = sharedData.GetData().Find("Actions");
			Assert::AreEqual(size_t(2), datum->Size());

			Scope* ascope = datum->Get<Scope*>(1);
			ActionListIf* actionListIf = ascope->As<ActionListIf>();
			Assert::IsNotNull(actionListIf);

			Datum* a = sharedData.GetData().Find("A"s);
			Assert::IsNotNull(a);
			Assert::AreEqual(DatumTypes::Integer, a->Type());
			Assert::AreEqual(0, a->Get<std::int32_t>());
			GameTime gameTime;
			actionListIf->Update(gameTime);
			Assert::AreEqual(1, a->Get<std::int32_t>());
			actionListIf->Condition = 1;
			actionListIf->Update(gameTime);
			Assert::AreEqual(3, a->Get<std::int32_t>());
		}

		TEST_METHOD(ActionIncrementTest)
		{
			const std::string inputstring = R"({
			  "A": {
			    "type": "float",
			    "value": 0
			  },
			  "Name": {
			    "type": "string",
			    "value": "Our World"
			  },
			  "Actions": {
			      "type": "table",
			      "value": [
			        {
			          "class": "ActionIncrement",
			          "Name": {
			            "type": "string",
			            "value": "Increment A"
			          },
			          "Target": {
			            "type": "string",
			            "value": "A"
			          },
			          "Step": {
			            "type": "float",
			            "value": 1
			          }
			        }
				]
			}
			})";

			ActionCreateActionFactory actionCreateActionFactory;
			ActionIncrementFactory actionIncrementFactory;
			ActionListFactory actionListFactory;
			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Parse(inputstring);

			Datum* datum = sharedData.GetData().Find("Name");
			Assert::AreEqual(datum->Get<std::string>(), "Our World"s);
			datum = sharedData.GetData().Find("Children");
			Assert::AreEqual(DatumTypes::Table, datum->Type());
			datum = sharedData.GetData().Find("Actions");
			Assert::AreEqual(size_t(1), datum->Size());
			Scope* ascope = datum->Get<Scope*>(0);
			ActionIncrement* actionIncrement = ascope->As<ActionIncrement>();
			Assert::IsNotNull(actionIncrement);
			std::string cmrString = "A";
			Assert::AreEqual("A"s, actionIncrement->Target());
			Assert::AreEqual("Increment A"s, actionIncrement->Name());


			GameTime gameTime;
			Datum* a = sharedData.GetData().Find("A"s);
			Assert::IsNotNull(a);
			Assert::AreEqual(DatumTypes::Float, a->Type());
			Assert::AreEqual(0.f, a->Get<float>());
			actionIncrement->Update(gameTime);
			Assert::AreEqual(1.f, a->Get<float>());
			actionIncrement->Update(gameTime);
			Assert::AreEqual(2.f, a->Get<float>());
			actionIncrement->Update(gameTime);
			Assert::AreEqual(3.f, a->Get<float>());

			Assert::AreEqual(1.f, actionIncrement->Step());
		}

		TEST_METHOD(ActionCreateActionTest)
		{
			const std::string inputstring = R"({
				 "A": {
				   "type": "integer",
				   "value": 0
				 },
				"Name": {
				"type": "string",
					"value" : "Our World"},
				"Actions" : {
					"type": "table",
						"value" : [
						{
						"class": "ActionCreateAction",
							"Name" : {
							"type": "string",
								"value" : "ActionCreate"
							},
							"Prototype" : {
								"type": "string",
									"value" : "ActionIncrement"
							},
								"CreateName" : {
									"type": "string",
										"value" : "Action1"
									}
						}
					]
				}
			})";

			ActionCreateActionFactory actionCreateActionFactory;
			ActionIncrementFactory actionIncrementFactory;
			ActionListFactory actionListFactory;
			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Parse(inputstring);

			GameState gameState;
			Datum* datum = sharedData.GetData().Find("Name");
			Assert::AreEqual(datum->Get<std::string>(), "Our World"s);

			datum = sharedData.GetData().Find("Children");
			Assert::AreEqual(DatumTypes::Table, datum->Type());

			datum = sharedData.GetData().Find("Actions");
			Assert::AreEqual(size_t(1), datum->Size());
			Scope* ascope = datum->Get<Scope*>(0);
			ActionCreateAction* actionCreate = ascope->As<ActionCreateAction>();
			Assert::IsNotNull(actionCreate);
			Assert::AreEqual("ActionCreate"s, actionCreate->Name());

			GameTime gameTime;
			actionCreate->Update(gameTime);
			gameState.DoActions();
			Assert::AreEqual(size_t(2), datum->Size());
			ascope = datum->Get<Scope*>(1);
			ActionIncrement* actionIncrement = ascope->As<ActionIncrement>();
			Assert::IsNotNull(actionIncrement);
			Assert::AreEqual("Action1"s, actionIncrement->Name());
			actionIncrement->SetTarget("A");
			actionIncrement->SetStep(1.f);
			
			Datum* a = sharedData.GetData().Find("A"s);
			Assert::IsNotNull(a);
			Assert::AreEqual(DatumTypes::Integer, a->Type());
			Assert::AreEqual(0, a->Get<std::int32_t>());
			actionIncrement->Update(gameTime);
			Assert::AreEqual(1, a->Get<int32_t>());
			actionIncrement->Update(gameTime);
			Assert::AreEqual(2, a->Get<int32_t>());
			actionIncrement->Update(gameTime);
			Assert::AreEqual(3, a->Get<int32_t>());
		}
		
		TEST_METHOD(ActionDestroyActionTest)
		{
			const std::string inputstring = R"({
			  "A": {
			    "type": "integer",
			    "value": 0
			  },
			  "Name": {
			    "type": "string",
			    "value": "Our World"
			  },
			  "Actions": {
			      "type": "table",
			      "value": [
			        {
			          "class": "ActionIncrement",
			          "Name": {
			            "type": "string",
			            "value": "Increment A"
			          },
			          "Target": {
			            "type": "string",
			            "value": "A"
			          },
			          "Step": {
			            "type": "float",
			            "value": 1.0
			          }
			        },
					{
						"class": "ActionDestroyAction",
						"Name":{
							"type": "string",
							"value": "destroy"
						},
						"DeleteName":{
							"type": "string",
							"value": "Increment A"
						}
					},
					{
						"class": "ActionDestroyAction",
						"Name":{
							"type": "string",
							"value": "destroy2"
						},
						"DeleteName":{
							"type": "string",
							"value": "Increment B"
						}
					}
			    ]
			  },
			  "Children": {
			    "type": "table",
			    "value": {
			    }
			  } 
			})";

			ScopeFactory scopefactory;
			ActionDestroyActionFactory actionDestroyActionFactory;
			ActionIncrementFactory actionIncrementFactory;
			ActionListFactory actionListFactory;
			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Parse(inputstring);

			GameState gameState;
			Datum* datum = sharedData.GetData().Find("Name");
			Assert::AreEqual(datum->Get<std::string>(), "Our World"s);

			datum = sharedData.GetData().Find("Actions");
			Assert::AreEqual(size_t(3), datum->Size());
			Scope* ascope = datum->Get<Scope*>(0);
			ActionIncrement* actionIncrement = ascope->As<ActionIncrement>();
			Assert::IsNotNull(actionIncrement);
			Assert::AreEqual("Increment A"s, actionIncrement->Name());
			std::string cmrString = "A";
			Assert::AreEqual("A"s, actionIncrement->Target());

			GameTime gameTime;
			ascope = datum->Get<Scope*>(1);
			ActionDestroyAction* actionDestroy = ascope->As<ActionDestroyAction>();
			Assert::IsNotNull(actionDestroy);
			Assert::AreEqual("destroy"s, actionDestroy->Name());

			actionDestroy->Update(gameTime);
			gameState.DoActions();
			Assert::AreEqual(size_t(2), datum->Size());
			ascope = datum->Get<Scope*>(0);
			actionDestroy = ascope->As<ActionDestroyAction>();
			Assert::IsNotNull(actionDestroy);
			ascope = datum->Get<Scope*>(1);
			actionDestroy = ascope->As<ActionDestroyAction>();
			Assert::IsNotNull(actionDestroy);
			Assert::AreEqual("destroy2"s, actionDestroy->Name());
			actionDestroy->Update(gameTime);
			gameState.DoActions();
		}

		TEST_METHOD(CloneTest)
		{
			{
				ActionIncrement action;
				ActionIncrement& cloneAction = *(action.Clone());
				Foo fooo;
				RTTI* rtti = &fooo;
				Assert::IsTrue(action.Equals(&cloneAction));
				Assert::IsFalse(action.Equals(rtti));
				delete& cloneAction;
			}

			{
				ActionCreateAction action;
				ActionCreateAction& cloneAction = *(action.Clone());
				Foo fooo;
				RTTI* rtti = &fooo;
				Assert::IsTrue(action.Equals(&cloneAction));
				Assert::IsFalse(action.Equals(rtti));
				delete& cloneAction;
			}

			{
				ActionDestroyAction action;
				ActionDestroyAction& cloneAction = *(action.Clone());
				Foo fooo;
				RTTI* rtti = &fooo;
				Assert::IsTrue(action.Equals(&cloneAction));
				Assert::IsFalse(action.Equals(rtti));
				delete& cloneAction;
			}

			{
				ActionList action;
				ActionList& cloneAction = *(action.Clone());
				Foo fooo;
				RTTI* rtti = &fooo;
				Assert::IsTrue(action.Equals(&cloneAction));
				Assert::IsFalse(action.Equals(rtti));
				delete& cloneAction;
			}

			{
				ActionListIf action;
				ActionListIf& cloneAction = *(action.Clone());
				Foo fooo;
				RTTI* rtti = &fooo;
				Assert::IsTrue(action.Equals(&cloneAction));
				Assert::IsFalse(action.Equals(rtti));
				delete& cloneAction;
			}
		}

		TEST_METHOD(ToStringTest)
		{ 
			{
				ActionIncrement action;
				Assert::AreEqual("ActionIncrement"s, action.ToString());
			}

			{
				ActionCreateAction action;
				Assert::AreEqual("ActionCreateAction"s, action.ToString());
			}

			{
				ActionDestroyAction action;
				Assert::AreEqual("ActionDestroyAction"s, action.ToString());
			}

			{
				ActionList action;
				Assert::AreEqual("ActionList"s, action.ToString());
			}

			{
				ActionListIf action;
				Assert::AreEqual("ActionListIf"s, action.ToString());
			}
		}

		TEST_METHOD(EqualTest)
		{
			{
				ActionIncrement action;
				ActionIncrement action2;
				RTTI* rtti = &action2;
				action.SetStep(1);
				action2.SetStep(1);
				action.SetTarget("A");
				action2.SetTarget("B");
				Assert::IsFalse(action.Equals(rtti));
				action2.SetTarget("A");
				Assert::IsTrue(action.Equals(rtti));
			}

			{
				ActionCreateAction action;
				ActionCreateAction action2;
				RTTI* rtti = &action2;
				action.SetName("A");
				action2.SetName("B");
				Assert::IsFalse(action.Equals(rtti));
				action2.SetName("A");
				Assert::IsTrue(action.Equals(rtti));
			}

			{
				ActionDestroyAction action;
				ActionDestroyAction action2;
				RTTI* rtti = &action2;
				action.SetName("A");
				action2.SetName("B");
				Assert::IsFalse(action.Equals(rtti));
				action2.SetName("A");
				Assert::IsTrue(action.Equals(rtti));
			}

			{
				ActionList action;
				ActionList action2;
				RTTI* rtti = &action2;
				action.SetName("A");
				action2.SetName("B");
				Assert::IsFalse(action.Equals(rtti));
				action2.SetName("A");
				Assert::IsTrue(action.Equals(rtti));
			}

			{
				ActionListIf action;
				ActionListIf action2;
				RTTI* rtti = &action2;
				action.SetName("A");
				action2.SetName("B");
				Assert::IsFalse(action.Equals(rtti));
				action2.SetName("A");
				Assert::IsTrue(action.Equals(rtti));
			}
		}

		TEST_METHOD(CreateActionTest)
		{
			ActionList actionList;
			ActionIncrementFactory actionIncrementFactory;
			actionList.CreateAction("ActionIncrement", "A");
			ActionIncrement* actionIncrement = actionList.Actions[0].As<ActionIncrement>();
			Assert::IsNotNull(actionIncrement);
		}

		TEST_METHOD(ActionListTest)
		{
			const std::string inputstring = R"({
			  "A": {
			    "type": "integer",
			    "value": 0
			  },
			  "Name": {
			    "type": "string",
			    "value": "Our World"
			  },
			  "Actions": {
			      "type": "table",
			      "value": 
			        {
						"class": "ActionList",
						"Name": {
							"type": "string",
							"value": "ActionList"
						},
						"Actions":{
						"type": "table",
						"value": 
								[
									{			          
										"class": "ActionIncrement",
									  "Name": {
										"type": "string",
										"value": "Increment A"
									  },
									  "Target": {
										"type": "string",
										"value": "A"
									  },
									  "Step": {
										"type": "float",
										"value": 1.0
									  }
								}
							]
					}
				}
			  },
			  "Children": {
			    "type": "table",
			    "value": {
			    }
			  } 
			})";

			ScopeFactory scopefactory;
			ActionIncrementFactory actionIncrementFactory;
			ActionListFactory actionListFactory;
			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Parse(inputstring);

			Datum* datum = sharedData.GetData().Find("Name");
			Assert::AreEqual(datum->Get<std::string>(), "Our World"s);

			datum = sharedData.GetData().Find("Children");
			Assert::AreEqual(DatumTypes::Table, datum->Type());

			datum = sharedData.GetData().Find("Actions");
			Assert::AreEqual(size_t(1), datum->Size());
			Scope* ascope = datum->Get<Scope*>(0);
			ActionList* actionList = ascope->As<ActionList>();
			Assert::IsNotNull(actionList);

			Assert::AreEqual(size_t(1),actionList->Actions.Size());

			GameTime gameTime;
			Datum* a = sharedData.GetData().Find("A"s);
			Assert::IsNotNull(a);
			Assert::AreEqual(DatumTypes::Integer, a->Type());
			Assert::AreEqual(0, a->Get<std::int32_t>());
			actionList->Update(gameTime);
			Assert::AreEqual(1, a->Get<int32_t>());
			actionList->Update(gameTime);
			Assert::AreEqual(2, a->Get<int32_t>());
			actionList->Update(gameTime);
			Assert::AreEqual(3, a->Get<int32_t>());
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState ActionTests::_startMemState;
}