#include <iostream>

#include "./klabkit/Project.h"
#include "./klabkit/Project_drawer.h"
#include "klib/file.h"

int main(int argc, char* args[]) {

	SDL_Window* l_window{ nullptr };
	SDL_Renderer* l_rnd{ nullptr };
	bool l_exit{ false };

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
	}
	else {
		// Event handler
		SDL_Event e;

		l_window = SDL_CreateWindow("Klabkit/SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_SHOWN);
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
			kkit::Project project("C:/Users/Kai/Downloads/klabkit");
			kkit::Project_drawer p_drawer(l_rnd, project);

			uint32_t last_logic_time = SDL_GetTicks() - 1;
			uint32_t last_draw_time = SDL_GetTicks() - 17;

			uint32_t delta = 1;
			uint32_t deltaDraw = 17;

			while (!l_exit) {

				uint32_t tick_time = SDL_GetTicks();	// current time

				deltaDraw = tick_time - last_draw_time;
				delta = tick_time - last_logic_time;

				SDL_PumpEvents();

				if (SDL_PollEvent(&e) != 0)
					if (e.type == SDL_QUIT)
						l_exit = true;

				if (deltaDraw >= 25) { // capped frame rate of ~40 is ok
					//mainwindow.draw(input, &texture_manager);
					last_draw_time = SDL_GetTicks();

					for (int i = 0; i < 10; ++i)
						for (int j = 0; j < 10; ++j)
							p_drawer.draw_board(l_rnd, project, 20, 20);

					//Update screen
					SDL_RenderPresent(l_rnd);
				}

				SDL_Delay(5);

			}
		}


	}





	//kkit::gfx::project_walls_to_bmps(project);

	//for (int i{ 0 }; i < 30; ++i) kkit::gfx::project_map_to_bmp(project, i);

	/*
	auto v_boards = project.get_board_bytes();
	klib::file::write_bytes_to_file(v_boards, "C:/Users/Kai/Downloads/klabkit/boards_out.dat");

	auto v_walls = project.get_wall_bytes();
	klib::file::write_bytes_to_file(v_walls, "C:/Users/Kai/Downloads/klabkit/walls_out.dat");
	*/

	// Destroy window
	SDL_DestroyWindow(l_window);

	// Quit SDL subsystems
	SDL_Quit();

	return 0;
}
