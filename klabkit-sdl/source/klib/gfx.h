#ifndef KLIB_GFX_H
#define KLIB_GFX_H

#include <SDL.h>

namespace klib {

	namespace gfx {

		// texture blitting
		void blit(SDL_Renderer* p_rnd, SDL_Texture* p_texture, int p_x, int p_y);

		// surface and texture functions and procedures
		SDL_Texture* surface_to_texture(SDL_Renderer* p_rnd, SDL_Surface* p_srf, bool p_destroy_surface = true);

		// utility functions
		void put_pixel(SDL_Surface* srf, int x, int y, Uint32 pixel);

	}

}

#endif
