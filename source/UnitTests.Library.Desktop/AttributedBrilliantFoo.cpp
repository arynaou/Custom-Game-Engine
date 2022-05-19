#include "pch.h"
#include "AttributedBrilliantFoo.h"

using namespace FieaGameEngine;

namespace UnitTests
{
	RTTI_DEFINITIONS(AttributedBrilliantFoo);

	AttributedBrilliantFoo::AttributedBrilliantFoo() :
		AttributedFoo(AttributedBrilliantFoo::TypeIdClass())
	{
	}

	const Vector<Signature> AttributedBrilliantFoo::Signatures()
	{
		return Vector<Signature>
		{
			{ "AnotherExternalInteger", DatumTypes::Integer, 1, offsetof(AttributedBrilliantFoo, ExternalInteger) },
		};
	}

	bool AttributedBrilliantFoo::Equals(const RTTI* rhs) const
	{
		const AttributedBrilliantFoo* const rhsFoo = rhs->As<AttributedBrilliantFoo>();
		if (rhsFoo == nullptr)
		{
			return false;
		}

		return ExternalInteger == rhsFoo->ExternalInteger;
	}

	gsl::owner<AttributedBrilliantFoo*> AttributedBrilliantFoo::Clone() const
	{
		return new AttributedBrilliantFoo(*this);
	}

	std::string AttributedBrilliantFoo::ToString() const
	{
		return "AttributedBrilliantFoo"s;
	}
}