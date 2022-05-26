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
	auto l_cbytes = kkit::compression::compress_lzw_block(out_bytes);

	std::string l_file_text = "constexpr std::array<byte," + std::to_string(l_cbytes.size()) + "> LZW_BYTES_X { {";
	for (byte b : l_cbytes)
		l_file_text += std::to_string(static_cast<int>(b)) + ",";

	l_file_text.pop_back();
	l_file_text += "} };";

	klib::file::append_string_to_file(l_file_text, "lzw_pic.cpp");
}

SDL_Texture* kkit::gfx::create_bg_texture(SDL_Renderer* p_rnd, int p_bg_no) {
	SDL_Surface* result = SDL_CreateRGBSurface(0, 256, 256, 24, 0, 0, 0, 0);

	if (p_bg_no == 0)
		for (int i = 0; i < 256; ++i)
			for (int j = 0; j < 256; ++j)
				klib::gfx::put_pixel(result, i, j, SDL_MapRGB(result->format, i % 64, j % 64, 255));

	else if (p_bg_no == 1)
		for (int i = 0; i < 256; ++i)
			for (int j = 0; j < 256; ++j)
				klib::gfx::put_pixel(result, i, j, SDL_MapRGB(result->format, 0, i % 64, j % 64));

	else if (p_bg_no == 2)
		for (int i = 0; i < 256; ++i)
			for (int j = 0; j < 256; ++j)
				klib::gfx::put_pixel(result, i, j, SDL_MapRGB(result->format, i % 64, 0, j % 64));

	else if (p_bg_no == 3)
		for (int i = 0; i < 256; ++i)
			for (int j = 0; j < 256; ++j)
				klib::gfx::put_pixel(result, i, j, SDL_MapRGB(result->format, i % 64, j % 64, (i + j) % 64));

	else if (p_bg_no == 4)
		for (int i = 0; i < 256; ++i)
			for (int j = 0; j < 256; ++j)
				klib::gfx::put_pixel(result, i, j, SDL_MapRGB(result->format, (i + j) % 256, i, j));

	else
		throw std::exception("Invalid background no");

	return(klib::gfx::surface_to_texture(p_rnd, result));
}

