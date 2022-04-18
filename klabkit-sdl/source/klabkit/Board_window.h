#ifndef KLABKIT_BOARD_WINDOW_H
#define KLABKIT_BOARD_WINDOW_H

#include <SDL.h>
#include "Project.h"
#include "Project_drawer.h"
#include "constants.h"
#include "./../klib/User_input.h"

namespace kkit {

	// pixel dimension and coordinates of level board
	constexpr int BW_BX{ 20 };
	constexpr int BW_BY{ 20 };
	constexpr int BW_BW{ c::WALL_IMG_W * 8 };

	class Board_window {

	public:
		Board_window(void) = default;
		void move(const klib::User_input& p_input, int p_delta_ms, kkit::Project& p_project, kkit::Project_drawer& p_pdrawer);
		void draw(SDL_Renderer* p_rnd, const kkit::Project& p_project, const kkit::Project_drawer& p_pdrawer) const;
	};
}

#endif