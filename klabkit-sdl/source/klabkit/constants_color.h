#ifndef SKC_CONSTANTS_COLOR_H
#define SKC_CONSTANTS_COLOR_H

#include <SDL.h>
#include <cstddef>
#include "./../imgui/imgui.h"
#include "./../imgui/imgui_impl_sdl.h"
#include "./../imgui/imgui_impl_sdlrenderer.h"

using byte = unsigned char;

namespace kkit {

	namespace c {

		constexpr ImVec4 sdl_to_imgui(SDL_Color p_col) {
			return ImVec4{ static_cast<float>(p_col.r) / 255.0f,
				static_cast<float>(p_col.g) / 255.0f,
				static_cast<float>(p_col.b) / 255.0f,
				1.0f };
		}

		// button style numbers
		constexpr std::size_t COLOR_STYLE_NORMAL{ 0 };
		constexpr std::size_t COLOR_STYLE_ORANGE{ 1 };
		constexpr std::size_t COLOR_STYLE_RED{ 2 };

		constexpr SDL_Color COL_BLACK{ 0,0,0 };
		constexpr SDL_Color COL_BLUE{ 66,150,250 };
		constexpr SDL_Color COL_BLUE_ACTIVE{ 15,135,250 };
		constexpr SDL_Color COL_BLUE_DARK{ 40,73,117 };
		constexpr SDL_Color COL_GREEN{ 0,255,0 };
		constexpr SDL_Color COL_GOLD{ 0xd9,0x86,0x20 };
		constexpr SDL_Color COL_ORANGE{ 0xff,0xa5,0x00 };
		constexpr SDL_Color COL_PINK{ 0xff,0xc0,0xcb };
		constexpr SDL_Color COL_PINK_DEEP{ 0xff,0x14,0x93 };
		constexpr SDL_Color COL_PINK_VR{ 0xdb,0x70,0x93 };
		constexpr SDL_Color COL_RED{ 255,0,0 };
		constexpr SDL_Color COL_YELLOW{ 255,255,0 };
		constexpr SDL_Color COL_WHITE{ 255,255,255 };

		constexpr ImVec4 COLI_BLACK{ sdl_to_imgui(COL_BLACK) };
		constexpr ImVec4 COLI_BLUE{ sdl_to_imgui(COL_BLUE) };
		constexpr ImVec4 COLI_BLUE_ACTIVE{ sdl_to_imgui(COL_BLUE_ACTIVE) };
		constexpr ImVec4 COLI_BLUE_DARK{ sdl_to_imgui(COL_BLUE_DARK) };
		constexpr ImVec4 COLI_GREEN{ sdl_to_imgui(COL_GREEN) };
		constexpr ImVec4 COLI_GOLD{ sdl_to_imgui(COL_GOLD) };
		constexpr ImVec4 COLI_ORANGE{ sdl_to_imgui(COL_ORANGE) };
		constexpr ImVec4 COLI_PINK{ sdl_to_imgui(COL_PINK) };
		constexpr ImVec4 COLI_PINK_DEEP{ sdl_to_imgui(COL_PINK_DEEP) };
		constexpr ImVec4 COLI_PINK_VR{ sdl_to_imgui(COL_PINK_VR) };
		constexpr ImVec4 COLI_RED{ sdl_to_imgui(COL_RED) };
		constexpr ImVec4 COLI_YELLOW{ sdl_to_imgui(COL_YELLOW) };
		constexpr ImVec4 COLI_WHITE{ sdl_to_imgui(COL_WHITE) };
	}

}

#endif
