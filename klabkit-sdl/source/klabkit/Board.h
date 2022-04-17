#ifndef KLABKIT_LEVEL_H
#define KLABKIT_LEVEL_H

#include <vector>
#include "Map_tile.h"

namespace kkit {

	using byte = unsigned char;

	class Board {

		std::vector<std::vector<kkit::Map_tile>> tiles;

	public:
		Board(const std::vector<byte>& p_bytes);

		// getters
		bool is_empty_tile(int p_x, int p_y) const;
		int get_tile_no(int p_x, int p_y) const;

	};

}

#endif
