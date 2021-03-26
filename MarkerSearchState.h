#pragma once
#include "BaseGameState.h"


//	State actuve when marker 1 is not in view.
class MarkerSearchState : public BaseGameState
{
public:
	MarkerSearchState();
	void Update(float delta_time);
	char* GetStateMessage();
	void HandleInput(gef::SonyControllerInputManager* controller_manager);
	std::vector<GameMessage*> GetStateMessages();
	GameState GetCurrentState();
};