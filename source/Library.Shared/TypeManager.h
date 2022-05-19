#pragma once

#include "Signature.h"
#include "HashMap.h"
#include "Vector.h"

namespace FieaGameEngine
{
	class TypeManager
	{
	public:
		/// <summary>
		/// Return the Signature of the assign typeID
		/// </summary>
		/// <param name="typeID">type ID</param>
		/// <returns>a vector of signature</returns>
		static const Vector<Signature>& GetSignature(std::size_t typeID);

		/// <summary>
		/// Insert signature to the registry
		/// </summary>
		/// <param name="typeID">the typeID</param>
		/// <param name="signature">the vector of signature</param>
		/// <param name="parent">the vector of signature if it has parent parent</param>
		static void Insert(std::size_t typeID, Vector<Signature> signature, std::size_t parent_id);


	private:
		static HashMap<std::size_t, Vector<Signature>> _registry;
	};

#define RegisterType(Type, ParentType) TypeManager::Insert(Type::TypeIdClass(), Type::Signatures(), ParentType::TypeIdClass())
}