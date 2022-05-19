#pragma once
#include "Action.h"
#include "Attributed.h"
#include "GameTime.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)

	public:
		/// <summary>
		/// ActionList Constructor
		/// </summary>
		ActionList();

		virtual ~ActionList() = default;
		ActionList(const ActionList& rhs) = default;
		ActionList& operator=(const ActionList& rhs) = default;
		ActionList(ActionList&& rhs) noexcept = default;
		ActionList& operator=(ActionList&& rhs) noexcept = default;

		/// <summary>
		/// The Update function of ActionList, will Update all Action in this ActionList
		/// </summary>
		/// <param name="gameTime">the game time</param>
		void Update(const GameTime& gameTime);

		/// <summary>
		/// Create an Action by giving the class name and the intended name
		/// </summary>
		/// <param name="className">the Class Name of the action</param>
		/// <param name="name">the name of the action</param>
		void CreateAction(const std::string& className, const std::string& name);

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
		/// Get "ActionList" as string
		/// </summary>
		/// <returns>ActionList string</returns>
		virtual std::string ToString() const override;

		/// <summary>
		/// Create an ActionList pointer
		/// </summary>
		/// <returns>a pointer of ActionList</returns>
		gsl::owner<ActionList*> Clone() const override;

		/// <summary>
		/// A Datum list of Actions inside of this Action List
		/// </summary>
		Datum& Actions {At("Actions")};

	protected:
		ActionList(std::size_t sizeID);
	};

	ConcreteFactory(ActionList, Scope);
}