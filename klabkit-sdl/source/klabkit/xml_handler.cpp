#include "constants.h"
#include "xml_handler.h"
#include "Map_tile.h"
#include "../klib/klib_util.h"
#include <filesystem>
#include <set>
#include <tuple>
#include <vector>

using byte = unsigned char;

kkit::Board kkit::xml::load_board_xml(const std::string& p_file_name) {
	pugi::xml_document doc;
	if (!doc.load_file(p_file_name.c_str()))
		throw std::runtime_error("Could not find board xml");

	pugi::xml_node n_meta = doc.child(XML_TAG_META);
	auto n_board = n_meta.child(XML_TAG_BOARD);

	int l_player_start_x = n_board.attribute(XML_ATTR_PLAYER_X).as_int();
	int l_player_start_y = n_board.attribute(XML_ATTR_PLAYER_Y).as_int();
	kkit::Player_direction l_player_dir = kkit::Board::get_player_direction_from_string(n_board.attribute(XML_ATTR_PLAYER_DIR).as_string());

	std::vector<std::vector<kkit::Map_tile>> l_board(c::MAP_W, std::vector<kkit::Map_tile>(c::MAP_H, kkit::Map_tile()));
	std::size_t x{ 0 }, y{ 0 };

	for (auto n_br_node = n_board.child(XML_TAG_ROW); n_br_node; n_br_node = n_br_node.next_sibling(XML_TAG_ROW)) {

		for (auto n_bc_node = n_br_node.child(XML_TAG_TILE); n_bc_node; n_bc_node = n_bc_node.next_sibling(XML_TAG_TILE)) {
			int l_tile_no = n_bc_node.attribute(XML_ATTR_WALL_NO).as_int() - 1;
			bool l_blast = (strcmp(n_bc_node.attribute(XML_ATTR_DESTRUCTIBLE).as_string(), XML_VALUE_TRUE) == 0);
			bool l_inside = (strcmp(n_bc_node.attribute(XML_ATTR_CLIP).as_string(), XML_VALUE_TRUE) == 0);
			bool l_vertical = (strcmp(n_bc_node.attribute(XML_ATTR_VERTICAL).as_string(), XML_VALUE_TRUE) == 0);

			l_board[y][x] = kkit::Map_tile(l_tile_no, l_inside, l_blast, l_vertical);
			++y;
		}

		++x;
		y = 0;
	}

	return kkit::Board(l_board, l_player_start_x, l_player_start_y, l_player_dir);
}

kkit::Wall kkit::xml::load_wall_xml(const std::string& p_file_name) {
	pugi::xml_document doc;
	if (!doc.load_file(p_file_name.c_str()))
		throw std::runtime_error("Could not find wall xml");

	pugi::xml_node n_meta = doc.child(XML_TAG_META);
	auto n_wall = n_meta.child(XML_TAG_WALL);

	std::vector<std::vector<byte>> l_image(c::WALL_IMG_W, std::vector<byte>(c::WALL_IMG_W, 0));
	std::size_t x{ 0 };

	bool l_blast = (strcmp(XML_VALUE_TRUE, n_wall.attribute(XML_ATTR_DESTRUCTIBLE).as_string()) == 0);
	bool l_inside = (strcmp(XML_VALUE_TRUE, n_wall.attribute(XML_ATTR_CLIP).as_string()) == 0);
	kkit::Wall_type l_wall_type{ kkit::Wall_type::Cube };
	std::string ls_wall_type = n_wall.attribute(XML_ATTR_WALL_TYPE).as_string();
	if (ls_wall_type == XML_VALUE_DIRECTIONAL)
		l_wall_type = kkit::Wall_type::Direction;
	else if (ls_wall_type == XML_VALUE_PLANE)
		l_wall_type = kkit::Wall_type::Plane;

	for (auto n_tpr_node = n_wall.child(XML_TAG_PIXEL_ROW); n_tpr_node; n_tpr_node = n_tpr_node.next_sibling(XML_TAG_PIXEL_ROW)) {
		std::vector<byte> l_tp_row = klib::util::string_split<byte>(n_tpr_node.attribute(XML_ATTR_VALUE).as_string(), ',');

		for (std::size_t y{ 0 }; y < l_tp_row.size(); ++y)
			l_image.at(y).at(x) = l_tp_row[y];

		++x;
	}

	return kkit::Wall(l_image, l_wall_type, l_blast, l_inside);
}

