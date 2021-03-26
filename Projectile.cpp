#include "Projectile.h"
#include "Portal.h"


Projectile::Projectile(const gef::Mesh* mesh, char* id) : GameObject(mesh, id)
{
	//	This game object's rendering should not be controlled by markers being detected, so set it's target marker to a value no associated with any markers.
	SetTargetMarker(-1);
	portal_ = NULL;
	velocity_ = gef::Vector4(0.0f, 5.0f, 0.0f);
	original_direction_ = velocity_;
	reflected_direction_ = original_direction_;
	reflected_ = false;
	teleported_ = false;

}

//	Boolean flag to check if the projectile has teleported after entering a collider.
//	Means the projectile will only teleport once when entering a portal, and can only teleport again after leavig the collider (set in gmae class).
bool Projectile::TeleportCoolDown()
{
	if (!teleported_)
	{
		teleported_ = true;
		return true;
	}
	else if (teleported_)
	{
		return false;
	}
}

void Projectile::SetTeleported(bool teleported)
{
	teleported_ = teleported;
}

void Projectile::Update(float delta_time, gef::Matrix44 player_transform)
{
	SetParentTransform(player_transform);

	//	Update the projectiles parent transform to prevent the projectile changing positions based on movement of the camera.
	//	Then calculate the direction the projectile should travel in, based on the current portal it is attached to, or the direction after hitting a barrier.
	Portal* portal = GetPortal();
	if (portal && !reflected_)
	{
		//	Set the projectile to travel relative to the portal instead of the player.
		SetParentTransform(portal->GetParentTransform());
		//	The projectile should still travel in the original direction, so calculate that direction in the portal's local space:
		//	First, get the inverse transform of the marker attached to the portal.
		gef::Matrix44 inv_portal_transform;
		inv_portal_transform.AffineInverse(portal->GetParentTransform());
		//	Next, get the transform of the player in local space to the portal.
		gef::Matrix44 player_local_to_portal = player_transform * inv_portal_transform;

		//	Y-axis of player local to portal is now the original direction of the projectile.
		gef::Vector4 original_direction = player_local_to_portal.GetRow(1);
		//	Normalise so the projectiles speed can be easily set.
		original_direction.Normalise();

		SetVelocity(original_direction * 5.0f);
		original_direction_ = original_direction * 5.0f;
	}
	else if (portal && reflected_)
	{
		//	Set the projectile to travel relative to the portal instead of the player.
		SetParentTransform(portal->GetParentTransform());

		//	The projectile should still travel in its original reflected direction, regardless of marker orientation.
		//	First, get the inverse transform of the marker attached to the linked portal.
		gef::Matrix44 inv_portal_transform;
		inv_portal_transform.AffineInverse(portal->GetParentTransform());
		
		//	Transform the original reflected vector by the inverse of the portal transform to keep the orientation of the reflection vector.
		gef::Vector4 original_direction = reflected_direction_.Transform(inv_portal_transform);

		//	Store the z-axis of the prjectile prior to normilisation.
		float z_store;
		z_store = original_direction.z();
		//	Normalise so the projectiles speed can be essiy set.
		original_direction.Normalise();
		original_direction.set_z(z_store);

		SetVelocity(original_direction * 5.0f);
		original_direction_ = original_direction * 5.0f;

	}
	else if (!portal && reflected_)
	{
		SetVelocity(reflected_direction_);
	}

	SetLocalTranslation(GetLocalTranslation() + (GetVelocity() * delta_time));

	BuildTransform();
}

void Projectile::SetPortal(Portal* portal)
{
	portal_ = portal;
}

void Projectile::SetVelocity(gef::Vector4 velocity)
{
	velocity_ = velocity;
}

void Projectile::SetReflectedDirection(gef::Vector4 direction)
{
	reflected_direction_ = direction;
}

void Projectile::SetReflected(bool reflected)
{
	reflected_ = reflected;
}

