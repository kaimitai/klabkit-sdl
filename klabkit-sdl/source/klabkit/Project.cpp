#include "Project.h"
#include "constants.h"
#include "./../klib/file.h"

using byte = unsigned char;

kkit::Project::Project(const std::string& p_folder) : project_folder{ p_folder } {
	initialize_palette();
	initialize_walls();
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
	auto wall_bytes = klib::file::read_file_as_bytes(get_dat_file_name("WALLS"));
	int l_num_walls = (static_cast<int>(wall_bytes.size()) - c::WALL_DATA_OFFSET) / (c::WALL_IMG_BYTES);

	for (int i{ 0 }; i < l_num_walls; ++i) {
		walls.push_back(kkit::Wall(std::vector<unsigned char>(begin(wall_bytes) + c::WALL_DATA_OFFSET + i * c::WALL_IMG_BYTES, begin(wall_bytes) + c::WALL_DATA_OFFSET + (i + 1) * c::WALL_IMG_BYTES), wall_bytes[i]));
	}
}

// utility functions
std::string kkit::Project::get_dat_file_name(const std::string& p_filename) const {
	return project_folder + "/" + p_filename + ".DAT";
}

std::string kkit::Project::get_bmp_folder(void) const {
	return project_folder + "/bmp";
}

std::string kkit::Project::get_wall_bmp_file_path(int p_frame_no) const {
	return this->get_bmp_folder() + "/WALL" + (p_frame_no > 0 ? std::to_string(p_frame_no + 1) : "") + ".bmp";
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
