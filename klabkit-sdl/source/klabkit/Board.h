#ifndef KLABKIT_LEVEL_H
#define KLABKIT_LEVEL_H

#include <string>
#include <vector>
#include "Map_tile.h"

namespace kkit {

	using byte = unsigned char;

	enum class Player_direction { Up, Down, Left, Right };

	class Board {

		Player_direction player_direction{ Player_direction::Up };
		int player_x{ 0 }, player_y{ 0 };
		std::vector<std::vector<kkit::Map_tile>> tiles;

	public:
		Board(const std::vector<byte>& p_bytes);

		// getters
		int get_tile_no(int p_x, int p_y) const;
		bool is_inside(int p_x, int p_y) const;
		bool is_blast(int p_x, int p_y) const;
		bool is_vertical(int p_x, int p_y) const;
		int get_player_start_x(void) const;
		int get_player_start_y(void) const;
		kkit::Player_direction get_player_start_direction(void) const;


		// logical getters
		bool is_empty_tile(int p_x, int p_y) const;
		std::vector<byte> get_bytes(void) const;
		bool is_start_tile(int p_x, int p_y) const;
		std::string get_player_direction_as_string(void) const;
		std::vector<std::vector<kkit::Map_tile>> get_rectangle(int p_x, int p_y, int p_w, int p_h) const;

		// manipulators
		void clear_tile(int p_x, int p_y);
		void set_tile(int p_x, int p_y, const kkit::Map_tile& p_tile);
		void set_player_start_position(int p_x, int p_y, kkit::Player_direction p_direction = kkit::Player_direction::Up);
		void toggle_player_start_direction(void);
		void toggle_direction(int p_x, int p_y);
		void toggle_blast(int p_x, int p_y);
		void toggle_inside(int p_x, int p_y);
	};

}

#endif
