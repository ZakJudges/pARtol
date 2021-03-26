#ifndef _RENDER_TARGET_APP_H
#define _RENDER_TARGET_APP_H

#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <vector>
#include <graphics/mesh_instance.h>
#include <platform/vita/graphics/texture_vita.h>

// Vita AR includes
#include <camera.h>
#include <gxm.h>
#include <motion.h>
#include <libdbg.h>
#include <libsmart.h>

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class RenderTarget;
	class TextureVita;
	class MeshInstance;
}


class ARApp : public gef::Application
{
public:
	ARApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();

	void RenderOverlay();
	void SetupLights();

	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;

	float fps_;

	gef::Renderer3D* renderer_3d_;
	
	float y_scaling_factor_;

	gef::Matrix44 ortho_matrix_;

	gef::Sprite camera_feed_sprite_;
	gef::TextureVita camera_feed_texture_;

	class Camera* camera_;
	
	class GameManager* game_manager_;



};




#endif // _RENDER_TARGET_APP_H