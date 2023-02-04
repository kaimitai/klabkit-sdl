#ifndef KLABKIT_XML_HANDLER_H
#define KLABKIT_XML_HANDLER_H

#include <string>
#include "../pugixml/pugixml.hpp"
#include "Board.h"
#include "Wall.h"
#include "Project_config.h"

namespace kkit {

	namespace xml {

		// load XMLs
		kkit::Project_config read_config_xml(const std::string& p_file_name);
		kkit::Wall load_wall_xml(const std::string& p_file_name);
		kkit::Board load_board_xml(const std::string& p_file_name);

		// save XMLs
		void save_board_xml(const kkit::Board& p_board, const std::string& p_directory, const std::string& p_filename, const std::string& p_klab_version);
		void save_wall_xml(const kkit::Wall& p_wall, const std::string& p_directory, const std::string& p_filename, const std::string& p_klab_version);

		pugi::xml_node create_header(pugi::xml_document& p_doc);

		constexpr char XML_COMMENTS[]{ "KKIT/SDL xml file (https://github.com/kaimitai/klabkit-sdl)" };

		// xml tags
		constexpr char XML_TAG_META[]{ "kkit_sdl" };

		constexpr char XML_TAG_CONFIG[]{ "config" };
		constexpr char XML_TAG_CLIP_OVERRIDES[]{ "clip_overrides" };
		constexpr char XML_TAG_TILE_PICKER[]{ "tile_picker" };
		constexpr char XML_TAG_TILE_OVERLAYS[]{ "tile_overlays" };

		constexpr char XML_TAG_BOARD[]{ "board" };
		constexpr char XML_TAG_ROW[]{ "row" };
		constexpr char XML_TAG_TILE[]{ "tile" };

		constexpr char XML_TAG_WALL[]{ "wall" };
		constexpr char XML_TAG_PIXEL_ROW[]{ "pixel_row" };

		// xml attributes
		constexpr char XML_ATTR_APP_V[]{ "app_version" };
		constexpr char XML_ATTR_LAB3D_V[]{ "lab3d_version" };

		constexpr char XML_ATTR_FTYPE[]{ "file_type" };

		constexpr char XML_ATTR_LABEL[]{ "label" };
		constexpr char XML_ATTR_PROJ_DIR[]{ "project_directory" };
		constexpr char XML_ATTR_BCOUNT[]{ "board_count" };
		constexpr char XML_ATTR_WCOUNT[]{ "wall_count" };
		constexpr char XML_ATTR_LZW_TYPE[]{ "lzw_header_type" };
		constexpr char XML_ATTR_EXT_BOARDS[]{ "ext_boards" };
		constexpr char XML_ATTR_EXT_WALLS[]{ "ext_walls" };

		constexpr char XML_ATTR_FLOOR_RGB[]{ "floor_rgb" };
		constexpr char XML_ATTR_DESCRIPTION[]{ "description" };

		constexpr char XML_ATTR_PLAYER_X[]{ "player_start_x" };
		constexpr char XML_ATTR_PLAYER_Y[]{ "player_start_y" };
		constexpr char XML_ATTR_PLAYER_DIR[]{ "player_start_direction" };

		constexpr char XML_ATTR_ACTIVE_CONFIG_NO[]{ "active_config_no" };
		constexpr char XML_ATTR_NO[]{ "no" };
		constexpr char XML_ATTR_WALL_NO[]{ "wall_no" };
		constexpr char XML_ATTR_VERTICAL[]{ "vertical" };
		constexpr char XML_ATTR_DESTRUCTIBLE[]{ "destructible" };
		constexpr char XML_ATTR_CLIP[]{ "clip" };

		constexpr char XML_ATTR_WALL_TYPE[]{ "wall_type" };
		constexpr char XML_ATTR_VALUE[]{ "value" };

		// XML constant values
		constexpr char XML_VALUE_FTYPE_WALL[]{ "wall" };
		constexpr char XML_VALUE_FTYPE_BOARD[]{ "board" };

		constexpr char XML_VALUE_CUBE[]{ "cube" };
		constexpr char XML_VALUE_PLANE[]{ "plane" };
		constexpr char XML_VALUE_DIRECTIONAL[]{ "directional" };
		constexpr char XML_VALUE_TRUE[]{ "true" };
		constexpr char XML_VALUE_FALSE[]{ "false" };

	}

}

#endif
