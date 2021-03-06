#include "Project.h"
#include "constants.h"
#include "compression.h"
#include "xml_handler.h"
#include "./../klib/file.h"

using byte = unsigned char;

kkit::Project::Project(const kkit::Project_config& p_config) : config{ p_config }, kzp_walls{ false }, kzp_boards{ false } {
	initialize_palette();

	if (this->is_walken())
		initialize_walls_walken();
	else
		initialize_walls();

	if (this->is_walken())
		initialize_maps_walken();
	else
		initialize_maps();
}

int kkit::Project::save_boards_kzp(bool p_compress) const {
	if (this->is_walken())
		return this->save_boards_dat_walken();
	else {
		auto l_bytes = (p_compress ? kkit::compression::compress_boards_kzp(this->get_board_bytes(), config.board_count, this->is_klab_v_1()) : this->get_board_bytes());
		klib::file::write_bytes_to_file(l_bytes, get_file_path(c::FILE_BOARDS, (p_compress ? c::FILE_EXT_KZP : c::FILE_EXT_DAT)));
		return static_cast<int>(l_bytes.size());
	}
}

// walken-specific wall save procedure
int kkit::Project::save_walls_kzp_walken(bool p_compress) const {
	std::vector<byte> l_bytes;

	// add all tile graphics
	for (int i{ 0 }; i < static_cast<int>(walls.size()); ++i) {
		auto l_img_bytes = walls[i].get_image_bytes();
		l_bytes.insert(end(l_bytes), begin(l_img_bytes), end(l_img_bytes));
	}

	// do the same for tile metadata (which comes at the end of the file for walken)
	for (int i{ 0 }; i < static_cast<int>(walls.size()); ++i) {
		byte l_header_byte{ 0 };
		if (!walls[i].is_inside())
			l_header_byte |= 0b1;
		if (walls[i].get_wall_type() != kkit::Wall_type::Cube)
			l_header_byte |= 0b10;

		l_bytes.push_back(l_header_byte);
	}

	if (p_compress)
		l_bytes = kkit::compression::compress_walls_kzp_walken(l_bytes, config.wall_count);

	klib::file::write_bytes_to_file(l_bytes, get_file_path(c::FILE_WALLS, (p_compress ? c::FILE_EXT_KZP : c::FILE_EXT_DAT)));

	return static_cast<int>(l_bytes.size());
}

int kkit::Project::save_walls_kzp(bool p_compress) const {
	if (this->is_walken())
		return this->save_walls_kzp_walken(p_compress);
	else {
		auto l_bytes = (p_compress ? kkit::compression::compress_walls_kzp(this->get_wall_bytes(), config.wall_count, this->is_klab_v_1()) : this->get_wall_bytes());
		klib::file::write_bytes_to_file(l_bytes, get_file_path(c::FILE_WALLS, (p_compress ? c::FILE_EXT_KZP : c::FILE_EXT_DAT)));
		return static_cast<int>(l_bytes.size());
	}
}

void kkit::Project::save_wall_xml(int p_wall_no) const {
	xml::save_wall_xml(this->walls.at(p_wall_no), this->get_file_directory(c::FILE_EXT_XML, p_wall_no), this->get_file_name(c::FILE_WALLS, c::FILE_EXT_XML, p_wall_no), config.label);
}

void kkit::Project::save_board_xml(int p_board_no) const {
	xml::save_board_xml(this->maps.at(p_board_no), this->get_file_directory(c::FILE_EXT_XML, p_board_no), this->get_file_name(c::FILE_BOARDS, c::FILE_EXT_XML, p_board_no), config.label);
}

// initializers
void kkit::Project::initialize_palette(void) {
	if (this->is_walken()) {
		byte c1{ 0 }, c2{ 0 }, c3{ 0 };

		for (int i{ 0 }; i <= 251; ++i) {
			byte l_r = (c1 * 64) / 6;
			byte l_g = (c2 * 64) / 7;
			byte l_b = (c3 * 64) / 6;
			palette.push_back(std::make_tuple(4 * l_r, 4 * l_g, 4 * l_b));

			++c1;
			if (c1 == 6) {
				c1 = 0;
				++c2;
			}
			if (c2 == 7) {
				c2 = 0;
				++c3;
			}
		}

		for (int i{ 0 }; i < 4; ++i)
			palette.push_back(std::make_tuple(255, 255, 255));

	}
	else {
		bool l_lab3d_v1{ this->is_klab_v_1() };

		for (int i{ 0 }; i < 16; ++i) {
			for (int j{ 0 }; j < 16; ++j) {
				byte l_r = (l_lab3d_v1 ? c::PAL_GEN_V1[i][0] : c::PAL_GEN[i][0]) * (j + 1) / 17;
				byte l_g = (l_lab3d_v1 ? c::PAL_GEN_V1[i][1] : c::PAL_GEN[i][1]) * (j + 1) / 17;
				byte l_b = (l_lab3d_v1 ? c::PAL_GEN_V1[i][2] : c::PAL_GEN[i][2]) * (j + 1) / 17;
				palette.push_back(std::make_tuple(4 * l_r, 4 * l_g, 4 * l_b));
			}
		}

	}
	palette[255] = c::TRANS_COL_RGB;
}

