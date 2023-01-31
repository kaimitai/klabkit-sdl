#include <stdexcept>
#include <string>
#include "./imgui/imgui.h"
#include "./imgui/imgui_impl_sdl.h"
#include "./imgui/imgui_impl_sdlrenderer.h"
#include "klabkit/constants.h"
#include "klabkit/Project.h"
#include "klabkit/Project_gfx.h"
#include "klabkit/Project_config.h"
#include "klabkit/kkit_gfx.h"
#include "klabkit/Main_window.h"
#include "klabkit/xml_handler.h"
#include "klib/User_input.h"
#include "klib/file.h"

#include "./klabkit/Board_ui.h"

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

constexpr char ERROR_LOG_FILE[]{ "kkit-sdl-err.log" };

int main(int argc, char* args[]) try {

	SDL_Window* l_window{ nullptr };
	SDL_Renderer* l_rnd{ nullptr };
	bool l_exit{ false };

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::runtime_error(std::string("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError())).c_str());
	else {
		// Event handler
		SDL_Event e;

		l_window = SDL_CreateWindow(kkit::c::get_application_window_caption().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, kkit::c::APP_W, kkit::c::APP_H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (l_window == nullptr)
			throw std::runtime_error(std::string("Window could not be created! SDL_Error: " + std::string(SDL_GetError())).c_str());
		else {
			l_rnd = SDL_CreateRenderer(l_window, -1, SDL_RENDERER_ACCELERATED);

			if (l_rnd == nullptr)
				throw std::runtime_error(std::string("Renderer could not be created!SDL Error: " + std::string(SDL_GetError())).c_str());
			else {
				//Initialize renderer color
				SDL_SetRenderDrawColor(l_rnd, 0x00, 0x00, 0x00, 0x00);
			}

			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsLight();

			// load resources
			kkit::Project project(kkit::xml::read_config_xml(kkit::c::CONF_FILE_NAME));
			kkit::gfx::set_application_icon(l_window, project);
			kkit::Project_gfx p_gfx(l_rnd, project);

			// Setup Platform/Renderer backends
			ImGui_ImplSDL2_InitForSDLRenderer(l_window, l_rnd);
			ImGui_ImplSDLRenderer_Init(l_rnd);
			std::string l_ini_filename{ "kkit-sdl-windows.ini" };
			ImGui::GetIO().IniFilename = l_ini_filename.c_str();
			ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

			// main window object to handle all logic and drawing
			//kkit::Main_window main_window(l_rnd, project.get_config());
			kkit::Board_ui board_window(l_rnd, project.get_config());

			// input handler
			klib::User_input input;
			int mouse_wheel_y{ 0 };
			bool mw_used{ false };

			uint32_t last_logic_time = SDL_GetTicks() - 1;
			uint32_t last_draw_time = SDL_GetTicks() - 17;

			uint32_t delta = 1;
			uint32_t deltaDraw = 17;

			int l_w{ kkit::c::APP_W }, l_h{ kkit::c::APP_H };

			p_gfx.add_toast_ok("Welcome to KKIT/SDL! Using configuration \"" + project.get_config_label() + "\"");
			p_gfx.add_toast_ok("Loaded " + std::to_string(project.get_board_count()) + " boards (" +
				std::string(project.is_boards_kzp() ? "KZP" : "DAT") +
				") and " +
				std::to_string(project.get_wall_image_count()) + " gfx tiles (" +
				std::string(project.is_walls_kzp() ? "KZP" : "DAT")
				+ ")");

			project.add_message("Build Date: " + std::string(__DATE__) + " " +
				std::string(__TIME__) + " CET");

			project.add_message("Loaded " + std::to_string(project.get_board_count()) + " boards (" +
				std::string(project.is_boards_kzp() ? "KZP" : "DAT") +
				") and " +
				std::to_string(project.get_wall_image_count()) + " gfx tiles (" +
				std::string(project.is_walls_kzp() ? "KZP" : "DAT")
				+ ")");

			project.add_message("Welcome to KKIT/SDL! Using configuration \"" + project.get_config_label() + "\"");

			while (!l_exit) {

				uint32_t tick_time = SDL_GetTicks();	// current time

				deltaDraw = tick_time - last_draw_time;
				delta = tick_time - last_logic_time;
				int32_t mw_y{ 0 };

				mw_used = false;
				SDL_PumpEvents();

				if (SDL_PollEvent(&e) != 0) {
					ImGui_ImplSDL2_ProcessEvent(&e);

					if (e.type == SDL_QUIT)
						l_exit = true;
					else if (e.type == SDL_MOUSEWHEEL) {
						mw_used = true;
						mouse_wheel_y = e.wheel.y;
					}
				}

				if (delta != 0) {
					uint32_t realDelta = std::min(delta, 5u);
					SDL_GetWindowSize(l_window, &l_w, &l_h);

					input.move(realDelta, mw_used ? mouse_wheel_y : 0);
					board_window.move(l_rnd, input, realDelta, project, p_gfx, l_w, l_h);
					//main_window.move(l_rnd, input, realDelta, project, p_gfx);

					last_logic_time = tick_time;
				}

				if (deltaDraw >= 25) { // capped frame rate of ~40 is ok
					//mainwindow.draw(input, &texture_manager);
					last_draw_time = SDL_GetTicks();

					board_window.draw(l_rnd, input, project, p_gfx, l_w, l_h);
					//main_window.draw(l_rnd, input, project, p_gfx);

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

catch (const std::exception& ex) {
	klib::file::append_string_to_file("Runtime error. Exception was:\n" + std::string(ex.what()) + "\n", ERROR_LOG_FILE);
	return 1;
}
catch (...) {
	klib::file::append_string_to_file("Unknown runtime error occurred.\n", ERROR_LOG_FILE);
	return 1;
}
