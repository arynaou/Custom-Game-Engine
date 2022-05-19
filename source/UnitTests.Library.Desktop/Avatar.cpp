#include "pch.h"
#include "Avatar.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(Avatar);

	Avatar::Avatar() :
		GameObject(Avatar::TypeIdClass())
	{
	}

	gsl::owner<Avatar*> Avatar::Clone() const
	{
		return new Avatar(*this);
	}

	Vector<Signature> Avatar::Signatures()
	{
		return Vector<Signature>
		{
			{"TestInt"s, DatumTypes::Integer,1, offsetof(Avatar,TestInt)}
		};
	}

	std::string Avatar::ToString() const
	{
		return Name;
	}

	void Avatar::Update(const GameTime& gameTime)
	{
		GameObject::Update(gameTime);
		++TestInt;
	}
}