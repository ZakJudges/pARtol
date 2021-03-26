#pragma once

#include "GameObject.h"

class Player : public GameObject
{
public:
	Player(const gef::Mesh* mesh, char* id);
};