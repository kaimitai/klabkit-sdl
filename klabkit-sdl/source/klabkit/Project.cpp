#include "Project.h"
#include "constants.h"
#include "compression.h"
#include "xml_handler.h"
#include "./../klib/file.h"

using byte = unsigned char;

kkit::Project::Project(const kkit::Project_config& p_config) :
	config{ p_config }, kzp_walls{ false }, kzp_boards{ false }, m_saves(8, std::nullopt)
{
	initialize_palette();

	if (this->is_walken())
		initialize_walls_walken();
	else
		initialize_walls();

	if (this->is_walken())
		initialize_maps_walken();
	else
		initialize_maps();

	std::vector<kkit::Savegame_variable> l_variables = {
		{c::SAVE_CODE_HISCORENAME, 16},
		{"hiscorenamstat", 1},
		{c::SAVE_CODE_BOARDNUM}, {"scorecount", 4}, {"scoreclock", 4},
		{c::SAVE_CODE_BOARD, 8192},
		{"skilevel"}, {"life"}, {"death"}, {"lifevests"}, {"lightnings"}, {"firepowers_0"}, {"firepowers_1"}, {"firepowers_2"}, {"bulchoose"}, {"keys", 4}, {"coins"}, {"compass"}, {"cheated"}, {"animate2"}, {"animate3"}, {"animate4"}, {"oscillate3"}, {"oscillate5"}, {"animate6"}, {"animate7"}, {"animate8"}, {"animate10"}, {"animate11"}, {"animate15"}, {"statusbar"}, {"statusbargoal"}, {"posx"}, {"posy"}, {"posz"}, {"ang"}, {"startx"}, {"starty"}, {"startang"}, {"angvel"}, {"vel"}, {"mxvel"}, {"myvel"}, {"svel"}, {"hvel"}, {"oldposx"}, {"oldposy"},
		{"bulnum", 2},
		{"bultype_todo", 2, "bulnum"}, {"bulang", 2, "bulnum"}, {"bulx", 2, "bulnum"}, {"buly", 2, "bulnum"}, {"bulstat", 4, "bulnum"},
		{"lastbulshoot", 4},
		{"mnum"},
		{"mposx", 2, "mnum"}, {"mposy", 2}, {"mgolx", 2}, {"mgoly", 2}, {"moldx", 2}, {"moldy", 2, "mnum"}, {"mstat", 2, "mnum"}, {"mshock", 2, "mnum"}, {"mshot", 1, "mnum"},
		{"doorx", 2}, {"doory", 2}, {"doorstat", 2},
		{"numwarps", 1},
		{"justwarped", 1},
		{"xwarp", 1, "numwarps"}, {"ywarp", 1, "numwarps"},
		{"totalclock", 4}, {"purpletime", 4}, {"greentime", 4}, {"capetime_0", 4}, {"capetime_1", 4}, {"musicstatus", 4}, {"clockspeed"}, {"count", 4}, {"countstop", 4}, {"nownote"}, {"junk"},
		{"chanage", 4, "18"}, {"chanfreq", 1, "18"},
		{"midiinst"}, {"mute"}, {"namrememberstat", 1}, {"fadewarpval"}, {"fadehurtval"}, {"slottime"}, {"slotpos_0"}, {"slotpos_1"}, {"slotpos_2"}, {"owecoins"}, {"owecoinwait"}
	};

	std::vector<kkit::Savegame_variable> l_var_walken = { {"board",4096},
	{"mboard",1,"4096"},
	{"boardnum"}, {"life"}, {"death"}, {"lifevests"}, {"lightnings"}, {"firepowers"}, {"keys"}, {"flowerpos"}, {"kenpos"}, {"ballpos"}, {"rogermode"}, {"statusbar"}, {"statusbargoal"}, {"posx"}, {"posy"}, {"posz"}, {"ang"}, {"startx"}, {"starty"}, {"startang"}, {"angvel"}, {"vel"}, {"hvel"}, {"oldposx"}, {"oldposy"}, {"youbulnum"},
	{"bulnum"},
	{"bulang",2,"bulnum"},{"bulkind",2,"bulnum"},{"bulx",2,"bulnum"},{"buly",2,"bulnum"},{"bulstat",4,"bulnum"},
	{"lastbulshoot",4},
	{"mnum"},
	{"mposx",2,"mnum"},{"mposy",2,"mnum"},{"mgolx",2,"mnum"},{"mgoly",2,"mnum"},{"moldx",2,"mnum"},{"moldy",2,"mnum"},{"mstat",1,"mnum"},{"mshock",2,"mnum"},{"mshot",1,"mnum"},
	{"totalclock",4},{"musicstatus",4},{"clockspeed"},{"count",4},{"countstop",4},{"nownote",4},
	{"chanage",4,"18"},
	{"chanfreq",1,"18"} };

	std::vector<kkit::Savegame_variable> l_var_10 = {
	{"board",8192},
	{"mboard",1,"4096"}, {"boardnum"}, {"life"}, {"death"}, {"lifevests"}, {"lightnings"}, {"firepowers_0"}, {"firepowers_1"}, {"firepowers_2"},{"bulchoose"},{"keys_0",4},{"compass"},{"cheated"},{"heatpos"},{"fanpos"},{"warpos"},{"kenpos"}, {"ballpos"},{"rogermode"},{"statusbar"},{"statusbargoal"},{"posx"},{"posy"},{"posz"},{"ang"},{"startx"},{"starty"},{"startang"},{"angvel"},{"vel"},{"mxvel"},{"myvel"},{"svel"},{"hvel"},{"oldposx"},{"oldposy"},
	{"bulnum"},
	{"bulang_0",2,"bulnum"},{"bulkind_0",2,"bulnum"},{"bulx_0",2,"bulnum"},{"buly_0",2,"bulnum"},{"bulstat_0",4,"bulnum"},
	{"lastbulshoot",4},
	{"mnum"},
	{"mposx_0",2,"mnum"}, {"mposy_0",2,"mnum"}, {"mgolx_0",2,"mnum"}, {"mgoly_0",2,"mnum"}, {"moldx_0",2,"mnum"},{"moldy_0",2,"mnum"},{"mstat_0",1,"mnum"},{"mshock_0",2,"mnum"}, {"mshot_0",1,"mnum"},
	{"doorx"},{"doory"},{"doorstat"},
	{"numwarps",1},{"justwarped",1},
	{"xwarp_0",1,"numwarps"},{"ywarp_0",1,"numwarps"},
	{"totalclock",4},{"scoreclock",4},{"scorecount",4},{"purpletime",4},{"greentime",4},{"capetime_0",4},{"capetime_1",4},
	{c::SAVE_CODE_HISCORENAME,16},
	{"hiscorenamstat",1}, {"musicstatus",4}, {"clockspeed"}, {"count",4}, {"countstop",4}, {"nownote",4},
	{"chanage",4,"18"},
	{"chanfreq",1,"18"}
	};

	std::vector<kkit::Savegame_variable> l_var_11 = {
		{"board",8192},
		{"mboard",1,"4096"}, {"boardnum"}, {"life"}, {"death"}, {"lifevests"}, {"lightnings"}, {"firepowers_0"}, {"firepowers_1"}, {"firepowers_2"},{"bulchoose"},{"keys_0",4},{"compass"},{"cheated"},{"heatpos"},{"fanpos"},{"warpos"},{"kenpos"},{"unknown",4}, {"ballpos"},{"rogermode"},{"statusbar"},{"statusbargoal"},{"posx"},{"posy"},{"posz"},{"ang"},{"startx"},{"starty"},{"startang"},{"angvel"},{"vel"},{"mxvel"},{"myvel"},{"svel"},{"hvel"},{"oldposx"},{"oldposy"},
		{"bulnum"},
		{"bulang_0",2,"bulnum"},{"bulkind_0",2,"bulnum"},{"bulx_0",2,"bulnum"},{"buly_0",2,"bulnum"},{"bulstat_0",4,"bulnum"},
		{"lastbulshoot",4},
		{"mnum"},
		{"mposx_0",2,"mnum"}, {"mposy_0",2,"mnum"}, {"mgolx_0",2,"mnum"}, {"mgoly_0",2,"mnum"}, {"moldx_0",2,"mnum"},{"moldy_0",2,"mnum"},{"mstat_0",1,"mnum"},{"mshock_0",2,"mnum"}, {"mshot_0",1,"mnum"},
		{"doorx"},{"doory"},{"doorstat"},
		{"numwarps",1},{"justwarped",1},
		{"xwarp_0",1,"numwarps"},{"ywarp_0",1,"numwarps"},
		{"totalclock",4},{"scoreclock",4},{"scorecount",4},{"purpletime",4},{"greentime",4},{"capetime_0",4},{"capetime_1",4},
		{c::SAVE_CODE_HISCORENAME,16},
		{"hiscorenamstat",1}, {"musicstatus",4}, {"clockspeed"}, {"count",4}, {"countstop",4}, {"nownote",4},
		{"chanage",4,"18"},
		{"chanfreq",1,"18"}
	};

	if (config.lzw_comp_type == 0)
		kkit::Savegame::set_variables(l_var_walken);
	if (config.lzw_comp_type == 2)
		kkit::Savegame::set_variables(l_variables);
	else if (config.lzw_comp_type == 1)
		kkit::Savegame::set_variables(l_var_10);
}

