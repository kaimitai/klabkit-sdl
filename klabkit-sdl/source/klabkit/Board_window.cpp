#include "Board_window.h"
#include "./../klib/file.h"
#include "./../klib/klib_util.h"
#include "./../klib/gfx.h"

void kkit::Board_window::draw_tile(SDL_Renderer* p_rnd, const kkit::Project_gfx& p_gfx, int p_tile_no, int p_x, int p_y) const {
	klib::gfx::blit_p2_scale(p_rnd, p_gfx.get_tile_texture(p_tile_no), p_x, p_y, board_zoom);
}

void kkit::Board_window::draw(SDL_Renderer* p_rnd, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx) const {
	draw_board(p_rnd, p_project, p_gfx, BW_BX, BW_BY);
	draw_minimap(p_rnd, BW_MX, BW_MY);
	draw_tile_picker(p_rnd, p_gfx, BW_MY, 20);
}

void kkit::Board_window::move(const klib::User_input& p_input, int p_delta_ms, kkit::Project& p_project) {
	bool mouse_over_board_grid{ klib::util::is_p_in_rect(p_input.mx(), p_input.my(), BW_BX, BW_BY, BW_BW, BW_BW) };

	if (mouse_over_board_grid && p_input.is_ctrl_pressed() && p_input.mw_up()) {
		auto l_tcoords = this->get_tile_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		this->move_grid_zoom(1);
		this->center_offset(l_tcoords);
	}
	else 	if (mouse_over_board_grid && p_input.is_ctrl_pressed() && p_input.mw_down()) {
		auto l_tcoords = this->get_tile_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		this->move_grid_zoom(-1);
		this->center_offset(l_tcoords);
	}
	else if (p_input.is_pressed(SDL_SCANCODE_UP) || p_input.mw_up())
		this->move_grid_offset_y(p_input.is_ctrl_pressed() ? -4 : -1);
	else if (p_input.is_pressed(SDL_SCANCODE_DOWN) || p_input.mw_down())
		this->move_grid_offset_y(p_input.is_ctrl_pressed() ? 4 : 1);
	else if (p_input.is_pressed(SDL_SCANCODE_LEFT))
		this->move_grid_offset_x(p_input.is_ctrl_pressed() ? -4 : -1);
	else if (p_input.is_pressed(SDL_SCANCODE_RIGHT))
		this->move_grid_offset_x(p_input.is_ctrl_pressed() ? 4 : 1);
	else if (p_input.is_pressed(SDL_SCANCODE_KP_MINUS))
		this->move_grid_zoom(-1);
	else if (p_input.is_pressed(SDL_SCANCODE_KP_PLUS))
		this->move_grid_zoom(1);
	else if (p_input.is_ctrl_pressed() && p_input.is_pressed(SDL_SCANCODE_S))
		klib::file::write_bytes_to_file(p_project.get_board_bytes(), "BOARDS.DAT");
	else if (p_input.is_pressed(SDL_SCANCODE_PAGEDOWN) && board_ind > 0)
		--board_ind;
	else if (p_input.is_pressed(SDL_SCANCODE_PAGEUP) && board_ind < p_project.get_board_count() - 1)
		++board_ind;

	if (p_input.mouse_held() && mouse_over_board_grid) {
		auto l_tcoords = this->get_tile_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		//p_project.clear_tile(p_pdrawer.get_board(), l_tcoords.first, l_tcoords.second);
		p_project.set_tile(this->board_ind, l_tcoords.first, l_tcoords.second, this->get_selected_tile(p_project));
	}
	else if (p_input.mouse_held() &&
		klib::util::is_p_in_rect(p_input.mx(), p_input.my(), BW_MX, BW_MY, BW_MW, BW_MW)) {
		this->click_minimap(p_input.mx() - BW_MX, p_input.my() - BW_MY);
	}
	else if (p_input.mouse_held() &&
		klib::util::is_p_in_rect(p_input.mx(), p_input.my(), BW_MY, 20, 12 * 32, 16 * 32)) {
		this->click_tile_picker(p_input.mx() - BW_MY, p_input.my() - 20);
	}

}


// internal calculations
int kkit::Board_window::c_tile_pw(void) const {
	if (board_zoom < 0)
		return c::WALL_IMG_W >> board_zoom * (-1);
	else
		return c::WALL_IMG_W << board_zoom;
}

int kkit::Board_window::c_tile_cnt(void) const {
	return BOARD_PW / this->c_tile_pw();
}

int kkit::Board_window::c_tile_offset_max(void) const {
	return c::MAP_W - this->c_tile_cnt();
}

void kkit::Board_window::validate_grid_offset(void) {
	if (board_zoom < -3)
		board_zoom = -3;
	else if (board_zoom > 2)
		board_zoom = 2;

	if (board_x < 0)
		board_x = 0;
	else if (board_x > c_tile_offset_max())
		board_x = c_tile_offset_max();

	if (board_y < 0)
		board_y = 0;
	else if (board_y > c_tile_offset_max())
		board_y = c_tile_offset_max();
}

