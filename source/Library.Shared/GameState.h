#pragma once

#include "Action.h"
#include "Vector.h"

namespace FieaGameEngine
{
	class GameState
	{
	public:
		GameState() = default;
		GameState(const GameState&) = delete;
		GameState& operator=(const GameState&) = delete;
		GameState(GameState&&) = delete;
		GameState& operator=(GameState&&) = delete;
		
		static void Add_CreateAction(Scope* parentScope, Scope* childScope);
		static void Add_DeleteAction(Scope* action);
		static void DoActions();

	private:
		inline static Vector<std::pair<Scope*, Scope*>> _createActionList;
		inline static Vector<Scope*> _deleteActionList;
	};
}