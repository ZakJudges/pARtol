#include "Game.h"
#include <cmath>
#include <graphics/mesh_instance.h>
#include "GameMessage.h"


Game::Game() : 
	player_(NULL),
	projectile_(NULL)
{
	ready_to_play_ = false;
}

//	Create all game objects. Return a vector of objects for the game manager to pass to the renderer.
std::vector<GameObject*> Game::InitObjects(PrimitiveBuilder* primitive_builder)
{
	//	Initialise the collision world.
	collision_manager_ = new CollisionManager();

	//	Used to pass meshes and transforms to the renderer.
	std::vector<GameObject*> objects;

	player_ = new Player(primitive_builder->GetDefaultCubeMesh(), "Player");
	objects.push_back(player_);
	collision_manager_->AddObject(player_);

	//	SetIsMarkerObject(false) used to separate rendering of object from marker detection.
	projectile_ = new Projectile(primitive_builder->CreateBoxMesh(gef::Vector4(0.25f, 0.25f, 0.25f)), "Projectile");
	projectile_->SetIsMarkerObject(false);
	projectile_->SetRenderPermission(false);
	objects.push_back(projectile_);
	collision_manager_->AddObject(projectile_);

	Portal* portal1 = new Portal(primitive_builder->GetDefaultSphereMesh(), "Portal", 2);
	objects.push_back(portal1);
	collision_manager_->AddObject(portal1);
	Portal* portal2 = new Portal(primitive_builder->GetDefaultSphereMesh(), "Portal", 3);
	objects.push_back(portal2);
	collision_manager_->AddObject(portal2);

	//	Link the portals together.
	portal1->LinkToPortal(portal2);
	portal2->LinkToPortal(portal1);

	barrier_ = new Barrier(primitive_builder->CreateBoxMesh(gef::Vector4(2.0f, 0.25f, 1.5f)), "Barrier");
	collision_manager_->AddObject(barrier_);
	objects.push_back(barrier_);

	goal_ = new GameObject(primitive_builder->GetDefaultCubeMesh(), "Goal");
	objects.push_back(goal_);
	collision_manager_->AddObject(goal_);
	
	//	Create the level manager.
	level_ = new Level();

	CreateMessages();

	//	Return a container of game objects so that the game manager can pass these to the renderer.
	return objects;
}

void Game::Update(float delta_time)
{
	delta_time_ = delta_time;

	if (level_->SwitchedLevel())
	{
		//	The level is complete, so do not render or monitor projectile collisions.
		projectile_->SetRenderPermission(false);
		//	Create the messages for the current level.
		CreateMessages();
		//	Reset the new level flag.
		level_->SetSwitchedLevel(false);
	}

	//	Update the transform of the game objects every frame so that they will move with the virutal world:

	barrier_->SetParentTransform(player_->GetParentTransform());
	barrier_->SetLocalRotation(level_->GetBarrierRotation());
	barrier_->SetLocalTranslation(level_->GetBarrierPosition());
	barrier_->BuildTransform();
	barrier_->SetNormal(barrier_->GetLocalTransform().GetRow(1));	// Set the normal of the barrier to its y-axis relative to the marker.

	goal_->SetParentTransform(player_->GetParentTransform());
	goal_->SetLocalTranslation(level_->GetGoalPosition());
	goal_->BuildTransform();

	if (collision_manager_->Update())
	{
		//	A collision has occured, determine respones here.	
		//	Note that any objects not being rendered will not trigger collisions.		
		ProjectileCollisions(delta_time);
	}

	//	Update the transform of the projectile if it has been fired.
	if (projectile_->GetRenderPermission())
	{
		projectile_->Update(delta_time, player_->GetParentTransform());
	}
}

void Game::SpawnProjectile()
{
	//	Move the projectile to the players marker, reset its local transform, velocity and associated portal.
	projectile_->SetParentTransform(player_->GetParentTransform());
	projectile_->SetLocalTranslation(gef::Vector4(0.0, 0.0f, 0.0f));
	projectile_->SetVelocity(gef::Vector4(0.0f, 5.0f, 0.0f));
	projectile_->SetPortal(nullptr);
	projectile_->BuildTransform();
	projectile_->SetRenderPermission(true);
	projectile_->SetReflected(false);

}