std::vector<SDL_Texture*> kkit::gfx::create_bg_textures(SDL_Renderer* p_rnd) {
	std::vector<SDL_Texture*> result;

	for (int i{ 0 }; i <= 4; ++i)
		result.push_back(create_bg_texture(p_rnd, i));

	return result;
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

void kkit::gfx::set_application_icon(SDL_Window* p_window, const kkit::Project& p_project) {
	SDL_Surface* l_icon_srf = image_to_sdl_surface(flat_image_to_2d(kkit::compression::decompress_lzw_block(std::vector<byte>(begin(LZW_BYTES_ICON), end(LZW_BYTES_ICON)))),
		p_project.get_palette());

	SDL_SetWindowIcon(p_window, l_icon_srf);
	SDL_FreeSurface(l_icon_srf);
}

std::vector<SDL_Texture*> kkit::gfx::get_project_textures(SDL_Renderer* p_rnd, const kkit::Project& p_project) {
	std::vector<SDL_Texture*> result;

	for (int i{ 0 }; i < p_project.get_wall_image_count(); ++i)
		result.push_back(get_project_texture(p_rnd, p_project, i));

	//bmp_to_lzw_file(l_palette, "C:\\Users\\Kai\\Documents\\star.bmp");

	return result;
}

SDL_Texture* kkit::gfx::get_project_texture(SDL_Renderer* p_rnd, const kkit::Project& p_project, int p_frame_no) {
	SDL_Surface* l_bmp = image_to_sdl_surface(p_project.get_wall(p_frame_no).get_image(), p_project.get_palette());
	SDL_Texture* result = klib::gfx::surface_to_texture(p_rnd, l_bmp);

	return result;
}

std::vector<std::vector<std::vector<byte>>> kkit::gfx::generate_project_gfx_2dv(void) {
	std::vector<std::vector<std::vector<byte>>> result;

	auto l_dir_sprite{ flat_image_to_2d(kkit::compression::decompress_lzw_block(std::vector<byte>(begin(LZW_BYTES_DIR_ARROW), end(LZW_BYTES_DIR_ARROW)))) };
	for (int i{ 0 }; i < 2; ++i) {
		result.push_back(l_dir_sprite);
		klib::util::rot_sq_matrix_ccw(l_dir_sprite);
	}

	l_dir_sprite = flat_image_to_2d(kkit::compression::decompress_lzw_block(std::vector<byte>(begin(LZW_BYTES_PLAYER_POS), end(LZW_BYTES_PLAYER_POS))));
	for (int i{ 0 }; i < 4; ++i) {
		result.push_back(l_dir_sprite);
		klib::util::rot_sq_matrix_ccw(l_dir_sprite);
	}

	result.push_back(flat_image_to_2d(kkit::compression::decompress_lzw_block(std::vector<byte>(begin(LZW_BYTES_STAR), end(LZW_BYTES_STAR)))));
	result.push_back(flat_image_to_2d(kkit::compression::decompress_lzw_block(std::vector<byte>(begin(LZW_BYTES_STAR2), end(LZW_BYTES_STAR2)))));

	return result;
}

std::vector<SDL_Texture*> kkit::gfx::get_program_textures(SDL_Renderer* p_rnd, const kkit::Project& p_project) {
	std::vector<SDL_Texture*> result;
	palette l_palette{ p_project.get_palette() };

	for (const auto& l_v : generate_project_gfx_2dv())
		result.push_back(klib::gfx::surface_to_texture(p_rnd, image_to_sdl_surface(l_v, l_palette)));

	return result;
}

std::vector<SDL_Texture*> kkit::gfx::get_minimap_textures(SDL_Renderer* p_rnd, const kkit::Project& p_project) {
	std::vector<SDL_Texture*> result;

	for (int i{ 0 }; i < p_project.get_board_count(); ++i)
		result.push_back(create_board_minimap_texture(p_rnd, p_project, i));

	return result;
}

SDL_Surface* kkit::gfx::create_board_minimap_surface(const kkit::Project& p_project, int p_board_no) {
	std::vector<std::vector<byte>> l_minimap_image;
	const kkit::Board l_board = p_project.get_board(p_board_no);

	for (int j{ 0 }; j < c::MAP_H; ++j) {
		std::vector<byte> l_pixel_row;
		for (int i{ 0 }; i < c::MAP_W; ++i) {
			byte l_pal_ind = static_cast<byte>((l_board.get_tile_no(j, i) + 1) & 255);
			l_pixel_row.push_back(l_pal_ind);
		}
		l_minimap_image.push_back(l_pixel_row);
	}

	SDL_Surface* result = image_to_sdl_surface(l_minimap_image, p_project.get_palette());

	return result;
}

SDL_Texture* kkit::gfx::create_board_minimap_texture(SDL_Renderer* p_rnd, const kkit::Project& p_project, int p_board_no, bool p_destroy_surface) {
	return klib::gfx::surface_to_texture(p_rnd, create_board_minimap_surface(p_project, p_board_no));
}

std::vector<std::vector<byte>> kkit::gfx::flat_image_to_2d(const std::vector<byte>& p_input) {
	std::vector<std::vector<byte>> result;

	for (int i{ 0 }; i < 64; ++i)
		result.push_back(std::vector<byte>(begin(p_input) + 64 * i, begin(p_input) + 64 * (i + 1)));

	return result;
}

SDL_Color kkit::gfx::get_pulse_color(int p_color_no, int p_frame_no) {
	int l_frame_no = (p_frame_no >= 128 ? 256 - p_frame_no : p_frame_no);

	if (p_color_no == 0)
		return SDL_Color(255 - l_frame_no, 255 - l_frame_no / 2, 0);
	else if (p_color_no == 1)
		return SDL_Color(255 - l_frame_no / 2, 255 - l_frame_no / 2, 255 - l_frame_no / 2);
	else if (p_color_no == 2)
		return SDL_Color(255 - l_frame_no, 255 - l_frame_no / 2, 255 - l_frame_no / 3);
	else if (p_color_no == 3)
		return SDL_Color(l_frame_no / 4, l_frame_no / 4, 255 - l_frame_no / 2);
	else
		return SDL_Color(180 - l_frame_no / 2, 240 - l_frame_no / 2, 0);
}

bool kkit::gfx::wall_to_bmp(const std::vector<std::vector<byte>>& p_image, const palette& p_palette, const std::string& p_directory, const std::string& p_file_full_path) {
	SDL_Surface* l_bmp = image_to_sdl_surface(p_image, p_palette, false);
	std::filesystem::create_directory(p_directory);
	int file_status = SDL_SaveBMP(l_bmp, p_file_full_path.c_str());

	SDL_FreeSurface(l_bmp);
	return (file_status != -1);
}

void kkit::gfx::tilemap_to_bmp(const kkit::Project& p_project) {
	constexpr int TILES_PER_ROW = 16;
	int l_tile_h = 1 + (p_project.get_wall_image_count() - 1) / TILES_PER_ROW;

	SDL_Surface* l_bmp = SDL_CreateRGBSurface(0, c::WALL_IMG_W * TILES_PER_ROW, c::WALL_IMG_H * l_tile_h, 8, 0, 0, 0, 0);
	set_surface_project_palette(l_bmp, p_project);

	for (int i{ 0 }; i < l_tile_h; ++i)
		for (int j{ 0 }; j < TILES_PER_ROW && (i * TILES_PER_ROW + j < p_project.get_wall_image_count()); ++j)
			draw_wall_tile_on_surface(l_bmp, p_project.get_wall(i * TILES_PER_ROW + j).get_image(),
				j * c::WALL_IMG_W, i * c::WALL_IMG_H);

	std::string l_out_file = p_project.get_file_name("tilemap", c::FILE_EXT_BMP);
	save_bmp_file(l_bmp, p_project.get_bmp_folder(), l_out_file);
}

void kkit::gfx::palette_to_bmp(const kkit::Project& p_project) {
	SDL_Surface* l_bmp = SDL_CreateRGBSurface(0, 16, 16, 8, 0, 0, 0, 0);
	set_surface_project_palette(l_bmp, p_project);

	for (int i{ 0 }; i < 16; ++i)
		for (int j{ 0 }; j < 16; ++j)
			klib::gfx::put_pixel(l_bmp, i, j, i * 16 + j);

	std::string l_out_file = p_project.get_file_name("palette", c::FILE_EXT_BMP);
	save_bmp_file(l_bmp, p_project.get_bmp_folder(), l_out_file);
}

void kkit::gfx::draw_wall_tile_on_surface(SDL_Surface* p_bmp, const std::vector<std::vector<byte>> p_image, int p_x, int p_y, int p_trans_index, bool p_skip_trans) {

	for (int i{ 0 }; i < static_cast<int>(p_image.size()); ++i)
		for (int j = 0; j < static_cast<int>(p_image[i].size()); ++j) {
			byte l_pal_ind = p_image[i][j];
			if (!p_skip_trans || l_pal_ind != c::TRANSP_PAL_INDEX) {
				klib::gfx::put_pixel(p_bmp, p_x + i, p_y + j,
					l_pal_ind == 255 ? p_trans_index : l_pal_ind);
			}
		}
}

void kkit::gfx::save_bmp_file(SDL_Surface* p_bmp, const std::string& p_out_folder, const std::string& p_out_file) {
	std::filesystem::create_directory(p_out_folder);
	std::string l_out_file = p_out_folder + "/" + p_out_file;

	if (SDL_SaveBMP(p_bmp, l_out_file.c_str()) == -1) {
		SDL_FreeSurface(p_bmp);
		throw std::exception("Could not save BMP files");
	}

	SDL_FreeSurface(p_bmp);
}

void kkit::gfx::project_map_to_bmp(const kkit::Project& p_project, int p_board_no, SDL_Color p_floor_col, bool p_flash_blast, bool p_flash_noclip) {
	SDL_Surface* l_bmp = SDL_CreateRGBSurface(0, c::WALL_IMG_W * c::MAP_W, c::WALL_IMG_H * c::MAP_H, 8, 0, 0, 0, 0);
	set_surface_project_palette(l_bmp, p_project);

	auto l_program_gfx = generate_project_gfx_2dv();
	auto l_star = klib::util::sparsify<byte>(l_program_gfx[6], 2);
	auto l_star_2 = klib::util::sparsify<byte>(l_program_gfx[7], 2);

	int l_floor_index = find_nearest_palette_index(p_floor_col, p_project.get_palette());

	const auto& l_board = p_project.get_board(p_board_no);

	// draw board tiles first, using floor color as "background"
	for (int t_y{ 0 }; t_y < c::MAP_H; ++t_y)
		for (int t_x{ 0 }; t_x < c::MAP_W; ++t_x) {
			int l_tile_no = l_board.get_tile_no(t_x, t_y);
			draw_wall_tile_on_surface(l_bmp, p_project.get_image_as_2dv(l_tile_no), c::WALL_IMG_W * t_x, c::WALL_IMG_H * t_y, l_floor_index);

			// draw flash if applicable
			if (p_flash_blast && l_board.is_blast(t_x, t_y))
				draw_wall_tile_on_surface(l_bmp, l_star_2, c::WALL_IMG_W * t_x, c::WALL_IMG_H * t_y, l_floor_index, true);

			bool l_inside = false;

			if (l_tile_no >= 0)
				l_inside |= l_board.is_inside(t_x, t_y) && (!p_project.is_inside(l_tile_no) || p_project.is_clip_override(l_tile_no));
			else
				l_inside |= !l_board.is_inside(t_x, t_y);

			if (p_flash_noclip && l_inside)
				draw_wall_tile_on_surface(l_bmp, l_star, c::WALL_IMG_W * t_x, c::WALL_IMG_H * t_y + c::WALL_IMG_H / 2, l_floor_index, true);
		}

	int l_player_dir = 0;
	if (l_board.get_player_start_direction() == kkit::Player_direction::Left)
		l_player_dir = 1;
	else if (l_board.get_player_start_direction() == kkit::Player_direction::Down)
		l_player_dir = 2;
	else if (l_board.get_player_start_direction() == kkit::Player_direction::Right)
		l_player_dir = 3;

	// draw player start position
	draw_wall_tile_on_surface(l_bmp, l_program_gfx[2 + l_player_dir],
		c::WALL_IMG_W * l_board.get_player_start_x(),
		c::WALL_IMG_H * l_board.get_player_start_y(),
		l_floor_index, true);

	std::filesystem::create_directory(p_project.get_bmp_folder());
	std::string l_out_file = p_project.get_file_full_path(c::FILE_BOARDS, c::FILE_EXT_BMP, p_board_no);
	auto file_status = SDL_SaveBMP(l_bmp, l_out_file.c_str());

	SDL_FreeSurface(l_bmp);
}

SDL_Surface* kkit::gfx::image_to_sdl_surface(const std::vector<std::vector<byte>>& p_image, const palette& p_palette, bool p_transp) {
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

	if (p_transp) {
		SDL_Color l_trans_rgb = l_palette.at(c::TRANSP_PAL_INDEX);
		SDL_SetColorKey(l_bmp, true, SDL_MapRGB(l_bmp->format, l_trans_rgb.r, l_trans_rgb.g, l_trans_rgb.b));
	}

	return l_bmp;
}

void kkit::gfx::set_surface_project_palette(SDL_Surface* p_surface, const kkit::Project& p_project) {
	auto l_palette = tuple_to_sdl_palette(p_project.get_palette());

	SDL_Color out_palette[256];
	for (int i{ 0 }; i < 256; ++i)
		out_palette[i] = l_palette[i];

	SDL_SetPaletteColors(p_surface->format->palette, out_palette, 0, 256);
}

SDL_Color kkit::gfx::tuple_to_sdl_color(const std::tuple<byte, byte, byte>& p_col) {
	return SDL_Color{ std::get<0>(p_col), std::get<1>(p_col), std::get<2>(p_col) };
}

std::vector<SDL_Color> kkit::gfx::tuple_to_sdl_palette(const std::vector<std::tuple<byte, byte, byte>>& p_palette) {
	std::vector<SDL_Color> result;

	for (const auto& l_col : p_palette)
		result.push_back(tuple_to_sdl_color(l_col));

	return result;
}
