#include "Toast_notification_handler.h"

void klib::Toast_notification_handler::add_toast(const std::vector<std::string>& p_msg, int p_status) {
	if (!exists(p_msg))
		toast_notifications.push(Toast_notification(p_msg, p_status));
}

void klib::Toast_notification_handler::add_toast_error(const std::vector<std::string>& p_msg) {
	add_toast(p_msg, 2);
}

void klib::Toast_notification_handler::add_toast_info(const std::vector<std::string>& p_msg) {
	add_toast(p_msg, 1);
}

void klib::Toast_notification_handler::add_toast_ok(const std::vector<std::string>& p_msg) {
	add_toast(p_msg, 0);
}

void klib::Toast_notification_handler::add_toast(const std::string& p_msg, int p_status) {
	add_toast(std::vector<std::string>({ p_msg }), p_status);
}

void klib::Toast_notification_handler::add_toast_error(const std::string& p_msg) {
	add_toast(p_msg, 2);
}

void klib::Toast_notification_handler::add_toast_info(const std::string& p_msg) {
	add_toast(p_msg, 1);
}

void klib::Toast_notification_handler::add_toast_ok(const std::string& p_msg) {
	add_toast(p_msg, 0);
}

bool klib::Toast_notification_handler::exists(const std::vector<std::string>& p_msg) const {
	return (toast_notifications.size() > 0 && toast_notifications.back().msg_equals(p_msg));
}

bool klib::Toast_notification_handler::exists(const std::string& p_msg) const {
	return exists(std::vector<std::string>({ p_msg }));
}

void klib::Toast_notification_handler::move(int p_delta_ms) {
	if (!this->empty()) {
		toast_notifications.front().move(p_delta_ms);
		if (toast_notifications.front().is_expired())
			toast_notifications.pop();
	}
}

void klib::Toast_notification_handler::draw(SDL_Renderer* p_rnd, const klib::Font& p_font) const {
	if (!this->empty())
		toast_notifications.front().draw(p_rnd, p_font);
}

bool klib::Toast_notification_handler::empty(void) const {
	return this->toast_notifications.empty();
}
