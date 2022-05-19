#include "pch.h"
#include "AttributedFoo.h"

using namespace FieaGameEngine;

namespace UnitTests
{
	RTTI_DEFINITIONS(AttributedFoo);

	AttributedFoo::AttributedFoo() :
		Attributed(AttributedFoo::TypeIdClass())
	{
	}

	AttributedFoo::AttributedFoo(std::size_t sizeID) :
		Attributed(sizeID), ExternalVector{}, ExternalMatrix{}, ExternalIntegerArray{}, ExternalFloatArray{}, ExternalVectorArray{}, ExternalMatrixArray{}
	{
	}

	const Vector<Signature> AttributedFoo::Signatures()
	{
		return Vector<Signature>
		{
			{ "ExternalInteger", DatumTypes::Integer, 1, offsetof(AttributedFoo, ExternalInteger) },
			{ "ExternalFloat", DatumTypes::Float,1,offsetof(AttributedFoo,ExternalFloat) },
			{ "ExternalString", DatumTypes::String, 1, offsetof(AttributedFoo, ExternalString) },
			{ "ExternalVector", DatumTypes::Vectors, 1, offsetof(AttributedFoo, ExternalVector) },
			{ "ExternalMatrix", DatumTypes::Matrix, 1, offsetof(AttributedFoo, ExternalMatrix) },
			{ "ExternalIntegerArray", DatumTypes::Integer,ArraySize,offsetof(AttributedFoo,ExternalIntegerArray) },
			{ "ExternalFloatArray", DatumTypes::Float,ArraySize,offsetof(AttributedFoo,ExternalFloatArray) },
			{ "ExternalStringArray", DatumTypes::String, ArraySize, offsetof(AttributedFoo, ExternalStringArray) },
			{ "ExternalVectorArray", DatumTypes::Vectors, ArraySize, offsetof(AttributedFoo, ExternalVectorArray) },
			{ "ExternalMatrixArray", DatumTypes::Matrix, ArraySize, offsetof(AttributedFoo, ExternalMatrixArray) },
			{ "NestedScope", DatumTypes::Table,0,0 },
			{ "NestedScopeArray",DatumTypes::Table,ArraySize,0 }
		};
	}

	bool AttributedFoo::Equals(const RTTI* rhs) const
	{
		const AttributedFoo* const rhsFoo = rhs->As<AttributedFoo>();
		if (rhsFoo == nullptr)
		{
			return false;
		}

		return ExternalInteger == rhsFoo->ExternalInteger &&
			ExternalFloat == rhsFoo->ExternalFloat &&
			ExternalString == rhsFoo->ExternalString &&
			ExternalVector == rhsFoo->ExternalVector &&
			ExternalMatrix == rhsFoo->ExternalMatrix &&
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsFoo->ExternalIntegerArray)) &&
			std::equal(std::begin(ExternalFloatArray), std::end(ExternalFloatArray), std::begin(rhsFoo->ExternalFloatArray)) &&
			std::equal(std::begin(ExternalStringArray), std::end(ExternalStringArray), std::begin(rhsFoo->ExternalStringArray)) &&
			std::equal(std::begin(ExternalVectorArray), std::end(ExternalVectorArray), std::begin(rhsFoo->ExternalVectorArray)) &&
			std::equal(std::begin(ExternalMatrixArray), std::end(ExternalMatrixArray), std::begin(rhsFoo->ExternalMatrixArray)) &&
			(*this).At("NestedScope"s) == (*rhsFoo).At("NestedScope"s);
	}

	gsl::owner<AttributedFoo*> AttributedFoo::Clone() const
	{
		return new AttributedFoo(*this);
	}

	std::string AttributedFoo::ToString() const
	{
		return "AttributedFoo"s;
	}
}