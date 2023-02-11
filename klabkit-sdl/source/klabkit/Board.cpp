#include <algorithm>
#include "Board.h"
#include "constants.h"

using byte = unsigned char;

kkit::Board::Board(const std::vector<byte>& p_bytes, const std::vector<Wall>& p_walls) {
	this->tiles = std::vector<std::vector<kkit::Map_tile>>(
		c::MAP_H, std::vector<kkit::Map_tile>(c::MAP_W, kkit::Map_tile()));
	player_x = 0;
	player_y = 0;

	int l_px{ 0 };
	int l_py{ 0 };
	this->player_direction = kkit::Player_direction::Up;

	for (int j{ 0 }; j < c::MAP_BYTES_WALKEN; ++j) {
		int l_x = j / c::MAP_W;
		int l_y = j % c::MAP_W;
		int l_tile_no = p_bytes.at(j);

		bool l_inside{ false };

		if (l_tile_no >= 252) {
			this->player_x = l_x;
			this->player_y = l_y;
			if (l_tile_no == 252)
				this->player_direction = kkit::Player_direction::Right;
			else if (l_tile_no == 253)
				this->player_direction = kkit::Player_direction::Down;
			else if (l_tile_no == 254)
				this->player_direction = kkit::Player_direction::Left;

			l_tile_no = 0;
			l_inside = true;
		}
		else if (l_tile_no >= 128) {
			l_tile_no -= 128;
			l_inside = (l_tile_no != 0);

		}
		else if (l_tile_no == 0)
			l_inside = true;

		if (l_tile_no > 0)
			l_inside |= (l_tile_no == 0 ? true : p_walls.at(l_tile_no - 1).is_inside());

		tiles[l_x][l_y] = kkit::Map_tile(l_tile_no - 1, l_inside, false, false);
	}
}

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
			// second bit from the left
			bool l_bit_2 = (l_value & 0b100000000000000) >> 14;
			// first bit from the left
			bool l_bit_1 = (l_value & 0b1000000000000000) >> 15;

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
				l_inside = true;
				l_blast = false;
				l_vertical = false;
			}

			l_tiles.push_back(kkit::Map_tile(l_tile_no, l_inside, l_blast, l_vertical, l_bit_1, l_bit_2));
		}

		this->tiles.push_back(l_tiles);
	}
}

kkit::Board::Board(const std::vector<std::vector<kkit::Map_tile>>& p_tiles, int p_player_x, int p_player_y, kkit::Player_direction p_player_dir) :
	tiles{ p_tiles }, player_x{ p_player_x }, player_y{ p_player_y }, player_direction{ p_player_dir }
{ }

kkit::Board::Board(void) :
	kkit::Board(std::vector<std::vector<kkit::Map_tile>>(
		c::MAP_H, std::vector<kkit::Map_tile>(c::MAP_W, 0)),
		0, 0, kkit::Player_direction::Up)
{ }

bool kkit::Board::is_empty_tile(int p_x, int p_y) const {
	return this->tiles.at(p_x).at(p_y).empty();
}

int kkit::Board::get_tile_no(int p_x, int p_y) const {
	return this->tiles.at(p_x).at(p_y).get_tile_no();
}

bool kkit::Board::is_inside(int p_x, int p_y) const {
	return this->tiles.at(p_x).at(p_y).is_inside();
}

bool kkit::Board::is_blast(int p_x, int p_y) const {
	return this->tiles.at(p_x).at(p_y).is_blast();
}

bool kkit::Board::is_vertical(int p_x, int p_y) const {
	return this->tiles.at(p_x).at(p_y).is_vertical();
}

bool kkit::Board::is_bit_1(int p_x, int p_y) const {
	return this->tiles.at(p_x).at(p_y).is_bit_1();
}

bool kkit::Board::is_bit_2(int p_x, int p_y) const {
	return this->tiles.at(p_x).at(p_y).is_bit_2();
}

int kkit::Board::get_player_start_x(void) const {
	return this->player_x;
}

int kkit::Board::get_player_start_y(void) const {
	return this->player_y;
}

kkit::Player_direction kkit::Board::get_player_start_direction(void) const {
	return this->player_direction;
}

bool  kkit::Board::is_start_tile(int p_x, int p_y) const {
	return this->player_x == p_x && this->player_y == p_y;
}

std::string kkit::Board::get_player_direction_as_string(void) const {
	if (this->player_direction == kkit::Player_direction::Up)
		return PLAYER_DIR_UP;
	else if (this->player_direction == kkit::Player_direction::Right)
		return PLAYER_DIR_RIGHT;
	if (this->player_direction == kkit::Player_direction::Down)
		return PLAYER_DIR_DOWN;
	else
		return PLAYER_DIR_LEFT;
}

std::vector<byte> kkit::Board::get_bytes(bool p_incl_player_start) const {
	std::vector<byte> result;

	for (int j{ 0 }; j < this->tiles.size(); ++j)
		for (int i{ 0 }; i < this->tiles[j].size(); ++i) {

			if (p_incl_player_start &&
				(j == this->player_x && i == this->player_y)) {

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
				if (tiles[j][i].is_bit_2())
					l_result |= 0b100000000000000;
				if (tiles[j][i].is_bit_1())
					l_result |= 0b1000000000000000;

				byte b1 = static_cast<byte>(l_result % 256);
				byte b2 = static_cast<byte>(l_result / 256);

				result.push_back(b1);
				result.push_back(b2);
			}


		}

	return result;
}

