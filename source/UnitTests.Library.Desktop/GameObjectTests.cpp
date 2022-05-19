#include "pch.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include "GameObject.h"
#include "Attributed.h"
#include "Avatar.h"
#include "GameTime.h"
#include "Action.h"
#include "ActionIncrement.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTests
{
	TEST_CLASS(GameObjectTests)
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

		TEST_METHOD(Constructor)
		{
			GameObject gameObject;

			Assert::IsTrue(gameObject.IsAttribute("this"));
			Assert::IsTrue(gameObject.IsAttribute("Position"));
			Assert::IsTrue(gameObject.IsAttribute("Rotation"));
			Assert::IsTrue(gameObject.IsAttribute("Scale"));
			Assert::IsTrue(gameObject.IsAttribute("Name"));
			Assert::IsTrue(gameObject.IsAttribute("Children"));
			Assert::IsFalse(gameObject.IsAttribute("Testing"));

			Assert::IsTrue(gameObject.IsPrescribedAttribute("this"));
			Assert::IsTrue(gameObject.IsPrescribedAttribute("Position"));
			Assert::IsTrue(gameObject.IsPrescribedAttribute("Rotation"));
			Assert::IsTrue(gameObject.IsPrescribedAttribute("Scale"));
			Assert::IsTrue(gameObject.IsPrescribedAttribute("Name"));
			Assert::IsTrue(gameObject.IsPrescribedAttribute("Children"));
			Assert::IsFalse(gameObject.IsPrescribedAttribute("Testing"));

			Assert::IsFalse(gameObject.IsAuxiliaryAttribute("Position"));
			Assert::IsFalse(gameObject.IsAuxiliaryAttribute("Rotation"));
			Assert::IsFalse(gameObject.IsAuxiliaryAttribute("Scale"));
			Assert::IsFalse(gameObject.IsAuxiliaryAttribute("Name"));
			Assert::IsFalse(gameObject.IsAuxiliaryAttribute("Children"));
			Assert::IsFalse(gameObject.IsAuxiliaryAttribute("Testing"));

			Assert::AreEqual(&gameObject, gameObject["this"].Get<RTTI*>()->As<GameObject>());
		}

		TEST_METHOD(RTTITest)
		{
			GameObject gameObject;

			RTTI* rtti = &gameObject;
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

			Foo* b = rtti->As<Foo>();
			Assert::IsNull(b);

			GameObject* f = rtti->As<GameObject>();
			Assert::IsNotNull(f);
			Assert::AreEqual(&gameObject, f);

			Attributed* fAsAttributed = rtti->As<Attributed>();
			Assert::IsNotNull(fAsAttributed);
			Assert::AreEqual(static_cast<Attributed*>(&gameObject), fAsAttributed);

			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(GameObject::TypeIdClass());
			Assert::IsNotNull(r);
			r = rtti->QueryInterface(Attributed::TypeIdClass());
			Assert::IsNotNull(r);

			GameObject otherObject;
			Assert::IsTrue(rtti->Equals(&otherObject));

			RTTI* nullPtr = nullptr;
			Assert::IsFalse(gameObject.Equals(nullPtr));

			Assert::AreEqual("GameObject"s, r->ToString());
		}
		
		TEST_METHOD(Parse)
		{
			std::string inputString = R"({
				"Name":{
					"type": "string",
					"value": "Our World"
				},
					"Children": 
						{
							"class": "GameObject",
							"type": "table",
							"value": {
								"Street": {
								"type": "string",
								"value": "123 Anystreet St."
								}
							}
						}
			})";

			ScopeFactory scopefactory;
			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			GameObjectFactory gameObjectFactory;
			parseMaster.Parse(inputString);
			
			Datum* datum = sharedData.GetData().Find("Name");
			Assert::AreEqual(datum->Get<std::string>(), "Our World"s);
			Assert::AreEqual(DatumTypes::String, datum->Type());
			Assert::AreEqual(sharedData.GetData().TypeIdClass(), GameObject::TypeIdClass());
			GameObject* gp = static_cast<GameObject*>(&(sharedData.GetData()));
			Assert::AreEqual(glm::vec4(0), gp->Transform.Position);
			Assert::AreEqual(glm::vec4(0), gp->Transform.Rotation);
			Assert::AreEqual(glm::vec4(0), gp->Transform.Scale);
			Assert::AreEqual("Our World"s, gp->ToString());

			datum = sharedData.GetData().Find("Children");
			Assert::AreEqual(DatumTypes::Table, datum->Type());

			Scope* scope = datum->Get<Scope*>(0);
			Assert::AreEqual("123 Anystreet St."s, (*scope)["Street"].Get<std::string>(0));
		}

		TEST_METHOD(UpdateTest)
		{
			std::string inputString = R"({
			"A": {
			    "type": "float",
			    "value": 0
			  },
				"Name":{
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
					},
					"Children": 
						{
							"type": "table",
							"value": {
								"class": "Avatar",
								"Name":{
									"type": "string",
									"value": "Avatarrrr"
								},
									"Zip Code": {
										"type": "integer",
										"value":  12345
								}
							}
						}
			})";

			JsonTableParseHelper::SharedData sharedData;
			ActionIncrementFactory actionIncrementFactory;
			ScopeFactory scopeFactory;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			GameObjectFactory gameObjectFactory;
			AvatarFactory avatarFactory;
			parseMaster.Parse(inputString);

			Datum* datum = sharedData.GetData().Find("Name");
			Assert::AreEqual(datum->Get<std::string>(), "Our World"s);
			Assert::AreEqual(DatumTypes::String, datum->Type());
			Assert::AreEqual(GameObject::TypeIdClass(),sharedData.GetData().TypeIdClass());
			GameObject* gp = static_cast<GameObject*>(&(sharedData.GetData()));
			Assert::AreEqual(glm::vec4(0), gp->Transform.Position);
			Assert::AreEqual(glm::vec4(0), gp->Transform.Rotation);
			Assert::AreEqual(glm::vec4(0), gp->Transform.Scale);
			Assert::AreEqual("Our World"s, gp->ToString());

			datum = sharedData.GetData().Find("Children");
			Assert::AreEqual(DatumTypes::Table, datum->Type());
			Scope* scope = datum->Get<Scope*>(0);
			Assert::IsTrue(scope->Is("Avatar"s));
			Assert::AreEqual(12345, (*scope)["Zip Code"].Get<int32_t>(0));

			GameTime gameTime;
			Datum* a = sharedData.GetData().Find("A"s);
			Assert::IsNotNull(a);
			Assert::AreEqual(DatumTypes::Float, a->Type());
			Assert::AreEqual(0.f, a->Get<float>());
			gp->Update(gameTime);
			Assert::AreEqual(1.f, a->Get<float>());
			Avatar* ap = static_cast<Avatar*>(scope);
			Assert::AreEqual(size_t(1), ap->TestInt);
			Assert::AreEqual("Avatarrrr"s, ap->ToString());
		}

		TEST_METHOD(ParseFile)
		{
			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);
			GameObjectFactory gameObjectFactory;
			AvatarFactory avatarFactory;
			const std::string fileName = R"(Content\GameWorldTest.json)"s;
			parseMaster.ParseFromFile(fileName);

			Datum* datum = sharedData.GetData().Find("Name");
			Assert::AreEqual(datum->Get<std::string>(), "Our World"s);
			Assert::AreEqual(DatumTypes::String, datum->Type());
			Assert::AreEqual(GameObject::TypeIdClass(), sharedData.GetData().TypeIdClass());
			GameObject* gp = static_cast<GameObject*>(&(sharedData.GetData()));
			Assert::AreEqual(glm::vec4(0), gp->Transform.Position);
			Assert::AreEqual(glm::vec4(0), gp->Transform.Rotation);
			Assert::AreEqual(glm::vec4(0), gp->Transform.Scale);
			Assert::AreEqual("Our World"s, gp->ToString());

			datum = sharedData.GetData().Find("Children");
			Assert::AreEqual(DatumTypes::Table, datum->Type());
			Scope* scope = datum->Get<Scope*>(0);
			Assert::IsTrue(scope->Is("Avatar"s));
			Assert::AreEqual(12345, (*scope)["Zip Code"].Get<int32_t>(0));

			GameTime gameTime;
			gp->Update(gameTime);
			Avatar* ap = static_cast<Avatar*>(scope);
			Assert::AreEqual(size_t(1), ap->TestInt);
		}

		TEST_METHOD(CloneTest)
		{
			GameObject gameObject;
			GameObject& cloneGO = *(gameObject.Clone());
			Assert::IsTrue(gameObject.Equals(&cloneGO));
			Assert::IsTrue(cloneGO.Is("GameObject"s));
			delete& cloneGO;

			Avatar avatar;
			Avatar& cloneA = *(avatar.Clone());
			Assert::IsTrue(avatar.Equals(&cloneA));
			Assert::IsTrue(avatar.Is("Avatar"s));
			delete& cloneA;
		}

		TEST_METHOD(CopyTest)
		{
			std::string inputString = R"({
				"Name":{
					"type": "string",
					"value": "Our World"
				},
					"Children": 
						{
							"class": "GameObject",
							"type": "table",
							"value": {
								"Street": {
								"type": "string",
								"value": "123 Anystreet St."
								}
							}
						}
			})";

			ScopeFactory scopefactory;
			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			GameObjectFactory gameObjectFactory;
			parseMaster.Parse(inputString);

			Datum* datum = sharedData.GetData().Find("Name");
			Assert::AreEqual(datum->Get<std::string>(), "Our World"s);
			Assert::AreEqual(DatumTypes::String, datum->Type());
			Assert::AreEqual(sharedData.GetData().TypeIdClass(), GameObject::TypeIdClass());
			GameObject* gp = static_cast<GameObject*>(&(sharedData.GetData()));

			GameObject gameObject = *gp;
			Assert::AreEqual("Our World"s, gameObject.ToString());
			Assert::AreEqual(gp->Transform.Position, gameObject.Transform.Position);
			Assert::AreEqual(gp->Transform.Rotation, gameObject.Transform.Rotation);
			Assert::AreEqual(gp->Transform.Scale, gameObject.Transform.Scale);
		}

		TEST_METHOD(GameObjectFactoryCreateTest)
		{
			GameObjectFactory gameObejctFactory;
			GameObject* GO = gameObejctFactory.Create();
			Assert::IsTrue(GO->Is("GameObject"s));
			delete GO;
		}

		TEST_METHOD(CreateActionTest)
		{
			GameObject gameObject;
			ActionIncrementFactory actionIncrementFactory;
			gameObject.CreateAction("ActionIncrement", "A");
			ActionIncrement* actionIncrement = gameObject.GetActions()[0].As<ActionIncrement>();
			Assert::IsNotNull(actionIncrement);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState GameObjectTests::_startMemState;
}