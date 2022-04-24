#include <map>
#include "Project.h"
#include "constants.h"
#include "compression.h"
#include "./../klib/file.h"

using byte = unsigned char;

kkit::Project::Project(const std::string& p_folder) : project_folder{ p_folder } {
	initialize_palette();
	initialize_walls();
	initialize_maps();

	kkit::c::populate_tilemap(this->get_wall_image_count());
}

int kkit::Project::save_boards_kzp(void) const {
	auto l_board_lzw_bytes = kkit::compression::compress_boards_kzp(this->get_board_bytes());
	klib::file::write_bytes_to_file(l_board_lzw_bytes, get_file_path(c::FILE_BOARDS, c::FILE_EXT_KZP));
	return static_cast<int>(l_board_lzw_bytes.size());
}

int kkit::Project::save_walls_kzp(void) const {
	auto l_walls_lzw_bytes = kkit::compression::compress_walls_kzp(this->get_wall_bytes());
	klib::file::write_bytes_to_file(l_walls_lzw_bytes, get_file_path(c::FILE_WALLS, c::FILE_EXT_KZP));
	return static_cast<int>(l_walls_lzw_bytes.size());
}

// initializers
void kkit::Project::initialize_palette(void) {
	for (int i{ 0 }; i < 16; ++i) {
		for (int j{ 0 }; j < 16; ++j) {
			byte l_r = c::PAL_GEN[i][0] * (j + 1) / 17;
			byte l_g = c::PAL_GEN[i][1] * (j + 1) / 17;
			byte l_b = c::PAL_GEN[i][2] * (j + 1) / 17;
			palette.push_back(std::make_tuple(4 * l_r, 4 * l_g, 4 * l_b));
		}
	}
}

void kkit::Project::initialize_walls(void) {
	std::vector<byte> wall_bytes;

	try {
		wall_bytes = klib::file::read_file_as_bytes(get_file_path(c::FILE_WALLS, c::FILE_EXT_DAT));
	}
	catch (std::exception& ex) {
		wall_bytes = kkit::compression::decompress_walls_kzp(klib::file::read_file_as_bytes(get_file_path(c::FILE_WALLS, c::FILE_EXT_KZP)));
	}

	int l_num_walls = (static_cast<int>(wall_bytes.size()) - c::WALL_DATA_OFFSET) / (c::WALL_IMG_BYTES);

	for (int i{ 0 }; i < l_num_walls; ++i) {
		walls.push_back(kkit::Wall(std::vector<unsigned char>(begin(wall_bytes) + c::WALL_DATA_OFFSET + i * c::WALL_IMG_BYTES, begin(wall_bytes) + c::WALL_DATA_OFFSET + (i + 1) * c::WALL_IMG_BYTES), wall_bytes[i]));
	}
}

void kkit::Project::initialize_maps(void) {
	std::vector<byte> map_bytes;

	try {
		map_bytes = klib::file::read_file_as_bytes(get_file_path(c::FILE_BOARDS, c::FILE_EXT_DAT));
	}
	catch (const std::exception& ex) {
		//map_bytes = kkit::compression::decompress_boards_kzp(klib::file::read_file_as_bytes(get_file_path("BOARDS_orig", c::FILE_EXT_KZP)));
		map_bytes = kkit::compression::decompress_boards_kzp(klib::file::read_file_as_bytes(get_file_path(c::FILE_BOARDS, c::FILE_EXT_KZP)));
	}

	int l_num_maps = static_cast<int>(map_bytes.size()) / c::MAP_BYTES;

	for (int i{ 0 }; i < l_num_maps; ++i)
		maps.push_back(kkit::Board(std::vector<unsigned char>(begin(map_bytes) + i * c::MAP_BYTES, begin(map_bytes) + (i + 1) * c::MAP_BYTES)));

	// DEBUG/TODO: Analysis
	/*
	std::map<bool, int> cnt;
	for (int i{ 0 }; i < 1; ++i) {
		for (int j{ 0 }; j < 64; ++j)
			for (int k{ 0 }; k < 64; ++k) {
				int l_tile_no = maps[i].get_tile_no(j, k);

				if(l_tile_no==-1)
					cnt[maps[i].is_vertical(j, k)]++;

				if (l_tile_no >= 0 && is_directional(l_tile_no))
						cnt[maps[i].is_vertical(j, k)]++;

			}

	}
	*/
}

// utility functions
std::string  kkit::Project::get_file_path(const std::string& p_subfolder, const std::string& p_file_prefix, const std::string& p_file_ext, int p_frame_no) const {
	std::string result{ this->project_folder };
	if (!p_subfolder.empty())
		result += "/" + p_subfolder;

	result += "/" + p_file_prefix + (p_frame_no == 0 ? "" : std::to_string(p_frame_no + 1)) + "." + p_file_ext;

	return result;
}

std::string  kkit::Project::get_file_path(const std::string& p_file_prefix, const std::string& p_file_ext, int p_frame_no) const {
	return get_file_path(std::string(), p_file_prefix, p_file_ext, p_frame_no);
}

std::string kkit::Project::get_dat_file_name(const std::string& p_filename) const {
	return project_folder + "/" + p_filename + ".DAT";
}

std::string  kkit::Project::get_bmp_file_path(const std::string& p_file_prefix, int p_frame_no) const {
	return get_file_path(c::FILE_EXT_BMP, p_file_prefix, c::FILE_EXT_BMP, p_frame_no);
}

int kkit::Project::get_board_count(void) const {
	return static_cast<int>(this->maps.size());
}