kkit::Project_config kkit::xml::read_config_xml(const std::string& p_file_name) {

	try {
		pugi::xml_document doc;
		if (!doc.load_file(p_file_name.c_str()))
			throw std::runtime_error("Could not find configuration file");

		pugi::xml_node n_meta = doc.child(XML_TAG_META);
		int l_config_no = n_meta.attribute(XML_ATTR_ACTIVE_CONFIG_NO).as_int();

		for (auto n_conf = n_meta.child(XML_TAG_CONFIG); n_conf; n_conf = n_conf.next_sibling(XML_TAG_CONFIG)) {
			if (l_config_no != n_conf.attribute(XML_ATTR_NO).as_int())
				continue;

			std::string l_label = n_conf.attribute(XML_ATTR_LABEL).as_string();
			std::string l_proj_dir = n_conf.attribute(XML_ATTR_PROJ_DIR).as_string();
			int l_bcount = n_conf.attribute(XML_ATTR_BCOUNT).as_int();
			int l_wcount = n_conf.attribute(XML_ATTR_WCOUNT).as_int();
			int l_lzw_type = n_conf.attribute(XML_ATTR_LZW_TYPE).as_int();
			std::string l_ext_boards{ n_conf.attribute(XML_ATTR_EXT_BOARDS).as_string() };
			std::string l_ext_walls{ n_conf.attribute(XML_ATTR_EXT_WALLS).as_string() };

			std::vector l_clip_overrides_v = klib::util::string_split<int>(n_conf.child(XML_TAG_CLIP_OVERRIDES).attribute(XML_ATTR_VALUE).as_string(), ',');
			std::vector<int> l_tile_picker;

			std::vector l_floor_rgb = klib::util::string_split<byte>(n_conf.attribute(XML_ATTR_FLOOR_RGB).as_string(), ',');


			std::tuple<byte, byte, byte> l_floor_rgb_t = (l_floor_rgb.empty() ?
				kkit::c::FLOOR_COL_RGB :
				std::make_tuple(l_floor_rgb.at(0), l_floor_rgb.at(1), l_floor_rgb.at(2)));

			if (auto n_tp_node = n_conf.child(XML_TAG_TILE_PICKER)) {
				for (auto n_tpr_node = n_tp_node.child(XML_TAG_ROW); n_tpr_node; n_tpr_node = n_tpr_node.next_sibling(XML_TAG_ROW)) {
					std::vector<int> l_tp_row = klib::util::string_split<int>(n_tpr_node.attribute(XML_ATTR_VALUE).as_string(), ',');
					for (int n : l_tp_row)
						l_tile_picker.push_back(n - 1);
					// add padding to the tile picker after each row
					while (l_tile_picker.size() % 14 != 0)
						l_tile_picker.push_back(-3);
				}
			}

			std::set<int> l_clip_overrides;
			for (int n : l_clip_overrides_v)
				l_clip_overrides.insert(n - 1);


			return kkit::Project_config(l_label, l_proj_dir, l_bcount, l_wcount, l_lzw_type,
				l_ext_boards, l_ext_walls,
				l_clip_overrides,
				l_tile_picker,
				l_floor_rgb_t);

		}

		throw std::runtime_error("Invalid value for default_config_no in the configuration xml");
	}
	catch (const std::exception) {
		return kkit::Project_config();
	}
}

pugi::xml_node kkit::xml::create_header(pugi::xml_document& p_doc) {
	auto n_comments = p_doc.append_child(pugi::node_comment);
	n_comments.set_value(XML_COMMENTS);

	auto n_metadata = p_doc.append_child(XML_TAG_META);

	n_metadata.append_attribute(XML_ATTR_APP_V);
	n_metadata.attribute(XML_ATTR_APP_V).set_value(c::APP_VERSION);

	n_metadata.append_attribute(XML_ATTR_FTYPE);
	n_metadata.append_attribute(XML_ATTR_LAB3D_V);

	return n_metadata;
}

