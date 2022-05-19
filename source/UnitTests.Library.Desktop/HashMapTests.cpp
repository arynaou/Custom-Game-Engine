#include "pch.h"
#include "HashMap.h"
#include "Foo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTests
{
	TEST_CLASS(HashMapTests)
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

		struct FooHash
		{
			std::size_t operator()(const Foo& value)
			{
				return abs(value.Data());
			}
		};

		TEST_METHOD(Constructor)
		{
			HashMap<Foo, Foo> hashmap(11, FooHash{});
			Assert::AreEqual(hashmap.Size(), size_t(0));
		}

		TEST_METHOD(Insert)
		{
			Foo f1 = 10;
			Foo f2 = 20;
			Foo f3 = 30;
			HashMap<Foo, Foo> hashmap(11, FooHash{});
			hashmap.Insert({ f1,f2 });
			Assert::AreEqual(hashmap[f1], f2);
			hashmap.Insert({ f1,f3 });
			Assert::AreNotEqual(hashmap[f1], f3);
7;
		}

		TEST_METHOD(CopyConstructor)
		{
			Foo f1 = 10;
			Foo f2 = 20;
			HashMap<Foo, Foo> hashmap(11, FooHash{});
			HashMap<Foo, Foo> hashmap2(11, FooHash{});
			hashmap.Insert({ f1,f2 });
			hashmap2 = hashmap;
			Assert::AreEqual(hashmap[f1], hashmap2[f1]);

			HashMap<Foo, Foo> hashmap3 = hashmap;
			Assert::AreEqual(hashmap3[f1], hashmap[f1]);
		}

		TEST_METHOD(Clear)
		{
			Foo f1 = 10;
			Foo f2 = 20;
			HashMap<Foo, Foo> hashmap(11, FooHash{});
			hashmap.Insert({ f1,f2 });
			Assert::IsTrue(hashmap.Find(f1) != hashmap.end());
			hashmap.Clear();

			Assert::AreEqual(hashmap.Size(), size_t(0));
			Assert::IsTrue(hashmap.Find(f1) == hashmap.end());
		}

		TEST_METHOD(Find)
		{
			Foo f1 = 10;
			Foo f2 = 20;
			Foo f3 = 30;
			HashMap<Foo, Foo> hashmap(11, FooHash{});
			hashmap.Insert({ f1,f2 });
			Assert::IsTrue(hashmap.Find(f1) != hashmap.end());
			Assert::IsTrue(hashmap.Find(f3)== hashmap.end());  

			const HashMap<Foo, Foo> constHashmap = hashmap;
			Assert::IsTrue(constHashmap.Find(f1) != constHashmap.cend());
			Assert::IsTrue(constHashmap.Find(f3) == constHashmap.cend());
		}

		TEST_METHOD(BracketOperator)
		{
			Foo f1 = 10;
			Foo f2 = 20;
			HashMap<Foo, Foo> hashmap(11, FooHash{});
			hashmap.Insert({ f1, f2 });
			Assert::AreEqual(hashmap[f1], f2);

			Foo f3 = 0;
			Assert::AreEqual(hashmap[f2], f3);
		}

		TEST_METHOD(Remove)
		{
			Foo f1 = 10;
			Foo f2 = 20;
			Foo f3 = 30;
			Foo f4 = 40;
			HashMap<Foo, Foo> hashmap(11, FooHash{});
			hashmap.Insert({ f1,f2 });
			Assert::IsTrue(hashmap.Find(f1) != hashmap.end());
			hashmap.Remove(f1);
			Assert::AreEqual(hashmap.Size(), size_t(0));
			Assert::IsTrue(hashmap.Find(f1)== hashmap.end());
		}
		
		TEST_METHOD(ContainsKey)
		{
			Foo f1 = 10;
			Foo f2 = 20;
			HashMap<Foo, Foo> hashmap(11, FooHash{});
			hashmap.Insert({ f1, f2 });
			Assert::IsTrue(hashmap.ContainsKey(f1));
			Assert::IsFalse(hashmap.ContainsKey(f2));

			const HashMap<Foo, Foo> constHashMap = hashmap;
			Assert::IsTrue(constHashMap.ContainsKey(f1));
			Assert::IsFalse(constHashMap.ContainsKey(f2));
		}

		TEST_METHOD(At)
		{
			Foo f1 = 10;
			Foo f2 = 20;
			HashMap<Foo, Foo> hashmap(2, FooHash{});
			hashmap.Insert({ f1, f2 });
			Assert::AreEqual(hashmap.At(f1), f2);

			const HashMap<Foo, Foo> constHashMap = hashmap;
			Assert::AreEqual(constHashMap.At(f1),f2);
		}

		TEST_METHOD(Iterator)
		{
			Foo f1 = 10;
			Foo f2 = 20;
			Foo f3 = 30;
			HashMap<Foo, Foo> hashmap(11, FooHash{});
			hashmap.Insert({ f1,f2 });

			size_t count = 0;
			for (HashMap<Foo,Foo>::Iterator it = hashmap.begin(); it != hashmap.end(); ++it, ++count)
			{
				;
			}
			Assert::AreEqual(hashmap.Size(), count);

			count = 0;
			for (HashMap<Foo, Foo>::Iterator it = hashmap.begin(); it != hashmap.end(); it++, ++count)
			{
				;
			}
			Assert::AreEqual(hashmap.Size(), count);
			
			count = 0;
			for (const std::pair<const Foo,Foo> value : hashmap)
			{
				value;
				count++;
			}
			Assert::AreEqual(hashmap.Size(), count);

			count = 0;
			HashMap<Foo, Foo> hashmap2(11, FooHash{});
			for (HashMap<Foo, Foo>::Iterator it = hashmap2.begin(); it != hashmap2.end(); ++it, ++count)
			{
				;
			}
			Assert::AreEqual(hashmap2.Size(), count);

			{
				//HashMap<Foo, Foo>::Iterator it = hashmap.begin();
				//Assert::AreEqual(it, hashmap.begin());
				//const HashMap<Foo, Foo>::Iterator constIt = hashmap.begin();
				//Assert::AreEqual(constIt, hashmap.begin());
			}
		}
		
		TEST_METHOD(cIterator)
		{
			Foo f1 = 10;
			Foo f2 = 20;
			Foo f3 = 30;
			HashMap<Foo, Foo> hashmap(11, FooHash{});
			hashmap.Insert({ f1,f2 });
			const HashMap<Foo, Foo> constHashmap = hashmap;

			size_t count = 0;
			for (HashMap<Foo, Foo>::ConstIterator it = constHashmap.cbegin(); it != constHashmap.cend(); ++it, ++count)
			{
				;
			}
			Assert::AreEqual(constHashmap.Size(), count);

			count = 0;
			const HashMap<Foo, Foo> constHashmap2(11, FooHash{});
			for (HashMap<Foo, Foo>::ConstIterator it = constHashmap2.cbegin(); it != constHashmap2.cend(); ++it, ++count)
			{
				;
			}
			Assert::AreEqual(constHashmap2.Size(), count);
		}


		TEST_METHOD(ConstIterator)
		{
			Foo f1 = 10;
			Foo f2 = 20;
			Foo f3 = 30;
			HashMap<Foo, Foo> hashmap(11, FooHash{});
			hashmap.Insert({ f1,f2 });
			const HashMap<Foo, Foo> constHashmap = hashmap;

			size_t count = 0;
			for (HashMap<Foo, Foo>::ConstIterator it = constHashmap.begin(); it != constHashmap.end(); ++it, ++count)
			{
				;
			}
			Assert::AreEqual(constHashmap.Size(), count);


			count = 0;
			for (const std::pair<const Foo, Foo> value : constHashmap)
			{
				value;
				count++;
			}
			Assert::AreEqual(constHashmap.Size(), count);

			count = 0;
			const HashMap<Foo, Foo> constHashmap2(11, FooHash{});
			for (HashMap<Foo, Foo>::ConstIterator it = constHashmap2.begin(); it != constHashmap2.end(); ++it, ++count)
			{
				;
			}
			Assert::AreEqual(constHashmap2.Size(), count);
		}

		TEST_METHOD(IteratorIncrementFail)
		{
			auto func = []() { HashMap<Foo,Foo>::Iterator it; ++it; };
			Assert::ExpectException<std::runtime_error>(func);
		}

		TEST_METHOD(ConstIteratorIncrementFail)
		{
			auto func = []() { HashMap<Foo,Foo>::ConstIterator it; ++it; };
			Assert::ExpectException<std::runtime_error>(func);
		}


		TEST_METHOD(ConstructorInitialize)
		{
			HashMap<std::string, std::int32_t> hashmap = {
				{"A",1},
				{"B",2},
				{"C",3}
			};

			Assert::AreEqual(size_t(3), hashmap.Size());
			Assert::AreEqual(1,hashmap.At("A"));
			Assert::AreEqual(2,hashmap.At("B"));
			Assert::AreEqual(3,hashmap.At("C"));
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState HashMapTests::_startMemState;
}