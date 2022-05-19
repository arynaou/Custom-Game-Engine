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
	TEST_CLASS(JsonTestParseHelperTests)
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

		TEST_METHOD(InitializeHelper)
		{
			JsonTestParseHelper::SharedData sharedData;
			JsonParseMaster parseMaster(sharedData);
			Assert::IsFalse(parseMaster.IsClone());
			Assert::AreEqual(size_t(0), parseMaster.Helpers().Size());
			Assert::AreEqual(&sharedData, parseMaster.GetSharedData().As<JsonTestParseHelper::SharedData>());

			JsonTestParseHelper testParseHelper;
			testParseHelper.Initialize();
			JsonIntegerParseHelper integerParseHelper;
			integerParseHelper.Initialize();
		}

		TEST_METHOD(StartHandler)
		{
			JsonIntegerParseHelper::SharedData sharedData;
			JsonIntegerParseHelper integerParseHelper;
			Assert::IsTrue(integerParseHelper.StartHandler(sharedData, "integer", 100));
			Assert::AreEqual(100, sharedData.Data[0]);
			Assert::IsFalse(integerParseHelper.StartHandler(sharedData,"nothing","200"));

			JsonTestParseHelper testParseHelper;
			JsonTestParseHelper::SharedData anotherSharedData;
			Assert::IsTrue(testParseHelper.StartHandler(anotherSharedData, "",""));
			Assert::IsFalse(testParseHelper.StartHandler(sharedData, "", ""));
		}

		TEST_METHOD(EndHandler)
		{
			JsonIntegerParseHelper::SharedData sharedData;
			JsonTestParseHelper::SharedData anotherSharedData;
			JsonIntegerParseHelper integerParseHelper;
			JsonTestParseHelper testParseHelper;

			Assert::IsFalse(integerParseHelper.EndHandler(sharedData, "integer",""));
			integerParseHelper.StartHandler(sharedData, "integer", 100);
			Assert::IsTrue(integerParseHelper.EndHandler(sharedData, "integer",""));
			Assert::IsFalse(integerParseHelper.EndHandler(sharedData, "nothing", ""));
			Assert::IsFalse(integerParseHelper.EndHandler(anotherSharedData, "integer", ""));

			Assert::IsTrue(testParseHelper.EndHandler(anotherSharedData, "", ""));
			Assert::IsFalse(testParseHelper.EndHandler(sharedData, "", ""));
		}

		TEST_METHOD(Create)
		{
			JsonIntegerParseHelper::SharedData sharedData;
			JsonIntegerParseHelper::SharedData* newSharedData = sharedData.Create();
			Assert::AreEqual(newSharedData->TypeIdClass(), sharedData.TypeIdClass());
			delete newSharedData;
		}


	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState JsonTestParseHelperTests::_startMemState;
}