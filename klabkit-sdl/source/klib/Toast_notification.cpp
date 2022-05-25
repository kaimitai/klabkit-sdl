#include "Toast_notification.h"
#include <algorithm>
#include "gfx.h"

klib::Toast_notification::Toast_notification(const std::vector<std::string>& p_msg, int p_status, int p_ms)
	:
	ms_counter{ p_ms },
	message{ p_msg },
	//texture{ nullptr },
	status{ p_status }
{ }

SDL_Color klib::Toast_notification::status_to_color(int p_status) const {
	switch (p_status) {
	case 0:
		return gc::COL_GREEN;
		break;
	case 1:
		return gc::COL_YELLOW;
		break;
	default:
		return gc::COL_RED;
	}
}

int klib::Toast_notification::get_w(const std::vector<std::string>& msg, const klib::Font& p_font) const {
	int result(0);
	for (const auto& msg_line : msg)
		result = std::max<int>(result, static_cast<int>(msg_line.size()));
	return 4 * TN_OFFSET + result * p_font.w();
}

int klib::Toast_notification::get_h(const std::vector<std::string>& msg, const klib::Font& p_font) const {
	return 4 * TN_OFFSET + static_cast<int>(msg.size()) * p_font.h();
}

void klib::Toast_notification::move(int delta_ms) {
	ms_counter -= delta_ms;
}

void klib::Toast_notification::draw(SDL_Renderer* p_rnd, const klib::Font& p_font) const {
	this->draw_texture(p_rnd, p_font, TN_X, TN_Y);
}

bool klib::Toast_notification::is_expired(void) const {
	return (ms_counter < 0);
}

void klib::Toast_notification::draw_texture(SDL_Renderer* rnd, const klib::Font& p_font, int p_x, int p_y) const {
	gfx::draw_rect(rnd, p_x, p_y, this->get_w(this->message, p_font), this->get_h(this->message, p_font), gc::COL_BLACK, 1);
	gfx::draw_rect(rnd, p_x + TN_OFFSET, p_y + TN_OFFSET, this->get_w(this->message, p_font) - 2 * TN_OFFSET, this->get_h(this->message, p_font) - 2 * TN_OFFSET, status_to_color(this->status), 0);

	for (int i = 0; i < static_cast<int>(message.size()); ++i)
		p_font.write_text(rnd, message.at(i), p_x + TN_OFFSET, p_y + TN_OFFSET + (TN_OFFSET + i * p_font.h()), gc::COL_BLACK);
}

bool klib::Toast_notification::msg_equals(const std::vector<std::string>& p_msg) const {
	return (this->message == p_msg);
}
