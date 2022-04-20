#include "Board_window.h"
#include "./../klib/file.h"
#include "./../klib/klib_util.h"
#include "./../klib/gfx.h"

kkit::Board_window::Board_window(SDL_Renderer* p_rnd) {
	grid_texture = SDL_CreateTexture(p_rnd, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 4096, 4096);
}

void kkit::Board_window::draw(SDL_Renderer* p_rnd, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx) const {
	draw_board(p_rnd, p_project, p_gfx, BW_BX, BW_BY);
	draw_minimap(p_rnd, BW_MX, BW_MY);
	draw_tile_picker(p_rnd, p_gfx, BW_MY, 20);

	// draw title
	klib::gfx::draw_label(p_rnd, p_gfx.get_font(), this->get_board_title(), 300, BW_MY, 100, 25);

	// draw selected tile (on board grid)
	std::string l_sel_tile{ "Tile: " + std::to_string(p_project.get_board(board_ind).get_tile_no(sel_tile_x, sel_tile_y)) };
	klib::gfx::draw_label(p_rnd, p_gfx.get_font(), l_sel_tile, 300, BW_MY, 200, 30);
}

void kkit::Board_window::move(const klib::User_input& p_input, int p_delta_ms, kkit::Project& p_project) {
	bool mouse_over_board_grid{ klib::util::is_p_in_rect(p_input.mx(), p_input.my(), BW_BX, BW_BY, BW_BW, BW_BW) };

	if (mouse_over_board_grid && p_input.is_ctrl_pressed() && p_input.mw_up()) {
		auto l_tcoords = this->get_tile_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		this->move_grid_zoom(0.2);
		//this->center_offset(l_tcoords);
	}
	else 	if (mouse_over_board_grid && p_input.is_ctrl_pressed() && p_input.mw_down()) {
		auto l_tcoords = this->get_tile_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		this->move_grid_zoom(-0.2);
		//this->center_offset(l_tcoords);
	}
	else if (p_input.is_pressed(SDL_SCANCODE_UP) || p_input.mw_up())
		this->move_grid_offset_y(p_input.is_ctrl_pressed() ? -4 * 64 : -64);
	else if (p_input.is_pressed(SDL_SCANCODE_DOWN) || p_input.mw_down())
		this->move_grid_offset_y(p_input.is_ctrl_pressed() ? 4 * 64 : 64);
	else if (p_input.is_pressed(SDL_SCANCODE_LEFT))
		this->move_grid_offset_x(p_input.is_ctrl_pressed() ? -4 * 64 : -1 * 64);
	else if (p_input.is_pressed(SDL_SCANCODE_RIGHT))
		this->move_grid_offset_x(p_input.is_ctrl_pressed() ? 4 * 64 : 1 * 64);
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

	if (p_input.mouse_held(false) && mouse_over_board_grid) {
		auto l_tcoords = this->get_tile_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		p_project.set_tile(this->board_ind, l_tcoords.first, l_tcoords.second, this->get_selected_tile(p_project));
	}
	else if (p_input.mouse_held(true) && mouse_over_board_grid) {
		auto l_tcoords = this->get_tile_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		sel_tile_x = l_tcoords.first;
		sel_tile_y = l_tcoords.second;
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
int kkit::Board_window::c_max_offset(void) const {
	return static_cast<int>(4096.0f - static_cast<float>(BW_BW) / this->zoom_factor);
}

void kkit::Board_window::validate_grid_offset(void) {
	zoom_factor = klib::util::validate(zoom_factor, 0.125f, 2.0f);
	board_px = klib::util::validate(board_px, 0, c_max_offset());
	board_py = klib::util::validate(board_py, 0, c_max_offset());
}

void kkit::Board_window::move_grid_offset_x(int p_dx) {
	this->board_px += p_dx;
	this->validate_grid_offset();
}

void kkit::Board_window::move_grid_offset_y(int p_dy) {
	this->board_py += p_dy;
	this->validate_grid_offset();
}

void kkit::Board_window::set_grid_offset(int p_x, int p_y) {
	this->board_px = p_x;
	this->board_py = p_y;
	this->validate_grid_offset();
}

void kkit::Board_window::click_minimap(int p_x, int p_y) {
	float f_x = p_x / static_cast<float>(BW_MW);
	float f_y = p_y / static_cast<float>(BW_MW);
	this->set_grid_offset((4096 * f_x), (4096 * f_y));
}

void kkit::Board_window::center_offset(void) {
	// TODO: Fix this function, the problem is centering tiles near the right and bottom edges
	//int l_dx = std::min(c_tile_cnt() / 2, (64 - board_x) / c_tile_cnt());
	//int l_dy = std::min(c_tile_cnt() / 2, (64 - board_y) / c_tile_cnt());
	int l_dx = 0;
	int l_dy = 0;

	this->set_grid_offset(board_px - l_dx, board_py - l_dy);
}

void kkit::Board_window::center_offset(std::pair<int, int> p_coords) {
	this->set_grid_offset(p_coords.first, p_coords.second);
	this->center_offset();
}

void kkit::Board_window::move_grid_zoom(float p_dz) {
	this->zoom_factor += p_dz;
	this->validate_grid_offset();
}

void kkit::Board_window::draw_board(SDL_Renderer* p_rnd, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx, int p_x, int p_y) const {
	//int l_tile_count = this->c_tile_cnt();
	//int l_tile_spacing = this->c_tile_pw();
	const auto& board = p_project.get_board(this->board_ind);

	SDL_SetRenderTarget(p_rnd, grid_texture);
	for (int i{ 0 }; i < 64; ++i)
		for (int j{ 0 }; j < 64; ++j)
			if (!board.is_empty_tile(i, j))
				klib::gfx::blit(p_rnd, p_gfx.get_tile_texture(board.get_tile_no(i, j)), 64 * i, 64 * j);

	// draw selected tile
	klib::gfx::draw_rect(p_rnd, sel_tile_x * 64, sel_tile_y * 64, 64, 64, SDL_Color{ 255,255,0 }, 2);

	SDL_SetRenderTarget(p_rnd, nullptr);

	klib::gfx::blit_full_spec(p_rnd, this->grid_texture, BW_BX, BW_BY, BW_BW, BW_BW, board_px, board_py, BW_BW / zoom_factor, BW_BW / zoom_factor);

	// draw outline
	klib::gfx::draw_rect(p_rnd, p_x, p_y, BOARD_PW, BOARD_PW, SDL_Color{ 255,255,255 }, 2);
}


void kkit::Board_window::draw_minimap(SDL_Renderer* p_rnd, int p_x, int p_y) const {
	klib::gfx::draw_rect(p_rnd, p_x, p_y, 128, 128, SDL_Color{ 0,0,0 }, 0);
	klib::gfx::draw_rect(p_rnd, p_x, p_y, 128, 128, SDL_Color{ 255,255,255 }, 2);

	float l_sel_factor = 16.0f / zoom_factor;

	klib::gfx::draw_rect(p_rnd, p_x + board_px * (128.0f / 4096.0f), p_y + board_py * (128.0f / 4096.0f), l_sel_factor, l_sel_factor, SDL_Color{ 255,255,0 }, 2);
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

int  kkit::Board_window::c_tile_pw(void) const {
	return static_cast<int>(64.0f * zoom_factor);
}

// the board was clicked, get its global coordinates from the pixels
// input pixel coordinates must be relative to the top left of the grid
// and do not call this function if clicking outside the actual level grid
std::pair<int, int> kkit::Board_window::get_tile_pos(int p_x, int p_y) const {
	int l_tx = board_px + p_x / zoom_factor;
	int l_ty = board_py + p_y / zoom_factor;
	return std::make_pair(l_tx / 64, l_ty / 64);
}

std::string kkit::Board_window::get_board_title(void) const {
	int l_episode_no{ board_ind / 10 + 1 };
	int l_map_no{ board_ind % 10 + 1 };

	return std::string("E" + std::to_string(l_episode_no) + "M" + std::to_string(l_map_no));
}
