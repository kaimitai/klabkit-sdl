#include "gfx.h"

// blitting
void klib::gfx::blit(SDL_Renderer* p_rnd, SDL_Texture* p_texture, int p_x, int p_y) {
	SDL_Rect t_rect;
	t_rect.x = p_x;
	t_rect.y = p_y;
	SDL_QueryTexture(p_texture, nullptr, nullptr, &t_rect.w, &t_rect.h);

	SDL_RenderCopy(p_rnd, p_texture, nullptr, &t_rect);
}

// blit the given texture at t_x, t_y and scale it by a power of two, where param p_p2 is the exponent
void klib::gfx::blit_p2_scale(SDL_Renderer* p_rnd, SDL_Texture* p_texture, int p_x, int p_y, int p_p2) {
	SDL_Rect target_rect;
	target_rect.x = p_x;
	target_rect.y = p_y;
	SDL_QueryTexture(p_texture, nullptr, nullptr, &target_rect.w, &target_rect.h);

	if (p_p2 >= 0) {
		target_rect.w <<= p_p2;
		target_rect.h <<= p_p2;
	}
	else {
		target_rect.w >>= (p_p2 * (-1));
		target_rect.h >>= (p_p2 * (-1));
	}

	SDL_RenderCopy(p_rnd, p_texture, nullptr, &target_rect);
}

// blit the given texture at t_x, t_y and scale it by an integral factor
// factor of 0 means half the size
void klib::gfx::blit_scale(SDL_Renderer* p_rnd, SDL_Texture* p_texture, int p_x, int p_y, int p_scale) {
	SDL_Rect target_rect;
	target_rect.x = p_x;
	target_rect.y = p_y;
	SDL_QueryTexture(p_texture, nullptr, nullptr, &target_rect.w, &target_rect.h);

	if (p_scale == 0) {
		target_rect.w /= 2;
		target_rect.h /= 2;
	}
	else {
		target_rect.w *= p_scale;
		target_rect.h *= p_scale;
	}

	SDL_RenderCopy(p_rnd, p_texture, nullptr, &target_rect);
}

void klib::gfx::blit_full_spec(SDL_Renderer* p_rnd, SDL_Texture* txt, int target_x, int target_y, int target_w, int target_h, int src_x, int src_y, int src_w, int src_h) {
	SDL_Rect src_rect, target_rect;
	target_rect.x = target_x;
	target_rect.y = target_y;
	target_rect.w = target_w;
	target_rect.h = target_h;

	src_rect.x = src_x;
	src_rect.y = src_y;
	src_rect.w = src_w;
	src_rect.h = src_h;

	SDL_RenderCopy(p_rnd, txt, &src_rect, &target_rect);
}

// utility functions
SDL_Texture* klib::gfx::surface_to_texture(SDL_Renderer* p_rnd, SDL_Surface* p_srf, bool p_destroy_surface) {
	SDL_Texture* result = SDL_CreateTextureFromSurface(p_rnd, p_srf);
	if (p_destroy_surface)
		SDL_FreeSurface(p_srf);
	return(result);
}

void klib::gfx::draw_rect(SDL_Renderer* p_rnd, int p_x, int p_y, int p_w, int p_h, SDL_Color p_color, int p_thickness) {
	SDL_SetRenderDrawColor(p_rnd, p_color.r, p_color.g, p_color.b, p_color.a);

	for (int i = 0; i <= p_thickness; ++i) {
		SDL_Rect r;
		r.x = p_x + i;
		r.y = p_y + i;
		r.w = p_w - 2 * i;
		r.h = p_h - 2 * i;
		if (p_thickness == 0) {
			SDL_RenderFillRect(p_rnd, &r);
			return;
		}
		else
			SDL_RenderDrawRect(p_rnd, &r);
	}
}

void  klib::gfx::put_pixel(SDL_Surface* srf, int x, int y, Uint32 pixel) {
	SDL_LockSurface(srf);

	int bpp = srf->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8* p = (Uint8*)srf->pixels + y * srf->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16*)p = pixel;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32*)p = pixel;
		break;

	default:
		break;           /* shouldn't happen, but avoids warnings */
	}

	SDL_UnlockSurface(srf);
}

Uint32 klib::gfx::get_pixel(SDL_Surface* surface, int x, int y) {
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp)
	{
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16*)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32*)p;
		break;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}

std::vector<SDL_Texture*> klib::gfx::split_surface(SDL_Renderer* rnd, SDL_Surface* full_surface, SDL_Color p_trans_col, int p_w, int p_h, bool p_destroy_surface) {
	std::vector<SDL_Texture*> result;

	SDL_Rect tmpRectangle;
	tmpRectangle.w = p_w;
	tmpRectangle.h = p_h;

	for (int j = 0; j < full_surface->h; j += p_h)
		for (int i = 0; i < full_surface->w; i += p_w) {
			tmpRectangle.x = i;
			tmpRectangle.y = j;

			SDL_Surface* tmp = SDL_CreateRGBSurface(0, p_w, p_h, 24, 0, 0, 0, 0);
			SDL_SetColorKey(tmp, true, SDL_MapRGB(tmp->format, p_trans_col.r, p_trans_col.g, p_trans_col.b));

			SDL_BlitSurface(full_surface, &tmpRectangle, tmp, nullptr);

			result.push_back(surface_to_texture(rnd, tmp, p_destroy_surface));
		}

	return(result);
}

void klib::gfx::draw_label(SDL_Renderer* p_rnd, const klib::Font& p_font, const std::string& p_msg, int p_x, int p_y, int p_w, int p_h, SDL_Color p_text_col, SDL_Color p_bg_col) {
	draw_rect(p_rnd, p_x, p_y, p_w, p_h, p_text_col, 1);
	draw_rect(p_rnd, p_x + 1, p_y + 1, p_w - 2, p_h - 2, p_bg_col, 0);

	int l_offset_x = (p_w - static_cast<int>(p_msg.size()) * p_font.w()) / 2;
	int l_offset_y = (p_h - p_font.h()) / 2;
	p_font.write_text(p_rnd, p_msg, p_x + l_offset_x, p_y + l_offset_y, p_text_col);
}

void klib::gfx::draw_window(SDL_Renderer* p_rnd, const klib::Font& p_font, const std::string& p_title, int p_x, int p_y, int p_w, int p_h, int p_header_h, SDL_Color p_text_col, SDL_Color p_bg_col) {
	draw_label(p_rnd, p_font, p_title, p_x, p_y, p_w, p_header_h, p_text_col, p_bg_col);
	draw_rect(p_rnd, p_x, p_y + p_header_h - 1, p_w, p_h - p_header_h - 1, p_text_col, 1);
}
