#pragma once

#include "BaseGameState.h"
#include <maths/vector2.h>

//	State active when marker 1 is in view and the user has pressed cross at the menu.
class PlayGameState : public BaseGameState
{
public:
	PlayGameState();
	void Update(float delta_time);
	void HandleInput(gef::SonyControllerInputManager* controller_manager);
	std::vector<GameMessage*> GetStateMessages();
	GameState GetCurrentState();
};