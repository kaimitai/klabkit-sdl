#include <filesystem>
#include <map>
#include <string>

#include "kkit_gfx.h"
#include "./../klib/gfx.h"
#include "./../klib/file.h"
#include "./../klib/klib_util.h"
#include "compression.h"
#include "constants.h"

using byte = unsigned char;
using palette = std::vector<std::tuple<byte, byte, byte>>;

// TODO: Dev function that is used only for turning a 64x64x256 color bmp into lzw bytes
// remove
void kkit::gfx::bmp_to_lzw_file(const palette& p_palette, const std::string& p_filename) {
	auto l_bytes = load_bmp(p_palette, p_filename);
	std::vector<byte> out_bytes;
	for (const auto& w : l_bytes)
		out_bytes.insert(end(out_bytes), begin(w), end(w));
	std::string p_out_file = p_filename + ".lzw";
	klib::file::write_bytes_to_file(kkit::compression::compress_lzw_block(out_bytes), p_out_file);
}

byte kkit::gfx::find_nearest_palette_index(SDL_Color p_color, const palette& p_palette) {
	int min_distance = 3 * 256 * 256 + 1; // higher than any possible color distance
	byte result{ 0 };

	for (int i{ 0 }; i < static_cast<int>(p_palette.size()); ++i) {
		int p_r = std::get<0>(p_palette.at(i));
		int p_g = std::get<1>(p_palette.at(i));
		int p_b = std::get<2>(p_palette.at(i));

		int dr = p_r - p_color.r;
		int dg = p_g - p_color.g;
		int db = p_b - p_color.b;

		int l_dist = dr * dr + dg * dg + db * db;

		if (l_dist < min_distance) {
			min_distance = l_dist;
			result = static_cast<byte>(i);
		}
	}

	return result;
}

std::vector<std::vector<byte>> kkit::gfx::load_bmp(const palette& p_palette, const std::string& p_filename) {
	auto frame_srf = SDL_LoadBMP(p_filename.c_str());

	if (frame_srf == nullptr)
		throw std::exception("Could not load BMP");
	else if (frame_srf->format->BitsPerPixel != 8) {
		SDL_FreeSurface(frame_srf);
		throw std::exception("Not a 256-color BMP");
	}
	else if (frame_srf->w != 64 || frame_srf->h != 64) {
		SDL_FreeSurface(frame_srf);
		throw std::exception("Image dimensions not 64x64 pixels");
	}

	// initialize empty result
	std::vector<std::vector<byte>> result(64, std::vector<byte>(64, 0));
	// recolor dictionary
	std::map<std::tuple<byte, byte, byte>, byte> recolors;

	for (int i{ 0 }; i < 64; ++i)
		for (int j{ 0 }; j < 64; ++j) {
			auto l_data = klib::gfx::get_pixel(frame_srf, i, j);
			SDL_Color rgb;

			SDL_GetRGB(l_data, frame_srf->format, &rgb.r, &rgb.g, &rgb.b);
			std::tuple<byte, byte, byte> l_col_tuple(rgb.r, rgb.g, rgb.b);

			if (recolors.find(l_col_tuple) == end(recolors)) {
				byte l_nearest_index = find_nearest_palette_index(rgb, p_palette);
				recolors.insert(std::make_pair(l_col_tuple, l_nearest_index));
			}
			result[i][j] = recolors[l_col_tuple];
		}

	SDL_FreeSurface(frame_srf);
	return result;
}

std::vector<SDL_Texture*> kkit::gfx::get_project_textures(SDL_Renderer* p_rnd, const kkit::Project& p_project) {
	std::vector<SDL_Texture*> result;

	auto l_palette = p_project.get_palette();

	for (int i{ 0 }; i < p_project.get_wall_image_count(); ++i) {
		SDL_Surface* l_bmp = image_to_sdl_surface(p_project.get_wall(i).get_image(), l_palette);
		SDL_SetColorKey(l_bmp, true, SDL_MapRGB(l_bmp->format, std::get<0>(l_palette.at(c::TRANSP_PAL_INDEX)), std::get<1>(l_palette.at(c::TRANSP_PAL_INDEX)), std::get<2>(l_palette.at(c::TRANSP_PAL_INDEX))));
		result.push_back(klib::gfx::surface_to_texture(p_rnd, l_bmp));
	}

	//bmp_to_lzw_file(l_palette, "C:\\Users\\Kai\\Documents\\star.bmp");

	return result;
}

