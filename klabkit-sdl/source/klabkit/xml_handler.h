#ifndef KLABKIT_XML_HANDLER_H
#define KLABKIT_XML_HANDLER_H

#include <string>
#include "../pugixml/pugixml.hpp"
#include "Board.h"
#include "Wall.h"

namespace kkit {

	namespace xml {

		void save_board_xml(const kkit::Board& p_board, const std::string& p_directory, const std::string& p_filename);
		void save_wall_xml(const kkit::Wall& p_wall, const std::string& p_directory, const std::string& p_filename);

		void create_header(pugi::xml_document& p_doc);

		constexpr char XML_COMMENTS[]{ "KKIT/SDL xml file (https://github.com/kaimitai/klabkit-sdl)" };

		// xml tags
		constexpr char XML_TAG_META[]{ "kkit_sdl" };

		constexpr char XML_TAG_BOARD[]{ "board" };
		constexpr char XML_TAG_ROW[]{ "row" };
		constexpr char XML_TAG_TILE[]{ "tile" };

		constexpr char XML_TAG_WALL[]{ "wall" };
		constexpr char XML_TAG_PIXEL_ROW[]{ "pixel_row" };

		// xml attributes
		constexpr char XML_ATTR_APP_V[]{ "app_version" };

		constexpr char XML_ATTR_APP_PLAYER_X[]{ "player_start_x" };
		constexpr char XML_ATTR_APP_PLAYER_Y[]{ "player_start_y" };
		constexpr char XML_ATTR_APP_PLAYER_DIR[]{ "player_start_direction" };

		constexpr char XML_ATTR_NO[]{ "no" };
		constexpr char XML_ATTR_WALL_NO[]{ "wall_no" };
		constexpr char XML_ATTR_VERTICAL[]{ "vertical" };
		constexpr char XML_ATTR_DESTRUCTIBLE[]{ "destructible" };
		constexpr char XML_ATTR_CLIP[]{ "clip" };

		constexpr char XML_ATTR_WALL_TYPE[]{ "wall_type" };
		constexpr char XML_ATTR_VALUE[]{ "value" };

		constexpr char XML_VALUE_BLOCK[]{ "block" };
		constexpr char XML_VALUE_PLANE[]{ "plane" };
		constexpr char XML_VALUE_DIRECTIONAL[]{ "directional" };
		constexpr char XML_VALUE_TRUE[]{ "true" };
		constexpr char XML_VALUE_FALSE[]{ "false" };
	}

}

#endif
