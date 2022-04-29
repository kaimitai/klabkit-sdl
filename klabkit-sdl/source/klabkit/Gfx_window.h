#ifndef KLABKIT_GFX_WINDOW_H
#define KLABKIT_GFX_WINDOW_H

#include <SDL.h>
#include <vector>
#include "Project.h"
#include "Project_gfx.h"
#include "constants.h"
#include "./../klib/User_input.h"
#include "./../klib/Button.h"
#include "./../klib/Timer.h"

namespace kkit {

	// tile selector offsets and constants
	constexpr int GW_TTPR{ 10 }; // tiles per row
	constexpr int GW_TTPC{ 10 }; // tiles per column

	constexpr int GW_TX{ 20 };
	constexpr int GW_TY{ 80 };
	constexpr int GW_TW{ c::WALL_IMG_W * GW_TTPR };
	constexpr int GW_TH{ c::WALL_IMG_H * GW_TTPC };

	// selected tile window
	constexpr int GW_SX{ GW_TX + GW_TW + 40 };
	constexpr int GW_SY{ GW_TY };
	constexpr int GW_SW{ 4 * c::WALL_IMG_W };
	constexpr int GW_SH{ 4 * c::WALL_IMG_H };

	//selected tile attribute buttons
	constexpr int GW_AB_X{ GW_SX };
	constexpr int GW_AB_Y{ GW_SY + GW_SH };
	constexpr int GW_AB_W{ GW_SW };
	constexpr int GW_AB_H{ 30 };
	constexpr int GW_AB_SPACING{ 5 };

	// bmp imp/exp buttons
	constexpr int GW_BMP_X{ GW_SX };
	constexpr int GW_BMP_Y{ GW_AB_Y + 5 * GW_AB_H };
	constexpr int GW_BMP_W{ GW_SW / 2 - 10 };
	constexpr int GW_BMP_H{ GW_AB_H };

	// save kzp/dat
	constexpr int GW_SAV_X{ GW_SX };
	constexpr int GW_SAV_Y{ GW_BMP_Y + 5 * GW_AB_H };
	constexpr int GW_SAV_W{ GW_SW};
	constexpr int GW_SAV_H{ GW_AB_H };

	class Gfx_window {
		int tile_row, tile_x, tile_y;
		std::vector<klib::Button> buttons;
		std::vector<klib::Timer> timers;

		int get_selected_tile_no(void) const;
		int c_max_tile_row(int p_total_tile_count) const;
		void button_click(SDL_Renderer* p_rnd, std::size_t p_button_no, kkit::Project& p_project, kkit::Project_gfx& p_gfx);

	public:
		Gfx_window(void);
		void move(SDL_Renderer* p_rnd, const klib::User_input& p_input, int p_delta_ms, kkit::Project& p_project, kkit::Project_gfx& p_gfx);
		void draw(SDL_Renderer* p_rnd, const klib::User_input& p_input, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx) const;
	};

}

#endif
