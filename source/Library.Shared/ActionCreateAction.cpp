#include "pch.h"
#include "ActionCreateAction.h"

namespace FieaGameEngine 
{
	RTTI_DEFINITIONS(ActionCreateAction);

	ActionCreateAction::ActionCreateAction() :
		Action(ActionCreateAction::TypeIdClass())
	{
	}

	void ActionCreateAction::Update(const GameTime&)
	{
		Scope* scope= Factory<Scope>::Create(_prototype);

		if (scope != nullptr)
		{
			scope->As<Action>()->SetName(_createName);
			GameState::Add_CreateAction(_parent, scope);
		}
	}

	const Vector<Signature> ActionCreateAction::Signatures()
	{
		return Vector<Signature>
		{
			{ "Prototype"s, DatumTypes::String, 1, offsetof(ActionCreateAction, _prototype) },
			{ "CreateName"s, DatumTypes::String,1,offsetof(ActionCreateAction, _createName) }
		};
	}

	bool ActionCreateAction::Equals(const RTTI* rhs) const
	{
		const ActionCreateAction* const rhsActionCreateAction = rhs->As<ActionCreateAction>();
		if (rhsActionCreateAction == nullptr)
		{
			return false;
		}

		return _name == rhsActionCreateAction->_name &&
			_prototype == rhsActionCreateAction->_prototype &&
			_createName == rhsActionCreateAction->_createName;
	}

	std::string ActionCreateAction::ToString() const
	{
		return "ActionCreateAction"s;
	}

	gsl::owner<ActionCreateAction*> ActionCreateAction::Clone() const
	{
		return new ActionCreateAction(*this);
	}
}