void kkit::Project::initialize_walls(void) {
	std::vector<byte> wall_bytes;

	try {
		wall_bytes = klib::file::read_file_as_bytes(get_file_path(c::FILE_WALLS, c::FILE_EXT_DAT));
	}
	catch (std::exception&) {
		wall_bytes = kkit::compression::decompress_walls_kzp(klib::file::read_file_as_bytes(get_file_path(c::FILE_WALLS, c::FILE_EXT_KZP)), config.wall_count, this->is_klab_v_1());
		kzp_walls = true;
	}

	int l_num_walls = (static_cast<int>(wall_bytes.size()) - c::WALL_DATA_OFFSET) / (c::WALL_IMG_BYTES);

	for (int i{ 0 }; i < l_num_walls; ++i)
		walls.push_back(kkit::Wall(std::vector<unsigned char>(begin(wall_bytes) + c::WALL_DATA_OFFSET + i * c::WALL_IMG_BYTES, begin(wall_bytes) + c::WALL_DATA_OFFSET + (i + 1) * c::WALL_IMG_BYTES), wall_bytes[i]));
}

void kkit::Project::initialize_walls_walken(void) {
	std::vector<byte> wall_bytes;

	try {
		wall_bytes = klib::file::read_file_as_bytes(get_file_path(c::FILE_WALLS, c::FILE_EXT_DAT));
	}
	catch (std::exception&) {
		wall_bytes = kkit::compression::decompress_walls_kzp_walken(klib::file::read_file_as_bytes(get_file_path(c::FILE_WALLS, c::FILE_EXT_KZP)), config.wall_count);
		kzp_walls = true;
	}

	for (int i{ 0 }; i < config.wall_count; ++i) {
		byte l_wall_md = wall_bytes.at(c::WALL_IMG_BYTES * config.wall_count + i);
		kkit::Wall l_wall = kkit::Wall(std::vector<unsigned char>(begin(wall_bytes) + i * c::WALL_IMG_BYTES, begin(wall_bytes) + (i + 1) * c::WALL_IMG_BYTES),
			0);

		if ((l_wall_md & 1) == 0)
			l_wall.toggle_inside();

		if ((l_wall_md >> 1) & 1) {
			l_wall.toggle_type();
			l_wall.toggle_type();
		}

		walls.push_back(l_wall);
	}

}

void kkit::Project::initialize_maps(void) {
	std::vector<byte> map_bytes;

	try {
		map_bytes = klib::file::read_file_as_bytes(get_file_path(c::FILE_BOARDS, c::FILE_EXT_DAT));
	}
	catch (const std::exception&) {
		map_bytes = kkit::compression::decompress_boards_kzp(klib::file::read_file_as_bytes(get_file_path(c::FILE_BOARDS, c::FILE_EXT_KZP)), config.board_count, this->is_klab_v_1());
		kzp_boards = true;
	}

	int l_num_maps = static_cast<int>(map_bytes.size()) / c::MAP_BYTES;

	for (int i{ 0 }; i < l_num_maps; ++i)
		maps.push_back(kkit::Board(std::vector<unsigned char>(begin(map_bytes) + i * c::MAP_BYTES, begin(map_bytes) + (i + 1) * c::MAP_BYTES)));
}

