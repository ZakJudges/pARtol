#include "Camera.h"
#include <maths/math_utils.h>
#include <system/platform.h>


Camera::Camera(gef::Platform& platform) : platform_(platform)
{
	aspect_ratio_ = 0.0f;
}

void Camera::Init(gef::Vector4 eye, gef::Vector4 lookat, gef::Vector4 up, float fov, float near, float far, float y_scale_factor)
{
	camera_eye_ = eye;
	camera_lookat_ = lookat;
	camera_up_ = up;
	camera_fov_ = fov;
	near_plane_ = near;
	far_plane_ = far;
	y_scale_factor_ = y_scale_factor;

	GetViewMatrix();
	GetProjectionMatrix();
}

gef::Matrix44 Camera::GetViewMatrix()
{
	view_matrix_.LookAt(camera_eye_, camera_lookat_, camera_up_);

	return view_matrix_;
}

gef::Matrix44 Camera::GetProjectionMatrix()
{
	projection_matrix_ = platform_.PerspectiveProjectionFov(camera_fov_, aspect_ratio_, near_plane_, far_plane_);

	gef::Matrix44 scale_matrix;
	scale_matrix.Scale(gef::Vector4(1.0f, y_scale_factor_, 1.0f));

	projection_matrix_ = projection_matrix_ * scale_matrix;

	return projection_matrix_;

}


