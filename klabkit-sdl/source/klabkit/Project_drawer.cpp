#include <algorithm>

#include "Project_drawer.h"
#include "../klib/gfx.h"
#include "kkit_gfx.h"

kkit::Project_drawer::Project_drawer(SDL_Renderer* p_rnd, const kkit::Project& p_project) {
	this->wall_tiles = kkit::gfx::get_project_textures(p_rnd, p_project);
}

kkit::Project_drawer::~Project_drawer(void) {
	for (auto wt : this->wall_tiles)
		if (wt != nullptr)
			SDL_DestroyTexture(wt);
}

void kkit::Project_drawer::draw_tile(SDL_Renderer* p_rnd, int p_tile_no, int p_x, int p_y) const {
	klib::gfx::blit_scale(p_rnd, wall_tiles[p_tile_no], p_x, p_y, board_zoom == -1 ? 0 : 1 << board_zoom);
}

void kkit::Project_drawer::draw_board(SDL_Renderer* p_rnd, const kkit::Project& p_project, int p_x, int p_y) const {
	int l_tile_count = this->c_tile_cnt();
	int l_tile_spacing = this->c_tile_pw();
	const auto& board = p_project.get_board(this->board_ind);

	klib::gfx::draw_rect(p_rnd, p_x, p_y, BOARD_PW, BOARD_PW, SDL_Color{ 0, 0, 0 }, 0);

	for (int i{ 0 }; i < l_tile_count; ++i)
		for (int j{ 0 }; j < l_tile_count; ++j)
			if (!board.is_empty_tile(board_x + i, board_y + j))
				this->draw_tile(p_rnd, board.get_tile_no(board_x + i, board_y + j), p_x + l_tile_spacing * i, p_y + l_tile_spacing * j);

	// draw outline
	klib::gfx::draw_rect(p_rnd, p_x, p_y, BOARD_PW, BOARD_PW, SDL_Color{ 255,255,255 }, 2);
}

void kkit::Project_drawer::move_grid_offset_x(int p_dx) {
	this->board_x += p_dx;
	this->validate_grid_offset();
}

void kkit::Project_drawer::move_grid_offset_y(int p_dy) {
	this->board_y += p_dy;
	this->validate_grid_offset();
}

void kkit::Project_drawer::set_grid_offset(int p_x, int p_y) {
	this->board_x = p_x;
	this->board_y = p_y;
	this->validate_grid_offset();
}

void kkit::Project_drawer::click_minimap(int p_x, int p_y) {
	this->set_grid_offset((p_x - c_tile_cnt()) / 2, (p_y - c_tile_cnt()) / 2);
}

void kkit::Project_drawer::center_offset(void) {
	this->set_grid_offset((board_x - c_tile_cnt()) / 2, (board_y - c_tile_cnt()) / 2);
}

void kkit::Project_drawer::move_grid_zoom(int p_dz) {
	this->board_zoom += p_dz;
	this->validate_grid_offset();
}

int kkit::Project_drawer::c_tile_pw(void) const {
	if (board_zoom == -1)
		return c::WALL_IMG_W / 2;
	else
		return c::WALL_IMG_W * (1 << board_zoom);
}

int kkit::Project_drawer::c_tile_cnt(void) const {
	return BOARD_PW / this->c_tile_pw();
}

int kkit::Project_drawer::c_tile_offset_max(void) const {
	return c::MAP_W - this->c_tile_cnt();
}

void kkit::Project_drawer::validate_grid_offset(void) {
	if (board_zoom < -1)
		board_zoom = -1;
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

void kkit::Project_drawer::draw_minimap(SDL_Renderer* p_rnd, int p_x, int p_y) const {
	klib::gfx::draw_rect(p_rnd, p_x, p_y, 128, 128, SDL_Color{ 0,0,0 }, 0);
	klib::gfx::draw_rect(p_rnd, p_x, p_y, 128, 128, SDL_Color{ 255,255,255 }, 2);

	klib::gfx::draw_rect(p_rnd, p_x + board_x * 2, p_y + board_y * 2, c_tile_cnt() * 2, c_tile_cnt() * 2, SDL_Color{ 255,255,0 }, 2);
}

void kkit::Project_drawer::draw_tile_picker(SDL_Renderer* p_rnd, int p_x, int p_y) const {
	klib::gfx::draw_rect(p_rnd, p_x, p_y, 32 * 12, 32 * 16, SDL_Color{ 0,0,0 }, 0);

	for (int i{ 0 }; i < static_cast<int>(c::TILES.size()); ++i) {
		int l_y = i / 12;
		int l_x = i % 12;
		int l_index = c::TILES[i];
		if (l_index >= 0)
			klib::gfx::blit_scale(p_rnd, this->wall_tiles.at(l_index), p_x + l_x * 32, p_y + l_y * 32, 0);
	}

	klib::gfx::draw_rect(p_rnd, p_x + 32 * tile_x, p_y + 32 * tile_y, 32, 32, SDL_Color{ 255, 255, 0 }, 2);
}

// get selected tile
// need the project to fetch metadata
kkit::Map_tile kkit::Project_drawer::get_selected_tile(const kkit::Project& p_project) const {
	int l_tile_no = c::TILES.at(tile_y * 12 + tile_x);
	return kkit::Map_tile(l_tile_no, false, false, false);
}

void kkit::Project_drawer::click_tile_picker(int p_x, int p_y) {
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
std::pair<int, int> kkit::Project_drawer::get_tile_pos(int p_x, int p_y) const {
	return std::make_pair(board_x + p_x / c_tile_pw(), board_y + p_y / c_tile_pw());
}

int kkit::Project_drawer::get_board(void) const {
	return this->board_ind;
}
