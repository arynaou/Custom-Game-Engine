#include "pch.h"
#include "Foo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTests
{
	TEST_CLASS(FooTests)
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
			{
				Foo f{1};
				Assert::AreEqual(f.Data(), 1);
			}

			{
				std::int32_t testInt = 1;
				Assert::AreEqual(Foo(testInt).Data() , testInt);
			}
		}

		TEST_METHOD(Data)
		{
			{
				Foo f{ 1 };
				Assert::AreEqual(f.Data(), 1);
				std::int32_t& data = f.Data();
				data = 2;
				Assert::AreEqual(data, f.Data());
			}

			{
				const Foo f{ 1 };
				Assert::AreEqual(f.Data(), 1);
			}
		}

		TEST_METHOD(CopyConstructor)
		{
			Foo f{ 1 };
			
			{
				Foo copyF = f;
				Assert::AreEqual(f.Data(), copyF.Data());
			}

			{
				Foo copyF{ 2 };
				Assert::AreNotEqual(f.Data(), copyF.Data());
				copyF = f;
				Assert::AreEqual(f.Data(), copyF.Data());
			}
		}

		TEST_METHOD(EqualOperator)
		{
			Foo f{ 1 };
			Foo f2{ 2 };
			Assert::IsTrue(f == f);
			Assert::IsFalse(f == f2);

			f2 = f;
			Assert::IsTrue(f == f2);
		}

		TEST_METHOD(NotEqualOperator)
		{
			Foo f{ 1 };
			Foo f2{ 2 };
			Assert::IsTrue(f != f2);

			f2 = f;
			Assert::IsFalse(f != f2);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState FooTests::_startMemState;
}
