#include "pch.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include "JsonParseMaster.h"
#include "JsonTableParseHelper.h"
#include "JsonIntegerParseHelper.h"
#include "AttributedBrilliantFoo.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTests
{
	TEST_CLASS(JsonTableParseHelperTests)
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

		TEST_METHOD(JsonTestParseHelperRTTI)
		{
			JsonTableParseHelper helper;

			RTTI* rtti = &helper;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("IJsonParseHelper"s));
			Assert::IsTrue(rtti->Is("JsonTableParseHelper"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(JsonTableParseHelper::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(JsonTableParseHelper::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			JsonTableParseHelper* h = rtti->As<JsonTableParseHelper>();
			Assert::IsNotNull(h);
			Assert::AreEqual(&helper, h);
			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(JsonTableParseHelper::TypeIdClass());
			Assert::IsNotNull(r);

			r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(JsonTableParseHelper::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(JsonTestParseHelperSharedDataRTTI)
		{
			JsonTableParseHelper::SharedData sharedData;

			RTTI* rtti = &sharedData;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("SharedData"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(JsonTableParseHelper::SharedData::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(JsonTableParseHelper::SharedData::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			JsonTableParseHelper::SharedData* h = rtti->As<JsonTableParseHelper::SharedData>();
			Assert::IsNotNull(h);
			Assert::AreEqual(&sharedData, h);
			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(JsonTableParseHelper::SharedData::TypeIdClass());
			Assert::IsNotNull(r);

			r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(JsonTableParseHelper::SharedData::TypeIdClass());
			Assert::IsNotNull(r);
		}


		TEST_METHOD(IntegerTest)
		{

			std::string inputString = R"({
			"Test": {
				"type": "integer",
				"value": 100
			}})";

			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Parse(inputString);
			
			Datum* datum = sharedData.GetData().Find("Test");
			Assert::AreEqual(100, datum->Get<std::int32_t>());
		}

		TEST_METHOD(FloatTest)
		{
			std::string inputString = R"({
			"Test": {
				"type": "float",
				"value": 100.0
			}})";

			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Parse(inputString);

			Datum* datum = sharedData.GetData().Find("Test");
			Assert::AreEqual(100.f,datum->Get<float>());
		}

		TEST_METHOD(StringTest)
		{
			std::string inputString = R"({
			"Test": {
				"type": "string",
				"value": "testing"
			}})";

			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Parse(inputString);

			std::string testString = "testing";

			Datum* datum = sharedData.GetData().Find("Test");
			Assert::AreEqual(testString,datum->Get<std::string>());
		}

		TEST_METHOD(VectorTest)
		{
			std::string inputString = R"({
			"Test": {
				"type": "vector",
				"value": "vec4(1,0,0,1) "
			}})";

			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Parse(inputString);

			Datum* datum = sharedData.GetData().Find("Test");
			Assert::AreEqual(glm::vec4(1,0,0,1), datum->Get<glm::vec4>());
		}

		TEST_METHOD(MatrixTest)
		{
			std::string inputString = R"({
			"Test": {
				"type": "matrix",
				"value": "mat4x4((1,0,0,0),(0,1,0,0),(0,0,1,0),(0,0,0,1)) "
			}})";

			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Parse(inputString);

			Datum* datum = sharedData.GetData().Find("Test");
			Assert::AreEqual(glm::mat4(1, 0, 0, 0,0, 1, 0, 0,0, 0, 1, 0,0, 0, 0, 1), datum->Get<glm::mat4>());
		}

		TEST_METHOD(TableTest)
		{
			std::string inputString = R"({
			"Test": {
				"type": "table",
				"value": {
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
						"value": {
							"Zip Code": {
								"type": "integer",
								"value":  12345
								},
							"Zip Code+4": {
								"type": "integer",
								"value": 6789
								}
							}
			}}}})";

			ScopeFactory scopefactory;
			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Parse(inputString);

			Datum* datum = sharedData.GetData().Find("Test");
			Assert::AreEqual(DatumTypes::Table, datum->Type());

			std::string a = "123 Anystreet St.";
			Datum anotherDatum = a;
			Scope* scope = datum->Get<Scope*>(0);
			Assert::AreEqual(anotherDatum, (*scope)["Street"]);

			a = "Orlando";
			anotherDatum = a;
			Assert::AreEqual(anotherDatum, (*scope)["City"]);

			Assert::AreEqual(DatumTypes::Table, (*scope)["Postal Code"].Type());
			datum = &(*scope)["Postal Code"];
			scope = datum->Get<Scope*>(0);
			anotherDatum = 12345;
			Assert::AreEqual(anotherDatum, (*scope)["Zip Code"]);

			anotherDatum = 6789;
			Assert::AreEqual(anotherDatum, (*scope)["Zip Code+4"]);
		}

		TEST_METHOD(ArrayTest)
		{
			std::string inputString = R"({
			"Test": {
				"type": "string",
				"value": [ "Bob", "Jim", "Fred" ]
			}})";

			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Parse(inputString);

			std::string testString = "Fred";
			std::string testString1 = "Jim";
			std::string testString2 = "Bob";

			Datum* datum = sharedData.GetData().Find("Test");
			Assert::AreEqual(testString, datum->Back<std::string>());
			datum->PopBack();
			Assert::AreEqual(testString1, datum->Back<std::string>());
			datum->PopBack();
			Assert::AreEqual(testString2, datum->Back<std::string>());
		}

		TEST_METHOD(ParseFromFile)
		{
			ScopeFactory scopefactory;
			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);
			const std::string fileName = R"(Content\test.json)"s;
			parseMaster.ParseFromFile(fileName);

			Datum* datum = sharedData.GetData().Find("Name");
			std::string test = "Test";
			Assert::AreEqual(test, datum->Get<std::string>());

			datum = sharedData.GetData().Find("Health");
			Assert::AreEqual(100, datum->Get<std::int32_t>());

			datum = sharedData.GetData().Find("Damage");
			Assert::AreEqual(100.f, datum->Get<float>());

			datum = sharedData.GetData().Find("Direction");
			Assert::AreEqual(glm::vec4(1, 0, 0, 1), datum->Get<glm::vec4>());

			datum = sharedData.GetData().Find("Transform");
			Assert::AreEqual(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1), datum->Get<glm::mat4>());

			datum = sharedData.GetData().Find("Aliases");
			std::string testString = "Fred";
			std::string testString1 = "Jim";
			std::string testString2 = "Bob";
			Assert::AreEqual(testString, datum->Back<std::string>());
			datum->PopBack();
			Assert::AreEqual(testString1, datum->Back<std::string>());
			datum->PopBack();
			Assert::AreEqual(testString2, datum->Back<std::string>());

			datum = sharedData.GetData().Find("Address");

			std::string a = "123 Anystreet St.";
			Datum anotherDatum = a;
			Scope* scope = datum->Get<Scope*>(0);
			Assert::AreEqual(anotherDatum, (*scope)["Street"]);

			a = "Orlando";
			anotherDatum = a;
			Assert::AreEqual(anotherDatum, (*scope)["City"]);

			Assert::AreEqual(DatumTypes::Table, (*scope)["Postal Code"].Type());
			datum = &(*scope)["Postal Code"];
			scope = datum->Get<Scope*>(0);
			anotherDatum = 12345;
			Assert::AreEqual(anotherDatum, (*scope)["Zip Code"]);

			anotherDatum = 6789;
			Assert::AreEqual(anotherDatum, (*scope)["Zip Code+4"]);
		}


		TEST_METHOD(TableArrayTest)
		{
			std::string inputString = R"({
				"Test":{
					"type": "table",
					"value": [
						{
							"One": {
								"type": "integer",
								"value": 123
							}
						},
						{
							"Two": {
								"type": "integer",
								"value": 456
							}
						},
						{
							"Three": {
								"type": "integer",
								"value": 567
							}
						}
					]
			}})";

			ScopeFactory scopefactory;
			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			parseMaster.Parse(inputString);

			Datum* datum = sharedData.GetData().Find("Test");
			Assert::AreEqual(DatumTypes::Table, datum->Type());

			const std::int32_t a = 123;
			Datum anotherDatum = a;
			Scope *scope = datum->Get<Scope*>(0);
			Assert::AreEqual(anotherDatum, (*scope)["One"]);

			const std::int32_t b = 456;
			anotherDatum = b;
			scope = datum->Get<Scope*>(1);
			Assert::AreEqual(anotherDatum, (*scope)["Two"]);

			const std::int32_t c = 567;
			anotherDatum = c;
			scope = datum->Get<Scope*>(2);
			Assert::AreEqual(anotherDatum, (*scope)["Three"]);
		}

		TEST_METHOD(Create)
		{
			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			auto clone = tableParseHelper.Create();
			Assert::IsNotNull(clone);
			delete clone;

			auto sharedDataClone = sharedData.Create();
			Assert::IsNotNull(sharedDataClone);
			delete sharedDataClone;
		}

		TEST_METHOD(Handler)
		{
			JsonIntegerParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster;
			parseMaster.AddHelper(tableParseHelper);

			Assert::IsFalse(tableParseHelper.StartHandler(sharedData, "", ""));
			Assert::IsFalse(tableParseHelper.EndHandler(sharedData, "",""));
		}

		TEST_METHOD(ClassTest)
		{
			std::string inputString = R"({
			"Test": {
				"type": "table",
				"value":  {
							"class": "AttributedBrilliantFoo",
							"Zip Code": {
								"type": "integer",
								"value":  12345
							}}
			}})";

			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			AttributedBrilliantFooFactory attributedFactory;
			parseMaster.Parse(inputString);

			Datum* datum = sharedData.GetData().Find("Test");
			Assert::AreEqual(DatumTypes::Table, datum->Type());
			
			Datum anotherDatum = 12345;
			Scope* scope = datum->Get<Scope*>(0);
			Assert::AreEqual(anotherDatum, (*scope)["Zip Code"]);
			Assert::IsTrue(scope->Is("AttributedBrilliantFoo"s));
		}

		TEST_METHOD(TableArrayClassTest)
		{
			std::string inputString = R"({
				"Test":{
					"type": "table",
					"value": [
						{
							"class": "AttributedFoo",
							"One": {
								"type": "integer",
								"value": 123
							}
						},
						{
							"class": "AttributedBrilliantFoo",
							"Two": {
								"type": "integer",
								"value": 456
							}
						},
						{
							"Three": {
								"type": "integer",
								"value": 567
							}
						}
					]
			}})";

			ScopeFactory scopefactory;
			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			AttributedBrilliantFooFactory attributedBrilliantFactory;
			AttributedFooFactory attributedFooFactory;
			parseMaster.Parse(inputString);

			Datum* datum = sharedData.GetData().Find("Test");
			Assert::AreEqual(DatumTypes::Table, datum->Type());

			const std::int32_t a = 123;
			Datum anotherDatum = a;
			Scope* scope = datum->Get<Scope*>(0);
			Assert::AreEqual(anotherDatum, (*scope)["One"]);
			Assert::IsFalse(scope->Is("AttributedBrilliantFoo"s));
			Assert::IsTrue(scope->Is("AttributedFoo"s));

			const std::int32_t b = 456;
			anotherDatum = b;
			scope = datum->Get<Scope*>(1);
			Assert::AreEqual(anotherDatum, (*scope)["Two"]);
			Assert::IsTrue(scope->Is("AttributedBrilliantFoo"s));

			const std::int32_t c = 567;
			anotherDatum = c;
			scope = datum->Get<Scope*>(2);
			Assert::AreEqual(anotherDatum, (*scope)["Three"]);
			Assert::IsFalse(scope->Is("AttributedBrilliantFoo"s));
			Assert::IsFalse(scope->Is("AttributedFoo"s));
		}

		TEST_METHOD(ExternalTest)
		{
			std::string inputString = R"({
				"Test":{
					"type": "table",
					"value": 
						{
							"class": "AttributedFoo",
							"ExternalInteger": {
								"type": "integer",
								"value": 123
							}
						}
			}})";

			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			AttributedFooFactory attributedFooFactory;
			parseMaster.Parse(inputString);

			Datum* datum = sharedData.GetData().Find("Test");
			Assert::AreEqual(DatumTypes::Table, datum->Type());

			const std::int32_t a = 123;
			Datum anotherDatum = a;
			Scope* scope = datum->Get<Scope*>(0);
			Assert::AreEqual(anotherDatum, (*scope)["ExternalInteger"]);
			Assert::IsFalse((*scope)["ExternalInteger"].IsInternal());
			Assert::IsTrue(scope->Is("AttributedFoo"s));
		}

		TEST_METHOD(ExternalArrayTest)
		{
			std::string inputString = R"({
				"Test":{
					"type": "table",
					"value": 
						[{
							"class": "AttributedFoo",
							"ExternalIntegerArray": {
								"type": "integer",
								"value": [123,456,789]
							}
						},
						{
							"class": "AttributedFoo",
							"ExternalFloatArray": {
								"type": "float",
								"value": [123,456,789]
							}
						}]
			}})";

			JsonTableParseHelper::SharedData sharedData;
			JsonTableParseHelper tableParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(tableParseHelper);

			AttributedFooFactory attributedFooFactory;
			parseMaster.Parse(inputString);

			Datum* datum = sharedData.GetData().Find("Test");
			Assert::AreEqual(DatumTypes::Table, datum->Type());

			std::int32_t a = 123;
			Scope* scope = datum->Get<Scope*>(0);
			Assert::AreEqual(a, (*scope)["ExternalIntegerArray"].Get<std::int32_t>(0));
			a = 456;
			Assert::AreEqual(a, (*scope)["ExternalIntegerArray"].Get<std::int32_t>(1));
			a = 789;
			Assert::AreEqual(a, (*scope)["ExternalIntegerArray"].Get<std::int32_t>(2));
			Assert::IsFalse((*scope)["ExternalIntegerArray"].IsInternal());
			Assert::IsTrue(scope->Is("AttributedFoo"s));

			float b = 123;
			scope = datum->Get<Scope*>(1);
			Assert::AreEqual(b, (*scope)["ExternalFloatArray"].Get<float>(0));
			b = 456;
			Assert::AreEqual(b, (*scope)["ExternalFloatArray"].Get<float>(1));
			b = 789;
			Assert::AreEqual(b, (*scope)["ExternalFloatArray"].Get<float>(2));
			Assert::IsFalse((*scope)["ExternalFloatArray"].IsInternal());
			Assert::IsTrue(scope->Is("AttributedFoo"s));
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState JsonTableParseHelperTests::_startMemState;
}