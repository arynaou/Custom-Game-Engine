#include "pch.h"
#include "SList.h"
#include "Foo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTests
{
	TEST_CLASS(SListTests)
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
			SList<Foo> list;
			Assert::AreEqual((size_t)0, list.Size());
		}

		TEST_METHOD(Front)
		{
			SList<Foo> list;
			auto func = [&list]() { list.Front(); };
			Assert::ExpectException<std::runtime_error>(func);
			Assert::IsTrue(list.IsEmpty());

			{
				Foo f1{ 1 };
				Foo f2{ 2 };
				list.PushFront(f1);
				Assert::AreEqual(f1,list.Front());
				list.PushFront(f2);
				Assert::AreEqual(f2, list.Front());
			}

			{
				const SList<Foo> constList = list;
				Assert::AreEqual(constList.Front(), list.Front());
			}
			
			{
				const SList<Foo> constList;
				auto func2 = [&constList]() { constList.Front(); };
				Assert::ExpectException<std::runtime_error>(func2);
			}
		}  

		TEST_METHOD(Back)
		{
			SList<Foo> list;
			auto func = [&list]() { list.Back(); };
			Assert::ExpectException<std::runtime_error>(func);
			Assert::IsTrue(list.IsEmpty());
			{
				Foo f1{ 1 };
				Foo f2{ 2 };
				list.PushBack(f1);
				Assert::AreEqual(f1, list.Back());
				list.PushBack(f2);
				Assert::AreEqual(f2, list.Back());
			}

			{
				const SList<Foo> constList = list;
				Assert::AreEqual(constList.Back(), list.Back());
			}

			{
				const SList<Foo> constList;
				auto func2 = [&constList]() { constList.Back(); };
				Assert::ExpectException<std::runtime_error>(func2);
			}
		}

		TEST_METHOD(Empty)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());
			
			Foo f{ 1 };
			list.PushFront(f);
			Assert::IsFalse(list.IsEmpty());
		}

		TEST_METHOD(PushFront)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());
			Foo f{ 1 };
			list.PushFront(f);
			Assert::AreEqual(f,list.Front());

			Foo f2{ 2 };
			list.PushFront(f2);
			Assert::AreEqual(f2, list.Front());
		}

		TEST_METHOD(PopFront)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());
			list.PopFront();

			Foo f{ 1 };
			list.PushFront(f);
			list.PopFront();
			Assert::IsTrue(list.IsEmpty());
				 
			Foo f2{ 2 };
			list.PushFront(f);
			list.PushFront(f2);
			list.PopFront();
			Assert::AreEqual(f, list.Front());
		}

		TEST_METHOD(PushBack)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());
			Foo f{ 1 };
			list.PushBack(f);
			Assert::AreEqual(f, list.Back());

			Foo f2{ 2 };
			list.PushBack(f2);
			Assert::AreEqual(f2, list.Back());
		}

		TEST_METHOD(PopBack)
		{
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());
			list.PopBack();

			Foo f1{ 1 };
			Foo f2{ 2 };
			Foo f3{ 3 };

			list.PushBack(f1);
			list.PushBack(f2);
			list.PushBack(f3);
			list.PopBack();
			Assert::AreEqual(f2, list.Back());

			list.PopBack();
			Assert::AreEqual(f1, list.Back());

			list.PopBack();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(Size)
		{
			SList<Foo> list;
			Assert::AreEqual((size_t)0, list.Size());

			Foo f{ 1 };
			list.PushFront(f);
			Assert::AreEqual((size_t)1, list.Size());

			list.PopFront();
			Assert::AreEqual((size_t)0, list.Size());
		}

		TEST_METHOD(Clear)
		{
			SList<Foo> list;
			list.Clear();
			Assert::IsTrue(list.IsEmpty());

			Foo f{ 1 };
			Foo f2{ 2 };
			list.PushFront(f);
			list.PushFront(f2);
			Assert::IsFalse(list.IsEmpty());
			list.Clear();
			Assert::AreEqual((size_t)0, list.Size());
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(CopyConstructor)
		{
			SList<Foo> list;

			Foo f1{ 1 };
			Foo f2{ 2 };
			Foo f3{ 3 };
			
			{
				list.PushFront(f1);
				list.PushFront(f2);
				list.PushFront(f3);

				SList<Foo> listCopy{ list };
				Assert::AreEqual(list.Size(), listCopy.Size());
				while (!list.IsEmpty())
				{
					Assert::AreEqual(list.Front(), listCopy.Front());
					list.PopFront();
					listCopy.PopFront();
				}
			}

			{
				list.PushFront(f1);
				list.PushFront(f2);
				list.PushFront(f3);

				SList<Foo> listCopy;
				listCopy = list;
				Assert::AreEqual(list.Size(), listCopy.Size());
				while (!list.IsEmpty())
				{
					Assert::AreEqual(list.Front(), listCopy.Front());
					list.PopFront();
					listCopy.PopFront();
				}
			}
		}

		TEST_METHOD(Iterator)
		{
			{
				SList<Foo> list;
				Assert::AreEqual(list.end(), list.begin());

				Foo a(1);
				list.PushBack(a);
				Assert::AreNotEqual(list.end(), list.begin());
			}

			{
				Foo a(1);
				Foo b(2);
				Foo c(3);
				SList<Foo> list{a,b,c};

				Assert::AreEqual(list.Front(), a);
				size_t count = 0;
				for (auto it = list.begin(); it != list.end(); ++it, ++count)
				{
					;
				}
				Assert::AreEqual(list.Size(), count);

				count = 0;
				for (auto it = list.begin(); it != list.end(); it++, count++)
				{
					;
				}
				Assert::AreEqual(list.Size(), count);


				count = 0;
				for (const Foo& value : list)
				{
					value;
					count++;
				}
				Assert::AreEqual(list.Size(), count);
			}

			{
				Foo a(1);
				Foo b(2);
				Foo c(3);
				SList<Foo> list{ a,b,c };

				Assert::AreEqual(list.Front(), a);
				size_t count = 0;
				for (auto it = list.cbegin(); it != list.cend(); ++it, ++count)
				{
					;
				}
				Assert::AreEqual(list.Size(), count);

				count = 0;
				for (auto it = list.cbegin(); it != list.cend(); it++, count++)
				{
					;
				}
				Assert::AreEqual(list.Size(), count);
			}

			{
				Foo a(1);
				Foo b(2);
				Foo c(3);
				SList<Foo> list{ a,b,c };
				const SList<Foo> constList{ list };

				Assert::AreEqual(constList.Front(), a);
				size_t count = 0;
				for (auto it = constList.begin(); it != constList.end(); ++it, ++count)
				{
					;
				}
				Assert::AreEqual(constList.Size(), count);

				count = 0;
				for (auto it = constList.cbegin(); it != constList.cend(); it++, count++)
				{
					;
				}
				Assert::AreEqual(constList.Size(), count);
			}
		}

		TEST_METHOD(ConstIterator)
		{
			{
				Foo a(1);
				const SList<Foo> constList{a};
				Assert::AreNotEqual(constList.end(), constList.begin());

				SList<Foo>::ConstIterator constIt = constList.begin();
				Assert::AreEqual(constIt, constList.begin());

				SList<Foo> list{ a };
				SList<Foo>::Iterator it(list.begin());
				SList<Foo>::ConstIterator constIt2(it);
				Assert::AreEqual(constIt2, list.cbegin());
			}

			{
				Foo a(1);
				Foo b(2);
				Foo c(3);
				const SList<Foo> list{ a,b,c };

				Assert::AreEqual(list.Front(), a);
				size_t count = 0;
				for (auto it = list.begin(); it != list.end(); ++it, ++count)
				{
					;
				}
				Assert::AreEqual(list.Size(), count);

				count = 0;
				for (const Foo& value : list)
				{
					value;
					count++;
				}
				Assert::AreEqual(list.Size(), count);
			}
		}

		TEST_METHOD(IteratorCBeginCEnd)
		{
			Foo a(1);
			Foo b(2);
			Foo c(3);
			SList<Foo> list{ a,b,c };

			Assert::AreEqual(a, *list.cbegin());
			size_t count = 0;
			for (auto it = list.cbegin(); it != list.cend(); ++it, ++count)
			{
				;
			}
			Assert::AreEqual(list.Size(), count);
		}

		TEST_METHOD(IteratorIncrementFail)
		{
			auto func = []() { SList<Foo>::Iterator it; ++it; };
			Assert::ExpectException<std::runtime_error>(func);
		}

		TEST_METHOD(IteratorDereferenceFail)
		{
			auto func = []() { SList<Foo>::Iterator it; *it; };
			Assert::ExpectException<std::runtime_error>(func);
		}

		TEST_METHOD(ConstIteratorIncrementFail)
		{
			auto func = []() { SList<Foo>::ConstIterator it; ++it; };
			Assert::ExpectException<std::runtime_error>(func);
		}

		TEST_METHOD(Find)
		{
			
			SList<Foo>::Iterator it;
			Foo a(1);
			Foo b(2);
			Foo c(3);
			
			{
				SList<Foo> list;
				Assert::AreEqual(list.Find(a), it);
			}

			{
				SList<Foo> list{ a,b,c };
				Assert::AreNotEqual(list.Find(a), it);
			
				const SList<Foo> constlist = list;
				SList<Foo>::ConstIterator constIt;
				Assert::AreNotEqual(constlist.Find(a), constIt);
			}
		}

		TEST_METHOD(InsertAfter)
		{
			SList<Foo> list;
			Foo a(1);
			Foo b(2);
			Foo c(3);
			list.PushFront(a);
			list.PushFront(b);
			Assert::AreEqual(list.Front(), b);
			SList<Foo>::Iterator it=list.begin();

			list.InsertAfter(it, c);
			list.PopBack();
			Assert::AreEqual(list.Back(), c);
		}

		TEST_METHOD(Remove)
		{
			{
				Foo a(1);
				SList<Foo> list{ a };
				Assert::IsFalse(list.IsEmpty());

				list.Remove(a);
				Assert::IsTrue(list.IsEmpty());
			}

			{
				Foo a(1);
				Foo b(2);
				Foo c(3);
				SList<Foo> list{ a,b,c };

				list.Remove(c);
				Assert::AreEqual(list.Back(), b);
			}

			{
				Foo a(1);
				Foo b(2);
				Foo c(3);
				Foo d(4);
				SList<Foo> list{a,b,c,d};

				list.Remove(b);
				Assert::AreEqual(size_t(3), list.Size());
			}

		}

		private:
			static _CrtMemState _startMemState;
	};

	_CrtMemState SListTests::_startMemState;
}
