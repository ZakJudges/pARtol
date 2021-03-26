#pragma once
#include "Player.h"
#include "primitive_builder.h"
#include "CollisionManager.h"
#include <maths/vector2.h>
#include <vector>
#include <list>
#include "Projectile.h"
#include "Portal.h"
#include "Barrier.h"
#include "GameMessage.h"
#include "Level.h"


//	Class contaning all game objects.
//	This class provides the game manager class with all game objects which get passed to the renderer.
class Game
{
public:
	Game();
	std::vector<GameObject*> InitObjects(PrimitiveBuilder* primitive_builder);
	void Update(float dt);
	void SpawnProjectile();
	void PlayGame();
	void ExitToMenu();
	inline bool IsReady() { return ready_to_play_; }
	inline std::vector<GameMessage*> GetGameMessages() { CreateMessages();  return game_messages_; }
	~Game();
private:
	void ProjectileCollisions(float delta_time);
	void CreateMessages();
private:
	Player* player_;
	CollisionManager* collision_manager_;
	Projectile* projectile_;
	Barrier* barrier_;
	Portal* portal1_;
	Portal* portal2_;
	GameObject* goal_;

	bool ready_to_play_;
	float delta_time_;

	std::vector<GameMessage*> game_messages_;

	Level* level_;
};