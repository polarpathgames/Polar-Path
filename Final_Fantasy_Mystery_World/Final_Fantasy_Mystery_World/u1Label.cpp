#include "App.h"
#include "m1Fonts.h"
#include "u1Label.h"
#include "m1Textures.h"
#include "m1Render.h"
#include "p2Log.h"

u1Label::u1Label(const int & pos_x, const int & pos_y, const char * txt, const Color & c, FontType font, u1GUI * parent,
	bool interactable, bool draggable, uint32 wrap_length, bool focus,
	bool has_background, const SDL_Color& bg_color, SDL_Rect * clip_zone)
	:u1GUI(LABEL, pos_x, pos_y, parent, { 0,0,0,0 }, true, false, false, focus),

	id_font(font), has_background(has_background), background_color(bg_color), wrap(wrap_length)
{
	text.assign(txt);
	if (clip_zone != nullptr)
		this->clip_zone = clip_zone;
	SetColor(c);
	if (wrap_length == 0U) {
		texture = App->fonts->Print(text.data(), color, id_font);
		App->fonts->CalcSize(txt, section.w, section.h, id_font);
	}
	else {
		texture = App->fonts->PrintWrapped(text.data(), color, id_font, wrap_length);
		App->fonts->CalcSizeWrapped(txt, section.w, section.h, id_font, wrap_length);
	}
}

u1Label::~u1Label()
{
	text.clear();
	if (texture != nullptr) {
		App->tex->UnLoad(texture);
		texture = nullptr;
	}
}

void u1Label::InnerDraw()
{

	if (!drawable)
		return;

	if (has_background) {
		App->render->DrawQuad({ draw_offset.x,draw_offset.y,section.w,section.h }, background_color.r, background_color.g, background_color.b, background_color.a, true, false);
	}

	if (texture != nullptr)
		App->render->Blit(texture, draw_offset.x, draw_offset.y, NULL, false, SDL_FLIP_NONE, 0.0F, clip_zone);
}

void u1Label::SetText(const char * txt)
{
	text.assign(txt);
	App->tex->UnLoad(texture);
	texture = App->fonts->Print(text.data(), color, id_font);
	App->fonts->CalcSize(text.data(), section.w, section.h, id_font);
}

void u1Label::SetTextWrapped(const char * txt)
{
	text.assign(txt);
	App->tex->UnLoad(texture);
	texture = App->fonts->PrintWrapped(text.data(), color, id_font, wrap);
	App->fonts->CalcSizeWrapped(text.data(), section.w, section.h, id_font, wrap);
}

void u1Label::SetColor(const Color& c)
{
	switch (c) {
	case RED:
		color = { 255,0,0,color.a };
		break;
	case GREEN:
		color = { 0,255,0,color.a };
		break;
	case BLUE:
		color = { 0,0,255,color.a };
		break;
	case YELLOW:
		color = { 255,255,0,color.a };
		break;
	case GREY:
		color = { 150,150,150,color.a };
		break;
	case BLACK:
		color = { 0,0,0,color.a };
		break;
	case WHITE:
		color = { 255,255,255,color.a };
		break;
	default:
		color = { 255,255,255,color.a };
		break;
	}
}

void u1Label::SetColor(const SDL_Color & c)
{
	color = c;
}

std::string u1Label::GetText()
{
	return text;
}

void u1Label::ChangeFont(const char * f, const int & size)
{
	id_font = App->fonts->Load(f, size)->type;
	texture = App->fonts->Print(text.data(), color, id_font);
	App->fonts->CalcSize(text.data(), section.w, section.h, id_font);
}
