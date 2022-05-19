#pragma once
#include "ActionList.h"
#include "GameTime.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class ActionListIf final : public ActionList
	{
		RTTI_DECLARATIONS(ActionListIf, ActionList)

	public:
		/// <summary>
		/// Action List If Constructor
		/// </summary>
		ActionListIf();
		~ActionListIf() = default;
		ActionListIf(const ActionListIf& rhs) = default;
		ActionListIf& operator=(const ActionListIf & rhs) = default;
		ActionListIf(ActionListIf && rhs) noexcept = default;
		ActionListIf& operator=(ActionListIf && rhs) noexcept = default;

		/// <summary>
		/// The Update function of Action List if, the first action will be false, the second will be true
		/// </summary>
		/// <param name="gameTime">the game time</param>
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
		virtual bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Get "ActionListIf" as string
		/// </summary>
		/// <returns>ActionListIf string</returns>
		virtual std::string ToString() const override;

		/// <summary>
		/// Create an ActionListIf pointer
		/// </summary>
		/// <returns>a pointer of ActionListIf</returns>
		gsl::owner<ActionListIf*> Clone() const override;

		/// <summary>
		/// The Condition, 0 be false, 1 be true
		/// </summary>
		int32_t Condition{0};
	};

	ConcreteFactory(ActionListIf, Scope);
}