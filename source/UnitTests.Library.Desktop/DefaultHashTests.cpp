#include "pch.h"
#include "DefaultHash.h"
#include "Foo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FieaGameEngine
{
	template<>
	struct DefaultHash<Foo>
	{
		std::size_t operator()(const Foo& key)
		{
			return key.Data();
		}
	};
}

using namespace FieaGameEngine;
namespace UnitTests
{
	TEST_CLASS(DefaultHashTests)
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

		TEST_METHOD(Integers)
		{
			const std::uint32_t a = 10;
			const std::uint32_t b = 20;
			const std::uint32_t c = a;

			DefaultHash<std::uint32_t> func;
			Assert::AreEqual(func(a), func(a));
			Assert::AreNotEqual(func(a), func(b));
			Assert::AreNotSame(a, c);
			Assert::AreEqual(a, c);
			Assert::AreEqual(func(a), func(c));
		}

		TEST_METHOD(Foos)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ a };

			DefaultHash<Foo> func;
			Assert::AreEqual(func(a), func(a));
			Assert::AreNotEqual(func(a), func(b));
			Assert::AreNotSame(a, c);
			Assert::AreEqual(a, c);
			Assert::AreEqual(func(a), func(c));
		}

		TEST_METHOD(CStyleStrings)
		{
			const char* a = "hello";
			const char* b = "goodbye";
			char c[6];
			strcpy_s(c, a);
			
			DefaultHash<char*> func;
			Assert::AreEqual(func(a), func(a));
			Assert::AreNotEqual(func(a), func(b));
			Assert::IsTrue(strcmp(a, c) == 0);
			Assert::AreEqual(func(a), func(c));
		}

		TEST_METHOD(ConstCStyleStrings)
		{
			const char* a = "hello";
			const char* b = "goodbye";
			char c[6];
			strcpy_s(c, a);

			DefaultHash<const char*> func;
			Assert::AreEqual(func(a), func(a));
			Assert::AreNotEqual(func(a), func(b));
			Assert::IsTrue(strcmp(a, c) == 0);
			Assert::AreEqual(func(a), func(c));
		}

		TEST_METHOD(Strings)
		{
			std::string a = "hello";
			std::string b = "goodbye";
			std::string c = "hello";

			DefaultHash<std::string> func;
			Assert::AreEqual(func(a), func(a));
			Assert::AreNotEqual(func(a), func(b));
			Assert::AreEqual(a, c);
			Assert::AreEqual(func(a), func(c));
		}

		TEST_METHOD(ConstStrings)
		{
			const std::string a = "hello";
			const std::string b = "goodbye";
			const std::string c = "hello";

			DefaultHash<const std::string> func;
			Assert::AreEqual(func(a), func(a));
			Assert::AreNotEqual(func(a), func(b));
			Assert::AreEqual(a, c);
			Assert::AreEqual(func(a), func(c));
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState DefaultHashTests::_startMemState;
}
