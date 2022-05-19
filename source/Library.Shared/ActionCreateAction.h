#pragma once
#include "Action.h"
#include "Factory.h"
#include "TypeManager.h"
#include "GameState.h"

namespace FieaGameEngine
{
	class ActionCreateAction final: public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)

	public:
		/// <summary>
		/// the constructor of ActionCreateAction
		/// </summary>
		ActionCreateAction();

		~ActionCreateAction() = default;
		ActionCreateAction(const ActionCreateAction& rhs) = default;
		ActionCreateAction(ActionCreateAction&& rhs) noexcept = default;
		ActionCreateAction& operator=(const ActionCreateAction& rhs) = default;
		ActionCreateAction& operator=(ActionCreateAction&& rhs) noexcept = default;

		/// <summary>
		/// Update method of Action Create Action, will create the functions and put it into GameState
		/// </summary>
		/// <param name="gameTime">the Game time</param>
		virtual void Update(const GameTime& gameTime) override;

		/// <summary>
		/// a vector of signature
		/// </summary>
		/// <returns>a vector of signature</returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Compare if it's equal to RTTI pointer
		/// </summary>
		/// <param name="rhs">RTTI pointer</param>
		/// <returns>if it's equal or not</returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Get "ActionCreateAction" as string
		/// </summary>
		/// <returns>ActionCreateAction string</returns>
		std::string ToString() const override;

		/// <summary>
		/// Create an ActionCreateAction pointer
		/// </summary>
		/// <returns>a pointer of ActionCreateAction</returns>
		gsl::owner<ActionCreateAction*> Clone() const override;

	protected:
		/// <summary>
		/// the name of Action class to make
		/// </summary>
		std::string _prototype{ "" };

		/// <summary>
		/// the name to give the new Action
		/// </summary>
		std::string _createName{ "" };

	};

	ConcreteFactory(ActionCreateAction, Scope);
}