void kkit::Board_window::move_grid_offset_x(int p_dx) {
	this->board_x += p_dx;
	this->validate_grid_offset();
}

void kkit::Board_window::move_grid_offset_y(int p_dy) {
	this->board_y += p_dy;
	this->validate_grid_offset();
}

void kkit::Board_window::set_grid_offset(int p_x, int p_y) {
	this->board_x = p_x;
	this->board_y = p_y;
	this->validate_grid_offset();
}

void kkit::Board_window::click_minimap(int p_x, int p_y) {
	this->set_grid_offset((p_x - c_tile_cnt()) / 2, (p_y - c_tile_cnt()) / 2);
}

void kkit::Board_window::center_offset(void) {
	// TODO: Fix this function, the problem is centering tiles near the right and bottom edges
	int l_dx = std::min(c_tile_cnt() / 2, (64 - board_x) / c_tile_cnt());
	int l_dy = std::min(c_tile_cnt() / 2, (64 - board_y) / c_tile_cnt());

	this->set_grid_offset(board_x - l_dx, board_y - l_dy);
}

void kkit::Board_window::center_offset(std::pair<int, int> p_coords) {
	this->set_grid_offset(p_coords.first, p_coords.second);
	this->center_offset();
}

void kkit::Board_window::move_grid_zoom(int p_dz) {
	this->board_zoom += p_dz;
	this->validate_grid_offset();
}


void kkit::Board_window::draw_board(SDL_Renderer* p_rnd, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx, int p_x, int p_y) const {
	int l_tile_count = this->c_tile_cnt();
	int l_tile_spacing = this->c_tile_pw();
	const auto& board = p_project.get_board(this->board_ind);

	klib::gfx::draw_rect(p_rnd, p_x, p_y, BOARD_PW, BOARD_PW, SDL_Color{ 0, 0, 0 }, 0);

	for (int i{ 0 }; i < l_tile_count; ++i)
		for (int j{ 0 }; j < l_tile_count; ++j)
			if (!board.is_empty_tile(board_x + i, board_y + j))
				this->draw_tile(p_rnd, p_gfx, board.get_tile_no(board_x + i, board_y + j), p_x + l_tile_spacing * i, p_y + l_tile_spacing * j);

	// draw outline
	klib::gfx::draw_rect(p_rnd, p_x, p_y, BOARD_PW, BOARD_PW, SDL_Color{ 255,255,255 }, 2);
}





void kkit::Board_window::draw_minimap(SDL_Renderer* p_rnd, int p_x, int p_y) const {
	klib::gfx::draw_rect(p_rnd, p_x, p_y, 128, 128, SDL_Color{ 0,0,0 }, 0);
	klib::gfx::draw_rect(p_rnd, p_x, p_y, 128, 128, SDL_Color{ 255,255,255 }, 2);

	klib::gfx::draw_rect(p_rnd, p_x + board_x * 2, p_y + board_y * 2, c_tile_cnt() * 2, c_tile_cnt() * 2, SDL_Color{ 255,255,0 }, 2);
}

void kkit::Board_window::draw_tile_picker(SDL_Renderer* p_rnd, const kkit::Project_gfx& p_gfx, int p_x, int p_y) const {
	klib::gfx::draw_rect(p_rnd, p_x, p_y, 32 * 12, 32 * 16, SDL_Color{ 0,0,0 }, 0);

	for (int i{ 0 }; i < static_cast<int>(c::TILES.size()); ++i) {
		int l_y = i / 12;
		int l_x = i % 12;
		int l_index = c::TILES[i];
		if (l_index >= 0)
			klib::gfx::blit_p2_scale(p_rnd, p_gfx.get_tile_texture(l_index), p_x + l_x * 32, p_y + l_y * 32, -1);
	}

	klib::gfx::draw_rect(p_rnd, p_x + 32 * tile_x, p_y + 32 * tile_y, 32, 32, SDL_Color{ 255, 255, 0 }, 2);
}

// get selected tile
// need the project to fetch metadata
kkit::Map_tile kkit::Board_window::get_selected_tile(const kkit::Project& p_project) const {
	int l_tile_no = c::TILES.at(tile_y * 12 + tile_x);
	return kkit::Map_tile(l_tile_no, false, false, false);
}

void kkit::Board_window::click_tile_picker(int p_x, int p_y) {
	int l_x = p_x / 32;
	int l_y = p_y / 32;
	int l_index = l_y * 12 + l_x;
	if (l_index < c::TILES.size() && c::TILES[l_index] >= -1) {
		this->tile_x = l_x;
		this->tile_y = l_y;
	}
}

// the board was clicked, get its global coordinates from the pixels
// input pixel coordinates must be relative to the top left of the grid
// and do not call this function if clicking outside the actual level grid
std::pair<int, int> kkit::Board_window::get_tile_pos(int p_x, int p_y) const {
	return std::make_pair(board_x + p_x / c_tile_pw(), board_y + p_y / c_tile_pw());
}
