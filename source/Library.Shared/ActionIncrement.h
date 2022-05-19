#pragma once
#include "Action.h"
#include "Factory.h"
#include "TypeManager.h"

namespace FieaGameEngine
{
	class ActionIncrement final : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action)

	public:
		/// <summary>
		/// ActionIncrement's constructor 
		/// </summary>
		ActionIncrement();

		~ActionIncrement() = default;
		ActionIncrement(const ActionIncrement& rhs) = default;
		ActionIncrement(ActionIncrement && rhs) noexcept = default;
		ActionIncrement& operator=(const ActionIncrement & rhs) = default;
		ActionIncrement& operator=(ActionIncrement && rhs) noexcept = default;

		/// <summary>
		/// The update function of Action Increment, will increment target the amount of step 
		/// </summary>
		/// <param name="gameTime">the game time</param>
		virtual void Update(const GameTime& gameTime) override;

		/// <summary>
		/// Set the Increment Target 
		/// </summary>
		/// <param name="target">the target intend to increase</param>
		void SetTarget(const std::string& target);

		/// <summary>
		/// Set the step
		/// </summary>
		/// <param name="step">the step intend to increase of the object</param>
		void SetStep(float step);

		/// <summary>
		/// Get the target (a string)
		/// </summary>
		/// <returns>the target</returns>
		const std::string& Target();

		/// <summary>
		/// Get the Step (a float)
		/// </summary>
		/// <returns>the step</returns>
		const float Step();

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
		/// Get "ActionIncrement" as string
		/// </summary>
		/// <returns>ActionIncrement string</returns>
		std::string ToString() const override;

		/// <summary>
		/// Create an ActionIncrement pointer
		/// </summary>
		/// <returns>a pointer of ActionIncrement</returns>
		gsl::owner<ActionIncrement*> Clone() const override;

	protected:
		/// <summary>
		/// the intend to increase target, can be integer or float
		/// </summary>
		std::string _target{""};

		/// <summary>
		/// the step of how much it increased
		/// </summary>
		float _step{1.0f};

	private :
		Datum* foundDatum{ nullptr };
	};

	ConcreteFactory(ActionIncrement, Scope);
}