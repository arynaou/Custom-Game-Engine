#include "pch.h"
#include "ActionDestroyAction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionDestroyAction);

	ActionDestroyAction::ActionDestroyAction() :
		Action(ActionDestroyAction::TypeIdClass())
	{
	}

	void ActionDestroyAction::Update(const GameTime&)
	{
		Datum* datum = Search("Actions"s);

		for (std::size_t i = 0; i < datum->Size(); ++i)
		{
			if ((*datum)[i].As<Action>()->Name() == _deleteName)
			{
				GameState::Add_DeleteAction(datum->Get<Scope*>(i));
				return;
			}
		}
	}

	const Vector<Signature> ActionDestroyAction::Signatures()
	{
		return Vector<Signature>
		{
			{ "DeleteName"s, DatumTypes::String, 1, offsetof(ActionDestroyAction, _deleteName) }
		};
	}

	bool ActionDestroyAction::Equals(const RTTI* rhs) const
	{
		const ActionDestroyAction* const rhsActionDestroyAction = rhs->As<ActionDestroyAction>();
		if (rhsActionDestroyAction == nullptr)
		{
			return false;
		}

		return _name == rhsActionDestroyAction->_name &&
			_deleteName == rhsActionDestroyAction->_deleteName;
	}

	std::string ActionDestroyAction::ToString() const
	{
		return "ActionDestroyAction"s;
	}

	gsl::owner<ActionDestroyAction*> ActionDestroyAction::Clone() const
	{
		return new ActionDestroyAction(*this);
	}
}