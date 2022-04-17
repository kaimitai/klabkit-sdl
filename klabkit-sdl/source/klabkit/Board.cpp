#include "Board.h"
#include "constants.h"

using byte = unsigned char;

kkit::Board::Board(const std::vector<byte>& p_bytes) {

	for (int j{ 0 }; j < c::MAP_H; ++j) {
		std::vector<kkit::Map_tile> l_tiles;

		for (int i{ 0 }; i < c::MAP_W; ++i) {
			int l_byte_no = 2 * (j * c::MAP_W + i);
			int l_b1 = static_cast<int>(p_bytes.at(l_byte_no));
			int l_b2 = static_cast<int>(p_bytes.at(l_byte_no + 1));

			int l_value = l_b1 + 256 * l_b2;

			// tile number (wall) is the first 9 bytes - but offset is not 0
			int l_tile_no = (l_value & 0b111111111) - 1;
			// 10th bit is inside indicator
			bool l_inside = (l_value & 0b10000000000) >> 10;
			// 11th bit is blast indicator
			bool l_blast = (l_value & 0b100000000000) >> 11;
			// 12th bit is vertical indicator
			bool l_vertical = (l_value & 0b1000000000000) >> 12;

			l_tiles.push_back(kkit::Map_tile(l_tile_no, l_inside, l_blast, l_vertical));
		}

		this->tiles.push_back(l_tiles);
	}
}

bool kkit::Board::is_empty_tile(int p_x, int p_y) const {
	return this->tiles.at(p_x).at(p_y).empty();
}

int kkit::Board::get_tile_no(int p_x, int p_y) const {
	return this->tiles.at(p_x).at(p_y).get_tile_no();
}
