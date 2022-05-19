#include "pch.h"
#include "Scope.h"
#include "Foo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTests
{
	TEST_CLASS(ScopeTests)
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
			Scope scope;
			Assert::IsTrue(scope.GetParent() == nullptr);
		}

		TEST_METHOD(Append)
		{
			Scope scope;
			Foo foo = 10;
			Datum datum = &foo;
			scope["A"] = datum;
			Assert::AreEqual(*(scope.Find("A")), datum);
			Assert::AreEqual(scope.Append("A"), datum);
		}

		TEST_METHOD(AppendScope)
		{
			Scope parentScope,scope, scopeB, scopeC;

			Foo a = 10;
			RTTI* ar = &a;
			Datum datumA = ar;

			parentScope.AppendScope("A") = scope;
			Scope* appendedscope = parentScope.Find("A")->Get<Scope*>(0);
			auto find = parentScope.FindContainedScope(appendedscope);
			Assert::AreEqual(find.first->Type(), DatumTypes::Table);

			parentScope.Append("B");
			parentScope.AppendScope("B") = scopeB;

			parentScope.Append("C") = ar;
			Assert::ExpectException<std::runtime_error>([&parentScope,&scopeC] {parentScope.AppendScope("C") = scopeC; });
			
		}

		TEST_METHOD(Find)
		{
			Scope* scope = new Scope();
			Foo a = 10;
			Foo b = 20;
			Foo c = 30;
			RTTI* ar = &a;
			RTTI* br = &b;
			RTTI* cr = &c;
			Datum datumA = ar;
			Datum datumB = br;
			Datum datumC = cr;
			scope->Append("A") = datumA;
			scope->Append("B") = datumB;
			scope->Append("C") = datumC;

			Assert::AreEqual(*(scope->Find("A")), datumA);
			Assert::AreEqual(*(scope->Find("B")), datumB);
			Assert::AreEqual(*(scope->Find("C")), datumC);

			const Scope otherScope = *scope;
			Assert::AreEqual(*(otherScope.Find("A")), datumA);
			Assert::AreEqual(*(otherScope.Find("B")), datumB);
			Assert::AreEqual(*(otherScope.Find("C")), datumC);
			Assert::IsTrue(otherScope.Find("D") == nullptr);
			delete scope;


		}

		TEST_METHOD(FindContainedScope)
		{
			Scope parentScope;
			Scope* scope = new Scope();

			scope->Append("A");
			parentScope.Adopt(*scope, "B");
			Assert::AreEqual(scope->GetParent(), &parentScope);
			Assert::AreEqual(parentScope["B"].Get<Scope*>(0), scope);
			auto find = parentScope.FindContainedScope(scope);
			Assert::AreEqual(find.first->Type(), DatumTypes::Table);
		}

		TEST_METHOD(Adopt)
		{
			Scope* scope = new Scope();
			Scope parentScope;
			Foo a = 10;
			Foo b = 20;
			RTTI* ar = &a;
			RTTI* br = &b;
			Datum datumA = ar;
			Datum datumB = br;

			scope->Append("A") = datumA;
			parentScope["B"] = datumB;
			parentScope.Adopt(*scope, "C");
			Assert::AreEqual(scope->GetParent(), &parentScope);
			Assert::AreEqual(parentScope["C"].Get<Scope*>(0), scope);
			Assert::AreEqual(parentScope["C"].Type(), DatumTypes::Table);

			Scope* anotherScope = new Scope();
			scope->Adopt(*anotherScope, "D");
			Assert::AreEqual(anotherScope->GetParent(), scope);
			parentScope.Adopt(*anotherScope, "D");
			Assert::AreEqual(anotherScope->GetParent(), &parentScope);

			Scope* newScope = new Scope();
			scope->Adopt(*newScope, "D");
			Assert::AreEqual(newScope->GetParent(), scope);
		}

		TEST_METHOD(Search)
		{
			{
				Scope parentScope;
				Scope* scope = new Scope();
				Foo a = 10;
				RTTI* ar = &a;
				Datum datum = ar;

				parentScope["A"] = datum;
				parentScope.Adopt(*scope, "B");

				Assert::AreEqual(scope->GetParent(), &parentScope);
				Assert::AreEqual(*(scope->Search("A")), datum);
			}

			{
				Scope parentScope;
				Scope* scope = new Scope();
				Foo a = 10;
				RTTI* ar = &a;
				Datum datum = ar;

				parentScope["A"] = datum;
				parentScope.Adopt(*scope, "B");

				const Scope& constScope = *scope;
				Assert::AreEqual(*(constScope.Search("A")), datum);
			}

			{
				Scope parentScope;
				Scope* scope = new Scope();
				Foo a = 10;
				RTTI* ar = &a;
				Datum datum = ar;

				scope->Append("A");
				parentScope["A"] = datum;
				parentScope.Adopt(*scope, "B");

				Scope* scopePtr;
				Assert::AreEqual(scope->GetParent(), &parentScope);
				Assert::AreNotEqual(*(scope->Search("A", scopePtr)),datum);
			}
		}

		TEST_METHOD(SetParent)
		{
			Scope parentScope;
			Scope* scope = new Scope();

			scope->Append("A");
			scope->SetParent(&parentScope);
			Assert::AreEqual(scope->GetParent(), &parentScope);

			delete scope;
		}

		TEST_METHOD(GetParent)
		{
			Scope parentScope;
			Scope* scope = new Scope();
			const Scope otherScope = *scope;

			scope->Append("A");
			parentScope.Adopt(*scope, "B");
			const Scope& constScope = *scope;
			Assert::AreEqual(scope->GetParent(), &parentScope);
			Assert::AreEqual(constScope.GetParent(), &parentScope);
		}

		TEST_METHOD(BracketOperaotr)
		{
			Scope scope;
			Foo a = 10;
			Foo b = 20;
			Datum datumA = &a;
			Datum datumB = &b;

			scope["A"] = datumA;
			scope["B"] = datumB;
			Assert::AreEqual(scope["A"], datumA);
			Assert::AreEqual(scope[0], datumA);
			Assert::AreEqual(scope["B"], datumB);
			Assert::AreEqual(scope[1], datumB);
		}

		TEST_METHOD(CopyConstructor)
		{
			{
				Scope* scope = new Scope();
				Foo a = 10;
				RTTI* ar = &a;
				Datum datum = ar;
				scope->Append("A") = datum;

				Scope otherScope = *scope;

				Assert::IsTrue(otherScope.GetParent()== scope->GetParent());
				Assert::AreEqual(*(scope->Find("A")), *(otherScope.Find("A")));

				delete scope;
			}

			{
				Scope* scope = new Scope();
				Scope parentScope;
				Foo a = 10;
				Foo b = 20;
				RTTI* ar = &a;
				RTTI* br = &b;
				Datum datumA = ar;
				Datum datumB = br;

				scope->Append("A") = datumA;
				parentScope["B"] = datumB;
				parentScope.Adopt(*scope, "C");
				Assert::AreEqual(scope->GetParent(), &parentScope);
				Assert::AreEqual(parentScope["C"].Get<Scope*>(0), scope);
				Assert::AreEqual(parentScope["C"].Type(), DatumTypes::Table);

				Scope otherScope = parentScope;
				Assert::IsTrue(otherScope.GetParent() == parentScope.GetParent());
				Assert::AreEqual(*(otherScope.Find("B")), *(parentScope.Find("B")));
			}
		}

		TEST_METHOD(CopyAssignment)
		{
			{
				Scope* scope = new Scope();
				Foo a = 10;
				RTTI* ar = &a;
				Datum datum = ar;
				scope->Append("A") = datum;

				Scope otherScope;
				otherScope = *scope;

				Assert::IsTrue(otherScope.GetParent() == scope->GetParent());
				Assert::AreEqual(*(scope->Find("A")), *(otherScope.Find("A")));

				delete scope;
			}

			{
				Scope* scope = new Scope();
				Scope parentScope;
				Foo a = 10;
				Foo b = 20;
				RTTI* ar = &a;
				RTTI* br = &b;
				Datum datumA = ar;
				Datum datumB = br;

				scope->Append("A") = datumA;
				parentScope["B"] = datumB;
				parentScope.Adopt(*scope, "C");
				Assert::AreEqual(scope->GetParent(), &parentScope);
				Assert::AreEqual(parentScope["C"].Get<Scope*>(0), scope);
				Assert::AreEqual(parentScope["C"].Type(), DatumTypes::Table);

				Scope otherScope;
				otherScope = parentScope;
				Assert::IsTrue(otherScope.GetParent() == parentScope.GetParent());
				Assert::AreEqual(*(otherScope.Find("B")), *(parentScope.Find("B")));
			}
		}

		TEST_METHOD(MoveConstructor)
		{
			Scope* scope = new Scope();
			const std::string a = "A"s;
			const std::string b = "B"s;
			const std::string c = "C"s;

			Datum& aDatum = scope->Append(a);
			aDatum = "Hello, World"s;
			Datum& bDatum = scope->Append(b);
			bDatum = glm::mat4(5);
			Scope& cScope = scope->AppendScope(c);

			auto [cDatum, datumIndex] = scope->FindContainedScope(&cScope);
			Assert::IsNotNull(cDatum);

			Scope* movedScope = new Scope(std::move(*scope));
			Datum* foundDatum = movedScope->Find(a);
			Assert::AreEqual(aDatum, *foundDatum);
			foundDatum = movedScope->Find(b);
			Assert::AreEqual(bDatum, *foundDatum);
			foundDatum = movedScope->Find(c);
			Assert::AreEqual(*cDatum, *foundDatum);
			Assert::AreEqual(foundDatum->Get<Scope*>()->GetParent(), movedScope);

			Scope* reparentedScope = new Scope(std::move(cScope));
			Assert::IsNull(cScope.GetParent());
			auto parent = reparentedScope->GetParent();
			Assert::AreEqual(movedScope, parent);
			foundDatum = movedScope->Find(c);
			Assert::AreEqual(*cDatum, *foundDatum);
			Assert::AreEqual(*cDatum, *(movedScope->FindContainedScope(reparentedScope).first));

			delete &cScope;
			delete scope;
			delete movedScope;
		}

		TEST_METHOD(MoveAssignment)
		{
			Scope* scope = new Scope();
			const std::string a = "A"s;
			const std::string b = "B"s;
			const std::string c = "C"s;

			Datum& aDatum = scope->Append(a);
			aDatum = "Hello, World"s;
			Datum& bDatum = scope->Append(b);
			bDatum = glm::mat4(5);
			Scope& cScope = scope->AppendScope(c);

			auto [cDatum, datumIndex] = scope->FindContainedScope(&cScope);
			Assert::IsNotNull(cDatum);

			Scope* movedScope;
			movedScope = new Scope(std::move(*scope));
			Datum* foundDatum = movedScope->Find(a);
			Assert::AreEqual(aDatum, *foundDatum);
			foundDatum = movedScope->Find(b);
			Assert::AreEqual(bDatum, *foundDatum);
			foundDatum = movedScope->Find(c);
			Assert::AreEqual(*cDatum, *foundDatum);
			Assert::AreEqual(foundDatum->Get<Scope*>()->GetParent(), movedScope);

			Scope* reparentedScope;
			reparentedScope = new Scope(std::move(cScope));
			Assert::IsNull(cScope.GetParent());
			auto parent = reparentedScope->GetParent();
			Assert::AreEqual(movedScope, parent);
			foundDatum = movedScope->Find(c);
			Assert::AreEqual(*cDatum, *foundDatum);
			Assert::AreEqual(*cDatum, *(movedScope->FindContainedScope(reparentedScope).first));

			delete& cScope;
			delete scope;
			delete movedScope;
		}

		TEST_METHOD(EqualOperator)
		{
			{
				Scope* scope = new Scope();
				Foo a = 10;
				Foo b = 20;
				RTTI* ar = &a;
				RTTI* br = &b;
				Datum datum = ar;
				Datum datumB = br;
				scope->Append("A") = datum;
				Scope anotherScope;
				anotherScope.Append("A");

				Scope otherScope;
				otherScope = *scope;

				Assert::IsTrue(*scope == otherScope);
				Assert::IsFalse(*scope != otherScope);
				Assert::IsFalse(*scope == anotherScope);


				delete scope;
			}

			{
				Scope* scope = new Scope();
				Scope* scope2 = new Scope();
				Foo a = 10;
				Foo b = 20;
				RTTI* ar = &a;
				RTTI* br = &b;
				Datum datum = ar;
				Datum datumB = br;
				scope->Append("A") = datum;
				scope2->Append("A") = datumB;
				Scope firstScope, secondScope;
				firstScope.AppendScope("A") = scope;
				secondScope.AppendScope("A") = scope2;
				Assert::IsFalse(firstScope == secondScope);

				delete scope;
				delete scope2;
			}
		}

		TEST_METHOD(Equal)
		{
			Scope* scope = new Scope();
			Scope anotherScope;
			Foo a = 10;
			Foo b = 20;
			RTTI* ar = &a;
			RTTI* ScopePointer = &anotherScope;
			Datum datum = ar;
			Datum datumB = ScopePointer;
			scope->Append("A") = datum;
			scope->Append("B") = datumB;

			Assert::IsFalse(scope->Equals(ar));
			Assert::IsFalse(scope->Equals(ScopePointer));
			delete scope;

		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState ScopeTests::_startMemState;
}