#pragma once

#include "MarkerManager.h"
#include "Game.h"
#include <input/sony_controller_input_manager.h>

#include "GameMessage.h"
#include <vector>

#include "GameState.h"

class BaseGameState
{
public:
	BaseGameState();
	void SetGame(Game* game);
	void SetMarkerManager(MarkerManager* marker_manager);
	virtual void Update(float delta_time);
	virtual void HandleInput(gef::SonyControllerInputManager* controller_manager);
	virtual std::vector<GameMessage*> GetStateMessages();
	virtual GameState GetCurrentState();
protected:
	
protected:
	char* state_message_;
	Game* game_;
	MarkerManager* marker_manager_;

	std::vector<GameMessage*> messages_;
};