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

		// utility functions
		std::vector<SDL_Color> tuple_to_sdl_palette(const std::vector<std::tuple<byte, byte, byte>>&);
	}

}

#endif
