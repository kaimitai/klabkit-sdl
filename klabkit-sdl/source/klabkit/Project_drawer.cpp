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
	int l_tile_count = (board_zoom == -1 ? 16 : 8 / (1 << board_zoom));
	int l_tile_spacing = (board_zoom == -1 ? 32 : 64 * (1 << board_zoom));
	const auto& board = p_project.get_board(this->board_ind);

	for (int i{ 0 }; i < l_tile_count; ++i)
		for (int j{ 0 }; j < l_tile_count; ++j) {
			if (!board.is_empty_tile(board_x + i, board_y + j))
				this->draw_tile(p_rnd, board.get_tile_no(board_x + i, board_y + j), p_x + l_tile_spacing * i, p_y + l_tile_spacing * j);
		}
}
