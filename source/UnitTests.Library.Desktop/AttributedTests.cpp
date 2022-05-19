#include "pch.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include "AttributedBrilliantFoo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTests
{
	TEST_CLASS(AttributedTests)
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

		TEST_METHOD(Attributes)
		{
			AttributedFoo foo;

			Assert::IsTrue(foo.IsAttribute("this"));
			Assert::IsTrue(foo.IsAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsAttribute("ExternalString"));
			Assert::IsTrue(foo.IsAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsAttribute("NestedScope"));
			Assert::IsTrue(foo.IsAttribute("NestedScopeArray"));
			Assert::IsFalse(foo.IsAttribute("Testing"));

			Assert::IsTrue(foo.IsPrescribedAttribute("this"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScopeArray"));
			Assert::IsFalse(foo.IsPrescribedAttribute("Testing"));

			Assert::IsFalse(foo.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScope"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScopeArray"));
			Assert::IsFalse(foo.IsPrescribedAttribute("Testing"));

			Assert::AreEqual(&foo, foo["this"].Get<RTTI*>()->As<AttributedFoo>());
		}

		TEST_METHOD(AuxiliaryAttributes)
		{
			AttributedFoo foo;

			foo.AppendAuxiliaryAttribute("AuxiliaryInteger");
			Assert::IsTrue(foo.IsAttribute("AuxiliaryInteger"));
			Assert::IsTrue(foo.IsAuxiliaryAttribute("AuxiliaryInteger"));
			Assert::IsFalse(foo.IsPrescribedAttribute("AuxiliaryInteger"));

			Datum d1;
			foo.AppendAuxiliaryAttribute("AuxiliaryIntegerWithDatum") =d1;
			Assert::IsTrue(foo.IsAttribute("AuxiliaryIntegerWithDatum"));
			Assert::IsTrue(foo.IsAuxiliaryAttribute("AuxiliaryIntegerWithDatum"));
			Assert::IsFalse(foo.IsPrescribedAttribute("AuxiliaryIntegerWithDatum"));
		
			Assert::ExpectException<std::runtime_error>([&foo] {foo.AppendAuxiliaryAttribute("ExternalInteger"); });
		}

		TEST_METHOD(ExternalAssignment)
		{
			{
				AttributedFoo foo;

				Datum* datum = foo.Find("ExternalInteger");
				Assert::AreEqual(std::size_t(1), datum->Size());
				Assert::AreSame(datum->Get<int>(), foo.ExternalInteger);

				int a = 5;
				datum->Set(a);
				Assert::AreEqual(a, datum->Get<int>());
				Assert::AreEqual(a, foo.ExternalInteger);
			}

			{
				AttributedFoo foo;

				Datum* datum = foo.Find("ExternalFloat");
				Assert::AreEqual(std::size_t(1), datum->Size());
				Assert::AreSame(datum->Get<float>(), foo.ExternalFloat);

				float b = 6.0f;
				datum->Set(b);
				foo["ExternalFloat"] = *datum;
				Assert::AreEqual(b, foo.ExternalFloat);
			}
		}

		TEST_METHOD(MoveConstructor)
		{
			AttributedFoo foo;
			AttributedFoo anotherAttributedFoo = std::move(foo);

			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("this"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("NestedScopeArray"));
			Assert::IsFalse(foo.IsPrescribedAttribute("Testing"));
		}

		TEST_METHOD(MoveAssignment)
		{
			AttributedFoo foo;
			AttributedFoo anotherAttributedFoo;
			anotherAttributedFoo = std::move(foo);

			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("this"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("NestedScopeArray"));
			Assert::IsFalse(foo.IsPrescribedAttribute("Testing"));
		}

		TEST_METHOD(CopyConstructor)
		{
			AttributedFoo foo;
			AttributedFoo anotherAttributedFoo = foo;

			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("this"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("NestedScopeArray"));
			Assert::IsFalse(foo.IsPrescribedAttribute("Testing"));
		}

		TEST_METHOD(CopyAssignment)
		{
			AttributedFoo foo;
			AttributedFoo anotherAttributedFoo;
			anotherAttributedFoo = foo;

			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("this"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(anotherAttributedFoo.IsPrescribedAttribute("NestedScopeArray"));
			Assert::IsFalse(foo.IsPrescribedAttribute("Testing"));
		}

		TEST_METHOD(AttributedBrilliantFooTests)
		{
			AttributedBrilliantFoo bFoo;
			Assert::IsTrue(bFoo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(bFoo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(bFoo.IsPrescribedAttribute("this"));
			Assert::IsTrue(bFoo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(bFoo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(bFoo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(bFoo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(bFoo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(bFoo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(bFoo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(bFoo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(bFoo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(bFoo.IsPrescribedAttribute("NestedScopeArray"));
			Assert::IsTrue(bFoo.IsPrescribedAttribute("AnotherExternalInteger"));
			Assert::IsFalse(bFoo.IsPrescribedAttribute("Testing"));
		}

		TEST_METHOD(RTTITest)
		{
			AttributedFoo a;

			RTTI* rtti = &a;
			Assert::IsFalse(rtti->Is("Foo"s));
			Assert::IsTrue(rtti->Is("AttributedFoo"s));
			Assert::IsTrue(rtti->Is("Attributed"s));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(AttributedFoo::TypeIdClass(), rtti->TypeIdInstance());

			Foo* b = rtti->As<Foo>();
			Assert::IsNull(b);

			AttributedFoo* f = rtti->As<AttributedFoo>();
			Assert::IsNotNull(f);
			Assert::AreEqual(&a, f);

			Attributed* fAsAttributed = rtti->As<Attributed>();
			Assert::IsNotNull(fAsAttributed);
			Assert::AreEqual(static_cast<Attributed*>(&a), fAsAttributed);

			RTTI* r = rtti->QueryInterface(Foo::TypeIdClass());
			Assert::IsNull(r);
			r = rtti->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsNotNull(r);
			r = rtti->QueryInterface(Attributed::TypeIdClass());
			Assert::IsNotNull(r);

			AttributedFoo otherFoo;
			Assert::IsTrue(rtti->Equals(&otherFoo));

			Foo anotherFoo(100);
			Assert::IsFalse(rtti->Equals(&anotherFoo));

			Assert::AreEqual("AttributedFoo"s, r->ToString());
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState AttributedTests::_startMemState;
}