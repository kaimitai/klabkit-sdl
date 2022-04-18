#ifndef KLABKIT_PROJECT_DRAWER_H
#define KLABKIT_PROJECT_DRAWER_H

#include <SDL.h>
#include "Project.h"
#include "constants.h"

namespace kkit {

	constexpr int BOARD_TILE_W{ 8 };
	constexpr int BOARD_PW{ BOARD_TILE_W * c::WALL_IMG_W };

	class Project_drawer {

		// internal variables for level board position
		int board_ind{ 17 }, board_x{ 0 }, board_y{ 0 }, board_zoom{ -1 };

		std::vector<SDL_Texture*> wall_tiles;


		void draw_tile(SDL_Renderer* p_rnd, int p_tile_no, int p_x, int p_y) const;

		// internal calculations
		int c_tile_pw(void) const;
		int c_tile_cnt(void) const;
		int c_tile_offset_max(void) const;

		void validate_grid_offset(void);

	public:
		Project_drawer(SDL_Renderer* p_rnd, const kkit::Project& p_project);
		~Project_drawer(void);

		void draw_board(SDL_Renderer* p_rnd, const kkit::Project& p_project, int p_x, int p_y) const;
		void draw_minimap(SDL_Renderer* p_rnd, int p_x, int p_y) const;

		void move_grid_offset_x(int p_dx);
		void move_grid_offset_y(int p_dy);
		void move_grid_zoom(int p_dz);
	};

}

#endif
