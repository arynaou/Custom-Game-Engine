#include "pch.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionList);

	ActionList::ActionList() :
		Action(ActionList::TypeIdClass())
	{
	}

	ActionList::ActionList(std::size_t sizeID) :
		Action(sizeID)
	{
	}
	
	void ActionList::Update(const GameTime& gameTime)
	{
		for (std::size_t i = 0; i < Actions.Size(); ++i)
		{
			Action* action = Actions.Get<Scope*>(i)->As<Action>();
			if (action != nullptr)
			{
				action->Update(gameTime);
			}
		}
	}

	void ActionList::CreateAction(const std::string& className, const std::string& name)
	{
		Scope* scope = Factory<Scope>::Create(className);

		if (scope != nullptr)
		{
			scope->As<Action>()->SetName(name);
			Actions.PushBack(scope);
		}
	}

	const Vector<Signature> ActionList::Signatures()
	{
		return Vector<Signature>
		{
			{ "Actions"s, DatumTypes::Table, 0, 0 }
		};
	}

	bool ActionList::Equals(const RTTI* rhs) const
	{
		const ActionList* const rhsActionList = rhs->As<ActionList>();
		if (rhsActionList == nullptr)
		{
			return false;
		}

		return _name == rhsActionList->_name &&
			(*this).At("Actions"s) == (*rhsActionList).At("Actions"s);;
	}

	std::string ActionList::ToString() const
	{
		return "ActionList"s;
	}

	gsl::owner<ActionList*> ActionList::Clone() const
	{
		return new ActionList(*this);
	}
}