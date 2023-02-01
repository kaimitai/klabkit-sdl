#include <algorithm>
#include <cmath>
#include <filesystem>
#include "./Board_ui.h"
#include "./../imgui/imgui.h"
#include "./../imgui/imgui_impl_sdl.h"
#include "./../imgui/imgui_impl_sdlrenderer.h"
#include "imgui_helper.h"
#include "./constants_color.h"
#include "./../klib/gfx.h"
#include "./../klib/klib_util.h"
#include "./kkit_gfx.h"

void kkit::Board_ui::draw_ui(SDL_Renderer* p_rnd,
	const klib::User_input& p_input, kkit::Project& p_project,
	kkit::Project_gfx& p_gfx, int p_w, int p_h) {

	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	draw_ui_main(p_rnd, p_input, p_project, p_gfx, p_w, p_h);
	draw_ui_minimap(p_rnd, p_input, p_project, p_gfx, p_w, p_h);
	draw_ui_selected_board_tile(p_rnd, p_input, p_project, p_gfx);

	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}

void kkit::Board_ui::draw_ui_main(SDL_Renderer* p_rnd,
	const klib::User_input& p_input, kkit::Project& p_project,
	kkit::Project_gfx& p_gfx, int p_w, int p_h) {
	bool l_shift{ p_input.is_shift_pressed() };
	bool l_ctrl{ p_input.is_ctrl_pressed() };

	ImGui::Begin("Main");
	auto l_new_lvl_ind{ imgui::slider("Level", m_board_ind + 1, 1, p_project.get_board_count()) };
	if (l_new_lvl_ind)
		m_board_ind = l_new_lvl_ind.value() - 1;

	ImGui::Separator();
	ImGui::Text("File Operations");

	bool l_pref_kzp{ p_project.get_config().get_ext_boards() == kkit::Data_ext::KZP };

	if (imgui::button("Export xml")) {
		int l_exported{ 0 };

		for (int i{ l_shift ? 0 : m_board_ind }; i < (l_shift ? p_project.get_board_count() : m_board_ind + 1); ++i) {
			this->xml_export(p_project, i);
			++l_exported;
		}
		p_project.add_message(std::to_string(l_exported) + " xml file(s) exported");
	}
	ImGui::SameLine();

	if (l_pref_kzp) {
		if (imgui::button("Save KZP", c::COLOR_STYLE_GREEN)) {
			save_boards_kzp(p_project, true);
		}
		ImGui::SameLine();
	}

	if (imgui::button("Save DAT", l_pref_kzp ? c::COLOR_STYLE_NORMAL : c::COLOR_STYLE_GREEN)) {
		save_boards_kzp(p_project, false);
	}
	ImGui::SameLine();

	if (imgui::button("Save bmp")) {
		int l_exported{ 0 };

		for (int i{ l_shift ? 0 : m_board_ind };
			i < (l_shift ? p_project.get_board_count() : m_board_ind + 1);
			++i) {
			kkit::gfx::project_map_to_bmp(p_project, i, p_gfx.get_floor_color(), m_toggles[1], m_toggles[2]);
			kkit::gfx::project_minimap_to_bmp(p_project, i);
			++l_exported;
		}
		p_project.add_message(std::to_string(l_exported) + " board and minimap bmp file(s) saved");
	}

	if (imgui::button("Import xml")) {
		int l_imported{ 0 };

		for (int i{ l_shift ? 0 : m_board_ind }; i < (l_shift ? p_project.get_board_count() : m_board_ind + 1); ++i)
			if (this->xml_import(p_project, i)) {
				++l_imported;
				p_gfx.reload_minimap_texture(p_rnd, p_project, i);
			}

		if (l_imported > 0)
			p_project.add_message(std::to_string(l_imported) + " xml file(s) imported");
		else
			p_project.add_message("No xml file(s) found");
	}

	ImGui::Separator();
	ImGui::Text("Navigation");

	if (imgui::button("Previous Tile")) {
		prev_tile(p_project, l_ctrl, p_w, p_h);
	}
	ImGui::SameLine();
	if (imgui::button("Next Tile")) {
		next_tile(p_project, l_ctrl, p_w, p_h);
	}
	ImGui::SameLine();
	if (imgui::button("Count Tiles")) {
		//int l_tile_no = (l_ctrl_held ? this->get_selected_tile_no(p_project) : this->get_selected_board_tile_no(p_project));
		int l_tile_no{ get_selected_board_tile_no(p_project) };
		int l_tile_cnt = this->count_tiles(p_project, l_tile_no, l_shift);

		std::string l_msg{ "Count selected " };
		l_msg += (l_ctrl ? "tile picker" : "board");
		l_msg += " tile (#" + std::to_string(l_tile_no + 1) + ", ";
		l_msg += (l_shift ? "all boards" : "board " + std::to_string(m_board_ind + 1));
		l_msg += "): " + std::to_string(l_tile_cnt);

		p_project.add_message(l_msg);
	}

	ImGui::Separator();
	ImGui::Text("Flash");

	static std::vector<std::string> ls_labels{
		"Directions", "Destructible", "Noclip", "Selected"
	};

	for (std::size_t i{ 0 }; i < m_toggles.size(); ++i) {
		bool l_toggled{ m_toggles[i] };

		if (imgui::button(ls_labels[i], l_toggled ? c::COLOR_STYLE_GREEN : c::COLOR_STYLE_GRAY))
			m_toggles[i] = !m_toggles[i];

		ImGui::SameLine();
	}
	ImGui::NewLine();

	ImGui::Separator();
	ImGui::Text("Output Messages");
	ImGui::Separator();

	for (const auto& l_msg : p_project.get_messages())
		ImGui::Text(l_msg.first.c_str());

	ImGui::End();
}

