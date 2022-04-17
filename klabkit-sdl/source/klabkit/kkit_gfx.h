#ifndef KLABKIT_GFX_H
#define KLABKIT_GFX_H

#include <SDL.h>
#include <tuple>
#include <vector>
#include "Project.h"


namespace kkit {

	using byte = unsigned char;

	namespace gfx {

		// bmp functions
		void project_walls_to_bmps(const kkit::Project& p_project);
		void project_map_to_bmp(const kkit::Project& p_project, int p_board_no);
		void set_surface_project_palette(SDL_Surface* p_surface, const kkit::Project& p_project);

		// utility functions
		std::vector<SDL_Color> tuple_to_sdl_palette(const std::vector<std::tuple<byte, byte, byte>>&);

	}

}

#endif
