#include "MenuState.h"
#include <assets/png_loader.h>
#include <graphics/image_data.h>

void MenuState::Init(gef::Platform& platform) 
{
	//	Create the menu sprites texture:

	gef::PNGLoader png_loader;
	gef::ImageData image_data;
	gef::Texture* texture = NULL;
	// load image data from PNG file
	png_loader.Load("Menu.png", platform, image_data);
	// if the image data is valid, then create a texture from it
	if (image_data.image() != NULL) {
		menu_texture_ = gef::Texture::Create(platform, image_data);
	}

	menu_ = new gef::Sprite();
	menu_->set_texture(menu_texture_);
	menu_->set_position(gef::Vector4(0.0f, 0.0f, 1.0f));
	menu_->set_width(2);
	menu_->set_height(2);


}

void MenuState::Update(float delta_time)
{
}

void MenuState::HandleInput(gef::SonyControllerInputManager* controller_manager)
{
	if (controller_manager)
	{
		const gef::SonyController* controller = controller_manager->GetController(0);
		if (controller->buttons_pressed() & gef_SONY_CTRL_CROSS)
		{
			game_->PlayGame();
		}
	}
}

std::vector<GameMessage*> MenuState::GetStateMessages()
{
	return std::vector<GameMessage*>();
}

GameState MenuState::GetCurrentState()
{
	return GameState::MENU;
}

MenuState::~MenuState()
{
	if (menu_texture_)
	{
		delete menu_texture_;
		menu_texture_ = nullptr;
	}

	if (menu_)
	{
		delete menu_;
		menu_ = nullptr;
	}
}