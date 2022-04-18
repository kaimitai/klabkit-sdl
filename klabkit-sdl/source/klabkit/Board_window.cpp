#include "Board_window.h"

void kkit::Board_window::draw(SDL_Renderer* p_rnd, const kkit::Project& p_project, const kkit::Project_drawer& p_pdrawer) const {
	p_pdrawer.draw_board(p_rnd, p_project, 20, 20);
	p_pdrawer.draw_minimap(p_rnd, 20, 20 + 64 * 8 + 20);
}

void kkit::Board_window::move(const klib::User_input& p_input, int p_delta_ms, kkit::Project& p_project, kkit::Project_drawer& p_pdrawer) {
	if (p_input.is_pressed(SDL_SCANCODE_UP) || p_input.mw_up())
		p_pdrawer.move_grid_offset_y(p_input.is_ctrl_pressed() ? -4 : -1);
	else if (p_input.is_pressed(SDL_SCANCODE_DOWN) || p_input.mw_down())
		p_pdrawer.move_grid_offset_y(p_input.is_ctrl_pressed() ? 4 : 1);
	else if (p_input.is_pressed(SDL_SCANCODE_LEFT))
		p_pdrawer.move_grid_offset_x(p_input.is_ctrl_pressed() ? -4 : -1);
	else if (p_input.is_pressed(SDL_SCANCODE_RIGHT))
		p_pdrawer.move_grid_offset_x(p_input.is_ctrl_pressed() ? 4 : 1);
	else if (p_input.is_pressed(SDL_SCANCODE_KP_MINUS))
		p_pdrawer.move_grid_zoom(-1);
	else if (p_input.is_pressed(SDL_SCANCODE_KP_PLUS))
		p_pdrawer.move_grid_zoom(1);
}
