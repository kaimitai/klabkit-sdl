#include <algorithm>
#include "Project_config.h"

using byte = unsigned char;

kkit::Project_config::Project_config(void) :
	project_folder{ CONF_DEF_PROJ_FOLDER },
	label{ CONF_DEF_LABEL },
	board_count{ CONF_DEF_BCOUNT },
	wall_count{ CONF_DEF_WCOUNT },
	lzw_comp_type{ CONF_DEF_LZW_CTYPE },
	m_ext_boards{ Data_ext::KZP },
	m_ext_walls{ Data_ext::KZP },
	floor_color{ kkit::c::FLOOR_COL_RGB }
{
	this->fill_tile_picker();
}

kkit::Project_config::Project_config(const std::string& p_label, const std::string& p_folder,
	int p_bcount, int p_wcount, int p_lzw_ctype,
	const std::string& p_ext_boards, const std::string& p_ext_walls,
	const std::set<int>& p_clip_overrides, const std::vector<int>& p_tile_picker,
	const std::tuple<byte, byte, byte>& p_floor_rgb) :
	project_folder{ p_folder },
	label{ p_label },
	board_count{ p_bcount },
	wall_count{ p_wcount },
	lzw_comp_type{ p_lzw_ctype },
	m_ext_boards{ string_to_data_ext(p_ext_boards) },
	m_ext_walls{ string_to_data_ext(p_ext_walls) },
	tile_picker{ p_tile_picker },
	clip_overrides{ p_clip_overrides },
	floor_color{ p_floor_rgb }
{
	this->fill_tile_picker();
}

kkit::Data_ext kkit::Project_config::get_ext_boards(void) const {
	return m_ext_boards;
}

kkit::Data_ext kkit::Project_config::get_ext_walls(void) const {
	return m_ext_walls;
}

bool kkit::Project_config::is_clip_override(int p_tile_no) const {
	return this->clip_overrides.find(p_tile_no) != end(this->clip_overrides);
}

void kkit::Project_config::fill_tile_picker(void) {
	for (int i{ -2 }; i < this->wall_count; ++i)
		if (std::find(begin(tile_picker), end(tile_picker), i) == end(this->tile_picker))
			tile_picker.push_back(i);
}

kkit::Data_ext kkit::Project_config::string_to_data_ext(const std::string& p_ext) {
	return (p_ext == c::FILE_EXT_KZP ?
		kkit::Data_ext::KZP : kkit::Data_ext::DAT);
}
