#include <filesystem>
#include <string>

#include "kkit_gfx.h"
#include "./../klib/gfx.h"
#include "constants.h"

void kkit::gfx::project_walls_to_bmps(const kkit::Project& p_project) {
	for (int i{ 0 }; i < p_project.get_wall_image_count(); ++i) {
		const auto& l_wall = p_project.get_wall(i);

		SDL_Surface* l_bmp = SDL_CreateRGBSurface(0, c::WALL_IMG_W, c::WALL_IMG_H, 8, 0, 0, 0, 0);
		set_surface_project_palette(l_bmp, p_project);

		for (int i{ 0 }; i < c::WALL_IMG_W; ++i)
			for (int j = 0; j < c::WALL_IMG_H; ++j)
				klib::gfx::put_pixel(l_bmp, i, j, l_wall.get_palette_index(i, j));

		std::filesystem::create_directory(p_project.get_bmp_folder());
		std::string l_out_file = p_project.get_bmp_file_path(c::FILE_WALLS, i);
		auto file_status = SDL_SaveBMP(l_bmp, l_out_file.c_str());

		SDL_FreeSurface(l_bmp);
	}
}

void kkit::gfx::project_map_to_bmp(const kkit::Project& p_project, int p_board_no) {
	SDL_Surface* l_bmp = SDL_CreateRGBSurface(0, c::WALL_IMG_W * c::MAP_W, c::WALL_IMG_H * c::MAP_H, 8, 0, 0, 0, 0);
	set_surface_project_palette(l_bmp, p_project);

	const auto& l_board = p_project.get_board(p_board_no);

	for (int t_y{ 0 }; t_y < c::MAP_H; ++t_y)
		for (int t_x{ 0 }; t_x < c::MAP_W; ++t_x) {
			// skip the empty tiles, otherwise draw the wall gfx
			if (!l_board.is_empty_tile(t_x, t_y)) {
				const auto& l_wall = p_project.get_wall(l_board.get_tile_no(t_x, t_y));

				for (int w_x{ 0 }; w_x < c::WALL_IMG_W; ++w_x)
					for (int w_y{ 0 }; w_y < c::WALL_IMG_H; ++w_y) {
						int l_pal_index = l_wall.get_palette_index(w_x, w_y);
						// skip transparent pixels
						if (l_pal_index != c::TRANSP_PAL_INDEX)
							klib::gfx::put_pixel(l_bmp, c::WALL_IMG_W * t_x + w_x, c::WALL_IMG_H * t_y + w_y, l_pal_index);
					}

			}
		}

	std::filesystem::create_directory(p_project.get_bmp_folder());
	std::string l_out_file = p_project.get_bmp_file_path(c::FILE_BOARDS, p_board_no);
	auto file_status = SDL_SaveBMP(l_bmp, l_out_file.c_str());

	SDL_FreeSurface(l_bmp);
}

void kkit::gfx::set_surface_project_palette(SDL_Surface* p_surface, const kkit::Project& p_project) {
	auto l_palette = tuple_to_sdl_palette(p_project.get_palette());

	SDL_Color out_palette[256];
	for (int i{ 0 }; i < 256; ++i)
		out_palette[i] = l_palette[i];

	SDL_SetPaletteColors(p_surface->format->palette, out_palette, 0, 256);
}

std::vector<SDL_Color> kkit::gfx::tuple_to_sdl_palette(const std::vector<std::tuple<byte, byte, byte>>& p_palette) {
	std::vector<SDL_Color> result;

	for (const auto& l_col : p_palette)
		result.push_back(SDL_Color{ std::get<0>(l_col), std::get<1>(l_col), std::get<2>(l_col) });

	return result;
}
