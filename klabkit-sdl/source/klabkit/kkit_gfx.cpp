#include <filesystem>
#include <string>

#include "kkit_gfx.h"
#include "./../klib/gfx.h"
#include "constants.h"

void kkit::gfx::project_walls_to_bmps(const kkit::Project& p_project) {
	auto l_palette = tuple_to_sdl_palette(p_project.get_palette());
	std::filesystem::create_directory(p_project.get_bmp_folder());

	for (int i{ 0 }; i < p_project.get_wall_image_count(); ++i) {
		const auto& l_wall = p_project.get_wall(i);

		SDL_Surface* l_bmp = SDL_CreateRGBSurface(0, c::WALL_IMG_W, c::WALL_IMG_H, 8, 0, 0, 0, 0);

		SDL_Color out_palette[256];
		for (int i{ 0 }; i < 256; ++i)
			out_palette[i] = l_palette[i];

		SDL_SetPaletteColors(l_bmp->format->palette, out_palette, 0, 256);

		for (int i{ 0 }; i < c::WALL_IMG_W; ++i)
			for (int j = 0; j < c::WALL_IMG_H; ++j)
				klib::gfx::put_pixel(l_bmp, i, j, l_wall.get_palette_index(i, j));


		std::string l_out_file = p_project.get_wall_bmp_file_path(i);
		auto file_status = SDL_SaveBMP(l_bmp, l_out_file.c_str());

		SDL_FreeSurface(l_bmp);
	}
}

std::vector<SDL_Color> kkit::gfx::tuple_to_sdl_palette(const std::vector<std::tuple<byte, byte, byte>>& p_palette) {
	std::vector<SDL_Color> result;

	for (const auto& l_col : p_palette)
		result.push_back(SDL_Color{ std::get<0>(l_col), std::get<1>(l_col), std::get<2>(l_col) });

	return result;
}
