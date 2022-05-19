#include "pch.h"
#include "ActionIncrement.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionIncrement);

	ActionIncrement::ActionIncrement() :
		Action(ActionIncrement::TypeIdClass())
	{
	}

	void ActionIncrement::SetTarget(const std::string& target)
	{
		_target = target;
	}

	void ActionIncrement::SetStep(const float step)
	{
		_step = step;
	}

	const std::string& ActionIncrement::Target()
	{
		return _target;
		
	}

	const float ActionIncrement::Step()
	{
		return _step;
	}

	void ActionIncrement::Update(const GameTime&)
	{
		if (foundDatum == nullptr)
		{
			foundDatum = Search(_target);
		}
		if (foundDatum == nullptr) return;

		if (foundDatum->Type() == DatumTypes::Integer)
		{
			foundDatum->Get<int32_t>(0) += (int32_t)_step;
		}
		else if (foundDatum->Type() == DatumTypes::Float)
		{
			foundDatum->Get<float>(0) +=_step;
		}
	}

	const Vector<Signature> ActionIncrement::Signatures()
	{
		return Vector<Signature>
		{
			{ "Target"s, DatumTypes::String, 1, offsetof(ActionIncrement, _target) },
			{"Step"s, DatumTypes::Float,1,offsetof(ActionIncrement, _step)}
		};
	}

	bool ActionIncrement::Equals(const RTTI* rhs) const
	{
		const ActionIncrement* const rhsActionIncrement = rhs->As<ActionIncrement>();
		if (rhsActionIncrement == nullptr)
		{
			return false;
		}

		return _name == rhsActionIncrement->_name &&
			_target == rhsActionIncrement->_target &&
			_step == rhsActionIncrement->_step;
	}

	std::string ActionIncrement::ToString() const
	{
		return "ActionIncrement"s;
	}

	gsl::owner<ActionIncrement*> ActionIncrement::Clone() const
	{
		return new ActionIncrement(*this);
	}
}