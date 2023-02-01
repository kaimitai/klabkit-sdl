#include <algorithm>
#include <cmath>
#include <filesystem>
#include "./Board_ui.h"
#include "./../imgui/imgui.h"
#include "./../imgui/imgui_impl_sdl.h"
#include "./../imgui/imgui_impl_sdlrenderer.h"
#include "imgui_helper.h"
#include "./../klib/gfx.h"
#include "./../klib/klib_util.h"
#include "./kkit_gfx.h"

void kkit::Board_ui::draw_ui(SDL_Renderer* p_rnd,
	const klib::User_input& p_input, kkit::Project& p_project,
	kkit::Project_gfx& p_gfx) {

	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	draw_ui_main(p_rnd, p_input, p_project, p_gfx);
	draw_ui_minimap(p_rnd, p_input, p_project, p_gfx);

	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}

void kkit::Board_ui::draw_ui_main(SDL_Renderer* p_rnd,
	const klib::User_input& p_input, kkit::Project& p_project,
	kkit::Project_gfx& p_gfx) {
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
		if (imgui::button("Save KZP")) {
			save_boards_kzp(p_project, true);
		}
		ImGui::SameLine();
	}

	if (imgui::button("Save DAT")) {
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
				// TODO: Minimap
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

	}
	ImGui::SameLine();
	if (imgui::button("Next Tile")) {

	}
	ImGui::SameLine();
	if (imgui::button("Count Tiles")) {

	}

	ImGui::Separator();
	ImGui::Text("Flash");

	static std::vector<std::string> ls_labels{
		"Directions", "Destructible", "Clippable", "Selected"
	};

	for (std::size_t i{ 0 }; i < m_toggles.size(); ++i) {
		bool l_toggled{ m_toggles[i] };

		if (imgui::button(ls_labels[i], l_toggled ? 1 : 2))
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
	kkit::Project_gfx& p_gfx) {
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

	ImGui::Image(p_gfx.get_texture(c::INDEX_MM_TEXTURES, m_board_ind),
		ImVec2(l_ow * scale, l_oh * scale));

	ImGui::End();
}
