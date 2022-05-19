#include "pch.h"
#include "GameObject.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(GameObject);

	GameObject::GameObject():
		Attributed(GameObject::TypeIdClass())
	{
	}

	GameObject::GameObject(std::size_t sizeID) :
		Attributed(sizeID)
	{
	}

	gsl::owner<GameObject*> GameObject::Clone() const
	{
		return new GameObject();
	}

	bool GameObject::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr)
		{
			return false;
		}

		const GameObject* const rhsGameObject = rhs->As<GameObject>();

		return Name == rhsGameObject->Name &&
			Transform.Position == rhsGameObject->Transform.Position &&
			Transform.Rotation == rhsGameObject->Transform.Rotation &&
			Transform.Scale == rhsGameObject->Transform.Scale &&
			(*this).At("Children"s) == (*rhsGameObject).At("Children"s)&&
			(*this).At("Actions"s) == (*rhsGameObject).At("Actions"s);
	}

	std::string GameObject::ToString() const
	{
		return Name;
	}

	const Vector<Signature> GameObject::Signatures()
	{
		return Vector<Signature>
		{
			{ "Position"s, DatumTypes::Vectors, 1, offsetof(GameObject, Transform.Position) },
			{ "Rotation"s, DatumTypes::Vectors, 1, offsetof(GameObject,Transform.Rotation) },
			{ "Scale"s, DatumTypes::Vectors, 1, offsetof(GameObject,Transform.Scale) },
			{ "Name"s, DatumTypes::String,1,offsetof(GameObject,Name) },
			{ "Children"s, DatumTypes::Table, 0, 0 },
			{ "Actions"s, DatumTypes::Table,0,0}
		};
	}

	void GameObject::Update(const GameTime& gameTime)
	{		
		for (size_t i = 0; i < Children.Size(); ++i)
		{
			GameObject* gameObject = Children.Get<Scope*>(i)->As<GameObject>();
			if (gameObject != nullptr)
			{
				gameObject->Update(gameTime);
			}
		}

		for (size_t i = 0; i < Actions.Size(); ++i)
		{
			Action* action = Actions.Get<Scope*>(i)->As<Action>();
			if (action != nullptr)
			{
				action->Update(gameTime);
			}
		}
	}

	Datum& GameObject::GetActions()
	{
		return Actions;
	}

	void GameObject::CreateAction(const std::string& className, const std::string& name)
	{
		Scope* scope = Factory<Scope>::Create(className);

		if (scope != nullptr)
		{
			scope->As<Action>()->SetName(name);
			Actions.PushBack(scope);
		}
	}
}