void kkit::Board_ui::draw_ui_minimap(SDL_Renderer* p_rnd,
	const klib::User_input& p_input, kkit::Project& p_project,
	kkit::Project_gfx& p_gfx, int p_w, int p_h) {
	ImGui::Begin("Minimap");

	ImVec2 l_wmin = ImGui::GetWindowContentRegionMin();
	ImVec2 l_wmax = ImGui::GetWindowContentRegionMax();

	constexpr float l_ow{ static_cast<float>(64) };
	constexpr float l_oh{ static_cast<float>(64) };
	float l_iw = (l_wmax.x - l_wmin.x);
	float l_ih = (l_wmax.y - l_wmin.y);

	float l_scale_w = l_iw / l_ow;
	float l_scale_h = l_ih / l_oh;
	float scale = std::min(l_scale_w, l_scale_h);

	ImGui::Image(m_minimap_texture, //p_gfx.get_texture(c::INDEX_MM_TEXTURES, m_board_ind),
		ImVec2(l_ow * scale, l_oh * scale));

	if (p_input.mouse_held() && ImGui::IsItemHovered()) {
		ImGuiIO& io = ImGui::GetIO();

		int lx = static_cast<int>(ImGui::GetItemRectMin().x);
		int ly = static_cast<int>(ImGui::GetItemRectMin().y);
		int lw = static_cast<int>(ImGui::GetItemRectMax().x - ImGui::GetItemRectMin().x);
		int lh = static_cast<int>(ImGui::GetItemRectMax().y - ImGui::GetItemRectMin().y);

		int tx = c::MAP_W * (static_cast<int>(io.MousePos.x) - lx) / lw;
		int ty = c::MAP_H * (static_cast<int>(io.MousePos.y) - ly) / lh;

		int supw{ static_cast<int>(m_cam_zoom * p_w) };
		int suph{ static_cast<int>(m_cam_zoom * p_h) };

		if (is_valid_tile_pos(std::make_pair(tx, ty))) {
			set_cam_x(c::WALL_IMG_W * tx - supw / 2, p_w);
			set_cam_y(c::WALL_IMG_H * ty - suph / 2, p_h);
		}
	}

	ImGui::End();
}

void kkit::Board_ui::draw_ui_selected_board_tile(SDL_Renderer* p_rnd,
	const klib::User_input& p_input, kkit::Project& p_project,
	kkit::Project_gfx& p_gfx) {

	int l_sel_tile_no = get_selected_board_tile_no(p_project);
	bool l_is_start_tile = p_project.get_board(m_board_ind).is_start_tile(m_sel_tile_x, m_sel_tile_y);
	bool l_blast = p_project.get_board(m_board_ind).is_blast(m_sel_tile_x, m_sel_tile_y);
	bool l_inside = p_project.get_board(m_board_ind).is_inside(m_sel_tile_x, m_sel_tile_y);
	bool l_is_dir = (p_project.get_wall_type(l_sel_tile_no) == kkit::Wall_type::Direction);

	std::string l_label{ "Selected Board Tile (" + std::to_string(m_sel_tile_x) +
		"," + std::to_string(m_sel_tile_y) + ")###sbt" };
	ImGui::Begin(l_label.c_str());

	// draw tile
	if (l_sel_tile_no >= 0) {
		ImGui::Image(p_gfx.get_texture(c::INDEX_WALL_TEXTURES, l_sel_tile_no), ImVec2(128, 128));
		std::string l_sel_text{ "Selected Tile: #" + std::to_string(l_sel_tile_no + 1) };
		ImGui::Text(l_sel_text.c_str());
	}
	else
		ImGui::Text("Empty Tile");

	ImGui::Separator();
	ImGui::Text("Properties");
	// toggle board tile: destructible
	if (imgui::button("Destructible", l_blast ? c::COLOR_STYLE_GREEN : c::COLOR_STYLE_GRAY))
		p_project.toggle_mt_blast(m_board_ind, m_sel_tile_x, m_sel_tile_y);
	ImGui::SameLine();
	// toggle board tile: clip/noclip
	if (imgui::button("Noclip", l_inside ? c::COLOR_STYLE_GREEN : c::COLOR_STYLE_GRAY))
		p_project.toggle_mt_inside(m_board_ind, m_sel_tile_x, m_sel_tile_y);
	if (l_is_dir || l_is_start_tile) {
		ImGui::SameLine();
		// toggle board tile: direction
		std::string l_dir_descr{ l_is_start_tile ?
		"Face: " + p_project.get_board(m_board_ind).get_player_direction_as_string() :
		(p_project.get_board(m_board_ind).is_vertical(m_sel_tile_x, m_sel_tile_y) ? "Vertical" : "Horizontal") };

		if (imgui::button(l_dir_descr.c_str(), c::COLOR_STYLE_ORANGE))
			p_project.toggle_mt_direction(m_board_ind, m_sel_tile_x, m_sel_tile_y);
	}

	ImGui::End();
}
