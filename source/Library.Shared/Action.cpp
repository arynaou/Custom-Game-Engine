#include "pch.h"
#include "Action.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action);

	Action::Action(std::size_t sizeID) :
		Attributed(sizeID)
	{
	}

	const std::string& Action::Name()
	{
		return _name;
	}

	void Action::SetName(const std::string& name)
	{
		_name = name;
	}

	const Vector<Signature> Action::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name"s, DatumTypes::String, 1, offsetof(Action, _name) }
		};
	}
}