#pragma once
#include "Scope.h"
#include "Signature.h"
#include "TypeManager.h"

using namespace std::string_literals;

namespace FieaGameEngine
{
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);
	
	public:
		Attributed() = default;

		/// <summary>
		/// Copy Constructor of Attributed
		/// </summary>
		/// <param name="rhs">the attributed you want to copy</param>
		Attributed(const Attributed& rhs);

		/// <summary>
		/// Copy Assignment of Attributed
		/// </summary>
		/// <param name="rhs">the attributed you want to copy</param>
		/// <returns>attributed reference</returns>
		Attributed& operator=(const Attributed& rhs);

		/// <summary>
		/// Move constructor of Attributed
		/// </summary>
		/// <param name="rhs">the attributed you want to move</param>
		Attributed(Attributed&& rhs) noexcept;

		/// <summary>
		/// Move Assignment of Attributed
		/// </summary>
		/// <param name="rhs">the attributed you want to move</param>
		/// <returns>attributed reference</returns>
		Attributed& operator=(Attributed&& rhs) noexcept;

		/// <summary>
		/// take a std::string and return if it's attribute or not
		/// </summary>
		/// <param name="value">the string you want to test</param>
		/// <returns>if it's attribute</returns>
		bool IsAttribute(const std::string& value);

		/// <summary>
		/// take a std::string and return if it's prescribed attribute or not
		/// </summary>
		/// <param name="value">the string you want to test</param>
		/// <returns>if it's prescribe attribute</returns>
		bool IsPrescribedAttribute(const std::string& value);

		/// <summary>
		/// take a std::string and return if it's auxiliary attribute or not
		/// </summary>
		/// <param name="value">the string you want to test</param>
		/// <returns>if it's an attribute but not prescribed</returns>
		bool IsAuxiliaryAttribute(const std::string& value);

		/// <summary>
		/// Append a not prescribed attribute to the attribute
		/// </summary>
		/// <param name="name">a std::string</param>
		/// <returns>a datum reference</returns>
		Datum& AppendAuxiliaryAttribute(const std::string& name);

		//const Vector<Signature> Attributes() const;

		//const Vector<Signature> PrescribedAttributes() const;

		//const Vector<Signature> AuxiliaryAttributes() const;

		/// <summary>
		/// a vector of signature
		/// </summary>
		/// <returns>a vector of signature</returns>
		static const Vector<Signature> Signatures();


	protected:
		Attributed(std::size_t typeID);
		void Populate(std::size_t typeID);
	};
}