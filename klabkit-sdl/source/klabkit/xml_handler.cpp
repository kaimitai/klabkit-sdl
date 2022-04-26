#include "xml_handler.h"
#include "../klib/klib_util.h"
#include "constants.h"
#include <filesystem>
#include <vector>

void kkit::xml::create_header(pugi::xml_document& p_doc) {
	auto n_comments = p_doc.append_child(pugi::node_comment);
	n_comments.set_value(XML_COMMENTS);

	auto n_metadata = p_doc.append_child(XML_TAG_META);

	n_metadata.append_attribute(XML_ATTR_APP_V);
	n_metadata.attribute(XML_ATTR_APP_V).set_value(c::APP_VERSION);
}

void kkit::xml::save_wall_xml(const kkit::Wall& p_wall, const std::string& p_directory, const std::string& p_filename) {
	std::filesystem::create_directories(p_directory);
	std::string l_full_file_path = p_directory + "/" + p_filename;

	pugi::xml_document doc;
	create_header(doc);

	auto n_wall = doc.append_child(XML_TAG_WALL);
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
	create_header(doc);

	auto n_board = doc.append_child(XML_TAG_BOARD);
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
