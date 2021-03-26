#pragma once

#include "GameObject.h"

class Portal : public GameObject
{
public:
	Portal (const gef::Mesh* mesh, char* id, int marker);
	void LinkToPortal(Portal* portal);
	Portal* GetLinkedPortal();
private:
	Portal* linked_portal_;
};