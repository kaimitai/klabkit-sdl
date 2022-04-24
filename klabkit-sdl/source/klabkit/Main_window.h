#ifndef KLABKIT_MAINWINDOW_H
#define KLABKIT_MAINWINDOW_H

#include <SDL.h>
#include <string>
#include "Board_window.h"
#include "Project.h"
#include "Project_gfx.h"
#include "./../klib/User_input.h"

namespace kkit {

	class Main_window {
		std::size_t window_index;
		Board_window board_window;

	public:
		Main_window(SDL_Renderer* p_rnd);

		void move(const klib::User_input& p_input, int p_delta_ms, Project& p_project, Project_gfx& p_gfx);
		void draw(SDL_Renderer* p_rnd, const klib::User_input& p_input, Project& p_project, Project_gfx& p_gfx);
	};

}

#endif
