#ifndef KLIB_GFX_H
#define KLIB_GFX_H

#include <SDL.h>
#include <string>
#include <vector>

namespace klib {

	// gfx constants
	namespace gc {
		constexpr int BUTTON_H{ 28 };

		constexpr SDL_Color COL_BLACK{ 0, 0, 0 };
		constexpr SDL_Color COL_WHITE{ 255,255,255 };
		constexpr SDL_Color COL_BLUE{ 0, 0, 255 };
		constexpr SDL_Color COL_RED{ 255, 0, 0 };
		constexpr SDL_Color COL_RED_DARK{ 171, 35, 40 };
		constexpr SDL_Color COL_GREEN{ 0, 255, 0 };
		constexpr SDL_Color COL_YELLOW{ 255, 255, 0 };
		constexpr SDL_Color COL_GRAY{ 192, 192, 192 };
		constexpr SDL_Color COL_ORANGE{ 255, 140, 0 };
	}

	namespace gfx {

		// texture blitting
		void blit(SDL_Renderer* p_rnd, SDL_Texture* p_texture, int p_x, int p_y);
		void blit_scale(SDL_Renderer* p_rnd, SDL_Texture* p_texture, int p_x, int p_y, int p_scale);
		void blit_p2_scale(SDL_Renderer* p_rnd, SDL_Texture* p_texture, int p_x, int p_y, int p_p2);
		void blit_full_spec(SDL_Renderer* p_rnd, SDL_Texture* p_txt, int s_x, int s_y, int s_w, int s_h, int t_x, int t_y, int t_w, int t_h);
		void blit_factor(SDL_Renderer* p_rnd, SDL_Texture* p_txt, int p_x, int p_y, float p_factor);
		void blit_tiled(SDL_Renderer* p_rnd, SDL_Texture* p_texture, int p_x, int p_y);

		// surface and texture functions and procedures
		SDL_Texture* surface_to_texture(SDL_Renderer* p_rnd, SDL_Surface* p_srf, bool p_destroy_surface = true);

		// utility functions
		void draw_rect(SDL_Renderer* p_rnd, int p_x, int p_y, int p_w, int p_h, SDL_Color p_color, int p_thickness);
		void put_pixel(SDL_Surface* srf, int x, int y, Uint32 pixel);
		Uint32 get_pixel(SDL_Surface*, int, int);
		std::vector<SDL_Texture*> split_surface(SDL_Renderer* p_rnd, SDL_Surface* srf, SDL_Color p_trans_col, int p_w, int p_h, bool p_destroy_surface = true);
		SDL_Color pulse_color(SDL_Color a, SDL_Color b, int p_progress);
	}

}

#endif
