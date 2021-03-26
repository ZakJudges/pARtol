#pragma once

#include "GameObject.h"

class Portal;

class Projectile : public GameObject
{
	
public:
	Projectile(const gef::Mesh* mesh, char* id);
	bool TeleportCoolDown();
	inline Portal* GetPortal() { return portal_; }
	void SetPortal(Portal* portal);
	void SetVelocity(gef::Vector4 v);
	void Update(float delta_time, gef::Matrix44 player_transform);
	inline gef::Vector4 GetVelocity() { return velocity_; }
	inline gef::Vector4 GetOriginalDirection() { return original_direction_; }
	void SetReflectedDirection(gef::Vector4 direction);
	inline gef::Vector4 GetReflectedDirection() { return reflected_direction_; }
	void SetReflected(bool);
	inline bool GetReflected() { return reflected_; }
	void SetTeleported(bool);

private:
	Portal* portal_;
	gef::Vector4 velocity_;
	//	The original direction travelled, but in the local space of the last portal.
	gef::Vector4 original_direction_;
	gef::Vector4 reflected_direction_;
	bool reflected_;
	bool teleported_;

};