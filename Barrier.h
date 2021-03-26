#pragma once

#include "GameObject.h"

class Barrier : public GameObject
{
public:
	Barrier(const gef::Mesh* mesh, char* id);
	inline gef::Vector4 GetNormal() { return normal_; }
	void SetNormal(gef::Vector4);
	//	Reflect the incoming velocity to get a post collision direction.
	void CalculateReflectionVector(gef::Vector4 in_direction, gef::Vector4& reflection);
private:
	gef::Vector4 normal_;
};