void kkit::xml::save_wall_xml(const kkit::Wall& p_wall, const std::string& p_directory, const std::string& p_filename, const std::string& p_klab_version) {
	std::filesystem::create_directories(p_directory);
	std::string l_full_file_path = p_directory + "/" + p_filename;

	pugi::xml_document doc;
	auto n_root = create_header(doc);
	n_root.attribute(XML_ATTR_FTYPE).set_value(XML_VALUE_FTYPE_WALL);
	n_root.attribute(XML_ATTR_LAB3D_V).set_value(p_klab_version.c_str());

	auto n_wall = n_root.append_child(XML_TAG_WALL);
	n_wall.append_attribute(XML_ATTR_WALL_TYPE);
	n_wall.append_attribute(XML_ATTR_DESTRUCTIBLE);
	n_wall.append_attribute(XML_ATTR_CLIP);

	auto l_wall_type = p_wall.get_wall_type();
	if (l_wall_type == kkit::Wall_type::Cube)
		n_wall.attribute(XML_ATTR_WALL_TYPE).set_value(XML_VALUE_CUBE);
	else if (l_wall_type == kkit::Wall_type::Plane)
		n_wall.attribute(XML_ATTR_WALL_TYPE).set_value(XML_VALUE_PLANE);
	else
		n_wall.attribute(XML_ATTR_WALL_TYPE).set_value(XML_VALUE_DIRECTIONAL);

	n_wall.attribute(XML_ATTR_DESTRUCTIBLE).set_value(p_wall.is_blast() ? XML_VALUE_TRUE : XML_VALUE_FALSE);
	n_wall.attribute(XML_ATTR_CLIP).set_value(p_wall.is_inside() ? XML_VALUE_TRUE : XML_VALUE_FALSE);

	for (int y{ 0 }; y < c::WALL_IMG_H; ++y) {
		auto n_row = n_wall.append_child(XML_TAG_PIXEL_ROW);
		n_row.append_attribute(XML_ATTR_NO);
		n_row.attribute(XML_ATTR_NO).set_value(y);

		std::vector<byte> l_row;
		for (int x{ 0 }; x < c::WALL_IMG_W; ++x)
			l_row.push_back(p_wall.get_palette_index(x, y));

		std::string l_pixels = klib::util::string_join(l_row, ',');
		n_row.append_attribute(XML_ATTR_VALUE);
		n_row.attribute(XML_ATTR_VALUE).set_value(l_pixels.c_str());
	}

	if (!doc.save_file(l_full_file_path.c_str()))
		throw std::runtime_error("Could not save XML");
}

void kkit::xml::save_board_xml(const kkit::Board& p_board, const std::string& p_directory, const std::string& p_filename, const std::string& p_klab_version) {
	std::filesystem::create_directories(p_directory);
	std::string l_full_file_path = p_directory + "/" + p_filename;

	pugi::xml_document doc;
	auto n_root = create_header(doc);
	n_root.attribute(XML_ATTR_FTYPE).set_value(XML_VALUE_FTYPE_BOARD);
	n_root.attribute(XML_ATTR_LAB3D_V).set_value(p_klab_version.c_str());

	auto n_board = n_root.append_child(XML_TAG_BOARD);
	n_board.append_attribute(XML_ATTR_PLAYER_X);
	n_board.append_attribute(XML_ATTR_PLAYER_Y);
	n_board.append_attribute(XML_ATTR_PLAYER_DIR);
	n_board.attribute(XML_ATTR_PLAYER_X).set_value(p_board.get_player_start_x());
	n_board.attribute(XML_ATTR_PLAYER_Y).set_value(p_board.get_player_start_y());
	n_board.attribute(XML_ATTR_PLAYER_DIR).set_value(p_board.get_player_direction_as_string().c_str());

	for (int y{ 0 }; y < c::MAP_H; ++y) {

		auto n_row = n_board.append_child(XML_TAG_ROW);
		n_row.append_attribute(XML_ATTR_NO);
		n_row.attribute(XML_ATTR_NO).set_value(y);

		for (int x{ 0 }; x < c::MAP_W; ++x) {
			auto n_tile = n_row.append_child(XML_TAG_TILE);
			n_tile.append_attribute(XML_ATTR_NO);
			n_tile.attribute(XML_ATTR_NO).set_value(x);

			n_tile.append_attribute(XML_ATTR_WALL_NO);
			n_tile.attribute(XML_ATTR_WALL_NO).set_value(p_board.get_tile_no(x, y) + 1);

			if (p_board.is_blast(x, y)) {
				n_tile.append_attribute(XML_ATTR_DESTRUCTIBLE);
				n_tile.attribute(XML_ATTR_DESTRUCTIBLE).set_value(XML_VALUE_TRUE);
			}

			if (p_board.is_inside(x, y)) {
				n_tile.append_attribute(XML_ATTR_CLIP);
				n_tile.attribute(XML_ATTR_CLIP).set_value(XML_VALUE_TRUE);
			}

			if (p_board.is_vertical(x, y)) {
				n_tile.append_attribute(XML_ATTR_VERTICAL);
				n_tile.attribute(XML_ATTR_VERTICAL).set_value(XML_VALUE_TRUE);
			}

		}
	}

	if (!doc.save_file(l_full_file_path.c_str()))
		throw std::runtime_error("Could not save XML");

}
