#include "GameManager.h"
#include "GameObject.h"
#include "MarkerManager.h"
#include <input/input_manager.h>
#include <input/sony_controller_input_manager.h>

#include "GameMessage.h"


GameManager::GameManager(gef::Platform& platform) : platform_(platform)
{
	primitive_builder_ = NULL;
	marker_manager_ = NULL;
	input_manager_ = NULL;

	game_ = new Game();
}

void GameManager::Init()
{
	primitive_builder_ = new PrimitiveBuilder(platform_);
	input_manager_ = gef::InputManager::Create(platform_);

	//	Get all of the game objects for the renderer.
	objects_ = game_->InitObjects(primitive_builder_);

	//	Loop through the objects and find the ones set to be rendered with a marker.
	for (int i = 0; i < objects_.size(); i++)
	{
		if (objects_[i]->IsMarkerObject())
		{
			marker_objects_.push_back(objects_[i]);
		}
	}

	marker_manager_ = new MarkerManager();
	marker_manager_->InitMarkers();

	CreateGameStates();

}

//	Passes the transform of markers to the marker manager class.
void GameManager::SetMarkerTransform(int marker, gef::Matrix44 transform)
{
	marker_manager_->SetMarkerTransform(marker, transform);
}

//	Passes the marker deteced Boolean flag for the marker to the marker manager.
void GameManager::SetMarkerDetected(int marker, bool marker_detected)
{
	marker_manager_->SetMarkerDetected(marker, marker_detected);
}

void GameManager::Update(float delta_time)
{
	if (marker_manager_)
	{
		//	Update all objects attached to markers.
		UpdateMarkerObjects();
	}

	//	GAME STATE LOGIC HERE:
	//	Decide which state the game should be in and then update.
	if (game_->IsReady())
	{
		//	The player has started the game, so check for markers.
		if (!marker_manager_->IsPlayerReady())
		{
			//	The player marker is not in view, so pause the game and prompt the user to look at the marker with the camera
			game_state_ = &marker_search_state_;
			//	Get the text to be displayed on screen during this state.
			messages_ = game_state_->GetStateMessages();
		}
		else if (marker_manager_->IsPlayerReady())
		{
			//	The player marker is detected, so update the game as normal.
			game_state_ = &play_game_state_;
			//	Get the text to be displayed on screen during this state.
			messages_ = game_state_->GetStateMessages();
		}
	}
	else
	{
		game_state_ = &menu_state_;
	}
	
	if (input_manager_)
	{
		input_manager_->Update();
		// controller input
		gef::SonyControllerInputManager* controller_manager = input_manager_->controller_input();
		game_state_->HandleInput(controller_manager);
	}

	game_state_->Update(delta_time);
}

void GameManager::UpdateMarkerObjects()
{
	//	Loop through all markers and check against each game object to check if the object should be moved onto the marker.
	for (int i = 1; i < marker_manager_->GetNumberOfMarkers() + 1; i++)
	{
		if (marker_manager_->IsMarkerDetected(i))
		{
			//	Marker detected, so attach a game object set to use this marker.
			for (int j = 0; j < marker_objects_.size(); j++)
			{
				if (marker_objects_[j]->GetTargetMarker() == i)
				{
					marker_objects_[j]->SetRenderPermission(true);
					marker_objects_[j]->AttachToMarker(i, marker_manager_->GetMarkerTransform(i));
					marker_objects_[j]->BuildTransform();
				}
			}
		}
		else
		{
			for (int j = 0; j < marker_objects_.size(); j++)
			{
				//	If this game object has a target marker that is not detected, do not render the game object.
				if (marker_objects_[j]->GetTargetMarker() == i)
				{
					marker_objects_[j]->SetRenderPermission(false);
				}
			}
		}
	}
}

//	Return the mesh of a game object.
gef::MeshInstance* GameManager::GetMesh(int id)
{
	if (objects_.at(id))
	{
		//	If this object should be rendered then return the mesh, else return a null pointer.
		if (objects_.at(id)->GetRenderPermission())
		{
			return objects_.at(id);
		}
		else
		{
			return nullptr;
		}
	}

	return nullptr;

}

void GameManager::CreateGameStates()
{
	game_state_ = new BaseGameState();

	menu_state_.Init(platform_);
	menu_state_.SetGame(game_);
	menu_state_.SetMarkerManager(marker_manager_);

	marker_search_state_.SetGame(game_);
	marker_search_state_.SetMarkerManager(marker_manager_);

	play_game_state_.SetGame(game_);
	play_game_state_.SetMarkerManager(marker_manager_);

	//	Initialise the game state to the menu.
	game_state_ = &menu_state_;
	menu_sprite_ = menu_state_.GetMenuSprite();
}

GameState GameManager::GetGameState()
{
	return game_state_->GetCurrentState();
}

//	Clean up.
GameManager::~GameManager()
{
	if (game_state_)
	{
		delete game_state_;
		game_state_ = nullptr;
	}
	if (marker_manager_)
	{
		delete marker_manager_;
		marker_manager_ = nullptr;
	}
	for (int i = 0; i < objects_.size(); i++)
	{
		if (objects_[i])
		{
			delete objects_[i];
			objects_[i] = nullptr;
		}
	}
	for (int j = 0; j < menu_state_.GetStateMessages().size(); j++)
	{
		if (menu_state_.GetStateMessages()[j])
		{
			delete menu_state_.GetStateMessages()[j];
			menu_state_.GetStateMessages()[j] = nullptr;
		}
	}
	for (int x = 0; x < marker_search_state_.GetStateMessages().size(); x++)
	{
		if (marker_search_state_.GetStateMessages()[x])
		{
			delete marker_search_state_.GetStateMessages()[x];
			marker_search_state_.GetStateMessages()[x] = nullptr;
		}
	}
	for (int y = 0; y < play_game_state_.GetStateMessages().size(); y++)
	{
		if (play_game_state_.GetStateMessages()[y])
		{
			delete play_game_state_.GetStateMessages()[y];
			play_game_state_.GetStateMessages()[y] = nullptr;
		}
	}
	if (game_state_)
	{
		delete game_state_;
		game_state_ = nullptr;
	}
	if (input_manager_)
	{
		delete input_manager_;
		input_manager_ = nullptr;
	}
	if (menu_sprite_)
	{
		delete menu_sprite_;
		menu_sprite_ = nullptr;
	}
	if (primitive_builder_)
	{
		delete primitive_builder_;
		primitive_builder_ = nullptr;
	}
	if (game_)
	{
		delete game_;
		game_ = nullptr;
	}
}
gef::Material GameManager::GetGoalMaterial()
{
	return primitive_builder_->green_material();
}
