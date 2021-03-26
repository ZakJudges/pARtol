#pragma once

#include <maths/vector2.h>

struct GameMessage
{
	gef::Vector2 position;
	char* message;
	float variable;
};