//	Check the projectile and possible colliding objects with the objects that are currently colliding.
void Game::ProjectileCollisions(float delta_time)
{
	//	Get the colliding objects.
	CollisionPair collision_pair = collision_manager_->GetCollisionPair();

	Portal* portal = (Portal*)collision_pair.object0;
	Projectile* projectile = (Projectile*)collision_pair.object1;
	if (portal && projectile)
	{
		if (portal->GetId() == "Portal" && projectile->GetId() == "Projectile")
		{
			//	object0 is a portal, object1 is a projectile.
			if (projectile_->TeleportCoolDown())
			{
				projectile->SetLocalTranslation(gef::Vector4(0.0f, 0.0f, 0.0f));
				projectile_->SetPortal(portal->GetLinkedPortal());
				projectile_->BuildTransform();
			}
		}
		else
		{
			portal = (Portal*)collision_pair.object1;
			projectile = (Projectile*)collision_pair.object0;
			if (portal && projectile)
			{
				if (portal->GetId() == "Portal" && projectile->GetId() == "Projectile")
				{
					//	object1 is a portal, object0 is a projectile.
					if (projectile_->TeleportCoolDown())
					{
						projectile->SetLocalTranslation(gef::Vector4(0.0f, 0.0f, 0.0f));
						projectile_->SetPortal(portal->GetLinkedPortal());
						projectile_->BuildTransform();
					}
				}
				else
				{
					//	The projectile was not involved with a portal collision. So reset it's teleported flag.
					projectile_->SetTeleported(false);
				}
			}
		}
	}

	Barrier* barrier = (Barrier*)collision_pair.object0; 
	projectile = (Projectile*)collision_pair.object1;
	if (barrier && projectile)
	{
		if (barrier->GetId() == "Barrier" && projectile->GetId() == "Projectile")
		{
			//	object0 is a barrier, object1 is a projectile.
			//	Projectiles velocity is relative to the portal its attached to, so calculate the velocity relative to the player for the reflection vector.
			if (!projectile_->GetReflected())
			{
				gef::Vector4 reflection;
				barrier_->CalculateReflectionVector(projectile_->GetVelocity(), reflection);
				projectile_->SetReflected(true);
				projectile_->SetReflectedDirection(reflection);
			}
		}
		else
		{
			barrier = (Barrier*)collision_pair.object1;
			projectile = (Projectile*)collision_pair.object0;
			if (barrier && projectile)
			{
				if (barrier->GetId() == "Barrier" && projectile->GetId() == "Projectile")
				{
					//	object1 is a barrier, object0 is a projectile.
					if (!projectile_->GetReflected())
					{
						gef::Vector4 reflection;
						barrier_->CalculateReflectionVector(projectile_->GetVelocity(), reflection);
						projectile_->SetReflected(true);
						projectile_->SetReflectedDirection(reflection);
					}
				}
			}
		}
	}

	GameObject* goal = collision_pair.object0;
	projectile = (Projectile*)collision_pair.object1;
	if (goal && projectile)
	{
		if (goal->GetId() == "Goal" && projectile->GetId() == "Projectile")
		{
			//	object0 is the goal, object1 is a projectile.
			level_->NextLevel();
		}
		else
		{
			goal = collision_pair.object1;
			projectile = (Projectile*)collision_pair.object0;
			if (goal && projectile)
			{
				if (goal->GetId() == "Goal" && projectile->GetId() == "Projectile")
				{
					//	object1 is the goal, object0 is a projectile.
					level_->NextLevel();
				}
			}
		}
	}
}

void Game::PlayGame()
{
	ready_to_play_ = true;
}
void Game::ExitToMenu()
{
	level_->Reset();
	ready_to_play_ = false;
}

void Game::CreateMessages()
{
	if (level_->GetLevel() == 1)
	{
		//	Messages for level 1
		game_messages_.clear();

		GameMessage* message0 = new GameMessage;
		message0->position = gef::Vector2(20.0f, 20.0f);
		message0->message = "Marker 1 is where the projectile will be launched from.";
		message0->variable = -1;
		game_messages_.push_back(message0);

		GameMessage* message1 = new GameMessage;
		message1->position = gef::Vector2(20.0f, 50.0f);
		message1->message = "Marker 2 and 3 are portals";
		message1->variable = -1;
		game_messages_.push_back(message1);

		GameMessage* message2 = new GameMessage;
		message2->position = gef::Vector2(20.0f, 80.0f);
		message2->message = "The aim of the game is to hit the goal with your projectile";
		message2->variable = -1;
		game_messages_.push_back(message2);

		GameMessage* message3 = new GameMessage;
		message3->position = gef::Vector2(20.0f, 110.0f);
		message3->message = "Press R2 to fire!";
		message3->variable = -1;
		game_messages_.push_back(message3);
	}
	else if (level_->GetLevel() == 2)
	{
		//	Message for level 2 and 3.
		game_messages_.clear();

		GameMessage* message0 = new GameMessage;
		message0->position = gef::Vector2(20.0f, 20.0f);
		message0->message = "The projectile will bounce off of walls";
		message0->variable = -1;
		game_messages_.push_back(message0);
	}
	else if (level_->GetLevel() > 3)
	{
		//	Game Over message.
		game_messages_.clear();

		GameMessage* message0 = new GameMessage;
		message0->position = gef::Vector2(20.0f, 20.0f);
		message0->message = "Game Complete!";
		message0->variable = -1;
		game_messages_.push_back(message0);

		GameMessage* message1 = new GameMessage;
		message1->position = gef::Vector2(20.0f, 50.0f);
		message1->message = "Well done";
		message1->variable = -1;
		game_messages_.push_back(message1);

		GameMessage* message2 = new GameMessage;
		message2->position = gef::Vector2(20.0f, 80.0f);
		message2->message = "Press start to exit to menu";
		message2->variable = -1;
		game_messages_.push_back(message2);
	}
}

Game::~Game()
{
	if (collision_manager_)
	{
		delete collision_manager_;
		collision_manager_ = nullptr;
	}
}


