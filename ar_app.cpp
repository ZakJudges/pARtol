#include "ar_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/font.h>
#include <input/touch_input_manager.h>
#include <graphics/mesh_instance.h>
#include <maths/vector2.h>
#include <input/input_manager.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>
#include <graphics/texture.h>
#include <graphics/render_target.h>
#include "Camera.h"
#include "GameManager.h"
#include "GameObject.h"
#include <sony_sample_framework.h>
#include <sony_tracking.h>

#include "GameMessage.h"

ARApp::ARApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	font_(NULL),
	renderer_3d_(NULL),
	camera_(NULL),
	game_manager_(NULL)
{
}

void ARApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	camera_ = new Camera(platform_);
	game_manager_ = new GameManager(platform_);

	InitFont();
	SetupLights();

	// initialise sony framework
	sampleInitialize();
	smartInitialize();

	// reset marker tracking
	AppData* dat = sampleUpdateBegin();
	smartTrackingReset();
	sampleUpdateEnd(dat);

	//	calculate the y scaling factor needed to prevent distortion of the camera feed image.
	float camera_aspect = (float)SCE_SMART_IMAGE_WIDTH / (float)SCE_SMART_IMAGE_HEIGHT;
	float display_aspect = (float)platform_.width() / (float)platform_.height();
	y_scaling_factor_ = display_aspect / camera_aspect;

	camera_feed_sprite_.set_position(gef::Vector4(0.0f, 0.0f, 1.0f));
	camera_feed_sprite_.set_width(2);
	camera_feed_sprite_.set_height(2 * y_scaling_factor_);
	camera_feed_sprite_.set_texture(&camera_feed_texture_);

	ortho_matrix_.OrthographicFrustumGL(-1, 1, -1, 1, -1, 1);

	camera_->Init(gef::Vector4(0.0f, 0.0f, 5.0f), gef::Vector4(0.0f, 0.0f, 0.0f), gef::Vector4(0.0f, 1.0f, 0.0f), gef::DegToRad(45.0f), 0.01f, 2000.0f, y_scaling_factor_);
	camera_->SetCameraAspectRatio(camera_aspect);
	
	game_manager_->Init();
}

void ARApp::CleanUp()
{
	smartRelease();
	sampleRelease();

	CleanUpFont();
	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	if (camera_)
	{
		delete camera_;
		camera_ = nullptr;
	}
	if (game_manager_)
	{
		delete game_manager_;
		game_manager_ = nullptr;
	}

}

bool ARApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	game_manager_->Update(frame_time);

	AppData* dat = sampleUpdateBegin();

	// use the tracking library to try and find markers
	smartUpdate(dat->currentImage);

	//	Loop through each marker and update transforms and detected booleans in the game manager.
	for (int i = 0; i < 6; i++)
	{
		bool is_marker_found = sampleIsMarkerFound(i);
		if (is_marker_found)
		{
			gef::Matrix44 marker_transform;
			sampleGetTransform(i, &marker_transform);

			gef::Matrix44 scale_matrix;
			scale_matrix.Scale(gef::Vector4(0.059f, 0.059f, 0.059f));

			game_manager_->SetMarkerTransform(i+1, scale_matrix * marker_transform);
			game_manager_->SetMarkerDetected(i+1, true);
		}
		else
		{
			game_manager_->SetMarkerDetected(i+1, false);
		}
	}


	sampleUpdateEnd(dat);

	return true;
}

void ARApp::Render()
{
	AppData* dat = sampleRenderBegin();

	//
	// Render the camera feed
	//

	// REMEMBER AND SET THE PROJECTION MATRIX HERE
	sprite_renderer_->set_projection_matrix(ortho_matrix_);
	sprite_renderer_->Begin(true);

	// DRAW CAMERA FEED SPRITE HERE
	if (dat->currentImage)
	{
		camera_feed_texture_.set_texture(dat->currentImage->tex_yuv);
		sprite_renderer_->DrawSprite(camera_feed_sprite_);
	}

	//	Draw the menu sprite.
	if (game_manager_->GetGameState() == GameState::MENU)
	{
		gef::Sprite* menu_sprite = game_manager_->GetMenuSprite();
		if (menu_sprite)
		{
			sprite_renderer_->DrawSprite(*menu_sprite);
		}
	}
	
	sprite_renderer_->End();

	//
	// Render 3D scene
	//

	// SET VIEW AND PROJECTION MATRIX HERE
	gef::Matrix44 view_matrix;
	view_matrix.SetIdentity();
	renderer_3d_->set_projection_matrix(camera_->GetProjectionMatrix());
	//renderer_3d_->set_view_matrix(camera_->GetViewMatrix());
	renderer_3d_->set_view_matrix(view_matrix);
	// Begin rendering 3D meshes, don't clear the frame buffer
	renderer_3d_->Begin(false);

	//	Render the game objects.
	if (game_manager_)
	{
		if (game_manager_->GetGameState() == GameState::PLAY_GAME)
		{
			for (int i = 0; i < game_manager_->GetNumObjects(); i++)
			{
				gef::MeshInstance* mesh = game_manager_->GetMesh(i);

				if (mesh)
				{
					renderer_3d_->set_override_material(nullptr);
					
					//	Make the goal be renderered with the green material.
					if (i == 5)
					{
						renderer_3d_->set_override_material(&game_manager_->GetGoalMaterial());
					}
					
					renderer_3d_->DrawMesh(*mesh);
				}
			}
		}
	}

	renderer_3d_->End();

	RenderOverlay();

	sampleRenderEnd();
}


void ARApp::RenderOverlay()
{
	//
	// render 2d hud on top
	//
	gef::Matrix44 proj_matrix2d;

	proj_matrix2d = platform_.OrthographicFrustum(0.0f, platform_.width(), 0.0f, platform_.height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}


void ARApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void ARApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void ARApp::DrawHUD()
{
	if(font_)
	{
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
		
		if (game_manager_)
		{
			//	Loop through the messages from the current state and display them on screen.
			std::vector<GameMessage*> messages = game_manager_->GetMessages();
			for (int i = 0; i < messages.size(); i++)
			{
				if (messages[i])
				{
					if (messages[i]->variable != -1)
					{
						font_->RenderText(sprite_renderer_, gef::Vector4(messages[i]->position.x, messages[i]->position.y, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, messages[i]->message, messages[i]->variable);
					}
					else
					{
						font_->RenderText(sprite_renderer_, gef::Vector4(messages[i]->position.x, messages[i]->position.y, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, messages[i]->message);

					}
				}
			}
		}
	}
}

void ARApp::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}
