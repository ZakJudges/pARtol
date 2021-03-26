#pragma once

#include <list>
#include <vector>

class GameObject;
class Projectile;
class Barrier;
class Portal;

namespace gef
{
	class Mesh;
}

struct CollisionPair
{
	GameObject* object0;
	GameObject* object1;
};

//	Class that monitors collisions on objects that are being rendered.
//		Stores collision pairs, which the game then uses to determine appropriate responses.
class CollisionManager
{

public:
	
	void AddObject(GameObject* game_object);
	void RemoveObject(GameObject* game_object);
	bool Update();
	inline CollisionPair GetCollisionPair() { return collision_pair_; }

private:
	//	The list of objects with collisions enabled.
	std::list<GameObject*> world_objects_;
	CollisionPair collision_pair_;

	bool AABBToAABBCollision(GameObject* object0, GameObject* object1);
	bool SphereToSphereCollision(GameObject* object0, GameObject* object1);
	bool SphereToAABBCollision(GameObject* object0, GameObject* object1);
};