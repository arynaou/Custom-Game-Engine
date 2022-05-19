#include "pch.h"
#include "Foo.h"
#include "Factory.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTests
{
	TEST_CLASS(FactoryTests)
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

		TEST_METHOD(AddRemoveFactory)
		{
			const Factory<RTTI>* foundFactory = Factory<RTTI>::Find("Foo"s);
			Assert::IsNull(foundFactory);

			{
				const FooFactory fooFactory;
				foundFactory = Factory<RTTI>::Find("Foo"s);
				Assert::AreEqual(&fooFactory, static_cast<const FooFactory*>(foundFactory));

				Assert::ExpectException<std::exception>([] {FooFactory anotherFactory; });
			}

			foundFactory = Factory<RTTI>::Find("Foo"s);
			Assert::IsNull(foundFactory);

		}

		TEST_METHOD(ProductCreation)
		{
			RTTI* rtti = Factory<RTTI>::Create("Foo"s);
			Assert::IsNull(rtti);

			{
				FooFactory foofactory;
				
				rtti = Factory<RTTI>::Create("Foo"s);
				Assert::IsNotNull(rtti);

				Foo* foo = rtti->As<Foo>();
				Assert::IsNotNull(foo);

				delete foo;
			}

			rtti = Factory<RTTI>::Create("Foo"s);
			Assert::IsNull(rtti);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState FactoryTests::_startMemState;
}