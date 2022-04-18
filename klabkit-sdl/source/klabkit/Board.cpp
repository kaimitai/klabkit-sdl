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

			// tile number (wall) is the first 10 bits - but offset is not 0
			int l_tile_no = (l_value & 0b1111111111) - 1;
			// 11th bit is inside indicator
			bool l_inside = (l_value & 0b10000000000) >> 10;
			// 12th bit is blast indicator
			bool l_blast = (l_value & 0b100000000000) >> 11;
			// 13th bit is player start sq indicator
			bool l_player_sq = (l_value & 0b1000000000000) >> 12;
			// 14th bit is vertical indicator
			bool l_vertical = (l_value & 0b10000000000000) >> 13;

			if (l_player_sq) {
				this->player_x = j;
				this->player_y = i;
				int l_direction = l_value & 0b11;
				if (l_direction == 0)
					this->player_direction = Player_direction::Right;
				else if (l_direction == 1)
					this->player_direction = Player_direction::Down;
				else if (l_direction == 2)
					this->player_direction = Player_direction::Left;
				else
					this->player_direction = Player_direction::Up;

				l_tile_no = -1;
				l_inside = false;
				l_blast = false;
				l_vertical = false;
			}

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

std::vector<byte> kkit::Board::get_bytes(void) const {
	std::vector<byte> result;

	for (int j{ 0 }; j < this->tiles.size(); ++j)
		for (int i{ 0 }; i < this->tiles[j].size(); ++i) {

			if (j == this->player_x && i == this->player_y) {

				int l_result = 0b1000000000000;
				if (this->player_direction == Player_direction::Down)
					l_result += 1;
				if (this->player_direction == Player_direction::Left)
					l_result += 2;
				if (this->player_direction == Player_direction::Up)
					l_result += 3;

				byte b1 = static_cast<byte>(l_result % 256);
				byte b2 = static_cast<byte>(l_result / 256);

				result.push_back(b1);
				result.push_back(b2);
			}
			else {
				int l_result = tiles[j][i].get_tile_no() + 1;

				if (tiles[j][i].is_inside())
					l_result |= 0b10000000000;
				if (tiles[j][i].is_blast())
					l_result |= 0b100000000000;
				if (tiles[j][i].is_vertical())
					l_result |= 0b10000000000000;

				byte b1 = static_cast<byte>(l_result % 256);
				byte b2 = static_cast<byte>(l_result / 256);

				result.push_back(b1);
				result.push_back(b2);
			}


		}

	return result;
}