#include "Project.h"
#include "constants.h"
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
	auto wall_bytes = klib::file::read_file_as_bytes(get_file_path(c::FILE_WALLS, c::FILE_EXT_DAT));
	int l_num_walls = (static_cast<int>(wall_bytes.size()) - c::WALL_DATA_OFFSET) / (c::WALL_IMG_BYTES);

	for (int i{ 0 }; i < l_num_walls; ++i) {
		walls.push_back(kkit::Wall(std::vector<unsigned char>(begin(wall_bytes) + c::WALL_DATA_OFFSET + i * c::WALL_IMG_BYTES, begin(wall_bytes) + c::WALL_DATA_OFFSET + (i + 1) * c::WALL_IMG_BYTES), wall_bytes[i]));
	}
}

void kkit::Project::initialize_maps(void) {
	auto map_bytes = klib::file::read_file_as_bytes(get_file_path(c::FILE_BOARDS, c::FILE_EXT_DAT));
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
