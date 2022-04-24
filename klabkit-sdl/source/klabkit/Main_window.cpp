#include "Main_window.h"
#include "./../klib/gfx.h"

kkit::Main_window::Main_window(SDL_Renderer* p_rnd) : board_window{ Board_window(p_rnd) }, window_index{ 0 }
{ }

void kkit::Main_window::move(const klib::User_input& p_input, int p_delta_ms, Project& p_project, Project_gfx& p_gfx) {
	if (window_index == 0)
		board_window.move(p_input, p_delta_ms, p_project, p_gfx);

	// move toast notifcations
	p_gfx.move(p_delta_ms);
}

void kkit::Main_window::draw(SDL_Renderer* p_rnd, const klib::User_input& p_input, Project& p_project, Project_gfx& p_gfx) {
	SDL_SetRenderDrawColor(p_rnd, klib::gc::COL_BLACK.r, klib::gc::COL_BLACK.g, klib::gc::COL_BLACK.b, 0);
	SDL_RenderClear(p_rnd);

	p_gfx.draw(p_rnd);

	if (window_index == 0)
		board_window.draw(p_rnd, p_input, p_project, p_gfx);

	// draw toast notifications
	p_gfx.draw(p_rnd);
}
