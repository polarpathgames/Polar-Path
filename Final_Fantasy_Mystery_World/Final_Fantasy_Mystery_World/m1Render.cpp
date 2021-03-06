#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "m1Window.h"
#include "m1Render.h"
#include "m1Cutscene.h"
#include "m1Map.h"
#include "Brofiler/Brofiler.h"
#include "m1Input.h"
#include "m1FadeToBlack.h"



#define VSYNC false

m1Render::m1Render() : m1Module()
{
	name.assign("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
m1Render::~m1Render()
{}

// Called before render is available
bool m1Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
		vsync = true;
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);
	SDL_RenderSetLogicalSize(renderer, App->win->width, App->win->height);

	if (renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = App->win->width;
		camera.h = App->win->height;
		camera.x = 0;
		camera.y = 0;
	}

	return ret;
}

// Called before the first frame
bool m1Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool m1Render::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdateRender", Profiler::Color::Orange);

	SDL_RenderClear(renderer);

	//ZOOM
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		debug_border = true;

		if (zoom < max_zoom)
		{
			zoom++;
			SDL_RenderSetLogicalSize(renderer, camera.w * zoom, camera.h * zoom);
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
	{
		if (zoom > 1)
		{
			zoom--;
			SDL_RenderSetLogicalSize(renderer, camera.w  * zoom, camera.h * zoom);
		}
		if (zoom <= 1)
		{
			debug_border = false;
		}
	}

	return true;
}

bool m1Render::Update(float dt)
{
	BROFILER_CATEGORY("UpdateRender", Profiler::Color::Aqua);
	
	return true;
}

bool m1Render::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdateRender", Profiler::Color::Purple);

	

	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		CameraTremble(0.83f, 1.5F);
		debug = true;
	}
	// Joss Camera Shake 
	// shake
	// if we have current trauma
	if (trauma > 0)
	{
		preShakePos.x = camera.x;
		preShakePos.y = camera.y;

		camera.x += maxOffset * GetShakeAmount() * GetFloatNegOneToOne();
		camera.y += maxOffset * GetShakeAmount() * GetFloatNegOneToOne();

		if (debug)
		{
			uint w, h;
			App->win->GetWindowSize(w, h);
			SDL_Rect traumaRect = { 10,h - 10, 20, -(trauma * (h - 20)) };
			SDL_Rect shakeRect = { 30, h - 10, 20, -(GetShakeAmount() * (h - 20)) };

			SDL_SetRenderDrawColor(App->render->renderer, 255, 132, 64, 200);
			SDL_RenderFillRect(App->render->renderer, &traumaRect);
			SDL_SetRenderDrawColor(App->render->renderer, 107, 186, 255, 200);
			SDL_RenderFillRect(App->render->renderer, &shakeRect);
		}

		// decay trauma
		trauma -= App->GetDeltaTime() * traumaDecay * (trauma + 0.3f);
	}
	else // update preshake pos
	{
		preShakePos.x = camera.x;
		preShakePos.y = camera.y;
		debug = false;
	}


	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool m1Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool m1Render::Load(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// Save Game State
bool m1Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

void m1Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void m1Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void m1Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool m1Render::Blit(SDL_Texture* texture, float x, float y, const SDL_Rect* section, bool apply_scale, SDL_RendererFlip flip, float speed, const SDL_Rect* clip_zone,double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	if (clip_zone != NULL) {
		SDL_RenderSetClipRect(renderer, clip_zone);
	}

	SDL_Rect rect;
	if (apply_scale) {
		rect.x = (int)(camera.x * speed) + (int)(x * (float)scale);
		rect.y = (int)(camera.y * speed) + (int)(y * (float)scale);
	}
	else {
		rect.x = x;
		rect.y = y;
	}

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	if (apply_scale)
	{
		rect.w *= scale;
		rect.h *= scale;
	}

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		//LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	if (clip_zone != NULL) {
		SDL_RenderSetClipRect(renderer, NULL);
	}

	return ret;
}

bool m1Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool m1Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if (use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool m1Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;

	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for (uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(camera.x + x * scale + radius * cos(i * factor));
		points[i].y = (int)(camera.y + y * scale + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

iPoint m1Render::ScreenToWorld(int x, int y) const
{
	iPoint ret;
	int scale = App->win->GetScale();

	ret.x = (x - camera.x / scale);
	ret.y = (y - camera.y / scale);

	return ret;
}

bool m1Render::IsOnCamera(const int & x, const int & y, const int & w, const int & h) const
{
	BROFILER_CATEGORY("IsOnCamera", Profiler::Color::Aqua);
	int scale = App->win->GetScale();

	SDL_Rect r = { x*scale,y*scale,w*scale,h*scale };
	SDL_Rect cam = { -camera.x,-camera.y,camera.w,camera.h };

	return SDL_HasIntersection(&r, &cam);
}

void m1Render::ResetCamera()
{
	camera.x = 0;
	camera.y = 0;
}

void m1Render::SmoothCamera(iPoint playerpos)
{
	BROFILER_CATEGORY("SmoothCamera", Profiler::Color::Aquamarine);
	if (App->fade_to_black->current_step != App->fade_to_black->fade_to_black && App->cutscene_manager->is_executing == false /*&& trauma < 0*/) {
		playerpos.x = (playerpos.x * (int)App->win->scale - camera.w * 0.5F);
		smoth_position.x -= (playerpos.x + camera.x) / smooth_speed * App->GetDeltaTime();
		camera.x = smoth_position.x;

		playerpos.y = (playerpos.y * (int)App->win->scale - camera.h * 0.5F);
		smoth_position.y -= (playerpos.y + camera.y) / smooth_speed * App->GetDeltaTime();
		camera.y = smoth_position.y;
	}
}

void m1Render::CenterCameraOnPlayer(iPoint playerpos)
{
	playerpos.x = (playerpos.x * (int)App->win->scale - camera.w * 0.5F);
	playerpos.y = (playerpos.y * (int)App->win->scale - camera.h * 0.5F);
	smoth_position.x -= (playerpos.x + camera.x);
	camera.x = smoth_position.x;
	smoth_position.y -= (playerpos.y + camera.y);
	camera.y = smoth_position.y;
}

float m1Render::GetFloatNegOneToOne()
{
	std::uniform_real_distribution<float> dis(-1.0f, std::nextafter(1, DBL_MAX));

	return dis(gen);
}

float m1Render::CameraTremble(float value, float traumaDecay)
{
	// values must be between 0 and 1 (percentages)
	if (value <= 1.0F)
	{
		this->traumaDecay = traumaDecay;
		trauma += value;
		return trauma = MAX(0.0f, MIN(trauma, 1.0f));
	}

	return 0.0F;
}

float m1Render::GetShakeAmount() const
{
	return trauma * trauma;
}