#ifndef KLIB_FONT_H
#define KLIB_FONT_H

#include <SDL.h>
#include <map>
#include <string>
#include <vector>

namespace klib {

	using byte = unsigned char;
	constexpr SDL_Color COL_TRANSPARENT{ 160, 64, 192 };

	class Font {
		int f_w, f_h;
		std::map<char, SDL_Texture*> letters;

	public:
		Font(SDL_Renderer* p_rnd, const std::vector<byte>& p_bytes, int p_w, int p_h);
		~Font(void);
		void write_text(SDL_Renderer* p_rnd, const std::string& p_text, int p_x, int p_y, SDL_Color p_color = SDL_Color{ 255, 255, 255 }) const;
		void clear(void);

		int w(void) const;
		int h(void) const;
	};

}

#endif
