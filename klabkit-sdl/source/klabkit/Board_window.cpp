#include "Board_window.h"
#include "./../klib/klib_util.h"

void kkit::Board_window::draw(SDL_Renderer* p_rnd, const kkit::Project& p_project, const kkit::Project_drawer& p_pdrawer) const {
	p_pdrawer.draw_board(p_rnd, p_project, BW_BX, BW_BY);
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

	if (p_input.mouse_held() &&
		klib::util::is_p_in_rect(p_input.mx(), p_input.my(), BW_BX, BW_BY, BW_BW, BW_BW)) {
		auto l_tcoords = p_pdrawer.get_tile_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		p_project.clear_tile(p_pdrawer.get_board(), l_tcoords.first, l_tcoords.second);
	}
	else if (p_input.mouse_held() &&
		klib::util::is_p_in_rect(p_input.mx(), p_input.my(), BW_MX, BW_MY, BW_MW, BW_MW)) {
		p_pdrawer.click_minimap(p_input.mx() - BW_MX, p_input.my() - BW_MY);
	}
}
