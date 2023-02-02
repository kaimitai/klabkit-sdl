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
	floor_color{ kkit::c::FLOOR_COL_RGB },
	m_tile_picker_width{ 8 }
{
	this->fill_tile_picker();
}

kkit::Project_config::Project_config(const std::string& p_label, const std::string& p_folder,
	int p_bcount, int p_wcount, int p_lzw_ctype,
	const std::string& p_ext_boards, const std::string& p_ext_walls,
	const std::set<int>& p_clip_overrides,
	const std::vector<std::pair<std::string, std::vector<int>>>& p_tile_picker,
	const std::tuple<byte, byte, byte>& p_floor_rgb) :
	project_folder{ p_folder },
	label{ p_label },
	board_count{ p_bcount },
	wall_count{ p_wcount },
	lzw_comp_type{ p_lzw_ctype },
	m_ext_boards{ string_to_data_ext(p_ext_boards) },
	m_ext_walls{ string_to_data_ext(p_ext_walls) },
	m_tile_picker{ p_tile_picker },
	clip_overrides{ p_clip_overrides },
	floor_color{ p_floor_rgb },
	m_tile_picker_width{ 8 }
{
	this->fill_tile_picker();
}

kkit::Data_ext kkit::Project_config::get_ext_boards(void) const {
	return m_ext_boards;
}

kkit::Data_ext kkit::Project_config::get_ext_walls(void) const {
	return m_ext_walls;
}

const std::vector<std::pair<std::string, std::vector<int>>>& kkit::Project_config::get_tile_picker(void) const {
	return m_tile_picker;
}

bool kkit::Project_config::is_clip_override(int p_tile_no) const {
	return this->clip_overrides.find(p_tile_no) != end(this->clip_overrides);
}

void kkit::Project_config::fill_tile_picker(void) {
	std::vector<int> l_tmp;
	bool l_first{ true };

	std::set<int> l_defined_tiles;
	for (const auto& kv : m_tile_picker)
		for (int n : kv.second)
			l_defined_tiles.insert(n);

	for (int i{ -2 }; i < this->wall_count; ++i)
		if (l_defined_tiles.find(i) == end(l_defined_tiles)) {
			l_tmp.push_back(i);
			if (l_tmp.size() == m_tile_picker_width) {
				m_tile_picker.push_back(std::make_pair(
					l_first ? "Uncategorized" : "",
					l_tmp
				));
				if (l_first)
					l_first = false;
				l_tmp.clear();
			}
		}

	if (!l_tmp.empty())
		m_tile_picker.push_back(std::make_pair(
			l_first ? "Uncategorized" : "",
			l_tmp
		));
}

kkit::Data_ext kkit::Project_config::string_to_data_ext(const std::string& p_ext) {
	return (p_ext == c::FILE_EXT_KZP ?
		kkit::Data_ext::KZP : kkit::Data_ext::DAT);
}