void kkit::Project::initialize_maps_walken(void) {
	std::vector<byte> map_bytes = klib::file::read_file_as_bytes(get_file_path(c::FILE_BOARDS, c::FILE_EXT_DAT));

	// calculate the entire board here
	for (int i{ 0 }; i < config.board_count; ++i) {
		std::vector<std::vector<kkit::Map_tile>> tiles(64, std::vector<kkit::Map_tile>(64, kkit::Map_tile()));
		int l_px{ 0 };
		int l_py{ 0 };
		kkit::Player_direction l_pdir = kkit::Player_direction::Up;

		for (int j{ 0 }; j < 4096; ++j) {
			int l_x = j / 64;
			int l_y = j % 64;
			int l_tile_no = map_bytes.at(i * 4096 + j);

			bool l_inside{ false };

			if (l_tile_no >= 252) {
				l_px = l_x;
				l_py = l_y;
				if (l_tile_no == 252)
					l_pdir = kkit::Player_direction::Right;
				else if (l_tile_no == 253)
					l_pdir = kkit::Player_direction::Down;
				else if (l_tile_no == 254)
					l_pdir = kkit::Player_direction::Left;
				else
					l_pdir = kkit::Player_direction::Up;

				l_tile_no = 0;
				l_inside = true;
			}
			else if (l_tile_no >= 128) {
				l_tile_no -= 128;
				l_inside = (l_tile_no != 0);

			}
			else if (l_tile_no == 0)
				l_inside = true;

			if (l_tile_no > 0)
				l_inside |= this->is_inside(l_tile_no - 1);

			tiles[l_x][l_y] = kkit::Map_tile(l_tile_no - 1, l_inside, false, false);
		}

		maps.push_back(kkit::Board(tiles, l_px, l_py, l_pdir));
	}

}

// walken-specific save boards to file
int kkit::Project::save_boards_dat_walken() const {
	std::vector<byte> l_bytes;

	// traverse all boards, and translate the board bytes based on clip indicators and player start position
	for (int i{ 0 }; i < this->get_board_count(); ++i) {
		std::vector<byte> l_brd_bytes;

		const auto& l_brd = this->get_board(i);
		int l_p_index = 64 * l_brd.get_player_start_x() + l_brd.get_player_start_y();

		byte l_pbyte{ 252 };
		if (l_brd.get_player_start_direction() == kkit::Player_direction::Down)
			l_pbyte = 253;
		else if (l_brd.get_player_start_direction() == kkit::Player_direction::Left)
			l_pbyte = 254;
		else if (l_brd.get_player_start_direction() == kkit::Player_direction::Up)
			l_pbyte = 255;

		kkit::Player_direction l_sdir = l_brd.get_player_start_direction();

		for (int x = 0; x < 64; ++x)
			for (int y = 0; y < 64; ++y) {
				int l_tile_no = l_brd.get_tile_no(x, y);

				bool l_clip_override = (l_tile_no != -1) && (l_brd.is_inside(x, y) && !is_inside(l_tile_no));
				l_clip_override |= (l_tile_no == -1 && !l_brd.is_inside(x, y));

				if (l_clip_override)
					l_tile_no += 128;

				l_brd_bytes.push_back(static_cast<byte>(l_tile_no + 1));
			}

		// overwrite the start direction tile

		l_brd_bytes.at(l_p_index) = l_pbyte;
		l_bytes.insert(end(l_bytes), begin(l_brd_bytes), end(l_brd_bytes));
	}

	klib::file::write_bytes_to_file(l_bytes, get_file_path(c::FILE_BOARDS, c::FILE_EXT_DAT));
	return static_cast<int>(l_bytes.size());
}

// utility functions
std::string  kkit::Project::get_file_path(const std::string& p_subfolder, const std::string& p_file_prefix, const std::string& p_file_ext, int p_frame_no) const {
	std::string result{ config.project_folder };
	if (!p_subfolder.empty())
		result += "/" + p_subfolder;

	result += "/" + p_file_prefix + (p_frame_no == 0 ? "" : std::to_string(p_frame_no + 1)) + "." + p_file_ext;

	return result;
}

std::string  kkit::Project::get_file_path(const std::string& p_file_prefix, const std::string& p_file_ext, int p_frame_no) const {
	return get_file_path(std::string(), p_file_prefix, p_file_ext, p_frame_no);
}

std::string kkit::Project::get_dat_file_name(const std::string& p_filename) const {
	return config.project_folder + "/" + p_filename + ".DAT";
}

std::string kkit::Project::get_file_directory(const std::string& p_extension, int p_frame_no) const {
	if (p_frame_no == -1)
		return this->config.project_folder;
	else
		return this->config.project_folder + "/" + p_extension;
}

std::string kkit::Project::get_file_name(const std::string& p_filename, const std::string& p_extension, int p_frame_no) const {
	std::string l_frame = "";

	if (p_frame_no != -1) {
		l_frame = std::to_string(p_frame_no + 1);
		while (l_frame.size() < 3)
			l_frame.insert(begin(l_frame), '0');
		l_frame.insert(begin(l_frame), '-');
	}

	return p_filename + l_frame + "." + p_extension;
}

