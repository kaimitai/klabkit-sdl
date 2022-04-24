#ifndef KLIB_BUTTON_H
#define KLIB_BUTTON_H

#include <SDL.h>
#include "Font.h"
#include "gfx.h"
#include "User_input.h"
#include <string>

namespace klib {

	class Button {
		std::string caption;
		int x, y, w, h;
		SDL_Color text_col, bg_col, outline_col, mouse_over_col;

	public:
		Button(const std::string& p_caption, int p_x, int p_y, int p_w, int p_h = gc::BUTTON_H,
			SDL_Color p_text_col = gc::COL_BLACK,
			SDL_Color p_bg_color = gc::COL_GRAY,
			SDL_Color p_mouse_over_color = gc::COL_YELLOW,
			SDL_Color p_outline_color = gc::COL_WHITE);

		void draw(SDL_Renderer* p_rnd, const klib::Font& p_font, const klib::User_input& p_input) const;
		void draw(SDL_Renderer* p_rnd, const klib::Font& p_font, const klib::User_input& p_input, const std::string& p_override_caption, SDL_Color p_override_bg) const;
		bool is_hit(int p_mx, int p_my) const;
		void set_bg_color(SDL_Color p_color);
	};

}

#endif
