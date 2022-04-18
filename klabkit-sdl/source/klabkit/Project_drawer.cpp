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
	klib::gfx::blit(p_rnd, wall_tiles[p_tile_no], p_x, p_y);
}
