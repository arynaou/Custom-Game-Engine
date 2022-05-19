#pragma once
#include "Action.h"
#include "Factory.h"
#include "TypeManager.h"
#include "GameState.h"

namespace FieaGameEngine
{
	class ActionDestroyAction final : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action)

	public:
		/// <summary>
		/// ActionDestroyAction Constructor
		/// </summary>
		ActionDestroyAction();

		~ActionDestroyAction() = default;
		ActionDestroyAction(const ActionDestroyAction& rhs) = default;
		ActionDestroyAction(ActionDestroyAction && rhs) noexcept = default;
		ActionDestroyAction& operator=(const ActionDestroyAction & rhs) = default;
		ActionDestroyAction& operator=(ActionDestroyAction && rhs) noexcept = default;

		/// <summary>
		/// Update method of Action Destroy Action, will put the function intend to destroy into GameState
		/// </summary>
		/// <param name="gameTime">The game time</param>
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
		/// Get "ActionDestroyAction" as string
		/// </summary>
		/// <returns>ActionDestroyAction string</returns>
		std::string ToString() const override;

		/// <summary>
		/// Create an ActionDestroyAction pointer
		/// </summary>
		/// <returns>a pointer of ActionDestroyAction</returns>
		gsl::owner<ActionDestroyAction*> Clone() const override;

	protected:
		/// <summary>
		/// the intend to delete Action's name
		/// </summary>
		std::string _deleteName{ "" };
	};

	ConcreteFactory(ActionDestroyAction, Scope);
}