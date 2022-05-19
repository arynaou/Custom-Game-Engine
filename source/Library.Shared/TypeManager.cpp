#include "pch.h"
#include "TypeManager.h"
#include "Attributed.h"

namespace FieaGameEngine
{
	HashMap<std::size_t, Vector<Signature>> TypeManager::_registry;

	const Vector<Signature>& TypeManager::GetSignature(std::size_t typeID)
	{
		return _registry[typeID];
	}

	void TypeManager::Insert(std::size_t typeID, Vector<Signature> signature, std::size_t parent_id)
	{
		if (parent_id == Attributed::TypeIdClass())
		{
			_registry.Insert(std::make_pair(typeID, signature));
		}
		else
		{
			Vector<Signature> parentSignature = GetSignature(parent_id);
		
			for (size_t i = 0; i < signature.Size(); ++i)
			{
				parentSignature.PushBack(signature[i]);
			}

			_registry.Insert(std::make_pair(typeID, parentSignature));
		}
	}
}