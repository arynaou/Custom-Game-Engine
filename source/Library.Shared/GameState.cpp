#include "pch.h"
#include "GameState.h"

namespace FieaGameEngine
{
	void GameState::Add_CreateAction(Scope* parentScope, Scope* childScope)
	{
		_createActionList.PushBack(std::make_pair(parentScope, childScope));
	}

	void GameState::Add_DeleteAction(Scope* action)
	{
		_deleteActionList.PushBack(action);
	}

	void GameState::DoActions()
	{
		for (std::size_t i = 0; i < _createActionList.Size(); ++i)
		{
			auto [pScope, cScope] = _createActionList[i];
			pScope->Adopt(*cScope,"Actions"s);
		}

		for (std::size_t i = 0; i < _deleteActionList.Size(); ++i)
		{
			delete _deleteActionList[i];
		}

		_createActionList.Clear();
		_deleteActionList.Clear();
		_createActionList.ShrinkToFit();
		_deleteActionList.ShrinkToFit();
	}
}