std::vector<SDL_Texture*> kkit::gfx::get_program_textures(SDL_Renderer* p_rnd, const kkit::Project& p_project) {
	std::vector<SDL_Texture*> result;
	palette l_palette{ p_project.get_palette() };

	auto l_dir_sprite{ flat_image_to_2d(kkit::compression::decompress_lzw_block(std::vector<byte>(begin(LZW_BYTES_DIR_ARROW), end(LZW_BYTES_DIR_ARROW)))) };
	SDL_Surface* l_bmp;

	for (int i{ 0 }; i < 2; ++i) {
		l_bmp = image_to_sdl_surface(l_dir_sprite, l_palette);
		SDL_SetColorKey(l_bmp, true, SDL_MapRGB(l_bmp->format, std::get<0>(l_palette.at(c::TRANSP_PAL_INDEX)), std::get<1>(l_palette.at(c::TRANSP_PAL_INDEX)), std::get<2>(l_palette.at(c::TRANSP_PAL_INDEX))));
		result.push_back(klib::gfx::surface_to_texture(p_rnd, l_bmp));

		klib::util::rot_sq_matrix_ccw(l_dir_sprite);
	}

	l_dir_sprite = flat_image_to_2d(kkit::compression::decompress_lzw_block(std::vector<byte>(begin(LZW_BYTES_PLAYER_POS), end(LZW_BYTES_PLAYER_POS))));

	for (int i{ 0 }; i < 4; ++i) {
		l_bmp = image_to_sdl_surface(l_dir_sprite, l_palette);
		SDL_SetColorKey(l_bmp, true, SDL_MapRGB(l_bmp->format, std::get<0>(l_palette.at(c::TRANSP_PAL_INDEX)), std::get<1>(l_palette.at(c::TRANSP_PAL_INDEX)), std::get<2>(l_palette.at(c::TRANSP_PAL_INDEX))));
		result.push_back(klib::gfx::surface_to_texture(p_rnd, l_bmp));

		klib::util::rot_sq_matrix_ccw(l_dir_sprite);
	}

	l_dir_sprite = flat_image_to_2d(kkit::compression::decompress_lzw_block(std::vector<byte>(begin(LZW_BYTES_STAR), end(LZW_BYTES_STAR))));
	l_bmp = image_to_sdl_surface(l_dir_sprite, l_palette);
	SDL_SetColorKey(l_bmp, true, SDL_MapRGB(l_bmp->format, std::get<0>(l_palette.at(c::TRANSP_PAL_INDEX)), std::get<1>(l_palette.at(c::TRANSP_PAL_INDEX)), std::get<2>(l_palette.at(c::TRANSP_PAL_INDEX))));
	result.push_back(klib::gfx::surface_to_texture(p_rnd, l_bmp));

	return result;
}

std::vector<std::vector<byte>> kkit::gfx::flat_image_to_2d(const std::vector<byte>& p_input) {
	std::vector<std::vector<byte>> result;

	for (int i{ 0 }; i < 64; ++i)
		result.push_back(std::vector<byte>(begin(p_input) + 64 * i, begin(p_input) + 64 * (i + 1)));

	return result;
}

void kkit::gfx::project_walls_to_bmps(const kkit::Project& p_project) {
	auto l_palette = p_project.get_palette();

	for (int i{ 0 }; i < p_project.get_wall_image_count(); ++i) {
		auto l_image = p_project.get_wall(i).get_image();
		SDL_Surface* l_bmp = image_to_sdl_surface(l_image, l_palette);

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

SDL_Surface* kkit::gfx::image_to_sdl_surface(const std::vector<std::vector<byte>>& p_image, const palette& p_palette) {
	auto l_palette = tuple_to_sdl_palette(p_palette);
	int l_h{ static_cast<int>(p_image.size()) };
	int l_w{ static_cast<int>(p_image.size() > 0 ? p_image[0].size() : 0) };

	SDL_Color out_palette[256];
	for (int i{ 0 }; i < 256; ++i)
		out_palette[i] = l_palette[i];

	SDL_Surface* l_bmp = SDL_CreateRGBSurface(0, l_w, l_h, 8, 0, 0, 0, 0);
	SDL_SetPaletteColors(l_bmp->format->palette, out_palette, 0, 256);

	for (int i{ 0 }; i < l_w; ++i)
		for (int j = 0; j < l_h; ++j)
			klib::gfx::put_pixel(l_bmp, i, j, p_image[i][j]);

	return l_bmp;
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
