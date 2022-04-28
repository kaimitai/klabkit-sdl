#include <iostream>
#include "./klabkit/Project.h"
#include "./klabkit/Project_gfx.h"
#include "./klabkit/Project_config.h"
#include "./klabkit/constants.h"
#include "klabkit/Main_window.h"
#include "klib/User_input.h"

// #include "klib/file.h"
// #include "klabkit/compression.h"
#include "klabkit/xml_handler.h"

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
			kkit::Project project(kkit::xml::read_config_xml("kkit-sdl-config.xml"));
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
