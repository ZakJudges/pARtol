#include "Barrier.h"

Barrier::Barrier(const gef::Mesh* mesh, char* id) : GameObject(mesh, id)
{
	SetIsMarkerObject(false);
	//	Normal set to up vector since the markers are placed horizontally.
	normal_ = GetForwardVector();
	
}

void Barrier::SetNormal(gef::Vector4 normal)
{
	normal_ = normal;
}

//	Calculate the reflection vector, given an input direction, and the normal of the barrier.
void Barrier::CalculateReflectionVector(gef::Vector4 in_direction, gef::Vector4& reflection)
{
	float r = 2 * in_direction.DotProduct(normal_);
	reflection = (normal_ * r);
	reflection = in_direction - reflection;
}
