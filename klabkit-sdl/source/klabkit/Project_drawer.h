#ifndef KLABKIT_PROJECT_DRAWER_H
#define KLABKIT_PROJECT_DRAWER_H

#include <SDL.h>
#include "Project.h"

namespace kkit {

	class Project_drawer {

		std::vector<SDL_Texture*> wall_tiles;

	public:
		Project_drawer(SDL_Renderer* p_rnd, const kkit::Project& p_project);
		~Project_drawer(void);

		void draw_tile(SDL_Renderer* p_rnd, int p_tile_no, int p_x, int p_y) const;
	};

}

#endif