std::string kkit::Project::get_file_full_path(const std::string& p_filename, const std::string& p_extension, int p_frame_no) const {
	return this->get_file_directory(p_extension, p_frame_no) + "/" + this->get_file_name(p_filename, p_extension, p_frame_no);
}

std::string  kkit::Project::get_bmp_file_path(const std::string& p_file_prefix, int p_frame_no) const {
	return get_file_full_path(c::FILE_WALLS, c::FILE_EXT_BMP, p_frame_no);
}

int kkit::Project::get_board_count(void) const {
	return static_cast<int>(this->maps.size());
}

std::string kkit::Project::get_config_label(void) const {
	return config.label;
}

std::string kkit::Project::get_bmp_folder(void) const {
	return config.project_folder + "/" + c::FILE_EXT_BMP;
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

const std::vector<int>& kkit::Project::get_tile_picker(void) const {
	return this->config.tile_picker;
}

bool kkit::Project::is_clip_override(int p_tile_no) const {
	return this->config.is_clip_override(p_tile_no);
}

int kkit::Project::get_tile_picker_index(int p_tile_no) const {
	for (std::size_t i{ 0 }; i < config.tile_picker.size(); ++i)
		if (config.tile_picker[i] == p_tile_no)
			return static_cast<int>(i);
	return -1;
}

bool kkit::Project::is_klab_v_1(void) const {
	return (config.lzw_comp_type == 1);
}

bool kkit::Project::is_walken(void) const {
	return (config.lzw_comp_type == 0);
}

bool kkit::Project::is_walls_kzp(void) const {
	return this->kzp_walls;
}

bool kkit::Project::is_boards_kzp(void) const {
	return this->kzp_boards;
}

void kkit::Project::clear_tile(int p_board_no, int p_x, int p_y) {
	maps[p_board_no].clear_tile(p_x, p_y);
}

void kkit::Project::set_tile(int p_board_no, int p_x, int p_y, const kkit::Map_tile& p_tile) {
	//if (p_x != maps.at(p_board_no).get_player_start_x() || p_y != maps.at(p_board_no).get_player_start_y())
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
	maps[p_board_no].toggle_blast(p_x, p_y);
}

void kkit::Project::toggle_mt_inside(int p_board_no, int p_x, int p_y) {
	maps[p_board_no].toggle_inside(p_x, p_y);
}

void kkit::Project::set_player_start_position(int p_board_no, int p_x, int p_y, kkit::Player_direction p_direction) {
	maps.at(p_board_no).set_player_start_position(p_x, p_y, p_direction);
}

void kkit::Project::flip_vertical(int p_board_no, int p_x, int p_y, int p_w, int p_h) {
	maps.at(p_board_no).flip_vertical(p_x, p_y, p_w, p_h);
}

void kkit::Project::flip_horizontal(int p_board_no, int p_x, int p_y, int p_w, int p_h) {
	maps.at(p_board_no).flip_horizontal(p_x, p_y, p_w, p_h);
}

void kkit::Project::reload_map_from_xml(int p_board_no) {
	this->maps.at(p_board_no) = xml::load_board_xml(this->get_file_full_path(c::FILE_BOARDS, c::FILE_EXT_XML, p_board_no));
}

void kkit::Project::reload_wall_from_xml(int p_wall_no) {
	this->walls.at(p_wall_no) = xml::load_wall_xml(this->get_file_full_path(c::FILE_WALLS, c::FILE_EXT_XML, p_wall_no));
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
	return (p_wall_no == -1 ? false : walls.at(p_wall_no).is_blast());
}

bool kkit::Project::is_inside(int p_wall_no) const {
	return (p_wall_no == -1 ? true : walls.at(p_wall_no).is_inside());
}

kkit::Wall_type kkit::Project::get_wall_type(int p_wall_no) const {
	return (p_wall_no == -1 ? kkit::Wall_type::Cube : walls.at(p_wall_no).get_wall_type());
}

bool kkit::Project::is_directional(int p_wall_no) const {
	return (p_wall_no == -1 ? false : get_wall_type(p_wall_no) == kkit::Wall_type::Direction);
}

const std::vector<std::vector<byte>> kkit::Project::get_image_as_2dv(int p_wall_no) const {
	if (p_wall_no == -1)
		return std::vector<std::vector<byte>>(64, std::vector<byte>(64, 255));
	return walls.at(p_wall_no).get_image();
}

std::tuple<byte, byte, byte> kkit::Project::get_floor_color(void) const {
	return this->config.floor_color;
}
