#include <iostream>
#include "./klabkit/Project.h"
#include "./klabkit/Project_gfx.h"
#include "./klabkit/Project_config.h"
#include "./klabkit/constants.h"
#include "klabkit/Main_window.h"
#include "klib/User_input.h"

// #include "klib/file.h"
// #include "klabkit/compression.h"

float resize_window(SDL_Renderer* p_rnd, int p_w, int p_h, float scale) {
	float l_scale_x = p_w / static_cast<float>(kkit::c::APP_W);
	float l_scale_y = p_h / static_cast<float>(kkit::c::APP_H);

	float result = std::max(1.0f, std::min(l_scale_x, l_scale_y));

	SDL_RenderSetScale(p_rnd, result, result);

	return result;
}

int main(int argc, char* args[]) {

	//auto v = klib::file::read_file_as_bytes("c:/users/kai/downloads/klabkit/songs.kzp");
	//auto w = kkit::compression::decompress_songs_kzp(v);
	//return 0;

	SDL_Window* l_window{ nullptr };
	SDL_Renderer* l_rnd{ nullptr };
	bool l_exit{ false };

	float l_scale{ 1.0f };

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
	}
	else {
		// Event handler
		SDL_Event e;

		l_window = SDL_CreateWindow(kkit::c::get_application_window_caption().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, kkit::c::APP_W, kkit::c::APP_H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (l_window == nullptr)
			std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
		else {
			l_rnd = SDL_CreateRenderer(l_window, -1, SDL_RENDERER_ACCELERATED);

			if (l_rnd == nullptr) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				exit(1);
			}
			else {
				//Initialize renderer color
				SDL_SetRenderDrawColor(l_rnd, 0x00, 0x00, 0x00, 0x00);
			}

			// load resources
			kkit::Project_config config("DEFAULT",
				"C:/users/kfrol/downloads/klabkit",
				30, 448, 0,
				{ 81, 122, 418, 419 },
				{ -1, -2, -3, -3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,
				// doors
				79, 80, 87,92,93,98,99,100,106,107,114,115,122,-3,
				// monsters
				214, 221, 222, 225, 228, 233, 238, 244, 250, 261, 267, 272,	277, 279,
				284, 285, 289, 292, 302, 340, 355, 369, 376, 406,310,-3,-3,-3,
				// walls
				0,1,2,3,4,5,6,7,8,9,10,11,14,15,
				18,19,20,21,22,23,24,25,26,27,28,29,30,31,
				32,33,34,35,36,37,38,39,40,41,42,43,44,45,
				46,47,48,49,50,51,52,53,54,55,56,57,58,59,
				62,63,64,65,66,67,68,69,70,71,72,73,75,76,
				 77, 78, 81, 82, 83,122,123,124,125,126,127,128,129,130,
				131,132,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,
				// decorative
				133,135,137,139,140,141,142,144,146,147,148,149,150,151,
				155,156,157,158,159,160,161,162,164,165,166,167,168,-3,
				// powerups & items
				169,170,171,172,173,174,175,176,177,178,179,180,181,182,
				183,184,185,186,187,188,189,190,191,192,196,199,208,209,
				210,211,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3,
				// rarely used sprites
				404,405,406,418,419,420,421,422,423,424,425,426,427,428,
				438,441,442,443,-3,-3,-3,-3,-3,-3,-3,-3,-3,-3
				});

			kkit::Project project(config);
			kkit::Project_gfx p_gfx(l_rnd, project);

			// main window object to handle all logic and drawing
			kkit::Main_window main_window(l_rnd);

			// input handler
			klib::User_input input;
			int mouse_wheel_y{ 0 };
			bool mw_used{ false };

			uint32_t last_logic_time = SDL_GetTicks() - 1;
			uint32_t last_draw_time = SDL_GetTicks() - 17;

			uint32_t delta = 1;
			uint32_t deltaDraw = 17;

			p_gfx.add_toast_ok("Welcome to Ken's Labyrinth Developer's Toolkit / SDL");
			p_gfx.add_toast_ok("Loaded " + std::to_string(project.get_board_count()) + " maps and " +
				std::to_string(project.get_wall_image_count()) + " gfx tiles");

			while (!l_exit) {

				uint32_t tick_time = SDL_GetTicks();	// current time

				deltaDraw = tick_time - last_draw_time;
				delta = tick_time - last_logic_time;
				int32_t mw_y{ 0 };

				mw_used = false;
				SDL_PumpEvents();

				if (SDL_PollEvent(&e) != 0)
					if (e.type == SDL_QUIT)
						l_exit = true;
					else if (e.type == SDL_MOUSEWHEEL) {
						mw_used = true;
						mouse_wheel_y = e.wheel.y;
					}
					else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
						l_scale = resize_window(l_rnd, e.window.data1, e.window.data2, l_scale);
						SDL_SetWindowSize(l_window,
							static_cast<int>(l_scale * kkit::c::APP_W),
							static_cast<int>(l_scale * kkit::c::APP_H));
					}

				if (delta != 0) {
					uint32_t realDelta = std::min(delta, 5u);

					input.move(realDelta, mw_used ? mouse_wheel_y : 0, l_scale, l_scale);
					main_window.move(l_rnd, input, realDelta, project, p_gfx);

					last_logic_time = tick_time;
				}

				if (deltaDraw >= 25) { // capped frame rate of ~40 is ok
					//mainwindow.draw(input, &texture_manager);
					last_draw_time = SDL_GetTicks();

					main_window.draw(l_rnd, input, project, p_gfx);

					//Update screen
					SDL_RenderPresent(l_rnd);
				}

				SDL_Delay(1);

			}
		}


	}

	// Destroy window
	SDL_DestroyWindow(l_window);

	// Quit SDL subsystems
	SDL_Quit();

	return 0;
}