std::string kkit::Project::get_bmp_folder(void) const {
	return project_folder + "/" + c::FILE_EXT_BMP;
}

// getters
const std::vector<std::tuple<byte, byte, byte>>& kkit::Project::get_palette(void) const {
	return this->palette;
}

int kkit::Project::get_wall_image_count(void) const {
	return static_cast<int>(this->walls.size());
}

const kkit::Wall& kkit::Project::get_wall(int p_frame_no) const {
	return this->walls.at(p_frame_no);
}

const kkit::Board& kkit::Project::get_board(int p_board_no) const {
	return this->maps.at(p_board_no);
}

std::vector<byte> kkit::Project::get_board_bytes(void) const {
	std::vector<byte> result;

	for (int i{ 0 }; i < this->maps.size(); ++i) {
		auto l_map_bytes = maps[i].get_bytes();
		result.insert(end(result), begin(l_map_bytes), end(l_map_bytes));
	}

	return result;
}

std::vector<byte> kkit::Project::get_wall_bytes(void) const {
	std::vector<byte> result;

	for (const auto& wall : this->walls)
		result.push_back(wall.get_header_byte());

	while (result.size() < 1024)
		result.push_back(0);

	for (const auto& wall : this->walls) {
		auto l_wall_bytes = wall.get_image_bytes();
		result.insert(end(result), begin(l_wall_bytes), end(l_wall_bytes));
	}

	return result;
}

// generate map tile based on walls metadata
// input is wall index. We leave the vertical indicator false in any case, but the caller can change this based on its own logic
// which is not accessible here
kkit::Map_tile kkit::Project::gen_map_tile(int p_tile_no) const {
	if (p_tile_no == -1)
		return kkit::Map_tile();
	else
		return kkit::Map_tile(p_tile_no, this->is_inside(p_tile_no), this->is_blast(p_tile_no), false);
}

std::string kkit::Project::get_block_type_as_string(int p_tile_no) const {
	if (this->walls.at(p_tile_no).get_wall_type() == kkit::Wall_type::Cube)
		return "Cube";
	else if (this->walls.at(p_tile_no).get_wall_type() == kkit::Wall_type::Plane)
		return "Plane";
	else
		return "Dir";
}

std::pair<int, int> kkit::Project::get_player_start_pos(int p_board_no) const {
	return std::make_pair(maps.at(p_board_no).get_player_start_x(), maps.at(p_board_no).get_player_start_y());
}

void kkit::Project::clear_tile(int p_board_no, int p_x, int p_y) {
	maps[p_board_no].clear_tile(p_x, p_y);
}

void kkit::Project::set_tile(int p_board_no, int p_x, int p_y, const kkit::Map_tile& p_tile) {
	if (p_x != maps.at(p_board_no).get_player_start_x() || p_y != maps.at(p_board_no).get_player_start_y())
		maps[p_board_no].set_tile(p_x, p_y, p_tile);
}

void kkit::Project::toggle_mt_direction(int p_board_no, int p_x, int p_y) {
	// if this is the start tile, we change the direction the player is facing, otherwise it changes between horizontal/vertical for directional tile types
	if (maps.at(p_board_no).is_start_tile(p_x, p_y))
		maps[p_board_no].toggle_player_start_direction();
	// verify that the wall metadata actually lists this as a directional tile before changing the direction
	else if (maps.at(p_board_no).get_tile_no(p_x, p_y) >= 0 && get_wall_type(maps.at(p_board_no).get_tile_no(p_x, p_y)) == kkit::Wall_type::Direction)
		maps[p_board_no].toggle_direction(p_x, p_y);
}

void kkit::Project::toggle_mt_blast(int p_board_no, int p_x, int p_y) {
	if (maps.at(p_board_no).get_tile_no(p_x, p_y) >= 0)
		maps[p_board_no].toggle_blast(p_x, p_y);
}

void kkit::Project::toggle_mt_inside(int p_board_no, int p_x, int p_y) {
	if (maps.at(p_board_no).get_tile_no(p_x, p_y) >= 0)
		maps[p_board_no].toggle_inside(p_x, p_y);
}

void kkit::Project::set_player_start_position(int p_board_no, int p_x, int p_y, kkit::Player_direction p_direction) {
	maps.at(p_board_no).set_player_start_position(p_x, p_y, p_direction);
}

// wall tile attribute manipulators
void kkit::Project::toggle_wt_type(int p_wall_no) {
	walls.at(p_wall_no).toggle_type();
}

void kkit::Project::toggle_wt_blast(int p_wall_no) {
	walls.at(p_wall_no).toggle_blast();
}

void kkit::Project::toggle_wt_inside(int p_wall_no) {
	walls.at(p_wall_no).toggle_inside();
}

void kkit::Project::set_wall_image(int p_wall_no, const std::vector<std::vector<byte>>& p_bytes) {
	walls.at(p_wall_no).set_image(p_bytes);
}

// wall attribute getters
bool kkit::Project::is_blast(int p_wall_no) const {
	return walls.at(p_wall_no).is_blast();
}

bool kkit::Project::is_inside(int p_wall_no) const {
	return walls.at(p_wall_no).is_inside();
}

kkit::Wall_type kkit::Project::get_wall_type(int p_wall_no) const {
	return walls.at(p_wall_no).get_wall_type();
}

bool kkit::Project::is_directional(int p_wall_no) const {
	return this->get_wall_type(p_wall_no) == kkit::Wall_type::Direction;
}
