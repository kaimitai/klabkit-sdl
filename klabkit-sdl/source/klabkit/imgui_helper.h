#ifndef KKIT_IMGUI_HELPER
#define KKIT_IMGUI_HELPER

#include <string>
#include <utility>
#include <vector>
#include <optional>
#include "./constants_color.h"
#include "./../imgui/imgui.h"
#include "./../imgui/imgui_impl_sdl.h"
#include "./../imgui/imgui_impl_sdlrenderer.h"
#include "./../klabkit/Project_config.h"
#include "./../klabkit/Project_gfx.h"

namespace kkit {

	namespace imgui {

		template<class T = int>
		std::optional<T> slider(const std::string& p_label,
			T p_value, T p_min, T p_max) {
			int l_value{ static_cast<int>(p_value) };
			if (ImGui::SliderInt(p_label.c_str(), &l_value, static_cast<int>(p_min), static_cast<int>(p_max)))
				return static_cast<T>(l_value);
			else
				return std::nullopt;
		}

		std::optional<bool> checkbox(const std::string& p_label, bool p_value);
		void tooltip_text(const std::string& p_text);
		void window(const std::string& p_title, int p_x, int p_y, int p_w, int p_h);
		bool button(const std::string& p_label,
			const std::size_t p_style_no = c::COLOR_STYLE_NORMAL,
			const std::string& p_tooltip = std::string());

	}

}

#endif

