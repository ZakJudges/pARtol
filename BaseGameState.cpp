#include "BaseGameState.h"

BaseGameState::BaseGameState()
{
	
}

void BaseGameState::SetGame(Game* game)
{
	game_ = game;
}

void BaseGameState::SetMarkerManager(MarkerManager * marker_manager)
{
	marker_manager_ = marker_manager;
}

void BaseGameState::Update(float delta_time)
{
	//	Implemented in derived classes.
}

void BaseGameState::HandleInput(gef::SonyControllerInputManager* controller_manager)
{
	//	Implmented in derived classes.
}

//	Return the messages created by  the dervide class.
std::vector<GameMessage*> BaseGameState::GetStateMessages()
{
	return messages_;
}

//	Return something not valid, since the base game state is not a valid state.
GameState BaseGameState::GetCurrentState()
{
	return (GameState)-1;
}

