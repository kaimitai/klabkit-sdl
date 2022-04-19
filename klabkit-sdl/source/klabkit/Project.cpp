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
		map_bytes = kkit::compression::decompress_boards_kzp(klib::file::read_file_as_bytes(get_file_path(c::FILE_BOARDS, c::FILE_EXT_KZP)));
	}

	int l_num_maps = static_cast<int>(map_bytes.size()) / c::MAP_BYTES;

	for (int i{ 0 }; i < l_num_maps; ++i)
		maps.push_back(kkit::Board(std::vector<unsigned char>(begin(map_bytes) + i * c::MAP_BYTES, begin(map_bytes) + (i + 1) * c::MAP_BYTES)));
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

void kkit::Project::clear_tile(int p_board_no, int p_x, int p_y) {
	maps[p_board_no].clear_tile(p_x, p_y);
}

void kkit::Project::set_tile(int p_board_no, int p_x, int p_y, const kkit::Map_tile& p_tile) {
	maps[p_board_no].set_tile(p_x, p_y, p_tile);
}
