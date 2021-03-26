#include "CollisionManager.h"
#include "Projectile.h"
#include "Portal.h"
#include "Barrier.h"

#include <graphics/mesh.h>

void CollisionManager::AddObject(GameObject* game_object)
{
	world_objects_.push_back(game_object);
}

void CollisionManager::RemoveObject(GameObject* game_object)
{
	//	Get a pointer to each game object and test against the object to be removed from the list.
	for (std::list<GameObject*>::iterator it = world_objects_.begin(); it != world_objects_.end(); ++it)
	{
		if (game_object == *it)
		{
			world_objects_.remove(*it);
		}
	}
}

bool CollisionManager::Update()
{
	bool collided = false;

	//	Compare each game object with the rest of the game objects to check for collisions.
	for (std::list<GameObject*>::iterator this_object = world_objects_.begin(); this_object != world_objects_.end(); ++this_object)
	{
		GameObject* object0 = *this_object;
		for (std::list<GameObject*>::iterator other_object = world_objects_.begin(); other_object != world_objects_.end(); ++other_object)
		{
			//	Make sure the object does not collide with itself ( may need fixed to be more efficient).
			if (other_object != this_object)
			{
				GameObject* object1 = *other_object;

				//	Detect what type of collision to check for.
				//		Sphere to sphere, AABB to AABB or sphere to AABB.

				//	Detect the collision, if the objects are being rendered.
				if (object0->GetRenderPermission() && object1->GetRenderPermission())
				{
					if (AABBToAABBCollision(object0, object1))
					{
						//collision_message_ = object0.Id();
						collided = true;
						collision_pair_.object0 = object0;
						collision_pair_.object1 = object1;
					}
				}
			}
		}
	}

	return collided;
}


bool CollisionManager::SphereToSphereCollision(GameObject* object0, GameObject* object1)
{

	gef::Vector4 pos0, pos1, dist;
	float r0, r1, r_sum, sq_r_sum, sq_dist;

	//	Get the centre of each bounding sphere by translating them to world space. 
	pos0 = object0->mesh()->bounding_sphere().Transform(object0->transform()).position();
	pos1 = object1->mesh()->bounding_sphere().Transform(object1->transform()).position();
	//	Get the radius of each bounding sphere.
	r0 = object0->mesh()->bounding_sphere().radius();
	r1 = object1->mesh()->bounding_sphere().radius();

	//	Get a vector representing the distance between the centres of the two spheres.
	dist = pos1 - pos0;
	//	For efficiency, get the squared distance between the two spheres.
	sq_dist = dist.LengthSqr();
	//	Get the sum of the radii of the two spheres.
	r_sum = r0 + r1;
	//	For efficiency, get the squared sum of the radii of the two spheres.
	sq_r_sum = r_sum * r_sum;

	if (sq_dist <= sq_r_sum)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool CollisionManager::SphereToAABBCollision(GameObject* object0, GameObject* object1)
{
	return false;
}

bool CollisionManager::AABBToAABBCollision(GameObject* object0, GameObject* object1)
{
	gef::Vector4 max0, min0, max1, min1;

	//	Get the AABBs min and max vectors by transforming to world space.
	max0 = object0->mesh()->aabb().Transform(object0->transform()).max_vtx();
	min0 = object0->mesh()->aabb().Transform(object0->transform()).min_vtx();
	max1 = object1->mesh()->aabb().Transform(object1->transform()).max_vtx();
	min1 = object1->mesh()->aabb().Transform(object1->transform()).min_vtx();

	if (max0.x() > min1.x() && min0.x() < max1.x() && max0.y() > min1.y() && min0.y() < max1.y() && max0.z() > min1.z() && min0.z() < max1.z())
	{
		return true;
	}
	else
	{
		return false;
	}
}