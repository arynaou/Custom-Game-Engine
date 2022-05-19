#include "pch.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include "JsonIntegerParseHelper.h"
#include "JsonParseMaster.h"
#include "JsonTestParseHelper.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTests
{
	TEST_CLASS(JsonParseMasterTests)
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

			r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(JsonTestParseHelper::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(JsonTestParseHelperSharedDataRTTI)
		{
			JsonTestParseHelper::SharedData sharedData;

			RTTI* rtti = &sharedData;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("SharedData"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(JsonTestParseHelper::SharedData::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(JsonTestParseHelper::SharedData::TypeIdClass(), rtti->TypeIdInstance());

			Foo* f = rtti->As<Foo>();
			Assert::IsNull(f);

			JsonTestParseHelper::SharedData* h = rtti->As<JsonTestParseHelper::SharedData>();
			Assert::IsNotNull(h);
			Assert::AreEqual(&sharedData, h);
			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(JsonTestParseHelper::SharedData::TypeIdClass());
			Assert::IsNotNull(r);

			r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);

			r = rtti->QueryInterface(JsonTestParseHelper::SharedData::TypeIdClass());
			Assert::IsNotNull(r);
		}

		TEST_METHOD(Constructor)
		{
			JsonTestParseHelper::SharedData sharedData;
			JsonParseMaster parseMaster(sharedData);
			Assert::IsFalse(parseMaster.IsClone());
			Assert::AreEqual(size_t(0), parseMaster.Helpers().Size());
			Assert::AreEqual(&sharedData, parseMaster.GetSharedData().As<JsonTestParseHelper::SharedData>());

			JsonTestParseHelper testParseHelper;
			parseMaster.AddHelper(testParseHelper);
			Assert::AreEqual(size_t(1), parseMaster.Helpers().Size());

			Assert::AreEqual(&sharedData, parseMaster.GetSharedData().As<JsonTestParseHelper::SharedData>());
			Assert::AreEqual(&parseMaster, sharedData.GetJsonParseMaster());

			const auto& constSharedData = sharedData;
			Assert::AreEqual(const_cast<const JsonParseMaster*>(&parseMaster), constSharedData.GetJsonParseMaster());

			const auto& constParseMaster = parseMaster;
			Assert::AreEqual(const_cast<const JsonTestParseHelper::SharedData*>(&sharedData), constParseMaster.GetSharedData().As<JsonTestParseHelper::SharedData>());
		
			JsonTestParseHelper::SharedData anotherSharedData;
			parseMaster.SetSharedData(&anotherSharedData);
			Assert::AreEqual(&anotherSharedData, parseMaster.GetSharedData().As<JsonTestParseHelper::SharedData>());

			JsonIntegerParseHelper::SharedData newSharedData;
			newSharedData.Initialize();
			Assert::AreEqual(sharedData.Depth(), size_t(0));
			anotherSharedData.Initialize();
			Assert::AreEqual(anotherSharedData.Depth(), size_t(0));

		}

		TEST_METHOD(MoveSemantics)
		{
			JsonTestParseHelper::SharedData sharedData;
			JsonTestParseHelper testParseHelper;

			{
				JsonParseMaster parseMaster(sharedData);
				parseMaster.AddHelper(testParseHelper);

				JsonParseMaster otherParseMaster = std::move(parseMaster);
				Assert::IsFalse(otherParseMaster.IsClone());
				Assert::AreEqual(std::size_t(1), otherParseMaster.Helpers().Size());
				Assert::AreEqual(&sharedData, otherParseMaster.GetSharedData().As<JsonTestParseHelper::SharedData>());
				Assert::AreEqual(&otherParseMaster, sharedData.GetJsonParseMaster());
			}

			{
				JsonParseMaster parseMaster(sharedData);
				parseMaster.AddHelper(testParseHelper);

				JsonTestParseHelper::SharedData otherSharedData;
				JsonParseMaster otherParseMaster(otherSharedData);
				otherParseMaster = std::move(parseMaster);
				Assert::IsFalse(otherParseMaster.IsClone());
				Assert::AreEqual(std::size_t(1), otherParseMaster.Helpers().Size());
				Assert::AreEqual(&sharedData, otherParseMaster.GetSharedData().As<JsonTestParseHelper::SharedData>());
				Assert::AreEqual(&otherParseMaster, sharedData.GetJsonParseMaster());
			}

			{
				JsonParseMaster parseMaster(sharedData);

				Assert::AreEqual(size_t(0), parseMaster.Helpers().Size());
				parseMaster.AddHelper(testParseHelper);
				Assert::AreEqual(size_t(1), parseMaster.Helpers().Size());

				JsonIntegerParseHelper integerParseHelper;

				JsonParseMaster anotherParseMaster;
				anotherParseMaster.AddHelper(testParseHelper);

				anotherParseMaster = std::move(parseMaster);
				Assert::AreEqual(size_t(1), anotherParseMaster.Helpers().Size());
				Assert::AreEqual(testParseHelper.TypeIdInstance(), anotherParseMaster.Helpers()[0]->TypeIdInstance());
			}
		}

		TEST_METHOD(ParseFromString)
		{
			const std::string inputString = R"(
			{
				"Address":
				{
					"type": "table",
					"value":
					{
						"Street":
						{
							"type":"string",
							"value": "123 Anystreet St."
						}
					}
				}
			})"s;

			JsonTestParseHelper::SharedData sharedData;
			JsonTestParseHelper testParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(testParseHelper);

			Assert::IsFalse(testParseHelper.InitializeCalled);
			parseMaster.Initialize();
			Assert::IsTrue(testParseHelper.InitializeCalled);

			Assert::AreEqual(0U, testParseHelper.StartHandlerCount);
			Assert::AreEqual(0U, testParseHelper.EndHandlerCount);
			Assert::AreEqual(size_t(0), sharedData.Depth());
			Assert::AreEqual(size_t(0), sharedData.MaxDepth);

			parseMaster.Parse(inputString);
			Assert::AreEqual(6U, testParseHelper.StartHandlerCount);
			Assert::AreEqual(6U, testParseHelper.EndHandlerCount);
			Assert::AreEqual(size_t(0), sharedData.Depth());
			Assert::AreEqual(size_t(4), sharedData.MaxDepth);
		}

		TEST_METHOD(ParseFromSteam)
		{
			std::stringstream inputStream;
				
			inputStream << R"(
			{
				"Health":
				{
					"type": "integer",
					"value": 100
				}
			})"s;

			JsonTestParseHelper::SharedData sharedData;
			JsonTestParseHelper testParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(testParseHelper);

			Assert::IsFalse(testParseHelper.InitializeCalled);
			parseMaster.Initialize();
			Assert::IsTrue(testParseHelper.InitializeCalled);

			parseMaster.Parse(inputStream);
			Assert::AreEqual(3U, testParseHelper.StartHandlerCount);
			Assert::AreEqual(3U, testParseHelper.EndHandlerCount);
			Assert::AreEqual(size_t(0), sharedData.Depth());
			Assert::AreEqual(size_t(2), sharedData.MaxDepth);
		}

		TEST_METHOD(FileParsing)
		{
			const std::string filename = "TestFile.json";
			const std::string inputString = R"({"Health": {"type" :"integer", "value":100}})";

			std::ofstream outputFile(filename);
			Assert::IsTrue(outputFile.good());

			outputFile << inputString;
			outputFile.close();

			JsonTestParseHelper::SharedData sharedData;
			JsonTestParseHelper testParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(testParseHelper);

			Assert::IsFalse(testParseHelper.InitializeCalled);
			parseMaster.Initialize();
			Assert::IsTrue(testParseHelper.InitializeCalled);

			parseMaster.ParseFromFile(filename);

			Assert::AreEqual(3U, testParseHelper.StartHandlerCount);
			Assert::AreEqual(3U, testParseHelper.EndHandlerCount);
			Assert::AreEqual(size_t(0), sharedData.Depth());
			Assert::AreEqual(size_t(2), sharedData.MaxDepth);

			Assert::ExpectException<std::exception>([&parseMaster] {parseMaster.ParseFromFile("NonExistentFile.json"); });

			//JsonParseMaster jsonParseMaster;
			//jsonParseMaster.ParseFromFile("../../../../../source/UnitTests.Library.Desktop/test.json");
		}

		TEST_METHOD(Clone)
		{
			JsonTestParseHelper::SharedData sharedData;
			JsonParseMaster parseMaster(sharedData);
			Assert::IsFalse(parseMaster.IsClone());

			JsonTestParseHelper testParseHelper;
			parseMaster.AddHelper(testParseHelper);

			JsonIntegerParseHelper integerParseHelper;
			parseMaster.AddHelper(integerParseHelper);

			auto clone = parseMaster.Clone();
			Assert::AreNotEqual(clone, &parseMaster);
			Assert::IsTrue(clone->IsClone());
			Assert::AreEqual(std::size_t(2), clone->Helpers().Size());

			auto clonedHelper = clone->Helpers()[0]->As<JsonTestParseHelper>();
			Assert::IsNotNull(clonedHelper);
			Assert::AreNotEqual(clonedHelper, parseMaster.Helpers()[0]->As<JsonTestParseHelper>());
			Assert::AreNotSame(&(clone->GetSharedData()), &(parseMaster.GetSharedData()));

			Assert::ExpectException<std::exception>([&clone] {JsonTestParseHelper helper; clone->AddHelper(helper); });
			Assert::ExpectException<std::exception>([&clone, &clonedHelper] {clone->RemoveHelper(*clonedHelper); });
			Assert::ExpectException<std::exception>([&clone] {JsonTestParseHelper::SharedData data; clone->SetSharedData(&data); });

			delete clone;
		}

		TEST_METHOD(ParsingInvalidJson)
		{
			JsonTestParseHelper::SharedData sharedData;
			JsonTestParseHelper testParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(testParseHelper);
			Assert::ExpectException<std::exception>([&parseMaster] {const std::string inputString = R"({"integer":{100}})"; parseMaster.Parse(inputString); });
		}

		TEST_METHOD(IntergerParsing)
		{
			std::string inputString = R"({"integer" : 100})";

			JsonIntegerParseHelper::SharedData sharedData;
			JsonIntegerParseHelper integerParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(integerParseHelper);

			parseMaster.Parse(inputString);
			Assert::AreEqual(std::size_t(1), sharedData.Data.Size());
			Assert::AreEqual(100, sharedData.Data.Front());
		}

		TEST_METHOD(IntergerArrayParsing)
		{
			std::string inputString = R"({"integer" : [10,20,30,40]})";

			JsonIntegerParseHelper::SharedData sharedData;
			JsonIntegerParseHelper integerParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(integerParseHelper);

			parseMaster.Parse(inputString);
			Assert::AreEqual(std::size_t(4), sharedData.Data.Size());
			Assert::AreEqual(10, sharedData.Data[0]);
			Assert::AreEqual(20, sharedData.Data[1]);
			Assert::AreEqual(30, sharedData.Data[2]);
			Assert::AreEqual(40, sharedData.Data[3]);
		}

		TEST_METHOD(ValueArrayParsing)
		{
			const std::string inputString = R"({"Number" : [1,2,3]})";

			JsonTestParseHelper::SharedData sharedData;
			JsonTestParseHelper testParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(testParseHelper);

			Assert::IsFalse(testParseHelper.InitializeCalled);
			parseMaster.Initialize();
			Assert::IsTrue(testParseHelper.InitializeCalled);

			Assert::AreEqual(0U, testParseHelper.StartHandlerCount);
			Assert::AreEqual(0U, testParseHelper.EndHandlerCount);
			Assert::AreEqual(size_t(0), sharedData.Depth());
			Assert::AreEqual(size_t(0), sharedData.MaxDepth);
		}

		TEST_METHOD(ObjectArrayParsing)
		{
			const std::string inputString = R"(
			{
				"Objects" : 
				[
					{
						"1":1,
						"2":1,
						"3":1,
						"4":1
					},
					{"2":2},
					{"3":3}
				]
			})"s;

			JsonTestParseHelper::SharedData sharedData;
			JsonTestParseHelper testParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(testParseHelper);

			Assert::IsFalse(testParseHelper.InitializeCalled);
			parseMaster.Initialize();
			Assert::IsTrue(testParseHelper.InitializeCalled);

			Assert::AreEqual(0U, testParseHelper.StartHandlerCount);
			Assert::AreEqual(0U, testParseHelper.EndHandlerCount);
			Assert::AreEqual(size_t(0), sharedData.Depth());
			Assert::AreEqual(size_t(0), sharedData.MaxDepth);

			parseMaster.Parse(inputString);

			Assert::AreEqual(9U, testParseHelper.StartHandlerCount);
			Assert::AreEqual(9U, testParseHelper.EndHandlerCount);
			Assert::AreEqual(size_t(0), sharedData.Depth());
			Assert::AreEqual(size_t(2), sharedData.MaxDepth);
		}

		TEST_METHOD(AddRemoveHelpers)
		{
			JsonTestParseHelper::SharedData sharedData;
			JsonParseMaster parseMaster(sharedData);

			JsonTestParseHelper testParseHelper;
			Assert::AreEqual(size_t(0), parseMaster.Helpers().Size());
			parseMaster.AddHelper(testParseHelper);
			Assert::AreEqual(size_t(1), parseMaster.Helpers().Size());

			Assert::ExpectException<std::exception>([&parseMaster, &testParseHelper] {parseMaster.AddHelper(testParseHelper); });
			Assert::ExpectException<std::exception>([&parseMaster] {JsonTestParseHelper helper;  parseMaster.AddHelper(helper); });

			JsonIntegerParseHelper integerParseHelper;
			parseMaster.AddHelper(integerParseHelper);
			Assert::AreEqual(size_t(2), parseMaster.Helpers().Size());

			parseMaster.RemoveHelper(testParseHelper);
			Assert::AreEqual(size_t(1), parseMaster.Helpers().Size());

			parseMaster.RemoveHelper(integerParseHelper);
			Assert::AreEqual(size_t(0), parseMaster.Helpers().Size());

			Assert::IsFalse(parseMaster.RemoveHelper(testParseHelper));
		}

		TEST_METHOD(FallingThroughTheChain)
		{
			const std::string inputString = R"(
			{
				"Test":100
			})"s;

			JsonTestParseHelper::SharedData sharedData;
			JsonIntegerParseHelper testParseHelper;
			JsonParseMaster parseMaster(sharedData);
			parseMaster.AddHelper(testParseHelper);

			parseMaster.Parse(inputString);

			Assert::AreEqual(size_t(0), sharedData.Depth());
			Assert::AreEqual(size_t(0), sharedData.MaxDepth);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState JsonParseMasterTests::_startMemState;
}