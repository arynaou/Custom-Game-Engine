#include "pch.h"
#include "Foo.h"
#include "Datum.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTests
{
	TEST_CLASS(DatumTests)
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
				Datum datum;
				Assert::AreEqual((size_t)0, datum.Size());
				Assert::AreEqual((size_t)0, datum.Capacity());
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());
			}

			{
				Datum datum(DatumTypes::Vectors);
				Assert::AreEqual((size_t)0, datum.Size());
				Assert::AreEqual((size_t)0, datum.Capacity());
				Assert::AreEqual(DatumTypes::Vectors, datum.Type());
			}

			{
				const std::size_t capacity = 5;
				Datum datum(DatumTypes::Vectors, 5);
				Assert::AreEqual((size_t)0, datum.Size());
				Assert::AreEqual((size_t)5, datum.Capacity());
				Assert::AreEqual(DatumTypes::Vectors, datum.Type());
			}
		}

		TEST_METHOD(TypeCastConstructor)
		{
			{
				const float a = 1.0f;
				Datum datum = a;
				Datum copyDatum = datum;
				Assert::AreEqual(datum.Size(), copyDatum.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum.Capacity());
				Assert::AreEqual(DatumTypes::Float, copyDatum.Type());
				Assert::AreEqual(a, copyDatum.Get<float>());
			}

			{
				const float a = 1.0f;
				Datum datum = a;
				Datum copyDatum;
				copyDatum = datum;
				Assert::AreEqual(datum.Size(), copyDatum.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum.Capacity());
				Assert::AreEqual(DatumTypes::Float, copyDatum.Type());
				Assert::AreEqual(a, copyDatum.Get<float>());
			}

			{
				const std::string a = "A";
				Datum datum = a;
				Datum copyDatum;
				copyDatum = datum;
				Assert::AreEqual(datum.Size(), copyDatum.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum.Capacity());
				Assert::AreEqual(DatumTypes::String, copyDatum.Type());
				Assert::AreEqual(a, copyDatum.Get<std::string>());
			}

			{
				const std::string a = "A";
				Datum datum = a;
				Datum copyDatum = datum;
				Assert::AreEqual(datum.Size(), copyDatum.Size());
				Assert::AreEqual(datum.Capacity(), copyDatum.Capacity());
				Assert::AreEqual(DatumTypes::String, copyDatum.Type());
				Assert::AreEqual(a, copyDatum.Get<std::string>());
			}

			{
				const std::int32_t a = 1;
				Datum datum = a;
				Assert::AreEqual((size_t)1, datum.Size());
				Assert::AreEqual((size_t)1, datum.Capacity());
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
				Assert::AreEqual(a, datum.Get<std::int32_t>());

				Assert::ExpectException<std::runtime_error>([&datum] {datum.Get<std::int32_t>(5); });
			}

			{
				const float a = 1.0f;
				Datum datum = a;
				Assert::AreEqual((size_t)1, datum.Size());
				Assert::AreEqual((size_t)1, datum.Capacity());
				Assert::AreEqual(DatumTypes::Float, datum.Type());
				Assert::AreEqual(a, datum.Get<float>());

				Assert::ExpectException<std::runtime_error>([&datum] {datum.Get<float>(5); });
			}

			{
				const glm::vec4 a{1.0f, 1.0f, 1.0f, 1.0f};
				Datum datum = a;
				Assert::AreEqual((size_t)1, datum.Size());
				Assert::AreEqual((size_t)1, datum.Capacity());
				Assert::AreEqual(DatumTypes::Vectors, datum.Type());
				Assert::AreEqual(a, datum.Get<glm::vec4>());

				Assert::ExpectException<std::runtime_error>([&datum] {datum.Get<glm::vec4>(5); });
			}

			{
				const glm::mat4 a{ 1.0f};
				Datum datum = a;
				Assert::AreEqual((size_t)1, datum.Size());
				Assert::AreEqual((size_t)1, datum.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, datum.Type());
				Assert::AreEqual(a, datum.Get<glm::mat4>());

				Assert::ExpectException<std::runtime_error>([&datum] {datum.Get<glm::mat4>(5); });
			}

			{
				const std::string a("ABC");
				Datum datum = a;
				Assert::AreEqual((size_t)1, datum.Size());
				Assert::AreEqual((size_t)1, datum.Capacity());
				Assert::AreEqual(DatumTypes::String, datum.Type());
				Assert::AreEqual(a, datum.Get<std::string>());

				Assert::ExpectException<std::runtime_error>([&datum] {datum.Get<std::string>(5); });
			}

			{
				Foo aFoo{ 1 };
				RTTI* a = &aFoo;
				Datum datum = a;
				Assert::AreEqual((size_t)1, datum.Size());
				Assert::AreEqual((size_t)1, datum.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, datum.Type());
				Assert::AreEqual(a, datum.Get<RTTI*>());

				Assert::ExpectException<std::runtime_error>([&datum] {datum.Get<RTTI*>(5); });
			}

			Assert::ExpectException<std::runtime_error>([] {Datum datum(DatumTypes::Unknown, 1); });
		}

		TEST_METHOD(SetType)
		{
			Datum datum;
			Assert::AreEqual(DatumTypes::Unknown, datum.Type());
			datum.SetType(DatumTypes::Integer);
			Assert::AreEqual(DatumTypes::Integer, datum.Type());
			Assert::ExpectException<std::runtime_error>([&datum] {datum.SetType(DatumTypes::Float); });
			Assert::ExpectException<std::runtime_error>([&datum] {datum.SetType(DatumTypes::Unknown); });
		}

		TEST_METHOD(InternalIntegers)
		{
			int a = 10;
			int b = 20;

			Datum datum(DatumTypes::Integer, 1);
			Assert::AreEqual(std::size_t(1), datum.Capacity());
			Assert::AreEqual(std::size_t(0), datum.Size());
			Assert::AreEqual(DatumTypes::Integer, datum.Type());
			Assert::IsTrue(datum.IsInternal());

			datum = a;
			Assert::AreEqual(std::size_t(1), datum.Capacity());
			Assert::AreEqual(std::size_t(1), datum.Size());
			Assert::AreEqual(a, datum.Get<int>());

			datum.Resize(3);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(3), datum.Size());
			Assert::AreEqual(a, datum.Get<int>(0));

			datum.Set(b, 1);
			Assert::AreEqual(a, datum.Get<int>(0));
			Assert::AreEqual(b, datum.Get<int>(1));
			
			Assert::AreEqual(datum.Find(b),std::size_t(1));

			datum.Remove(b);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(2), datum.Size());
			Assert::AreEqual(datum.Find(b),datum.Size());

			datum.Set(b, 1);
			datum.RemoveAt(1);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(1), datum.Size());
			Assert::AreEqual(datum.Find(b), datum.Size());
		}

		TEST_METHOD(InternalFloat)
		{
			float a = 10.f;
			float b = 20.f;

			Datum datum(DatumTypes::Float, 1);
			Assert::AreEqual(std::size_t(1), datum.Capacity());
			Assert::AreEqual(std::size_t(0), datum.Size());
			Assert::AreEqual(DatumTypes::Float , datum.Type());
			Assert::IsTrue(datum.IsInternal());

			datum = a;
			Assert::AreEqual(std::size_t(1), datum.Capacity());
			Assert::AreEqual(std::size_t(1), datum.Size());
			Assert::AreEqual(a, datum.Get<float>());

			datum.Resize(3);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(3), datum.Size());
			Assert::AreEqual(a, datum.Get<float>(0));

			datum.Set(b, 1);
			Assert::AreEqual(a, datum.Get<float>(0));
			Assert::AreEqual(b, datum.Get<float>(1));

			Assert::AreEqual(datum.Find(b), std::size_t(1));

			datum.Remove(b);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(2), datum.Size());
			Assert::AreEqual(datum.Find(b), datum.Size());

			datum.Set(b, 1);
			datum.RemoveAt(1);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(1), datum.Size());
			Assert::AreEqual(datum.Find(b), datum.Size());
		}

		TEST_METHOD(InternalVector)
		{
			glm::vec4 a{10.f};
			glm::vec4 b{20.f};

			Datum datum(DatumTypes::Vectors, 1);
			Assert::AreEqual(std::size_t(1), datum.Capacity());
			Assert::AreEqual(std::size_t(0), datum.Size());
			Assert::AreEqual(DatumTypes::Vectors, datum.Type());
			Assert::IsTrue(datum.IsInternal());

			datum = a;
			Assert::AreEqual(std::size_t(1), datum.Capacity());
			Assert::AreEqual(std::size_t(1), datum.Size());
			Assert::AreEqual(a, datum.Get<glm::vec4>());

			datum.Resize(3);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(3), datum.Size());
			Assert::AreEqual(a, datum.Get<glm::vec4>(0));

			datum.Set(b, 1);
			Assert::AreEqual(a, datum.Get<glm::vec4>(0));
			Assert::AreEqual(b, datum.Get<glm::vec4>(1));

			Assert::AreEqual(datum.Find(b), std::size_t(1));

			datum.Remove(b);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(2), datum.Size());
			Assert::AreEqual(datum.Find(b), datum.Size());

			datum.Set(b, 1);
			datum.RemoveAt(1);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(1), datum.Size());
			Assert::AreEqual(datum.Find(b), datum.Size());
		}

		TEST_METHOD(InternalMatrix)
		{
			glm::mat4 a{ 10.f };
			glm::mat4 b{ 20.f };

			Datum datum(DatumTypes::Matrix, 1);
			Assert::AreEqual(std::size_t(1), datum.Capacity());
			Assert::AreEqual(std::size_t(0), datum.Size());
			Assert::AreEqual(DatumTypes::Matrix, datum.Type());
			Assert::IsTrue(datum.IsInternal());

			datum = a;
			Assert::AreEqual(std::size_t(1), datum.Capacity());
			Assert::AreEqual(std::size_t(1), datum.Size());
			Assert::AreEqual(a, datum.Get<glm::mat4>());

			datum.Resize(3);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(3), datum.Size());
			Assert::AreEqual(a, datum.Get<glm::mat4>(0));

			datum.Set(b, 1);
			Assert::AreEqual(a, datum.Get<glm::mat4>(0));
			Assert::AreEqual(b, datum.Get<glm::mat4>(1));

			Assert::AreEqual(datum.Find(b), std::size_t(1));

			datum.Remove(b);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(2), datum.Size());
			Assert::AreEqual(datum.Find(b), datum.Size());

			datum.Set(b, 1);
			datum.RemoveAt(1);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(1), datum.Size());
			Assert::AreEqual(datum.Find(b), datum.Size());
		}

		TEST_METHOD(InternalString)
		{
			std::string a="A";
			std::string b = "B";

			Datum datum(DatumTypes::String, 1);
			Assert::AreEqual(std::size_t(1), datum.Capacity());
			Assert::AreEqual(std::size_t(0), datum.Size());
			Assert::AreEqual(DatumTypes::String, datum.Type());
			Assert::IsTrue(datum.IsInternal());

			datum = a;
			Assert::AreEqual(std::size_t(1), datum.Capacity());
			Assert::AreEqual(std::size_t(1), datum.Size());
			Assert::AreEqual(a, datum.Get<std::string>());

			datum.Resize(3);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(3), datum.Size());
			Assert::AreEqual(a, datum.Get<std::string>(0));

			datum.Set(b, 1);
			Assert::AreEqual(a, datum.Get<std::string>(0));
			Assert::AreEqual(b, datum.Get<std::string>(1));

			Assert::AreEqual(datum.Find(b), std::size_t(1));

			datum.Remove(b);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(2), datum.Size());
			Assert::AreEqual(datum.Find(b), datum.Size());

			datum.Set(b, 1);
			datum.RemoveAt(1);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(1), datum.Size());
			Assert::AreEqual(datum.Find(b), datum.Size());
		}

		TEST_METHOD(InternalPointer)
		{
			Foo aFoo{ 1 };
			Foo bFoo{ 2 };
			RTTI* a = &aFoo;
			RTTI* b = &bFoo;

			Datum datum(DatumTypes::Pointer, 1);
			Assert::AreEqual(std::size_t(1), datum.Capacity());
			Assert::AreEqual(std::size_t(0), datum.Size());
			Assert::AreEqual(DatumTypes::Pointer, datum.Type());
			Assert::IsTrue(datum.IsInternal());

			datum = a;
			Assert::AreEqual(std::size_t(1), datum.Capacity());
			Assert::AreEqual(std::size_t(1), datum.Size());
			Assert::AreEqual(a, datum.Get<RTTI*>());

			datum.Resize(3);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(3), datum.Size());
			Assert::AreEqual(a, datum.Get<RTTI*>(0));

			datum.Set(b, 1);
			Assert::AreEqual(a, datum.Get<RTTI*>(0));
			Assert::AreEqual(b, datum.Get<RTTI*>(1));

			Assert::AreEqual(datum.Find(b), std::size_t(1));

			datum.Remove(b);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(2), datum.Size());
			Assert::AreEqual(datum.Find(b), datum.Size());

			datum.Set(b, 1);
			datum.RemoveAt(1);
			Assert::AreEqual(std::size_t(3), datum.Capacity());
			Assert::AreEqual(std::size_t(1), datum.Size());
			Assert::AreEqual(datum.Find(b), datum.Size());
		}

		TEST_METHOD(Reserve)
		{
			{
				Datum datum;
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(0), datum.Capacity());

				Assert::ExpectException<std::runtime_error>([&datum] {datum.Reserve(5); });

				datum.SetType(DatumTypes::Integer);
				datum.Reserve(5);
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());
			
				datum.Reserve(3);
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());
			}

			{
				Datum datum;
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(0), datum.Capacity());

				datum.SetType(DatumTypes::Float);
				datum.Reserve(5);
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());

				datum.Reserve(3);
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());
			}

			{
				Datum datum;
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(0), datum.Capacity());

				datum.SetType(DatumTypes::Vectors);
				datum.Reserve(5);
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());

				datum.Reserve(3);
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());
			}

			{
				Datum datum;
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(0), datum.Capacity());

				datum.SetType(DatumTypes::Matrix);
				datum.Reserve(5);
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());

				datum.Reserve(3);
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());
			}

			{
				Datum datum;
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(0), datum.Capacity());

				datum.SetType(DatumTypes::String);
				datum.Reserve(5);
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());

				datum.Reserve(3);
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());
			}

			{
				Datum datum;
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(0), datum.Capacity());

				datum.SetType(DatumTypes::Pointer);
				datum.Reserve(5);
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());

				datum.Reserve(3);
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());
			}
		}

		TEST_METHOD(Resize)
		{
			{
				Datum datum;
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(0), datum.Capacity());

				Assert::ExpectException<std::runtime_error>([&datum] {datum.Resize(5); });

				datum.SetType(DatumTypes::Integer);
				datum.Resize(5);
				Assert::AreEqual(std::size_t(5), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());

				datum.Resize(3);
				Assert::AreEqual(std::size_t(3), datum.Size());
				Assert::AreEqual(std::size_t(3), datum.Capacity());
			}

			{
				Datum datum;
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(0), datum.Capacity());

				datum.SetType(DatumTypes::Float);
				datum.Resize(5);
				Assert::AreEqual(std::size_t(5), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());

				datum.Resize(3);
				Assert::AreEqual(std::size_t(3), datum.Size());
				Assert::AreEqual(std::size_t(3), datum.Capacity());
			}

			{
				Datum datum;
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(0), datum.Capacity());

				datum.SetType(DatumTypes::Vectors);
				datum.Resize(5);
				Assert::AreEqual(std::size_t(5), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());

				datum.Resize(3);
				Assert::AreEqual(std::size_t(3), datum.Size());
				Assert::AreEqual(std::size_t(3), datum.Capacity());
			}

			{
				Datum datum;
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(0), datum.Capacity());

				datum.SetType(DatumTypes::Matrix);
				datum.Resize(5);
				Assert::AreEqual(std::size_t(5), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());

				datum.Resize(3);
				Assert::AreEqual(std::size_t(3), datum.Size());
				Assert::AreEqual(std::size_t(3), datum.Capacity());
			}

			{
				Datum datum;
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(0), datum.Capacity());

				datum.SetType(DatumTypes::String);
				datum.Resize(5);
				Assert::AreEqual(std::size_t(5), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());

				datum.Resize(3);
				Assert::AreEqual(std::size_t(3), datum.Size());
				Assert::AreEqual(std::size_t(3), datum.Capacity());
			}

			{
				Datum datum;
				Assert::AreEqual(std::size_t(0), datum.Size());
				Assert::AreEqual(std::size_t(0), datum.Capacity());

				datum.SetType(DatumTypes::Pointer);
				datum.Resize(5);
				Assert::AreEqual(std::size_t(5), datum.Size());
				Assert::AreEqual(std::size_t(5), datum.Capacity());

				datum.Resize(3);
				Assert::AreEqual(std::size_t(3), datum.Size());
				Assert::AreEqual(std::size_t(3), datum.Capacity());
			}
		}

		TEST_METHOD(ShrinkToFit)
		{
			{
				std::string a = "A"s;
				std::string b = "B"s;
				std::string c = "C"s;

				Datum datum;

				//Assert::ExpectException<std::runtime_error>([&datum] {datum.ShrinkToFit(); });
				datum.PushBack(a);
				Assert::AreEqual(std::size_t(1), datum.Size());
				Assert::AreEqual(std::size_t(1), datum.Capacity());

				datum.PushBack(b);
				Assert::AreEqual(std::size_t(2), datum.Size());
				Assert::AreEqual(std::size_t(2), datum.Capacity());

				datum.PushBack(c);
				Assert::AreEqual(std::size_t(3), datum.Size());
				Assert::AreEqual(std::size_t(4), datum.Capacity());

				datum.PopBack();
				Assert::AreEqual(std::size_t(2), datum.Size());
				Assert::AreEqual(std::size_t(4), datum.Capacity());

				datum.PopBack();
				Assert::AreEqual(std::size_t(1), datum.Size());
				Assert::AreEqual(std::size_t(4), datum.Capacity());

				datum.ShrinkToFit();
				Assert::AreEqual(std::size_t(1), datum.Size());
				Assert::AreEqual(std::size_t(1), datum.Capacity());
			}
		}

		TEST_METHOD(ToString)
		{
			{
				const std::int32_t a = 1;
				Datum datum = a;
				std::string s = "1";
				Assert::AreEqual(datum.ToString<std::int32_t>(0), s);
			}

			{
				const float a = 1.f;
				Datum datum = a;
				std::string s = "1.000000";
				Assert::AreEqual(datum.ToString<float>(0), s);
			}

			{
				const glm::vec4 a{ 1.0f, 1.0f, 1.0f, 1.0f };
				Datum datum = a;
				std::string s = "vec4(1.000000,1.000000,1.000000,1.000000)";
				Assert::AreEqual(datum.ToString<glm::vec4>(0), s);
			}

			{
				const glm::mat4 a{1.0f};
				Datum datum = a;
				std::string s = "mat4x4(1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000)";
				Assert::AreEqual(datum.ToString<glm::mat4>(0), s);
			}

			{
				const std::string a("ABC");
				Datum datum = a;
				Assert::AreEqual(datum.ToString<std::string>(0), a);
			}

			{
				Foo aFoo{ 1 };
				RTTI* a = &aFoo;
				Datum datum = a;
				Assert::AreEqual(datum.ToString<RTTI>(0), a->ToString());
			}
		}

		TEST_METHOD(EqualOperator)
		{
			{
				const std::int32_t a = 1;
				Datum datum1 = a;
				
				Assert::IsFalse((datum1 == "A"));
				Assert::IsFalse((datum1 == 0.3f));
				Assert::IsFalse((datum1 == glm::vec4{ 0.1f }));
				Assert::IsFalse((datum1 == glm::mat4{ 0.1f }));
				Foo aFoo{ 1 };
				RTTI* b = &aFoo;
				Assert::IsFalse((datum1 == b));

				const std::string c= "A";
				Datum datum2 = c;
				Assert::IsFalse((datum2 == 1));

			}
			{
				const std::int32_t a = 1;
				const std::int32_t b = 2;

				Datum datum1 = a;
				Datum datum2 = b;
				Assert::IsTrue(datum1 != datum2);
				Assert::IsTrue(datum2 != a);

				datum2 = a;
				Assert::IsTrue(datum1 == datum2);
				Assert::IsTrue(datum2 == a);

				std::string c{ "AB" };
				Datum anotherDatum = c;
				Assert::IsFalse(anotherDatum == datum1);

				datum1.Resize(3);
				Assert::ExpectException<std::runtime_error>([&datum1, &a] {(datum1 == a); });
			}

			{
				const float a = 1.f;
				const float b = 2.f;

				Datum datum1 = a;
				Datum datum2 = b;
				Assert::IsTrue(datum1 != datum2);
				Assert::IsTrue(datum2 != a);

				datum2 = a;
				Assert::IsTrue(datum1 == datum2);
				Assert::IsTrue(datum2 == a);

				
				datum1.Resize(3);
				Assert::ExpectException<std::runtime_error>([&datum1, &a] {(datum1 == a); });
			}

			{
				const glm::vec4 a{1.0f};
				const glm::vec4 b{2.0f};

				Datum datum1 = a;
				Datum datum2 = b;
				Assert::IsTrue(datum1 != datum2);
				Assert::IsTrue(datum2 != a);

				datum2 = a;
				Assert::IsTrue(datum1 == datum2);
				Assert::IsTrue(datum2 == a);

				datum1.Resize(3);
				Assert::ExpectException<std::runtime_error>([&datum1, &a] {(datum1 == a); });
			}

			{
				const glm::mat4 a{ 1.0f };
				const glm::mat4 b{ 2.0f };

				Datum datum1 = a;
				Datum datum2 = b;
				Assert::IsTrue(datum1 != datum2);
				Assert::IsTrue(datum2 != a);

				datum2 = a;
				Assert::IsTrue(datum1 == datum2);
				Assert::IsTrue(datum2 == a);

				datum1.Resize(3);
				Assert::ExpectException<std::runtime_error>([&datum1, &a] {(datum1 == a); });
			}

			{
				const std::string a = "ABC";
				const std::string b = "QWE";

				Datum datum1 = a;
				Datum datum2 = b;
				Assert::IsTrue(datum1 != datum2);
				Assert::IsTrue(datum2 != a);

				datum2 = a;
				Assert::IsTrue(datum1 == datum2);
				Assert::IsTrue(datum2 == a);

				datum1.Resize(3);
				Assert::ExpectException<std::runtime_error>([&datum1, &a] {(datum1 == a); });
			}

			{
				Foo aFoo{ 1 };
				Foo bFoo{ 2 };
				RTTI* a = &aFoo;
				RTTI* b = &bFoo;
				Datum datum1 = a;
				Datum datum2 = b;
				Assert::IsTrue(datum1 != datum2);
				Assert::IsTrue(datum2 != a);

				datum2 = a;
				Assert::IsTrue(datum2 == a);
				Assert::IsTrue(datum2 == datum1);

				datum1.Resize(3);
				Assert::ExpectException<std::runtime_error>([&datum1, &a] {(datum1 == a); });
			}
		}

		TEST_METHOD(Get)
		{
			{
				const std::int32_t a = 1;
				const Datum datum = a;
				Assert::AreEqual(datum.Get<std::int32_t>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<std::int32_t>(5); });
			}

			{
				std::int32_t a = 1;
				Datum datum = a;
				Assert::AreEqual(datum.Get<std::int32_t>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<std::int32_t>(5); });
			}

			{
				const float a = 1.f;
				const Datum datum = a;
				Assert::AreEqual(datum.Get<float>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<float>(5); });
			}

			{
				float a = 1.f;
				Datum datum = a;
				Assert::AreEqual(datum.Get<float>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<float>(5); });
			}

			{
				const glm::vec4 a{0.1f};
				const Datum datum = a;
				Assert::AreEqual(datum.Get<glm::vec4>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<glm::vec4>(5); });
			}

			{
				glm::vec4 a{ 0.1f };
				Datum datum = a;
				Assert::AreEqual(datum.Get<glm::vec4>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<glm::vec4>(5); });
			}

			{
				const glm::mat4 a{ 0.1f };
				const Datum datum = a;
				Assert::AreEqual(datum.Get<glm::mat4>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<glm::mat4>(5); });
			}

			{
				glm::mat4 a{ 0.1f };
				Datum datum = a;
				Assert::AreEqual(datum.Get<glm::mat4>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<glm::mat4>(5); });
			}

			{
				const std::string a= "ABC";
				const Datum datum = a;
				Assert::AreEqual(datum.Get<std::string>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<std::string>(5); });
			}

			{
				std::string a = "ABC";
				Datum datum = a;
				Assert::AreEqual(datum.Get<std::string>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<std::string>(5); });
			}

			{
				Foo aFoo{ 1 };
				RTTI* a = &aFoo;
				Datum datum = a; 
				Assert::AreEqual(datum.Get<RTTI*>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<RTTI*>(5); });
			}

			{
				Foo aFoo{ 1 };
				RTTI* a = &aFoo;
				const Datum datum = a;
				Assert::AreEqual(datum.Get<RTTI*>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<RTTI*>(5); });
			}
		}

		TEST_METHOD(Set)
		{
			{
				std::int32_t a = 1;
				Datum datum = a;
				Assert::AreEqual(datum.Get<std::int32_t>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<std::int32_t>(5); });
			}

			{
				const float a = 1.f;
				const Datum datum = a;
				Assert::AreEqual(datum.Get<float>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<float>(5); });
			}

			{
				float a = 1.f;
				Datum datum = a;
				Assert::AreEqual(datum.Get<float>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<float>(5); });
			}

			{
				const glm::vec4 a{ 0.1f };
				const Datum datum = a;
				Assert::AreEqual(datum.Get<glm::vec4>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<glm::vec4>(5); });
			}

			{
				glm::vec4 a{ 0.1f };
				Datum datum = a;
				Assert::AreEqual(datum.Get<glm::vec4>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<glm::vec4>(5); });
			}

			{
				const glm::mat4 a{ 0.1f };
				const Datum datum = a;
				Assert::AreEqual(datum.Get<glm::mat4>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<glm::mat4>(5); });
			}

			{
				glm::mat4 a{ 0.1f };
				Datum datum = a;
				Assert::AreEqual(datum.Get<glm::mat4>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<glm::mat4>(5); });
			}

			{
				const std::string a = "ABC";
				const Datum datum = a;
				Assert::AreEqual(datum.Get<std::string>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<std::string>(5); });
			}

			{
				std::string a = "ABC";
				Datum datum = a;
				Assert::AreEqual(datum.Get<std::string>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<std::string>(5); });
			}

			{
				Foo aFoo{ 1 };
				RTTI* a = &aFoo;
				Datum datum = a;
				Assert::AreEqual(datum.Get<RTTI*>(), a);
				Assert::ExpectException<std::runtime_error>([datum] {datum.Get<RTTI*>(5); });
			}
		}

		TEST_METHOD(Front)
		{
			Datum emptyDatum;
			const Datum emptyConstDatum;

			{
				Assert::ExpectException<std::runtime_error>([&emptyDatum] {emptyDatum.Front<std::int32_t>(); });
				Assert::ExpectException<std::runtime_error>([&emptyConstDatum] {emptyConstDatum.Front<std::int32_t>(); });
				const std::int32_t a = 1;

				Datum datum = a;
				Assert::AreEqual(a, datum.Front<int32_t>());

				const Datum constDatum = a;
				Assert::AreEqual(a, constDatum.Front<int32_t>());
			}

			{
				Assert::ExpectException<std::runtime_error>([&emptyDatum] {emptyDatum.Front<float>(); });
				Assert::ExpectException<std::runtime_error>([&emptyConstDatum] {emptyConstDatum.Front<float>(); });
				const float a = 1;

				Datum datum = a;
				Assert::AreEqual(a, datum.Front<float>());

				const Datum constDatum = a;
				Assert::AreEqual(a, constDatum.Front<float>());
			}

			{
				Assert::ExpectException<std::runtime_error>([&emptyDatum] {emptyDatum.Front<glm::vec4>(); });
				Assert::ExpectException<std::runtime_error>([&emptyConstDatum] {emptyConstDatum.Front<glm::vec4>(); });
				const glm::vec4 a{0.1f};

				Datum datum = a;
				Assert::AreEqual(a, datum.Front<glm::vec4>());

				const Datum constDatum = a;
				Assert::AreEqual(a, constDatum.Front<glm::vec4>());
			}

			{
				Assert::ExpectException<std::runtime_error>([&emptyDatum] {emptyDatum.Front<glm::mat4>(); });
				Assert::ExpectException<std::runtime_error>([&emptyConstDatum] {emptyConstDatum.Front<glm::mat4>(); });
				const glm::mat4 a{ 0.1f };

				Datum datum = a;
				Assert::AreEqual(a, datum.Front<glm::mat4>());

				const Datum constDatum = a;
				Assert::AreEqual(a, constDatum.Front<glm::mat4>());
			}

			{
				Assert::ExpectException<std::runtime_error>([&emptyDatum] {emptyDatum.Front<RTTI*>(); });
				Assert::ExpectException<std::runtime_error>([&emptyConstDatum] {emptyConstDatum.Front<RTTI*>(); });
				Foo aFoo{ 1 };
				RTTI* a = &aFoo;

				Datum datum = a;
				Assert::AreEqual(a, datum.Front<RTTI*>());

				const Datum constDatum = a;
				Assert::AreEqual(a, constDatum.Front<RTTI*>());
			}

			{
				Assert::ExpectException<std::runtime_error>([&emptyDatum] {emptyDatum.Front<std::string>(); });
				Assert::ExpectException<std::runtime_error>([&emptyConstDatum] {emptyConstDatum.Front<std::string>(); });
				const std::string a = "A";

				Datum datum = a;
				Assert::AreEqual(a, datum.Front<std::string>());

				const Datum constDatum = a;
				Assert::AreEqual(a, constDatum.Front<std::string>());
			}
		}

		TEST_METHOD(Back)
		{
			Datum emptyDatum;
			const Datum emptyConstDatum;

			{
				Assert::ExpectException<std::runtime_error>([&emptyDatum] {emptyDatum.Back<std::int32_t>(); });
				Assert::ExpectException<std::runtime_error>([&emptyConstDatum] {emptyConstDatum.Back<std::int32_t>(); });

				const std::int32_t a = 1;
				const std::int32_t b = 2;

				Datum datum = a;
				datum.PushBack(b);
				Assert::AreEqual(b, datum.Back<int32_t>());

				const Datum constDatum = a;
				Assert::AreEqual(a, constDatum.Back<int32_t>());
			}

			{
				Assert::ExpectException<std::runtime_error>([&emptyDatum] {emptyDatum.Back<float>(); });
				Assert::ExpectException<std::runtime_error>([&emptyConstDatum] {emptyConstDatum.Back<float>(); });

				const float a = 1;
				const float b = 2;

				Datum datum = a;
				datum.PushBack(b);
				Assert::AreEqual(b, datum.Back<float>());

				const Datum constDatum = a;
				Assert::AreEqual(a, constDatum.Back<float>());
			}

			{
				Assert::ExpectException<std::runtime_error>([&emptyDatum] {emptyDatum.Back<glm::vec4>(); });
				Assert::ExpectException<std::runtime_error>([&emptyConstDatum] {emptyConstDatum.Back<glm::vec4>(); });

				const glm::vec4 a{1.0f};
				const glm::vec4 b{2.0f};

				Datum datum = a;
				datum.PushBack(b);
				Assert::AreEqual(b, datum.Back<glm::vec4>());

				const Datum constDatum = a;
				Assert::AreEqual(a, constDatum.Back<glm::vec4>());
			}

			{
				Assert::ExpectException<std::runtime_error>([&emptyDatum] {emptyDatum.Back<glm::mat4>(); });
				Assert::ExpectException<std::runtime_error>([&emptyConstDatum] {emptyConstDatum.Back<glm::mat4>(); });

				const glm::mat4 a{ 1.0f };
				const glm::mat4 b{ 2.0f };

				Datum datum = a;
				datum.PushBack(b);
				Assert::AreEqual(b, datum.Back<glm::mat4>());

				const Datum constDatum = a;
				Assert::AreEqual(a, constDatum.Back<glm::mat4>());
			}

			{
				Assert::ExpectException<std::runtime_error>([&emptyDatum] {emptyDatum.Back<std::string>(); });
				Assert::ExpectException<std::runtime_error>([&emptyConstDatum] {emptyConstDatum.Back<std::string>(); });

				const std::string a="A";
				const std::string b="B";

				Datum datum = a;
				datum.PushBack(b);
				Assert::AreEqual(b, datum.Back<std::string>());


				const Datum constDatum = a;
				Assert::AreEqual(a, constDatum.Back<std::string>());
			}

			{
				Assert::ExpectException<std::runtime_error>([&emptyDatum] {emptyDatum.Back<RTTI*>(); });
				Assert::ExpectException<std::runtime_error>([&emptyConstDatum] {emptyConstDatum.Back<RTTI*>(); });

				Foo aFoo{ 1 };
				Foo bFoo{ 2 };
				RTTI* a = &aFoo;
				RTTI* b = &bFoo;

				Datum datum = a;
				datum.PushBack(b);
				Assert::AreEqual(b, datum.Back<RTTI*>());


				const Datum constDatum = a;
				Assert::AreEqual(a, constDatum.Back<RTTI*>());
			}
		}

		TEST_METHOD(Remove)
		{
			Datum datum;
			Assert::ExpectException<std::runtime_error>([&datum] {datum.RemoveAt(0); });
		}

		TEST_METHOD(SetStorage)
		{
			{
				std::int32_t a = 1;
				std::int32_t* b = &a;
				
				Datum datum;
				datum.SetStorage(b,1);

				Assert::AreEqual((size_t)1, datum.Size());
				Assert::AreEqual((size_t)1, datum.Capacity());
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
				Assert::AreEqual(a, datum.Get<std::int32_t>());
				Assert::IsFalse(datum.IsInternal());
			}

			{
				float a = 1.f;
				float* b = &a;

				Datum datum;
				datum.SetStorage(b, 1);

				Assert::AreEqual((size_t)1, datum.Size());
				Assert::AreEqual((size_t)1, datum.Capacity());
				Assert::AreEqual(DatumTypes::Float, datum.Type());
				Assert::AreEqual(a, datum.Get<float>());
				Assert::IsFalse(datum.IsInternal());
			}

			{
				glm::vec4 a{1.f};
				glm::vec4* b = &a;

				Datum datum;
				datum.SetStorage(b, 1);

				Assert::AreEqual((size_t)1, datum.Size());
				Assert::AreEqual((size_t)1, datum.Capacity());
				Assert::AreEqual(DatumTypes::Vectors, datum.Type());
				Assert::AreEqual(a, datum.Get<glm::vec4>());
				Assert::IsFalse(datum.IsInternal());
			}

			{
				glm::mat4 a{ 1.f };
				glm::mat4* b = &a;

				Datum datum;
				datum.SetStorage(b, 1);

				Assert::AreEqual((size_t)1, datum.Size());
				Assert::AreEqual((size_t)1, datum.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, datum.Type());
				Assert::AreEqual(a, datum.Get<glm::mat4>());
				Assert::IsFalse(datum.IsInternal());
			}

			{
				std::string a{"A"};
				std::string* b = &a;

				Datum datum;
				datum.SetStorage(b, 1);

				Assert::AreEqual((size_t)1, datum.Size());
				Assert::AreEqual((size_t)1, datum.Capacity());
				Assert::AreEqual(DatumTypes::String, datum.Type());
				Assert::AreEqual(a, datum.Get<std::string>());
				Assert::IsFalse(datum.IsInternal());
			}

			{
				Foo aFoo{ 1 };
				RTTI* a = &aFoo;
				RTTI** b = &a;

				Datum datum;
				datum.SetStorage(b, 1);

				Assert::AreEqual((size_t)1, datum.Size());
				Assert::AreEqual((size_t)1, datum.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, datum.Type());
				Assert::AreEqual(a, datum.Get<RTTI*>());
				Assert::IsFalse(datum.IsInternal());
			}

			{
				std::int32_t c = 1;
				Datum datum = c;
				float d = 1.f;
				Datum anotherDatum = d;

				{
					std::int32_t a = 1;
					std::int32_t* b = &a;

					Assert::ExpectException<std::runtime_error>([&anotherDatum, &b] {anotherDatum.SetStorage(b, 1); });
				}

				{
					float a = 1.f;
					float* b = &a;

					Assert::ExpectException<std::runtime_error>([&datum, &b] {datum.SetStorage(b, 1); });
				}

				{
					glm::vec4 a{1.f};
					glm::vec4* b = &a;

					Assert::ExpectException<std::runtime_error>([&datum, &b] {datum.SetStorage(b, 1); });
				}

				{
					glm::mat4 a{ 1.f };
					glm::mat4* b = &a;

					Assert::ExpectException<std::runtime_error>([&datum, &b] {datum.SetStorage(b, 1); });
				}

				{
					std::string a = "ABC";
					std::string* b = &a;

					Assert::ExpectException<std::runtime_error>([&datum, &b] {datum.SetStorage(b, 1); });
				}

				{
					Foo aFoo{ 1 };
					RTTI* a = &aFoo;
					RTTI** b = &a;

					Assert::ExpectException<std::runtime_error>([&datum, &b] {datum.SetStorage(b, 1); });
				}
			}
		}

		TEST_METHOD(External)
		{
			{
				std::int32_t a = 1;
				std::int32_t* b = &a;

				Datum datum;
				datum.SetStorage(b, 1);
				Assert::ExpectException<std::runtime_error>([&datum, &b] {datum = 1;});

				Assert::IsFalse(datum.IsInternal());
			}
		}

		TEST_METHOD(SetFromString)
		{
			{
				Datum datum;
				datum.SetType(DatumTypes::Integer);
				datum.Resize(2);
				Assert::AreEqual(datum.Type(), DatumTypes::Integer);
				Assert::AreEqual(datum.Size(), std::size_t(2));
				datum.SetFromString("1", 0);
				Assert::AreEqual(datum.Get<std::int32_t>(0), 1);
			}

			{
				Datum datum;
				datum.SetType(DatumTypes::Float);
				datum.Resize(2);
				Assert::AreEqual(datum.Type(), DatumTypes::Float);
				Assert::AreEqual(datum.Size(), std::size_t(2));
				datum.SetFromString("1.f", 0);
				Assert::AreEqual(datum.Get<float>(0), 1.f);
			}

			{
				Datum datum;
				datum.SetType(DatumTypes::Vectors);
				datum.Resize(2);
				Assert::AreEqual(datum.Type(), DatumTypes::Vectors);
				Assert::AreEqual(datum.Size(), std::size_t(2));
				datum.SetFromString("vec4(2.0,2.0,2.0,2.0)", 0);
				Assert::AreEqual(datum.Get<glm::vec4>(0), glm::vec4{ 2.f,2.f,2.f,2.f });
			}

			{
				Datum datum;
				datum.SetType(DatumTypes::Matrix);
				datum.Resize(2);
				Assert::AreEqual(datum.Type(), DatumTypes::Matrix);
				Assert::AreEqual(datum.Size(), std::size_t(2));
				datum.SetFromString("mat4x4((1.0,1.0,1.0,1.0),(1.0,1.0,1.0,1.0),(1.0,1.0,1.0,1.0),(1.0,1.0,1.0,1.0))", 0);
				Assert::AreEqual(datum.Get<glm::mat4>(0), glm::mat4{ 1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f,1.f });
			}

			{
				Datum datum;
				datum.SetType(DatumTypes::String);
				datum.Resize(2);
				Assert::AreEqual(datum.Type(), DatumTypes::String);
				Assert::AreEqual(datum.Size(), std::size_t(2));
				datum.SetFromString("abc", 0);
				std::string s = "abc";
				Assert::AreEqual(datum.Get<std::string>(0), s);
			}

			{
				Datum datum;
				datum.SetType(DatumTypes::Pointer);
				datum.Resize(2);
				Assert::AreEqual(datum.Type(), DatumTypes::Pointer);
				Assert::AreEqual(datum.Size(), std::size_t(2));
				Assert::ExpectException<std::runtime_error>([&datum] {datum.SetFromString("", 0); });
			}

			{
				Datum datum;
				Assert::ExpectException<std::runtime_error>([&datum] {datum.SetFromString("", 0); });
			}
		}

		TEST_METHOD(PushBack)
		{
			{
				const std::int32_t a = 1;
				Datum datum;
				Assert::AreEqual(datum.Type(), DatumTypes::Unknown);
				datum.PushBack(a);
				Assert::AreEqual(datum.Type(), DatumTypes::Integer);
			}

			{
				const float a = 1.0f;
				Datum datum;
				Assert::AreEqual(datum.Type(), DatumTypes::Unknown);
				datum.PushBack(a);
				Assert::AreEqual(datum.Type(), DatumTypes::Float);
			}

			{
				const glm::vec4 a{ 1.0f, 1.0f, 1.0f, 1.0f };
				Datum datum;
				Assert::AreEqual(datum.Type(), DatumTypes::Unknown);
				datum.PushBack(a);
				Assert::AreEqual(datum.Type(), DatumTypes::Vectors);
			}

			{
				const glm::mat4 a{ 1.0f };
				Datum datum;
				Assert::AreEqual(datum.Type(), DatumTypes::Unknown);
				datum.PushBack(a);
				Assert::AreEqual(datum.Type(), DatumTypes::Matrix);
			}

			{
				const std::string a("ABC");
				Datum datum;
				Assert::AreEqual(datum.Type(), DatumTypes::Unknown);
				datum.PushBack(a);
				Assert::AreEqual(datum.Type(), DatumTypes::String);
			}

			{
				Foo aFoo{ 1 };
				RTTI* a = &aFoo;
				Datum datum;
				Assert::AreEqual(datum.Type(), DatumTypes::Unknown);
				datum.PushBack(a);
				Assert::AreEqual(datum.Type(), DatumTypes::Pointer);
			}
		}

		TEST_METHOD(MoveSegmantics)
		{
			{
				const std::int32_t a = 1;
				Datum datum = a;
				Assert::AreEqual(datum.Type(), DatumTypes::Integer);
				Datum anotherDatum;
				anotherDatum = std::move(datum);
				Assert::AreEqual(anotherDatum.Size(), (size_t)1);
			}

			{
				const std::int32_t a = 1;
				Datum datum = a;
				Assert::AreEqual(datum.Type(), DatumTypes::Integer);
				Datum anotherDatum = std::move(datum);
				Assert::AreEqual(anotherDatum.Size(), (size_t)1);
			}
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState DatumTests::_startMemState;
}