#include "xml_handler.h"
#include "../klib/klib_util.h"
#include "constants.h"
#include <filesystem>
#include <set>
#include <vector>

kkit::Project_config kkit::xml::read_config_xml(const std::string& p_file_name) {

	pugi::xml_document doc;
	if (!doc.load_file(p_file_name.c_str()))
		throw std::exception("Could not find configuration file");

	pugi::xml_node n_meta = doc.child(XML_TAG_META);
	auto n_conf = n_meta.child(XML_TAG_CONFIG);

	std::string l_label = n_conf.attribute(XML_ATTR_LABEL).as_string();
	std::string l_proj_dir = n_conf.attribute(XML_ATTR_PROJ_DIR).as_string();
	int l_bcount = n_conf.attribute(XML_ATTR_BCOUNT).as_int();
	int l_wcount = n_conf.attribute(XML_ATTR_WCOUNT).as_int();
	int l_lzw_type = n_conf.attribute(XML_ATTR_LZW_TYPE).as_int();

	std::vector l_clip_overrides_v = klib::util::string_split<int>(n_conf.child(XML_TAG_CLIP_OVERRIDES).attribute(XML_ATTR_VALUE).as_string(), ',');
	std::vector<int> l_tile_picker;

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
		l_clip_overrides,
		l_tile_picker);
}

pugi::xml_node kkit::xml::create_header(pugi::xml_document& p_doc) {
	auto n_comments = p_doc.append_child(pugi::node_comment);
	n_comments.set_value(XML_COMMENTS);

	auto n_metadata = p_doc.append_child(XML_TAG_META);

	n_metadata.append_attribute(XML_ATTR_APP_V);
	n_metadata.attribute(XML_ATTR_APP_V).set_value(c::APP_VERSION);

	n_metadata.append_attribute(XML_ATTR_FTYPE);

	return n_metadata;
}

void kkit::xml::save_wall_xml(const kkit::Wall& p_wall, const std::string& p_directory, const std::string& p_filename) {
	std::filesystem::create_directories(p_directory);
	std::string l_full_file_path = p_directory + "/" + p_filename;

	pugi::xml_document doc;
	auto n_root = create_header(doc);
	n_root.attribute(XML_ATTR_FTYPE).set_value(XML_VALUE_FTYPE_WALL);

	auto n_wall = n_root.append_child(XML_TAG_WALL);
	n_wall.append_attribute(XML_ATTR_WALL_TYPE);
	n_wall.append_attribute(XML_ATTR_DESTRUCTIBLE);
	n_wall.append_attribute(XML_ATTR_CLIP);

	auto l_wall_type = p_wall.get_wall_type();
	if (l_wall_type == kkit::Wall_type::Cube)
		n_wall.attribute(XML_ATTR_WALL_TYPE).set_value(XML_VALUE_BLOCK);
	else if (l_wall_type == kkit::Wall_type::Plane)
		n_wall.attribute(XML_ATTR_WALL_TYPE).set_value(XML_VALUE_PLANE);
	else
		n_wall.attribute(XML_ATTR_WALL_TYPE).set_value(XML_VALUE_DIRECTIONAL);

	n_wall.attribute(XML_ATTR_DESTRUCTIBLE).set_value(p_wall.is_blast() ? XML_VALUE_TRUE : XML_VALUE_FALSE);
	n_wall.attribute(XML_ATTR_CLIP).set_value(p_wall.is_inside() ? XML_VALUE_TRUE : XML_VALUE_FALSE);

	auto l_bytes = p_wall.get_image();

	for (int y{ 0 }; y < c::WALL_IMG_H; ++y) {
		auto n_row = n_wall.append_child(XML_TAG_PIXEL_ROW);
		n_row.append_attribute(XML_ATTR_NO);
		n_row.attribute(XML_ATTR_NO).set_value(y);

		std::string l_pixels = klib::util::string_join(l_bytes.at(y), ',');
		n_row.append_attribute(XML_ATTR_VALUE);
		n_row.attribute(XML_ATTR_VALUE).set_value(l_pixels.c_str());
	}

	if (!doc.save_file(l_full_file_path.c_str()))
		throw std::exception("Could not save XML");
}

void kkit::xml::save_board_xml(const kkit::Board& p_board, const std::string& p_directory, const std::string& p_filename) {
	std::filesystem::create_directories(p_directory);
	std::string l_full_file_path = p_directory + "/" + p_filename;

	pugi::xml_document doc;
	auto n_root = create_header(doc);
	n_root.attribute(XML_ATTR_FTYPE).set_value(XML_VALUE_FTYPE_BOARD);

	auto n_board = n_root.append_child(XML_TAG_BOARD);
	n_board.append_attribute(XML_ATTR_APP_PLAYER_X);
	n_board.append_attribute(XML_ATTR_APP_PLAYER_Y);
	n_board.append_attribute(XML_ATTR_APP_PLAYER_DIR);
	n_board.attribute(XML_ATTR_APP_PLAYER_X).set_value(p_board.get_player_start_x());
	n_board.attribute(XML_ATTR_APP_PLAYER_Y).set_value(p_board.get_player_start_y());
	n_board.attribute(XML_ATTR_APP_PLAYER_DIR).set_value(p_board.get_player_direction_as_string().c_str());

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
		throw std::exception("Could not save XML");

}
