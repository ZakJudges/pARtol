#pragma once

#include <graphics/mesh_instance.h>
#include <maths/vector4.h>
#include <maths/matrix44.h>

namespace gef
{
	class Mesh;
}

class GameObject : public gef::MeshInstance
{
public:
	GameObject(const gef::Mesh* mesh, char* id);
	
	//	Local transform.
	inline gef::Matrix44 GetLocalTransform() { return l_transform_; }
	inline gef::Matrix44 GetLocalRotation() { return l_rotation_; }
	inline gef::Vector4 GetLocalTranslation() { return l_translation_.GetTranslation(); }
	inline gef::Vector4 GetWorldPosition() { return w_position_; }
	void SetLocalRotation(gef::Vector4);
	void SetLocalRotation(gef::Matrix44);
	void SetLocalTranslation(gef::Vector4);
	void SetLocalTransform(gef::Matrix44 transform);

	//	World transform.
	void BuildTransform();
	void SetParentTransform(gef::Matrix44& parent_transform);
	inline gef::Matrix44& GetParentTransform() { return parent_transform_; }
	gef::Vector4 GetForwardVector();

	//	Rendering.
	inline bool GetRenderPermission() { return render_; }
	inline void SetRenderPermission(bool render) { render_ = render; }

	//	Markers.
	inline int GetTargetMarker() { return marker_; }
	inline bool IsMarkerObject() { return is_marker_object_; }
	void SetIsMarkerObject(bool);
	void SetTargetMarker(int marker);
	void AttachToMarker(int marker_num, gef::Matrix44 marker_transform);

	inline char* GetId() { return id_; }

protected:
	gef::Matrix44 l_transform_;
	gef::Matrix44 l_rotation_;
	gef::Matrix44 l_translation_;
	gef::Vector4 w_position_;
	gef::Matrix44 parent_transform_;

	//	If this object should be rendered or not.
	bool render_;
	//	The marker that this object should attach to.
	int marker_;
	//	If this object should be attached to a marker or not.
	bool is_marker_object_;
	//	The ID of this game object - used in collision respones.
	char* id_;
};