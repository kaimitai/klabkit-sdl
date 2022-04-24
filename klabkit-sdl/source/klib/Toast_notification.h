#ifndef KLIB_TOAST_NOTIFICATION_H
#define KLIB_TOAST_NOTIFICATION_H

#include <SDL.h>
#include <string>
#include <vector>
#include "Font.h"

namespace klib {

	constexpr int TN_X{ 16 };
	constexpr int TN_Y{ 16 };
	constexpr int TN_OFFSET{ 2 };

	class Toast_notification {

		mutable SDL_Texture* texture;
		std::vector<std::string> message;
		int status, ms_counter;

		void populate_texture(SDL_Renderer*, const klib::Font& p_font) const;

		SDL_Color status_to_color(int) const;
		int get_w(const std::vector<std::string>&, const klib::Font& p_font) const;
		int get_h(const std::vector<std::string>&, const klib::Font& p_font) const;

	public:
		Toast_notification(const std::vector<std::string>& p_msgs, int p_status, int p_ms = 2000);
		~Toast_notification(void);
		void move(int p_delta_ms);
		void draw(SDL_Renderer* rnd, const klib::Font& p_font) const;
		bool is_expired(void) const;
		bool msg_equals(const std::vector<std::string>&) const;
	};

}

#endif
