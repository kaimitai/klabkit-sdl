#ifndef KLIB_TOAST_NOTIFICATION_HANDLER_H
#define KLIB_TOAST_NOTIFICATION_HANDLER_H

#include <queue>
#include "Font.h"
#include "Toast_notification.h"

namespace klib {

	class Toast_notification_handler {

		std::queue<klib::Toast_notification> toast_notifications;
		bool empty(void) const;

	public:
		Toast_notification_handler(void) = default;
		void move(int p_delta_ms);
		void draw(SDL_Renderer* p_rnd, const klib::Font& p_font) const;

		void add_toast(const std::vector<std::string>&, int p_status);
		void add_toast_error(const std::vector<std::string>&);
		void add_toast_info(const std::vector<std::string>&);
		void add_toast_ok(const std::vector<std::string>&);
		void add_toast(const std::string&, int p_status);
		void add_toast_error(const std::string&);
		void add_toast_info(const std::string&);
		void add_toast_ok(const std::string&);

		bool exists(const std::vector<std::string>&) const;
		bool exists(const std::string&) const;

	};

}

#endif