std::vector<byte> kkit::Board::get_bytes(const std::vector<kkit::Wall>& p_walls,
	bool p_incl_player_start) const {
	std::vector<byte> result;

	int l_p_index = c::MAP_H * get_player_start_x() + get_player_start_y();

	byte l_pbyte{ 252 };
	if (get_player_start_direction() == kkit::Player_direction::Down)
		l_pbyte = 253;
	else if (get_player_start_direction() == kkit::Player_direction::Left)
		l_pbyte = 254;
	else if (get_player_start_direction() == kkit::Player_direction::Up)
		l_pbyte = 255;

	kkit::Player_direction l_sdir = get_player_start_direction();

	for (int x = 0; x < c::MAP_W; ++x)
		for (int y = 0; y < c::MAP_H; ++y) {
			int l_tile_no = get_tile_no(x, y);

			bool l_clip_override = (l_tile_no != -1) && (is_inside(x, y) && !p_walls.at(l_tile_no).is_inside());
			l_clip_override |= (l_tile_no == -1 && !is_inside(x, y));

			if (l_clip_override)
				l_tile_no += 128;

			result.push_back(static_cast<byte>(l_tile_no + 1));
		}

	// overwrite the start direction tile
	if (p_incl_player_start)
		result.at(l_p_index) = l_pbyte;

	return result;
}

void kkit::Board::clear_tile(int p_x, int p_y) {
	tiles.at(p_x).at(p_y) = kkit::Map_tile();
}

void kkit::Board::set_tile(int p_x, int p_y, const kkit::Map_tile& p_tile) {
	tiles.at(p_x).at(p_y) = p_tile;
}

void kkit::Board::set_player_start_position(int p_x, int p_y, kkit::Player_direction p_direction) {
	//tiles.at(p_x).at(p_y) = kkit::Map_tile();
	this->player_x = p_x;
	this->player_y = p_y;
	this->player_direction = p_direction;
}

void kkit::Board::toggle_player_start_direction(void) {
	if (this->player_direction == kkit::Player_direction::Up)
		this->player_direction = kkit::Player_direction::Right;
	else if (this->player_direction == kkit::Player_direction::Right)
		this->player_direction = kkit::Player_direction::Down;
	else if (this->player_direction == kkit::Player_direction::Down)
		this->player_direction = kkit::Player_direction::Left;
	else
		this->player_direction = kkit::Player_direction::Up;
}

void kkit::Board::toggle_direction(int p_x, int p_y) {
	tiles.at(p_x).at(p_y).toggle_direction();
}

void kkit::Board::toggle_blast(int p_x, int p_y) {
	tiles.at(p_x).at(p_y).toggle_blast();
}

void kkit::Board::toggle_inside(int p_x, int p_y) {
	tiles.at(p_x).at(p_y).toggle_inside();
}

std::vector<std::vector<kkit::Map_tile>> kkit::Board::get_rectangle(int p_x, int p_y, int p_w, int p_h) const {
	std::vector<std::vector<kkit::Map_tile>> result;

	for (int j{ p_x }; j < p_x + p_w; ++j)
		result.push_back(std::vector<kkit::Map_tile>(begin(tiles.at(j)) + p_y, begin(tiles.at(j)) + p_y + p_h));

	return result;
}

// flip a section of the board around its vertical center
void kkit::Board::flip_vertical(int p_x, int p_y, int p_w, int p_h) {
	int l_px = this->get_player_start_x();
	int l_py = this->get_player_start_y();
	auto l_dir = this->get_player_start_direction();

	for (int j{ 0 }; j < p_w / 2; ++j)
		for (int i{ 0 }; i < p_h; ++i) {
			std::swap(tiles.at(p_x + j).at(p_y + i), tiles.at(p_x + p_w - j - 1).at(p_y + i));
		}

	this->set_player_start_position(l_px, l_py, l_dir);
}

void kkit::Board::flip_horizontal(int p_x, int p_y, int p_w, int p_h) {
	int l_px = this->get_player_start_x();
	int l_py = this->get_player_start_y();
	auto l_dir = this->get_player_start_direction();

	for (int j{ 0 }; j < p_w; ++j)
		for (int i{ 0 }; i < p_h / 2; ++i) {
			std::swap(tiles.at(p_x + j).at(p_y + i), tiles.at(p_x + j).at(p_y + p_h - i - 1));
		}

	this->set_player_start_position(l_px, l_py, l_dir);
}

// static functions
kkit::Player_direction kkit::Board::get_player_direction_from_string(const std::string& p_dir) {
	if (p_dir == PLAYER_DIR_DOWN)
		return kkit::Player_direction::Down;
	else if (p_dir == PLAYER_DIR_RIGHT)
		return kkit::Player_direction::Right;
	else if (p_dir == PLAYER_DIR_LEFT)
		return kkit::Player_direction::Left;
	else
		return kkit::Player_direction::Up;
}
