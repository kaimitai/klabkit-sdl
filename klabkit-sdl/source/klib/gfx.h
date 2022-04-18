#ifndef KLIB_GFX_H
#define KLIB_GFX_H

#include <SDL.h>

namespace klib {

	namespace gfx {

		// texture blitting
		void blit(SDL_Renderer* p_rnd, SDL_Texture* p_texture, int p_x, int p_y);
		void blit_scale(SDL_Renderer* p_rnd, SDL_Texture* p_texture, int p_x, int p_y, int p_scale);

		// surface and texture functions and procedures
		SDL_Texture* surface_to_texture(SDL_Renderer* p_rnd, SDL_Surface* p_srf, bool p_destroy_surface = true);

		// utility functions
		void draw_rect(SDL_Renderer* p_rnd, int p_x, int p_y, int p_w, int p_h, SDL_Color p_color, int p_thickness);
		void put_pixel(SDL_Surface* srf, int x, int y, Uint32 pixel);

	}

}

#endif
