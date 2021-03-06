#ifndef KLABKIT_BOARD_WINDOW_H
#define KLABKIT_BOARD_WINDOW_H

#include <SDL.h>
#include "Project.h"
#include "Project_gfx.h"
#include "../klib/gfx.h"
#include "constants.h"
#include "./../klib/User_input.h"
#include "../klib/Timer.h"
#include "../klib/Button.h"

namespace kkit {

	// min and max values for interacting with the grid
	constexpr float ZOOM_MIN{ 0.125f };
	constexpr float ZOOM_MAX{ 2.0f };

	// pixel dimension and coordinates of level board
	constexpr int BW_BX{ 20 };
	constexpr int BW_BY{ 80 };
	constexpr int BOARD_TILE_W{ 8 };
	constexpr int BW_BW{ c::WALL_IMG_W * BOARD_TILE_W };
	constexpr int BOARD_PW{ BOARD_TILE_W * c::WALL_IMG_W };
	constexpr int BOARD_TPW{ c::WALL_IMG_W * c::MAP_W };

	// pixel dimensions and coordinates of minimap
	constexpr int BW_MX{ BW_BX };
	constexpr int BW_MY{ BW_BY + BW_BW + klib::gc::BUTTON_H + 1 };
	constexpr int BW_MW{ 128 };

	// pixel dimensions and coordinates of selected board tile
	constexpr int BW_SBTX{ BW_MX + BW_MW + 1 };
	constexpr int BW_SBTY{ BW_MY };
	constexpr int BW_SBTW{ BW_BW - BW_MW - 1 };
	constexpr int BW_SBTH{ BW_MW };

	// pixel dimensions and coordinates of selected metadata tile
	constexpr int BW_TPX{ BW_BX + BW_BW + 20 };
	constexpr int BW_TPY{ BW_BY };

	constexpr int BW_TP_TW{ 32 }; // tile picker tile width/height
	constexpr int BW_TPR{ 14 }; // tiles per row
	constexpr int BW_TPC{ 12 }; // tiles per column

	constexpr int BW_TPW{ BW_TP_TW * BW_TPR };
	constexpr int BW_TPH{ BW_TP_TW * BW_TPC };
	//constexpr int BW_TH {1}


	// buttons and labels pixel sizes and offsets
	constexpr int BW_SBT_LBL_DESTR_X{ BW_SBTX + BW_MW };
	constexpr int BW_SBT_LBL_DESTR_Y{ BW_SBTY };
	constexpr int BW_SBT_LBL_DESTR_W{ BW_SBTW - BW_MW };
	constexpr int BW_SBT_LBL_DESTR_H{ BW_SBTH / 3 };

	// flash buttons (used for direction, destructible, clip and tile no)
	constexpr int BW_FBX{ BW_TPX };
	constexpr int BW_FBY{ BW_TPY + BW_TPH + 40 };
	constexpr int BW_FBW{ 100 };
	constexpr int BW_FBH{ 28 };
	constexpr int BW_FB_SPACING{ 15 };

	// save/load buttons
	constexpr int BW_BTN_LOGIC{ 7 };
	constexpr int BW_BTN_SL_IND{ BW_BTN_LOGIC + 3 };

	constexpr int BW_EXML_BTN_X{ BW_TPX };
	constexpr int BW_EXML_BTN_Y{ BW_FBY + 2 * (BW_FBH + BW_FB_SPACING) };
	constexpr int BW_EXML_BTN_W{ BW_TPW / 2 - BW_FB_SPACING };
	constexpr int BW_EXML_BTN_H{ BW_FBH };

	class Board_window {

		// the texture which will hold the map data
		SDL_Texture* grid_texture;

		// internal variables for level board position
		int board_ind{ 0 }, board_px{ 0 }, board_py{ 0 };
		float zoom_factor{ 1.0f };

		// internal variables for selected tile position
		int sel_tile_x{ 0 }, sel_tile_y{ 0 };
		int sel_tile_2_x{ -1 }, sel_tile_2_y{ 0 };

		// internal variables for tile picker position
		int tile_row{ 0 };
		int tile_x{ 0 }, tile_y{ 0 };

