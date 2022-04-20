#ifndef KLABLKIT_PROJECT_GFX_H
#define KLABLKIT_PROJECT_GFX_H

#include <SDL.h>
#include <map>
#include <vector>
#include "Project.h"

namespace kkit {

	constexpr std::size_t INDEX_WALL_TEXTURES{ 0 };
	constexpr std::size_t INDEX_APP_TEXTURES{ INDEX_WALL_TEXTURES + 1 };

	class Project_gfx {

		std::map<int, std::vector<SDL_Texture*>> textures;

	public:
		Project_gfx(SDL_Renderer* p_rnd, const kkit::Project& p_project);
		~Project_gfx(void);

		SDL_Texture* get_tile_texture(int p_frame_no) const;
	};

}

#endif
