#include "Portal.h"

Portal::Portal(const gef::Mesh* mesh, char* id, int marker) : GameObject(mesh, id)
{
	marker_ = marker;
	linked_portal_ = NULL;
}

//	Store the portal that this portal is linked to.
void Portal::LinkToPortal(Portal* portal)
{
	linked_portal_ = portal;
}

Portal* Portal::GetLinkedPortal()
{
	return linked_portal_;
}
