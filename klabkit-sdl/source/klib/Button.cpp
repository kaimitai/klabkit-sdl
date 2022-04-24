#include "Button.h"
#include "gfx.h"
#include "klib_util.h"

klib::Button::Button(const std::string& p_caption, int p_x, int p_y, int p_w, int p_h,
	SDL_Color p_text_col, SDL_Color p_bg_color, SDL_Color p_mouse_over_color, SDL_Color p_outline_color)
	: caption{ p_caption }, x{ p_x }, y{ p_y }, w{ p_w }, h{ p_h }, text_col{ p_text_col }, bg_col{ p_bg_color },
	mouse_over_col{ p_mouse_over_color }, outline_col{ p_outline_color }
{}

void klib::Button::draw(SDL_Renderer* p_rnd, const klib::Font& p_font, const klib::User_input& p_input) const {
	klib::gfx::draw_label(p_rnd, p_font, caption, x, y, w, h, text_col, is_hit(p_input.mx(), p_input.my()) ? mouse_over_col : bg_col, outline_col);
}

void klib::Button::draw(SDL_Renderer* p_rnd, const klib::Font& p_font, const klib::User_input& p_input, const std::string& p_override_caption, SDL_Color p_override_bg) const {
	klib::gfx::draw_label(p_rnd, p_font, p_override_caption, x, y, w, h, text_col, is_hit(p_input.mx(), p_input.my()) ? mouse_over_col : p_override_bg, outline_col);
}

bool klib::Button::is_hit(int p_mx, int p_my) const {
	return klib::util::is_p_in_rect(p_mx, p_my, this->x, this->y, this->w, this->h);
}

void klib::Button::set_bg_color(SDL_Color p_color) {
	this->bg_col = p_color;
}
