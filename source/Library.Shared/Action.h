#pragma once
#include "Attributed.h"
#include "GameTime.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed)

	public:
		/// <summary>
		/// Update the Action of the the function
		/// </summary>
		virtual void Update(const GameTime& gameTime) = 0;

		/// <summary>
		/// Get the name of the action
		/// </summary>
		/// <returns>a string of the name</returns>
		const std::string& Name();

		/// <summary>
		/// Set the name to certain string
		/// </summary>
		/// <param name="name">std::string</param>
		void SetName(const std::string& name);

		/// <summary>
		/// a vector of signature
		/// </summary>
		/// <returns>a vector of signature</returns>
		static const Vector<Signature> Signatures();

	protected:
		Action(std::size_t typeID);

		/// <summary>
		/// the name of the Action
		/// </summary>
		std::string _name;
	};
}