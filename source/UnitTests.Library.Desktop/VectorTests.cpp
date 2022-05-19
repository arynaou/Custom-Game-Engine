#include "pch.h"
#include "Vector.h"
#include "Foo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTests
{
	TEST_CLASS(VectorTests)
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
			Vector<Foo> vector;
			Assert::AreEqual((size_t)0, vector.Size());
			Assert::AreEqual((size_t)0, vector.Capacity());
		}

		TEST_METHOD(Front)
		{
			Vector<Foo> vector;
			auto func = [&vector]() { vector.Front(); };
			Assert::ExpectException<std::runtime_error>(func);
			Assert::IsTrue(vector.IsEmpty());

			{
				Foo f1{ 1 };
				vector.PushBack(f1);
				Assert::AreEqual(f1, vector.Front());
			}

			{
				Foo f1{ 1 };
				vector.PushBack(f1);
				const Vector<Foo> constVector = vector;
				Assert::AreEqual(constVector.Front(), vector.Front());
			}

			{
				const Vector<Foo> constVector;
				auto func2 = [&constVector]() { constVector.Front(); };
				Assert::ExpectException<std::runtime_error>(func2);
			}
		}

		TEST_METHOD(Back)
		{
			Vector<Foo> vector;
			auto func = [&vector]() { vector.Back(); };
			Assert::ExpectException<std::runtime_error>(func);
			Assert::IsTrue(vector.IsEmpty());

			Foo f1{ 1 };
			Foo f2{ 2 };
			vector.PushBack(f1);
			Assert::AreEqual(f1, vector.Back());
			vector.PushBack(f2);
			Assert::AreEqual(f2, vector.Back());

			const Vector<Foo> constVector;
			auto func2 = [&constVector]() { constVector.Back(); };
			Assert::ExpectException<std::runtime_error>(func2);

			const Vector<Foo> constVector2 = vector;
			Assert::AreEqual(f2, constVector2.Back());
		}

		TEST_METHOD(Empty)
		{
			Vector<Foo> vector;
			Assert::IsTrue(vector.IsEmpty());

			Foo f{ 1 };
			vector.PushBack(f);
			Assert::IsFalse(vector.IsEmpty());
		}

		TEST_METHOD(PushBack)
		{
			Vector<Foo> vector;
			Assert::IsTrue(vector.IsEmpty());
			Foo f{ 1 };
			vector.PushBack(f);
			Assert::AreEqual(f, vector.Back());

			Foo f2{ 2 };
			vector.PushBack(f2);
			Assert::AreEqual(f2, vector.Back());
		}

		TEST_METHOD(PopBack)
		{
			Vector<Foo> vector;
			Assert::IsTrue(vector.IsEmpty());
			vector.PopBack();

			Foo f1{ 1 };
			Foo f2{ 2 };
			Foo f3{ 3 };

			vector.PushBack(f1);
			vector.PushBack(f2);
			vector.PushBack(f3);
			vector.PopBack();
			Assert::AreEqual(f2, vector.Back());

			vector.PopBack();
			Assert::AreEqual(f1, vector.Back());

			vector.PopBack();
			Assert::IsTrue(vector.IsEmpty());
		}

		TEST_METHOD(Size)
		{
			Vector<Foo> vector;
			Assert::AreEqual((size_t)0, vector.Size());

			Foo f{ 1 };
			vector.PushBack(f);
			Assert::AreEqual((size_t)1, vector.Size());

			vector.PopBack();
			Assert::AreEqual((size_t)0, vector.Size());
		}

		TEST_METHOD(Clear)
		{
			Vector<Foo> vector;
			vector.Clear();
			Assert::IsTrue(vector.IsEmpty());

			Foo f{ 1 };
			Foo f2{ 2 };
			vector.PushBack(f);
			vector.PushBack(f2);
			Assert::IsFalse(vector.IsEmpty());
			vector.Clear();
			Assert::AreEqual((size_t)0, vector.Size());
			Assert::IsTrue(vector.IsEmpty());
		}

		TEST_METHOD(CopyConstructor)
		{
			Vector<Foo> vector;

			Foo f1{ 1 };
			Foo f2{ 2 };
			Foo f3{ 3 };

			{
				vector.PushBack(f1);
				vector.PushBack(f2);
				vector.PushBack(f3);

				Vector<Foo> vectorCopy{ vector };
				Assert::AreEqual(vector.Size(), vectorCopy.Size());
				Assert::AreEqual(vector.Capacity(), vectorCopy.Capacity());
				while (!vector.IsEmpty())
				{
					Assert::AreEqual(vector.Back(), vectorCopy.Back());
					vector.PopBack();
					vectorCopy.PopBack();
				}
			}

			{
				vector.PushBack(f1);
				vector.PushBack(f2);
				vector.PushBack(f3);

				Vector<Foo> vectorCopy;
				vectorCopy = vector;
				Assert::AreEqual(vector.Size(), vectorCopy.Size());
				while (!vector.IsEmpty())
				{
					Assert::AreEqual(vector.Back(), vectorCopy.Back());
					vector.PopBack();
					vectorCopy.PopBack();
				}
			}
		}

		TEST_METHOD(BracketOperator)
		{
			Vector<Foo> vector;
			vector.Clear();
			Assert::IsTrue(vector.IsEmpty());

			Foo f{ 1 };
			Foo f2{ 2 };
			vector.PushBack(f);
			vector.PushBack(f2);
			Assert::AreEqual(f, vector[0]);
			Assert::AreEqual(f2, vector[1]);

			auto func = [&vector]() { vector[2]; };
			Assert::ExpectException<std::runtime_error>(func);

			const Vector<Foo> constVector = vector;
			Assert::AreEqual(f, constVector[0]);
			Assert::AreEqual(f2, constVector[1]);
			auto func2 = [&constVector]() { constVector[2]; };
			Assert::ExpectException<std::runtime_error>(func2);
		}

		TEST_METHOD(At)
		{
			Vector<Foo> vector;
			vector.Clear();
			Assert::IsTrue(vector.IsEmpty());

			Foo f{ 1 };
			Foo f2{ 2 };
			vector.PushBack(f);
			vector.PushBack(f2);
			Assert::AreEqual(f, vector.At(0));
			Assert::AreEqual(f2, vector.At(1));

			auto func = [&vector]() { vector.At(2); };
			Assert::ExpectException<std::runtime_error>(func);

			const Vector<Foo> constVector = vector;
			Assert::AreEqual(f, constVector.At(0));
			Assert::AreEqual(f2, constVector.At(1));
			auto func2 = [&constVector]() { constVector.At(2); };
			Assert::ExpectException<std::runtime_error>(func2);
		}

		TEST_METHOD(Remove)
		{
			Vector<Foo> vector;
			vector.Clear();
			Assert::IsTrue(vector.IsEmpty());

			Foo f1{ 1 };
			Foo f2{ 2 };
			Foo f3{ 3 };
			vector.PushBack(f1);
			vector.PushBack(f2);
			vector.PushBack(f3);
			vector.Remove(f2);
			Assert::AreEqual(size_t(2), vector.Size());
			vector.Remove(f3);
			Assert::AreEqual(vector.Front(), f1);
			Assert::AreEqual(size_t(1), vector.Size());
			vector.Remove(f3);
			Assert::AreEqual(size_t(1), vector.Size());
		}

		TEST_METHOD(Iterator)
		{
			{
				Vector<Foo> vector;
				Assert::AreEqual(vector.end(), vector.begin());

				Foo a(1);
				vector.PushBack(a);
				Assert::AreNotEqual(vector.end(), vector.begin());
			}

			Foo a(1);
			Foo b(2);
			Foo c(3);
			Vector<Foo> vector;
			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			{
				Assert::AreEqual(vector.Front(), a);
				size_t count = 0;
				for (Vector<Foo>::Iterator it = vector.begin(); it != vector.end(); ++it, ++count)
				{
					;
				}
				Assert::AreEqual(vector.Size(), count);

				count = 0;
				for (auto it = vector.begin(); it != vector.end(); it++, count++)
				{
					;
				}
				Assert::AreEqual(vector.Size(), count);


				count = 0;
				for (const Foo& value : vector)
				{
					value;
					count++;
				}
				Assert::AreEqual(vector.Size(), count);
			}

			{
				Vector<Foo> constVector;
				Assert::AreEqual(constVector.end(), constVector.begin());

				constVector.PushBack(a);
				Assert::AreNotEqual(constVector.end(), constVector.begin());
			}

			{
				Assert::AreEqual(vector.Front(), a);
				size_t count = 0;
				for (Vector<Foo>::ConstIterator constIt = vector.cbegin(); constIt != vector.cend(); ++constIt, ++count)
				{
					;
				}
				Assert::AreEqual(vector.Size(), count);
				
				count = 0;
				for (Vector<Foo>::ConstIterator constIt = vector.cbegin(); constIt != vector.cend(); constIt++, ++count)
				{
					;
				}
				Assert::AreEqual(vector.Size(), count);

				count = 0;
				for (const Foo& value : vector)
				{
					value;
					count++;
				}
				Assert::AreEqual(vector.Size(), count);
			}

			{
				const Vector<Foo> constVector = vector;
			
				Assert::AreEqual(constVector.Front(), a);
				size_t count = 0;
				for (Vector<Foo>::ConstIterator it = constVector.begin(); it != constVector.end(); ++it, ++count)
				{
					;
				}
				Assert::AreEqual(constVector.Size(), count);

				count = 0;
				for (Vector<Foo>::ConstIterator it = constVector.begin(); it != constVector.end(); it++, ++count)
				{
					;
				}
				Assert::AreEqual(constVector.Size(), count);


				count = 0;
				for (const Foo& value : constVector)
				{
					value;
					count++;
				}
				Assert::AreEqual(constVector.Size(), count);
			}

			{
				const Vector<Foo>::Iterator it = vector.begin();
				Assert::AreEqual(it, vector.begin());

				Vector<Foo>::Iterator anotherIt;
				anotherIt = it;
				Assert::AreEqual(it, anotherIt);

				Vector<Foo>::ConstIterator constIt;
				constIt = it;
				Assert::AreEqual(*constIt,a);

				Vector<Foo>::ConstIterator constIt2(it);
				Assert::AreEqual(*constIt2, a);
			}
		}

		TEST_METHOD(MoveSegmantics)
		{
			Foo a(1);
			Foo b(2);
			Foo c(3);
			Vector<Foo> vector;
			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);

			{
				Vector<Foo> anotherVector = std::move(vector);

				Assert::AreEqual(size_t(3), anotherVector.Size());
				Assert::AreEqual(a, anotherVector.Front());
				Assert::AreEqual(c, anotherVector.Back());
			}

			{
				Vector<Foo> anotherVector;
				vector.PushBack(c);
				vector.PushBack(b);
				vector.PushBack(a);
				anotherVector = std::move(vector);

				Assert::AreEqual(size_t(3), anotherVector.Size());
				Assert::AreEqual(c, anotherVector.Front());
				Assert::AreEqual(a, anotherVector.Back());
			}
		}

		TEST_METHOD(Find)
		{
			Foo a(1);
			Foo b(2);
			Foo c(3);
			Vector<Foo> vector;
			Vector<Foo>::Iterator it = vector.Find(a);
			Assert::AreEqual(it, vector.end());

			vector.PushBack(a);
			vector.PushBack(b);
			vector.PushBack(c);
			it = vector.Find(a);
			Assert::AreEqual(*it, a);

			const Vector<Foo> constVector = vector;
			Assert::AreEqual(*constVector.Find(a), a);
		}

		TEST_METHOD(IteratorIncrementFail)
		{
			auto func = []() { Vector<Foo>::Iterator it; ++it; };
			Assert::ExpectException<std::runtime_error>(func);
		}

		TEST_METHOD(ConstIteratorIncrementFail)
		{
			auto func = []() { Vector<Foo>::ConstIterator it; ++it; };
			Assert::ExpectException<std::runtime_error>(func);
		}

		TEST_METHOD(ShrinkToFit)
		{
			Foo a(1);
			Foo b(2);
			Foo c(3);
			Vector<Foo> vector{a,b,c};

			Assert::AreEqual(size_t(4), vector.Capacity());
			Assert::AreEqual(size_t(3), vector.Size());
			vector.ShrinkToFit();
			Assert::AreEqual(size_t(3), vector.Capacity());
			Assert::AreEqual(size_t(3), vector.Size());
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState VectorTests::_startMemState;
}