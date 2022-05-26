#include "About_window.h"
#include <cmath>
#include "./../klib/klib_util.h"
#include "./../klib/gfx.h"

void kkit::About_window::draw(SDL_Renderer* rnd, const klib::User_input& p_input, const kkit::Project_gfx& p_gfx) const {

	klib::gfx::blit_tiled(rnd,
		p_gfx.get_texture(this->snausty ? kkit::c::INDEX_WALL_TEXTURES : kkit::c::INDEX_BG_TEXTURES, bg_index),
		timers.at(0).get_frame(), 40 + static_cast <int>(30 * cos(2.0f * 3.14f * static_cast<float>(timers.at(0).get_frame()) / 256.0f)));
	auto co = get_coords(timers.at(1).get_frame());

	constexpr int x_offset = 80;
	constexpr int y_offset = 150;

	for (int j{ 0 }; j < static_cast<int>(co.second); ++j)
		p_gfx.get_font().write_text(rnd, ABOUT_MESSAGE.at(j), x_offset, y_offset + p_gfx.get_font().h() * j, klib::gc::COL_YELLOW);

	if (co.second < ABOUT_MESSAGE.size())
		p_gfx.get_font().write_text(rnd,
			ABOUT_MESSAGE[co.second].substr(0, co.first),
			x_offset,
			y_offset + p_gfx.get_font().h() * static_cast<int>(co.second),
			klib::gc::COL_YELLOW);

}

void kkit::About_window::move(unsigned int delta_ms, const klib::User_input& p_input, const kkit::Project_gfx& p_gfx) {
	for (auto& timer : timers)
		timer.move(delta_ms);

	if (p_input.mouse_clicked())
		if (klib::util::is_p_in_rect(p_input.mx(), p_input.my(), 0, 0, kkit::c::APP_W, 40))
			timers.at(1).reset();
		else
			++bg_index;

	if (p_input.mouse_clicked(false))
		this->snausty = !this->snausty;

	if (bg_index >= p_gfx.get_texture_count(snausty ? kkit::c::INDEX_WALL_TEXTURES : kkit::c::INDEX_BG_TEXTURES))
		bg_index = 0;
}

kkit::About_window::About_window(void) : snausty{ false } {
	timers.push_back(klib::Timer(256, 20));

	int text_size{ 0 };

	for (const auto& s : ABOUT_MESSAGE)
		text_size += static_cast<int>(s.size());

	timers.push_back(klib::Timer(text_size + 10000, 15, false));
}

std::pair<std::size_t, std::size_t> kkit::About_window::get_coords(int frame) const {
	int cur_cnt{ 0 };
	std::size_t r_x{ 0 }, r_y{ 0 };

	for (std::size_t j{ 0 }; j < ABOUT_MESSAGE.size(); ++j)
		if (static_cast<int>(ABOUT_MESSAGE.at(j).size()) < frame) {
			frame -= static_cast<int>(ABOUT_MESSAGE.at(j).size());
			++r_y;
		}
		else {
			r_x = frame;
			break;
		}

	return std::make_pair(r_x, r_y);
}
