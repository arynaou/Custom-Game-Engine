#pragma once

#include "GameObject.h"
#include "Vector.h"
#include "TypeManager.h"

using namespace FieaGameEngine;

namespace UnitTests
{
	class Avatar final :public FieaGameEngine::GameObject
	{
		RTTI_DECLARATIONS(Avatar, FieaGameEngine::GameObject);

	public:
		/// <summary>
		/// Avatar constructor
		/// </summary>
		Avatar();

		/// <summary>
		/// a test int will plus one after Update called
		/// </summary>
		std::size_t TestInt{0};

		/// <summary>
		/// Clone the avatar
		/// </summary>
		/// <returns>An avatar pointer</returns>
		gsl::owner<Avatar*> Clone() const override;

		/// <summary>
		/// an update function, will invoke all the children update and plus one to testInt 
		/// </summary>
		/// <param name="gametime">game time</param>
		virtual void Update(const GameTime& gametime) override;

		/// <summary>
		/// return the Game Object Name
		/// </summary>
		/// <returns>the name of Avatar</returns>
		std::string ToString() const override;

		/// <summary>
		/// Get the Signature of the avatar
		/// </summary>
		/// <returns>a vector of signature</returns>
		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	};

	ConcreteFactory(Avatar, FieaGameEngine::Scope);
}