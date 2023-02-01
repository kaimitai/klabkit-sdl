#ifndef KLABLKIT_PROJECT_GFX_H
#define KLABLKIT_PROJECT_GFX_H

#include <SDL.h>
#include <array>
#include <map>
#include <vector>
#include "Project.h"

namespace kkit {

	class Project_gfx {

		std::map<int, std::vector<SDL_Texture*>> textures;
		SDL_Color floor_color;

	public:
		Project_gfx(SDL_Renderer* p_rnd, const kkit::Project& p_project);
		~Project_gfx(void);

		void reload_texture(SDL_Renderer* p_rnd, const kkit::Project& p_project, int p_frame_no);
		void reload_minimap_texture(SDL_Renderer* p_rnd, const kkit::Project& p_project, int p_frame_no);

		SDL_Texture* get_texture(int p_type_no, int p_frame_no) const;
		SDL_Color get_floor_color(void) const;
		int get_texture_count(int p_type) const;
	};

}

#endif
