
#pragma once
#include <vector>
#include <maths/matrix44.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include "GameObject.h"
#include "Marker.h"
#include "MarkerSearchState.h"
#include "PlayGameState.h"
#include "Game.h"
#include "MenuState.h"



namespace gef
{
	class Platform;
	class InputManager;
}

class MarkerManager;
class GameMessage;

class GameManager
{
public:
	GameManager(gef::Platform& platform);
	void Init();
	void Update(float dt);
	gef::MeshInstance* GetMesh(int id);
	inline MarkerManager* GetMarkerManager() { return marker_manager_; }
	void SetMarkerTransform(int marker, gef::Matrix44 transform);
	void SetMarkerDetected(int marker, bool marker_detected);
	inline int GetNumObjects() { return objects_.size(); }
	inline std::vector<GameMessage*> GetMessages() { return messages_; }
	inline gef::Sprite* GetMenuSprite() { return menu_sprite_; }
	gef::Material GetGoalMaterial();
	GameState GetGameState();
	~GameManager();
private:
	void UpdateMarkerObjects();
	void CreateGameStates();
private:
	MarkerManager* marker_manager_;
	std::vector<GameObject*> objects_;
	std::vector<GameObject*> marker_objects_;
	PrimitiveBuilder* primitive_builder_;
	gef::Platform& platform_;

	//	Game state machine.
	BaseGameState* game_state_;
	MenuState menu_state_;
	MarkerSearchState marker_search_state_;
	PlayGameState play_game_state_;

	//Input input_;
	gef::InputManager* input_manager_;

	Game* game_;

	std::vector<GameMessage*> messages_;

	gef::Sprite* menu_sprite_;
};