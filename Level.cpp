#include "Level.h"

Level::Level()
{
	current_level_ = 1;
	switched_level_ = true;
}

//	Return positions/rotations for game objects that get placed relative to marker 1, based on the current level.

gef::Vector4 Level::GetBarrierPosition()
{
	if (current_level_ == 1)
	{
		barrier_position_ = gef::Vector4(-90.0f, 8.0f, 0.0f);
		return barrier_position_;
	}
	else if (current_level_ == 2)
	{
		barrier_position_ = gef::Vector4(0.5f, 8.0f, 0.0f);
		return barrier_position_;

	}
	else if (current_level_ == 3)
	{
		barrier_position_ = gef::Vector4(0.0f, 8.0f, 0.0f);
		return barrier_position_;

	}

	barrier_position_ = gef::Vector4(-90.0f, 8.0f, 0.0f);

	return barrier_position_;
}


gef::Vector4 Level::GetBarrierRotation()
{
	if (current_level_ == 1)
	{
		barrier_rotation_ = gef::Vector4(0.0f, 0.0f, 30.0f);
		return barrier_rotation_;
	}
	else if (current_level_ == 2)
	{
		barrier_rotation_ = gef::Vector4(0.0f, 0.0f, 30.0f);
		return barrier_rotation_;
	}
	else if (current_level_ == 3)
	{
		barrier_rotation_ = gef::Vector4(0.0f, 0.0f, 0.0f);
		return barrier_rotation_;
	}

	return gef::Vector4();
}

gef::Vector4 Level::GetGoalPosition()
{

	if (current_level_ == 1)
	{
		goal_position_ = gef::Vector4(4.0f, 4.0f, 0.0f);
		return goal_position_;
	}
	else if (current_level_ == 2)
	{
		goal_position_ = gef::Vector4(7.5f, 0.0f, 0.0f);
		return goal_position_;
	}
	else if (current_level_ == 3)
	{
		goal_position_ = gef::Vector4(2.0f, 0.0f, 0.0f);
		return goal_position_;
	}

	goal_position_ = gef::Vector4(90.0f, -90.0f, 0.0f);


	return goal_position_;
}

void Level::NextLevel()
{
	if (!switched_level_)
	{
		current_level_++;
		switched_level_ = true;
	}
}

void Level::SetSwitchedLevel(bool switched)
{
	switched_level_ = switched;
}
