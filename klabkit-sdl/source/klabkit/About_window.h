#ifndef KLABKIT_ABOUTWINDOW_H
#define KLABKIT_ABOUTWINDOW_H

#include <SDL.h>
#include <string>
#include <utility>
#include <vector>
#include "./../klib/User_input.h"
#include "./../klib/Timer.h"
#include "Project_gfx.h"
#include "constants.h"

namespace kkit {

	class About_window {

		mutable int bg_index{ 0 };
		std::size_t str_index{ 0 };
		std::pair<std::size_t, std::size_t> get_coords(int) const;
		std::vector<klib::Timer> timers;

		std::vector<std::string> ABOUT_MESSAGE{
			"KKIT/SDL - Ken's Labyrinth Developer's Toolkit",
			"Version: " + std::string(c::APP_VERSION),
			"",
			"Written by Kai E. Fr" + std::string(1, 127) + "land aka \"kaimitai\"",
			"Build date: " + std::string(__DATE__) + " at " + std::string(__TIME__) + " CET",
			" ",
			"The latest version can be found at",
			"https://github.com/kaimitai/klabkit-sdl",
			" ",
			"For questions, bug reports and suggestions,",
			"the author can be contacted at:",
			" ",
			" e-mail: kaimitai@pm.me",
			"Threema: 79K5EKHC"
		};

	public:
		About_window(void);
		void draw(SDL_Renderer*, const klib::User_input& p_input, const kkit::Project_gfx& p_gfx) const;
		void move(unsigned int delta_ms, const klib::User_input& p_input);
	};

}

#endif
