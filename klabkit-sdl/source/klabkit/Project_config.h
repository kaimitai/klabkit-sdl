#ifndef KLABKIT_PROJECT_CONFIG_H
#define KLABKIT_PROJECT_CONFIG_H

#include <set>
#include <string>
#include <tuple>
#include <vector>
#include "constants.h"

namespace kkit {

	using byte = unsigned char;

	constexpr int CONF_DEF_LZW_CTYPE{ 2 };
	constexpr int CONF_DEF_BCOUNT{ 30 };
	constexpr int CONF_DEF_WCOUNT{ 448 };
	constexpr char CONF_DEF_LABEL[]{ "2.1-DEFAULT" };
	constexpr char CONF_DEF_PROJ_FOLDER[]{ "." };

	class Project_config {
		friend class Project;

		// config label, and folder for file i/o
		std::string label, project_folder;
		// number of boards and walls
		// lzw_comp_type indicates compressed file layout for boards.kzp and walls.kzp:
		// 0 - klab 2.0 and 2.1, 1 - klab 1.0 and 1.1
		// in the future support walken? :)
		int board_count, wall_count, lzw_comp_type;
		// set of tiles that will be marked as clip when flashing tiles in the board grid,
		// even if it is not a metadata override
		std::set<int> clip_overrides;
		// the tiles in the tile picker, in the boards screen
		std::vector<int> tile_picker;
		std::tuple<byte, byte, byte> floor_color;

		// fill the tile picker with tiles that have not been explicitly set
		void fill_tile_picker(void);

	public:
		Project_config(void);
		Project_config(const std::string& p_label,
			const std::string& p_folder,
			int p_bcount, int p_wcount, int p_lzw_ctype,
			const std::set<int>& p_clip_overrides,
			const std::vector<int>& p_tile_picker,
			const std::tuple<byte, byte, byte>& p_floor_rgb);
		std::tuple<byte, byte, byte> get_floor_color(void) const;

		bool is_clip_override(int p_tile_no) const;
	};

}

#endif
