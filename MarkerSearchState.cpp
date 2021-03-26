#include "MarkerSearchState.h"
#include <input/sony_controller_input_manager.h>

MarkerSearchState::MarkerSearchState()
{
	GameMessage* marker_message = new GameMessage();
	marker_message->position = gef::Vector2(250.0f, 250.0f);
	marker_message->message = "Please ensure marker 1 is in view";
	marker_message->variable = -1;
	messages_.push_back(marker_message);
}


void MarkerSearchState::Update(float delta_time)
{
	
}

char* MarkerSearchState::GetStateMessage()
{
	return state_message_;
}

void MarkerSearchState::HandleInput(gef::SonyControllerInputManager* controller_manager)
{
	if (controller_manager)
	{
		const gef::SonyController* controller = controller_manager->GetController(0);
		if (controller->buttons_pressed() & gef_SONY_CTRL_START)
		{
			game_->ExitToMenu();
		}
	}
}


std::vector<GameMessage*> MarkerSearchState::GetStateMessages()
{
	return messages_;
}

GameState MarkerSearchState::GetCurrentState()
{
	return GameState::MARKER_SEARCH;
}
