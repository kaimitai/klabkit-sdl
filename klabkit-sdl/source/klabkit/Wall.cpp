#include "Wall.h"
#include "constants.h"

using byte = unsigned char;

kkit::Wall::Wall(const std::vector<byte>& bytes, byte p_flags) {
	for (std::size_t i{ 0 }; i < c::WALL_IMG_H; ++i)
		image.push_back(std::vector<byte>(begin(bytes) + c::WALL_IMG_W * i, begin(bytes) + c::WALL_IMG_W * (i + 1)));
}

byte kkit::Wall::get_palette_index(int p_x, int p_y) const {
	return this->image.at(p_x).at(p_y);
}
