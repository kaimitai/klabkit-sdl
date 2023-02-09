#ifndef KLABKIT_XML_HANDLER_H
#define KLABKIT_XML_HANDLER_H

#include <string>
#include "../pugixml/pugixml.hpp"
#include "Board.h"
#include "Wall.h"
#include "Savegame.h"
#include "Hiscore.h"
#include "Project_config.h"

namespace kkit {

	namespace xml {

		// load XMLs
		kkit::Project_config read_config_xml(const std::string& p_file_name);
		std::vector<kkit::Savegame_variable> get_savegame_variables(const pugi::xml_node& p_root_node, int p_config_no);
		kkit::Wall load_wall_xml(const std::string& p_file_name);
		kkit::Board load_board_xml(const std::string& p_file_name);
		kkit::Savegame load_savefile_xml(const std::string& p_file_name);
		kkit::Hiscore load_hiscore_xml(const std::string& p_file_name);

		// save XMLs
		void save_board_xml(const kkit::Board& p_board, const std::string& p_directory, const std::string& p_filename, const std::string& p_klab_version);
		void save_wall_xml(const kkit::Wall& p_wall, const std::string& p_directory, const std::string& p_filename, const std::string& p_klab_version);
		void save_savefile_xml(const kkit::Savegame& p_save, const std::string& p_directory, const std::string& p_filename, const std::string& p_klab_version);
		void save_hiscore_xml(const kkit::Hiscore& p_hiscore, const std::string& p_directory, const std::string& p_filename, const std::string& p_klab_version);

		pugi::xml_node create_header(pugi::xml_document& p_doc);
		void add_board_to_node(pugi::xml_node&, const kkit::Board& p_board);
		kkit::Board get_board_from_node(pugi::xml_node& p_node);

		constexpr char XML_COMMENTS[]{ "KKIT/SDL xml file (https://github.com/kaimitai/klabkit-sdl)" };

		// xml tags
		constexpr char XML_TAG_META[]{ "kkit_sdl" };

		constexpr char XML_TAG_CONFIG[]{ "config" };
		constexpr char XML_TAG_CLIP_OVERRIDES[]{ "clip_overrides" };
		constexpr char XML_TAG_TILE_PICKER[]{ "tile_picker" };
		constexpr char XML_TAG_TILE_OVERLAYS[]{ "tile_overlays" };
		constexpr char XML_TAG_SAVE_FILE_CONFIGS[]{ "saveconfigs" };
		constexpr char XML_TAG_SAVE_FILE_CONFIG[]{ "saveconfig" };
		constexpr char XML_TAG_VARIABLE[]{ "variable" };

		constexpr char XML_TAG_BOARD[]{ "board" };
		constexpr char XML_TAG_ROW[]{ "row" };
		constexpr char XML_TAG_TILE[]{ "tile" };
		
		constexpr char XML_TAG_WALL[]{ "wall" };
		constexpr char XML_TAG_PIXEL_ROW[]{ "pixel_row" };

		constexpr char XML_TAG_PLAYER_NAME[]{ "player_name" };
		constexpr char XML_TAG_SCORE[]{ "score" };
		constexpr char XML_TAG_UNK_BYTES[]{ "unknown_bytes" };

		// xml attributes
		constexpr char XML_ATTR_APP_V[]{ "app_version" };
		constexpr char XML_ATTR_LAB3D_V[]{ "lab3d_version" };

		constexpr char XML_ATTR_FTYPE[]{ "file_type" };

		constexpr char XML_ATTR_CONFIG_NO[]{ "config_no" };
		constexpr char XML_ATTR_NAME[]{ "name" };
		constexpr char XML_ATTR_SIZE[]{ "size" };
		constexpr char XML_ATTR_COUNT[]{ "count" };

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
		constexpr char XML_ATTR_BIT1[]{ "bit1" };
		constexpr char XML_ATTR_BIT2[]{ "bit2" };

		constexpr char XML_ATTR_WALL_TYPE[]{ "wall_type" };
		constexpr char XML_ATTR_VALUE[]{ "value" };

		// XML constant values
		constexpr char XML_VALUE_FTYPE_WALL[]{ "wall" };
		constexpr char XML_VALUE_FTYPE_BOARD[]{ "board" };
		constexpr char XML_VALUE_FTYPE_SAVEFILE[]{ "savefile" };
		constexpr char XML_VALUE_FTYPE_HISCORE[]{ "hiscore" };

		constexpr char XML_VALUE_CUBE[]{ "cube" };
		constexpr char XML_VALUE_PLANE[]{ "plane" };
		constexpr char XML_VALUE_DIRECTIONAL[]{ "directional" };
		constexpr char XML_VALUE_TRUE[]{ "true" };
		constexpr char XML_VALUE_FALSE[]{ "false" };

	}

}

#endif
