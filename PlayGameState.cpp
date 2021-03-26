#include "PlayGameState.h"

PlayGameState::PlayGameState()
{
}

void PlayGameState::Update(float delta_time)
{
	game_->Update(delta_time);
	
}


//	Handle input based on the current state that we are in.
void PlayGameState::HandleInput(gef::SonyControllerInputManager* controller_manager)
{
	if (controller_manager)
	{
		const gef::SonyController* controller = controller_manager->GetController(0);
		float left_x = controller->left_stick_x_axis();
		float left_y = controller->left_stick_y_axis();
	
		if (controller->buttons_pressed() & gef_SONY_CTRL_R2)
		{
			//	Fire a projectile in the direction that the player is currently rotated in.
			game_->SpawnProjectile();

		}
		if (controller->buttons_pressed() & gef_SONY_CTRL_START)
		{
			game_->ExitToMenu();

		}
	}
}

std::vector<GameMessage*> PlayGameState::GetStateMessages()
{
	messages_ = game_->GetGameMessages();
	return messages_;
}

GameState PlayGameState::GetCurrentState()
{
	return GameState::PLAY_GAME;
}
