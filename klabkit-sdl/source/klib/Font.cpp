#include "Font.h"
#include "gfx.h"

using byte = unsigned char;

klib::Font::Font(SDL_Renderer* p_rnd, const std::vector<byte>& p_bytes, int p_w, int p_h) : f_w{ p_w }, f_h{ p_h } {

	int l_letter_count = static_cast<int>(p_bytes.size()) * 8 / (p_w * p_h);

	SDL_Surface* font_bmp = SDL_CreateRGBSurface(0, l_letter_count * p_w, p_h, 24, 0, 0, 0, 0);

	for (int j = 0; j < p_h; ++j)
		for (int i = 0; i < 2 * l_letter_count; i++) {
			int x_pos = 8 * i;
			for (int b = 0; b < 8; ++b) {
				int byte_val = (p_bytes.at(j * 96 * 2 + i) >> (7 - b)) & 1;
				SDL_Color color = (byte_val == 1 ? COL_TRANSPARENT : SDL_Color{ 255,255,255 });

				gfx::put_pixel(font_bmp, x_pos + b, j, ((color.a) << 24) + ((color.r) << 16) + ((color.g) << 8) + color.b);
			}
		}

	// load font
	auto font_textures = klib::gfx::split_surface(p_rnd, font_bmp, COL_TRANSPARENT, this->f_w, this->f_h);
	for (std::size_t i = 0; i < font_textures.size(); ++i)
		letters.insert(std::make_pair(static_cast<char>(i + 32), font_textures.at(i)));

	SDL_FreeSurface(font_bmp);
}

void klib::Font::write_text(SDL_Renderer* p_rnd, const std::string& p_text, int p_x, int p_y, SDL_Color p_color) const {

	for (int i{ 0 }; i < static_cast<int>(p_text.size()); ++i) {
		auto l_letter = this->letters.at(p_text.at(i));
		SDL_SetTextureColorMod(l_letter, p_color.r, p_color.g, p_color.b);
		gfx::blit(p_rnd, l_letter, p_x + this->f_w * i, p_y);
	}
}

klib::Font::~Font(void) {
	this->clear();
}

void klib::Font::clear(void) {
	for (auto& kv : this->letters)
		if (kv.second != nullptr)
			SDL_DestroyTexture(kv.second);

	this->letters.clear();
}

int klib::Font::w(void) const {
	return this->f_w;
}

int klib::Font::h(void) const {
	return this->f_h;
}
