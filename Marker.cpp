#include "Marker.h"

Marker::Marker()
{
	is_detected_ = false;
	transform_.SetIdentity();
}

void Marker::SetDetected(bool is_detected)
{
	is_detected_ = is_detected;
}

void Marker::SetTransform(gef::Matrix44 transform)
{
	transform_ = transform;
}