void kkit::Project::add_message(const std::string& p_message, int p_status_code) {
	m_messages.push_front(std::make_pair(p_message, p_status_code));
	if (m_messages.size() > 25)
		m_messages.pop_back();
}

int kkit::Project::save_boards_kzp(bool p_compress) const {
	bool l_compress = p_compress && config.get_ext_boards() == Data_ext::KZP;

	if (this->is_walken())
		return this->save_boards_dat_walken();
	else {
		auto l_bytes = (l_compress ? kkit::compression::compress_boards_kzp(this->get_board_bytes(), config.board_count, this->is_klab_v_1()) : this->get_board_bytes());
		klib::file::write_bytes_to_file(l_bytes, get_file_path(c::FILE_BOARDS, (l_compress ? c::FILE_EXT_KZP : c::FILE_EXT_DAT)));
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
	bool l_compress = p_compress && config.get_ext_walls() == Data_ext::KZP;

	if (this->is_walken())
		return this->save_walls_kzp_walken(l_compress);
	else {
		auto l_bytes = (l_compress ? kkit::compression::compress_walls_kzp(this->get_wall_bytes(), config.wall_count, this->is_klab_v_1()) : this->get_wall_bytes());
		klib::file::write_bytes_to_file(l_bytes, get_file_path(c::FILE_WALLS, (l_compress ? c::FILE_EXT_KZP : c::FILE_EXT_DAT)));
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
		std::vector<byte> l_map_bytes(begin(map_bytes) + i * 4096,
			begin(map_bytes) + (i + 1) * 4096);
		maps.push_back(kkit::Board(l_map_bytes, walls));
	}
}

// walken-specific save boards to file
int kkit::Project::save_boards_dat_walken() const {
	std::vector<byte> l_bytes;

	// traverse all boards, and translate the board bytes based on clip indicators and player start position
	for (int i{ 0 }; i < this->get_board_count(); ++i) {
		std::vector<byte> l_brd_bytes{ get_board(i).get_bytes(walls) };
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

std::string kkit::Project::get_savegame_filename(std::size_t p_slot_no) {
	return c::FILE_SAVEGAME + std::to_string(p_slot_no);
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

bool kkit::Project::is_clip_override(int p_tile_no) const {
	return this->config.is_clip_override(p_tile_no);
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

// savegames
bool kkit::Project::has_savegame(std::size_t p_slot) const {
	return m_saves.at(p_slot).has_value();
}

const kkit::Savegame& kkit::Project::get_savegame(std::size_t p_slot) const {
	return m_saves.at(p_slot).value();
}

void kkit::Project::load_saveboard(std::size_t p_board_slot, std::size_t p_save_slot) {
	maps.at(p_board_slot) = m_saves.at(p_save_slot).value().get_board();
	add_message("Moved board from savegame #" + std::to_string(p_save_slot) + " into editor board #" + std::to_string(p_board_slot + 1));
}

void kkit::Project::export_board_to_save(std::size_t p_board_slot, std::size_t p_save_slot) {
	m_saves.at(p_save_slot).value().set_board(maps.at(p_board_slot));
	add_message("Moved editor board #" + std::to_string(p_board_slot + 1) + " into savegame #" + std::to_string(p_save_slot));
}

void kkit::Project::load_savefile_dat(std::size_t p_save_slot) {
	const auto l_filepath{ get_dat_file_name(c::FILE_SAVEGAME + std::to_string(p_save_slot)) };
	auto l_bytes{ klib::file::read_file_as_bytes(l_filepath) };

	m_saves.at(p_save_slot) = is_walken() ? Savegame(l_bytes, walls) : Savegame(l_bytes);

	add_message("Loaded " + l_filepath, c::MSG_CODE_SUCCESS);
}

void kkit::Project::save_savefile_dat(std::size_t p_save_slot) {
	const auto l_filepath{ get_dat_file_name(c::FILE_SAVEGAME + std::to_string(p_save_slot)) };

	if (is_walken())
		klib::file::write_bytes_to_file(m_saves.at(p_save_slot).value().get_bytes(walls),
			l_filepath);
	else
		klib::file::write_bytes_to_file(m_saves.at(p_save_slot).value().get_bytes(),
			l_filepath);

	add_message("Saved " + l_filepath, c::MSG_CODE_SUCCESS);
}

void kkit::Project::load_savefile_xml(std::size_t p_save_slot) {
	std::string l_in_xml_file{ c::FILE_SAVEGAME + std::to_string(p_save_slot) + "." + c::FILE_EXT_XML };
	std::string l_full_xml_path{ get_file_directory(c::FILE_EXT_XML, 0) + "/" + l_in_xml_file };

	m_saves.at(p_save_slot) = xml::load_savefile_xml(l_full_xml_path);

	add_message("Loaded " + l_full_xml_path, c::MSG_CODE_SUCCESS);
}

void kkit::Project::save_savefile_xml(std::size_t p_save_slot) {
	std::string l_out_xml_file{ c::FILE_SAVEGAME + std::to_string(p_save_slot) + "." + c::FILE_EXT_XML };

	xml::save_savefile_xml(m_saves.at(p_save_slot).value(),
		this->get_file_directory(c::FILE_EXT_XML, static_cast<int>(p_save_slot)),
		l_out_xml_file,
		config.label);

	add_message(l_out_xml_file + " saved", c::MSG_CODE_SUCCESS);
}

// hiscore
bool kkit::Project::has_hiscore(void) const {
	return m_hiscore.has_value();
}

const kkit::Hiscore& kkit::Project::get_hiscore(void) const {
	return m_hiscore.value();
}

void kkit::Project::load_hiscore_dat(void) {
	std::string l_file_path{ get_dat_file_name(c::FILE_HISCORE) };

	m_hiscore = kkit::Hiscore(klib::file::read_file_as_bytes(
		l_file_path
	));

	add_message("Loaded " + l_file_path, c::MSG_CODE_SUCCESS);
}

void kkit::Project::save_hiscore_dat(void) {
	std::string l_file_path{ get_dat_file_name(c::FILE_HISCORE) };

	klib::file::write_bytes_to_file(m_hiscore.value().get_bytes(),
		l_file_path);

	add_message("Saved " + l_file_path, c::MSG_CODE_SUCCESS);
}

void kkit::Project::load_hiscore_xml(void) {
	std::string l_file_path{ get_file_directory(c::FILE_EXT_XML, 0) +
		'/' + c::FILE_HISCORE + std::string(".xml") };

	m_hiscore = xml::load_hiscore_xml(l_file_path);

	add_message("Loaded " + l_file_path, c::MSG_CODE_SUCCESS);
}

void kkit::Project::save_hiscore_xml(void) {
	std::string l_filename{ c::FILE_HISCORE + std::string(".xml") };

	xml::save_hiscore_xml(m_hiscore.value(), get_file_directory(c::FILE_EXT_XML, 0),
		l_filename, config.label);

	add_message("Saved " + l_filename, c::MSG_CODE_SUCCESS);
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

const kkit::Project_config& kkit::Project::get_config(void) const {
	return config;
}

const std::deque<std::pair<std::string, int>>& kkit::Project::get_messages(void) const {
	return m_messages;
}

const std::string& kkit::Project::get_savegame_player_name(std::size_t p_slot) const {
	return m_saves.at(p_slot).value().get_hiscore_name();
}

unsigned int kkit::Project::get_savegame_board_num(std::size_t p_slot) const {
	return m_saves.at(p_slot).value().get_board_num();
}