		// clipboard
		std::vector<std::vector<kkit::Map_tile>> clipboard;

		// window elements
		std::vector<klib::Timer> timers;
		std::vector<klib::Button> buttons;

		std::vector<bool> toggles;

		// drawing routines
		//void draw_tile(SDL_Renderer* p_rnd, const kkit::Project_gfx& p_gfx, int p_tile_no, int p_x, int p_y) const;
		void draw_board(SDL_Renderer* p_rnd, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx, int p_x, int p_y) const;
		void draw_minimap(SDL_Renderer* p_rnd, int p_x, int p_y) const;
		void draw_tile_picker(SDL_Renderer* p_rnd, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx, int p_x, int p_y) const;
		void draw_selected_board_tile(SDL_Renderer* p_rnd, const klib::User_input& p_input, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx) const;

		// handle clicks on the various screen elements
		void click_tile_picker(const kkit::Project& p_project, int p_x, int p_y);
		std::pair<int, int> get_tile_pos(int p_x, int p_y) const;
		std::pair<int, int> get_pixel_pos(int p_x, int p_y) const;

		kkit::Map_tile get_selected_tile(const kkit::Project& p_project, int p_tile_no) const;
		int get_mouseover_tile_no(const kkit::Project& p_project, int p_x, int p_y) const;
		int get_selected_tile_no(const kkit::Project& p_project) const;
		int get_selected_board_tile_no(const kkit::Project& p_project) const;

		// internal calculations
		int c_max_offset(void) const;
		int c_bb_pixel_width(void) const;
		int c_bb_tile_pixel_width(void) const;
		int c_tile_row_max(const kkit::Project& p_project) const;

		void move_grid_offset_x(int p_dx);
		void move_grid_offset_y(int p_dy);
		void set_grid_offset(int p_x, int p_y);
		void click_minimap(int p_x, int p_y);
		void move_grid_zoom(float p_dz);
		void center_offset(void);
		void center_offset(std::pair<int, int> p_coords);
		void validate_grid_offset(void);

		// given an absolute underlying board pixel coordinate, make it be a given absolute screen grid coordinate
		void translate_grid_offset(int p_gx, int p_gy, int p_sx, int p_sy);

		void button_click(std::size_t p_button_no, SDL_Renderer* p_rnd, kkit::Project& p_project, kkit::Project_gfx& p_gfx, const klib::User_input& p_input);

		// selection operations, copy, paste, cut, flip, rotate etc
		bool is_empty_selection(const kkit::Project& p_project) const;
		std::tuple<int, int, int, int> get_selection_rectangle(void) const;
		void copy_to_clipboard(const kkit::Project& p_project, bool p_clear_secondary = false);
		void cut_selection(kkit::Project& p_project);
		void paste_from_clipboard(kkit::Project& p_project);
		bool selection_fits(void) const;
		void show_selection_rectangle(void);
		void clear_selection(kkit::Project& p_project);
		void rotate_selection(const kkit::Project& p_project, bool p_clockwise = false);
		void clear_secondary_selection(void);

		// board calculations
		int count_tiles(const kkit::Project& p_project, int p_tile_no, bool p_all_boards = false) const;
		void next_tile(const kkit::Project& p_project, bool p_tp_tile, bool p_wrap = false);
		void prev_tile(const kkit::Project& p_project, bool p_tp_tile, bool p_wrap = false);

		// save/load
		void xml_export(const kkit::Project& p_project, int p_board_no) const;
		bool xml_import(kkit::Project& p_project, int p_board_no) const;
		void save_boards_kzp(const kkit::Project& p_project, kkit::Project_gfx& p_gfx, bool p_compress) const;

		// call when board data is altered
		void board_changed(SDL_Renderer* p_rnd, const kkit::Project& p_project, kkit::Project_gfx& p_gfx);

	public:
		Board_window(SDL_Renderer* p_rnd, bool p_is_klab_v2);
		void move(SDL_Renderer* p_rnd, const klib::User_input& p_input, int p_delta_ms, kkit::Project& p_project, kkit::Project_gfx& p_gfx);
		void draw(SDL_Renderer* p_rnd, const klib::User_input& p_input, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx) const;
	};
}

#endif
