#pragma once

#include <maths/vector4.h>

//	Simple level class that returns the positions and orientations of level specific objects.
class Level
{
public:
	Level();
	inline int GetLevel() { return current_level_; }
	gef::Vector4 GetBarrierPosition();
	gef::Vector4 GetBarrierRotation();
	gef::Vector4 GetGoalPosition();
	void NextLevel();
	inline bool SwitchedLevel() { return switched_level_; }
	void SetSwitchedLevel(bool );
	inline void Reset() { current_level_ = 1; }
private:
	int current_level_;
	gef::Vector4 barrier_position_;
	gef::Vector4 barrier_rotation_;
	gef::Vector4 goal_position_;
	bool switched_level_;
};