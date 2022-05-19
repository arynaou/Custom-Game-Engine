#include "pch.h"
#include "ActionListIf.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionListIf);

	ActionListIf::ActionListIf() :
		ActionList(ActionListIf::TypeIdClass())
	{
	}

	void ActionListIf::Update(const GameTime& gameTime)
	{
		assert(!Actions.Get<Scope*>(Condition)->Is(Action::IdType()));
		static_cast<Action*>(Actions.Get<Scope*>(Condition))->Update(gameTime);
	}

	const Vector<Signature> ActionListIf::Signatures()
	{
		return Vector<Signature>
		{
			{ "Conditions"s, DatumTypes::Integer,1, offsetof(ActionListIf, Condition)}
		};
	}

	bool ActionListIf::Equals(const RTTI* rhs) const
	{
		const ActionListIf* const rhsActionListIf = rhs->As<ActionListIf>();
		if (rhsActionListIf == nullptr)
		{
			return false;
		}

		return _name == rhsActionListIf->_name &&
			Condition == rhsActionListIf->Condition &&
			(*this).At("Actions"s) == (*rhsActionListIf).At("Actions"s);
	}

	std::string ActionListIf::ToString() const
	{
		return "ActionListIf"s;
	}


	gsl::owner<ActionListIf*> ActionListIf::Clone() const
	{
		return new ActionListIf(*this);
	}
}