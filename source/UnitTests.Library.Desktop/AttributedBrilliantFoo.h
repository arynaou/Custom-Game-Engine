#pragma once

#include <gsl/gsl>
#include "AttributedFoo.h"
#include "Factory.h"

namespace UnitTests
{
	class AttributedBrilliantFoo : public AttributedFoo
	{
		RTTI_DECLARATIONS(AttributedBrilliantFoo, AttributedFoo);

	public:
		/// <summary>
		/// AttributedBrilliantFoo Constructor
		/// </summary>
		AttributedBrilliantFoo();
		~AttributedBrilliantFoo() = default;

		int ExternalInteger;

		/// <summary>
		/// the customized Clone of attributedBrilliantFoo 
		/// </summary>
		/// <returns>AttributedBrilliantFoo pointer</returns>
		gsl::owner<AttributedBrilliantFoo*> Clone() const override;

		/// <summary>
		/// Equals function for AttirbutedBrilliantFoo
		/// </summary>
		/// <param name="rhs">a comparision RTTI pointer</param>
		/// <returns>if the pointer and the attributed foo is equal</returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// turn this AttributedBrilliantFoo to string
		/// </summary>
		/// <returns>a string</returns>
		std::string ToString() const override;

		/// <summary>
		/// Get the signature of this attribute
		/// </summary>
		/// <returns>a vector of signature</returns>
		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	};

	ConcreteFactory(AttributedBrilliantFoo, FieaGameEngine::Scope)
}