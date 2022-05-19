#pragma once

#include "Attributed.h"
#include "TypeManager.h"
#include <gsl/gsl>

namespace UnitTests
{
	class AttributedFoo : public FieaGameEngine::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, FieaGameEngine::Attributed);
		
	public:
		static const std::size_t ArraySize = 5;

		/// <summary>
		/// AttributedFoo Constructor
		/// </summary>
		AttributedFoo();

		/// <summary>
		/// AttributedFoo copy constructor
		/// </summary>
		/// <param name="">the AttributedFoo you want to copy</param>
		AttributedFoo(const AttributedFoo&) = default;

		/// <summary>
		/// AttributedFoo move constructor
		/// </summary>
		/// <param name="">the AttributedFoo you want to move</param>
		AttributedFoo(AttributedFoo&&) = default;

		/// <summary>
		/// AttributedFoo Copy Assignment
		/// </summary>
		/// <param name="">the AttributedFoo you want to copy</param>
		/// <returns>the AttributedFoo reference</returns>
		AttributedFoo& operator=(const AttributedFoo&) = default;

		/// <summary>
		/// AttributedFoo Move Assignment
		/// </summary>
		/// <param name="">the AttributedFoo you want to move</param>
		/// <returns>the AttributedFoo reference</returns>
		AttributedFoo& operator=(AttributedFoo&&) = default;
		virtual ~AttributedFoo() = default;

		int ExternalInteger = 0;
		float ExternalFloat = 0;
		std::string ExternalString;
		glm::vec4 ExternalVector;
		glm::mat4 ExternalMatrix;

		int ExternalIntegerArray[ArraySize];
		float ExternalFloatArray[ArraySize];
		std::string ExternalStringArray[ArraySize];
		glm::vec4 ExternalVectorArray[ArraySize];
		glm::mat4 ExternalMatrixArray[ArraySize];

		/// <summary>
		/// the Clone() function of AttributedFoo
		/// </summary>
		/// <returns>the AttributedFoo pointer</returns>
		gsl::owner<AttributedFoo*> Clone() const override;

		/// <summary>
		/// if the AttributedFoo is equal to the RTTI pointer
		/// </summary>
		/// <param name="rhs">RTTI pointer</param>
		/// <returns>if they are equal</returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// turn this AttributedFoo to string
		/// </summary>
		/// <returns>a string</returns>
		std::string ToString() const override;

		/// <summary>
		/// Get the signature of this attribute
		/// </summary>
		/// <returns>a vector of signature</returns>
		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	protected:
		AttributedFoo(std::size_t typeID);
	};

	ConcreteFactory(AttributedFoo, FieaGameEngine::Scope)
}