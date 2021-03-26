#include "GameObject.h"
#include <maths/math_utils.h>
#include <graphics/mesh.h>

GameObject::GameObject(const gef::Mesh* mesh, char* id)
{
	set_mesh(mesh);
	id_ = id;

	//	Initialise local transform.
	l_rotation_.SetIdentity();
	l_translation_.SetIdentity();
	l_transform_.SetIdentity();

	//	Initialise world transform.
	gef::Matrix44 new_transform;
	new_transform.SetIdentity();
	set_transform(new_transform);
	w_position_ = gef::Vector4(0.0f, 0.0f, 0.f);
	parent_transform_.SetIdentity();

	render_ = true;
	is_marker_object_ = true;
}

void GameObject::AttachToMarker(int marker_num, gef::Matrix44 marker_transform)
{
	//	Make this game objects transform update relative to the marker transform.
	marker_ = marker_num;
	SetParentTransform(marker_transform);
}

//	The marker this object should attach to.
void GameObject::SetTargetMarker(int marker)
{
	marker_ = marker;
}

//	If this game object should be rendered on a marker or not.
void GameObject::SetIsMarkerObject(bool is_marker_object)
{
	is_marker_object_ = is_marker_object;
}

//	Combine rotations on x, y and z axis to get this objects local rotation.
void GameObject::SetLocalRotation(gef::Vector4 rotation)
{
	gef::Matrix44 x_rot, y_rot, z_rot;
	x_rot.SetIdentity();
	y_rot.SetIdentity();
	z_rot.SetIdentity();

	x_rot.RotationX(gef::DegToRad(rotation.x()));
	y_rot.RotationY(gef::DegToRad(rotation.y()));
	z_rot.RotationZ(gef::DegToRad(rotation.z()));

	l_rotation_ = x_rot * y_rot * z_rot;

	//	Store the local translation of rhis object so we can apply the rotation to the local transform, then translate back to the previous position.
	gef::Vector4 translation_store = l_translation_.GetTranslation();
	l_transform_ = l_rotation_;
	l_transform_.SetTranslation(translation_store);
}

void GameObject::SetLocalRotation(gef::Matrix44 rotation)
{
	l_rotation_ = rotation;
	gef::Vector4 translation_store = l_translation_.GetTranslation();
	l_transform_ = l_rotation_;
	l_transform_.SetTranslation(translation_store);
}


void GameObject::SetLocalTranslation(gef::Vector4 translation)
{
	l_translation_.SetTranslation(translation);
	l_transform_.SetTranslation(l_translation_.GetTranslation());
}

void GameObject::BuildTransform()
{
	//	Rotate then translate to build this objects local transform.
	//gef::Matrix44 new_transform;
	//new_transform = l_rotation_ * l_translation_;
	//l_transform_ = new_transform;

	//	Multiply by the parent transform to get this object world transform.
	gef::Matrix44 world_transform = l_transform_ * parent_transform_;
	//	Set the transform of the mesh instance.
	set_transform(world_transform);

	//	Set this objects world position.
	w_position_ = world_transform.GetTranslation();
}


void GameObject::SetParentTransform(gef::Matrix44& parent_transform)
{
	parent_transform_ = parent_transform;
	
}

void GameObject::SetLocalTransform(gef::Matrix44 transform)
{
	l_transform_ = transform;
}

//	Returns the forward vector for this object in world space.
//		Actually returns the up vector, but since the markers are positioned horizontally, up becomes forward.
gef::Vector4 GameObject::GetForwardVector()
{
	return gef::Vector4(transform().GetRow(1).x(), transform().GetRow(1).y(), transform().GetRow(1).z());
}
 