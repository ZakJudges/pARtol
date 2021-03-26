#pragma once

#include <platform/vita/graphics/texture_vita.h>
#include <graphics/sprite.h>

#include "BaseGameState.h"

//	State active at start up/ when the user presses start.
class MenuState : public BaseGameState
{
public:
	void Init(gef::Platform& platform);
	void Update(float delta_time);
	void HandleInput(gef::SonyControllerInputManager* controller_manager);
	std::vector<GameMessage*> GetStateMessages();
	inline gef::Sprite* GetMenuSprite() { return menu_; }
	GameState GetCurrentState();
	~MenuState();
private:
	std::vector<GameMessage*> messages_;

	gef::Sprite* menu_;
	gef::Texture* menu_texture_;
};
