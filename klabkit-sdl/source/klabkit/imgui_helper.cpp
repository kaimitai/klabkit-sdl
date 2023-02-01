#include "./imgui_helper.h"
#include "./constants_color.h"

std::optional<bool> kkit::imgui::checkbox(const std::string& p_label, bool p_value) {
	bool l_value{ p_value };
	if (ImGui::Checkbox(p_label.c_str(), &l_value))
		return l_value;
	else
		return std::nullopt;
}

void kkit::imgui::tooltip_text(const std::string& p_text) {
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
		ImGui::SetTooltip(p_text.c_str());
}

void kkit::imgui::window(const std::string& p_title, int p_x, int p_y, int p_w, int p_h) {
	ImGui::SetNextWindowPos(ImVec2(static_cast<float>(p_x), static_cast<float>(p_y)), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(p_w), static_cast<float>(p_h)), ImGuiCond_FirstUseEver);

	ImGui::Begin(p_title.c_str());
}

bool kkit::imgui::button(const std::string& p_label, const std::size_t p_style_no,
	const std::string& p_tooltip_text) {

	static std::vector<std::vector<ImVec4>> ls_color_styles{
		{c::COLI_BLUE_DARK, c::COLI_BLUE, c::COLI_BLUE_ACTIVE, c::COLI_WHITE},
		{c::COLI_GOLD, c::COLI_ORANGE, c::COLI_YELLOW, c::COLI_BLACK},
		{c::COLI_PINK_VR, c::COLI_PINK_DEEP, c::COLI_PINK, c::COLI_BLACK},
		{c::COLI_GRAY_DARK, c::COLI_GRAY, c::COLI_WHITE, c::COLI_BLACK},
		{c::COLI_GREEN_DARK, c::COLI_GREEN, c::COLI_GREEN_PALE, c::COLI_BLACK}
	};

	ImGui::PushStyleColor(ImGuiCol_Button, ls_color_styles.at(p_style_no).at(0));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ls_color_styles.at(p_style_no).at(1));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ls_color_styles.at(p_style_no).at(2));
	ImGui::PushStyleColor(ImGuiCol_Text, ls_color_styles.at(p_style_no).at(3));

	bool result{ ImGui::Button(p_label.c_str()) };

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	if (!p_tooltip_text.empty() && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
		ImGui::SetTooltip(p_tooltip_text.c_str());

	return result;
}
