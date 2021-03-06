#ifndef KLABKIT_MAINWINDOW_H
#define KLABKIT_MAINWINDOW_H

#include <SDL.h>
#include <vector>
#include "About_window.h"
#include "Board_window.h"
#include "Gfx_window.h"
#include "Project.h"
#include "Project_gfx.h"
#include "./../klib/User_input.h"
#include "./../klib/Button.h"

namespace kkit {

	constexpr int MW_BX{ 0 };
	constexpr int MW_BY{ 0 };
	constexpr int MW_BW{ 200 };
	constexpr int MW_BH{ 35 };
	constexpr SDL_Color MW_BG_COLOR{ 0, 0, 128 };

	class Main_window {
		std::size_t window_index;

		About_window about_window;
		Board_window board_window;
		Gfx_window gfx_window;

		std::vector<klib::Button> buttons;

		void button_click(std::size_t p_button_no);

	public:
		Main_window(SDL_Renderer* p_rnd, bool p_is_klab_v2);

		void move(SDL_Renderer* p_rnd, const klib::User_input& p_input, int p_delta_ms, Project& p_project, Project_gfx& p_gfx);
		void draw(SDL_Renderer* p_rnd, const klib::User_input& p_input, Project& p_project, Project_gfx& p_gfx);
	};

}

#endif
