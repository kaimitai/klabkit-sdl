#include "gfx.h"

// blitting
void klib::gfx::blit(SDL_Renderer* p_rnd, SDL_Texture* p_texture, int p_x, int p_y) {
	SDL_Rect t_rect;
	t_rect.x = p_x;
	t_rect.y = p_y;
	SDL_QueryTexture(p_texture, nullptr, nullptr, &t_rect.w, &t_rect.h);

	SDL_RenderCopy(p_rnd, p_texture, nullptr, &t_rect);
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
		if (p_thickness == 0)
			SDL_RenderFillRect(p_rnd, &r);
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
