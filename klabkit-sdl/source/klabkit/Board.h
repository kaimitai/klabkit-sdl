#ifndef KLABKIT_LEVEL_H
#define KLABKIT_LEVEL_H

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
		bool is_empty_tile(int p_x, int p_y) const;
		int get_tile_no(int p_x, int p_y) const;

	};

}

#endif
