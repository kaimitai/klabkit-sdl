#include "Main_window.h"
#include "./../klib/gfx.h"
#include "constants.h"

kkit::Main_window::Main_window(SDL_Renderer* p_rnd) : board_window{ Board_window(p_rnd) }, window_index{ 0 }, project_loaded{ false } {
	buttons.push_back(klib::Button("Boards", MW_BX, MW_BY, MW_BW, MW_BH, klib::gc::COL_BLACK,
		klib::gc::COL_ORANGE, klib::gc::COL_YELLOW));
	buttons.push_back(klib::Button("Gfx", MW_BX + MW_BW, MW_BY, MW_BW, MW_BH, klib::gc::COL_BLACK,
		klib::gc::COL_WHITE));
	buttons.push_back(klib::Button("About", c::APP_W - MW_BW, MW_BY, MW_BW, MW_BH, klib::gc::COL_BLACK,
		klib::gc::COL_WHITE));
}

void kkit::Main_window::load_project_files(SDL_Renderer* p_rnd, kkit::Project& p_project, kkit::Project_gfx& p_gfx) {
	try {
		if (p_project.get_wall_image_count() == 0) {
			p_project.initialize_walls();
			p_gfx.load_project_textures(p_rnd, p_project);
			p_gfx.add_toast_ok("Loaded " + std::to_string(p_project.get_wall_image_count()) + " gfx tiles");
		}

		if (p_project.get_board_count() == 0) {
			p_project.initialize_maps();
			p_gfx.add_toast_ok("Loaded " + std::to_string(p_project.get_board_count()) + " maps");
		}

		this->project_loaded = true;
	}
	catch (const std::exception& ex) {
		p_gfx.add_toast_error(ex.what());
		this->button_click(2, p_rnd, p_project, p_gfx);
	}
	catch (...) {
		p_gfx.add_toast_error("Unspecified error occurred when loading project files.");
		this->button_click(2, p_rnd, p_project, p_gfx);
	}
}

void kkit::Main_window::move(SDL_Renderer* p_rnd, const klib::User_input& p_input, int p_delta_ms, Project& p_project, Project_gfx& p_gfx) {
	if (p_input.mouse_clicked())
		for (std::size_t i{ 0 }; i < buttons.size(); ++i)
			if (buttons[i].is_hit(p_input.mx(), p_input.my())) {
				this->button_click(i, p_rnd, p_project, p_gfx);
			}

	if (!project_loaded && window_index < 2)
		this->load_project_files(p_rnd, p_project, p_gfx);

	if (window_index == 0)
		board_window.move(p_input, p_delta_ms, p_project, p_gfx);
	else if (window_index == 1)
		gfx_window.move(p_rnd, p_input, p_delta_ms, p_project, p_gfx);
	else
		about_window.move(p_delta_ms, p_input);

	// move toast notifcations
	p_gfx.move(p_delta_ms);
}

void kkit::Main_window::draw(SDL_Renderer* p_rnd, const klib::User_input& p_input, Project& p_project, Project_gfx& p_gfx) {
	SDL_SetRenderDrawColor(p_rnd, MW_BG_COLOR.r, MW_BG_COLOR.g, MW_BG_COLOR.b, 0);
	SDL_RenderClear(p_rnd);

	if (window_index == 0)
		board_window.draw(p_rnd, p_input, p_project, p_gfx);
	else if (window_index == 1)
		gfx_window.draw(p_rnd, p_input, p_project, p_gfx);
	else
		about_window.draw(p_rnd, p_input, p_gfx);

	klib::gfx::draw_rect(p_rnd, 2 * MW_BW, 0, c::APP_W - 3 * MW_BW, MW_BH, klib::gc::COL_WHITE, 0);

	for (const auto& button : buttons)
		button.draw(p_rnd, p_gfx.get_font(), p_input);

	p_gfx.draw(p_rnd);

}

void kkit::Main_window::button_click(std::size_t p_button_no, SDL_Renderer* p_rnd, kkit::Project& p_project, kkit::Project_gfx& p_gfx) {
	if (!project_loaded && p_button_no < 2)
		this->load_project_files(p_rnd, p_project, p_gfx);
	else {

		if (window_index != p_button_no) {
			buttons[p_button_no].set_bg_color(klib::gc::COL_ORANGE);
			buttons[window_index].set_bg_color(klib::gc::COL_WHITE);
			window_index = p_button_no;
		}

	}
}
