#ifndef KLABKIT_GFX_H
#define KLABKIT_GFX_H

#include <SDL.h>
#include <tuple>
#include <vector>
#include "Project.h"


namespace kkit {

	using byte = unsigned char;
	using palette = std::vector<std::tuple<byte, byte, byte>>;

	namespace gfx {

		// bmp functions
		void project_walls_to_bmps(const kkit::Project& p_project);
		void project_map_to_bmp(const kkit::Project& p_project, int p_board_no);
		void set_surface_project_palette(SDL_Surface* p_surface, const kkit::Project& p_project);
		SDL_Surface* image_to_sdl_surface(const std::vector<std::vector<byte>>& p_image, const palette& p_palette);

		// program graphics
		std::vector<SDL_Texture*> get_project_textures(SDL_Renderer* p_rnd, const kkit::Project& p_project);

		// utility functions
		std::vector<SDL_Color> tuple_to_sdl_palette(const palette&);

	}

}

#endif
