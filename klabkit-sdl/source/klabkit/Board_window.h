#ifndef KLABKIT_BOARD_WINDOW_H
#define KLABKIT_BOARD_WINDOW_H

#include <SDL.h>
#include "Project.h"
#include "Project_gfx.h"
#include "constants.h"
#include "./../klib/User_input.h"

namespace kkit {

	// pixel dimension and coordinates of level board
	constexpr int BW_BX{ 20 };
	constexpr int BW_BY{ 20 };
	constexpr int BW_BW{ c::WALL_IMG_W * 8 };
	constexpr int BOARD_TILE_W{ 8 };
	constexpr int BOARD_PW{ BOARD_TILE_W * c::WALL_IMG_W };

	// pixel dimensions and coordinates of minimap
	constexpr int BW_MX{ 20 };
	constexpr int BW_MY{ 40 + 8 * 64 };
	constexpr int BW_MW{ 128 };

	class Board_window {

		// the texture which will hold the map data
		SDL_Texture* grid_texture;

		// internal variables for level board position
		int board_ind{ 0 }, board_px{ 0 }, board_py{ 0 };
		float zoom_factor{ 1.0f };

		// internal variables for selected tile position
		int sel_tile_x{ 0 }, sel_tile_y{ 0 };

		// internal variables for tile picker position
		int tile_x{ 0 }, tile_y{ 0 };

		// drawing routines
		//void draw_tile(SDL_Renderer* p_rnd, const kkit::Project_gfx& p_gfx, int p_tile_no, int p_x, int p_y) const;
		void draw_board(SDL_Renderer* p_rnd, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx, int p_x, int p_y) const;
		void draw_minimap(SDL_Renderer* p_rnd, int p_x, int p_y) const;
		void draw_tile_picker(SDL_Renderer* p_rnd, const kkit::Project_gfx& p_gfx, int p_x, int p_y) const;

		// handle clicks on the various screen elements
		void click_tile_picker(int p_x, int p_y);
		std::pair<int, int> get_tile_pos(int p_x, int p_y) const;
		std::pair<int, int> get_pixel_pos(int p_x, int p_y) const;

		kkit::Map_tile get_selected_tile(const kkit::Project& p_project) const;

		// internal calculations
		int c_max_offset(void) const;
		int c_bb_pixel_width(void) const;

		void move_grid_offset_x(int p_dx);
		void move_grid_offset_y(int p_dy);
		void set_grid_offset(int p_x, int p_y);
		void click_minimap(int p_x, int p_y);
		void move_grid_zoom(float p_dz);
		void center_offset(void);
		void center_offset(std::pair<int, int> p_coords);
		void validate_grid_offset(void);

		std::string get_board_title(void) const;

	public:
		Board_window(SDL_Renderer* p_rnd);
		void move(const klib::User_input& p_input, int p_delta_ms, kkit::Project& p_project);
		void draw(SDL_Renderer* p_rnd, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx) const;
	};
}

#endif
