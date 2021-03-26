#pragma once

#include <maths/matrix44.h>

//	Represents a marker from the real world. Stores the transform of the mareker and if it is detected or not.
class Marker
{
public:
	Marker();
	inline gef::Matrix44 GetTransform() { return transform_; }
	inline bool IsDetected() { return is_detected_; }
	void SetTransform(gef::Matrix44 transform);
	void SetDetected(bool is_detected);
private:
	gef::Matrix44 transform_;
	bool is_detected_;
};