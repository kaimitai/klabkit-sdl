#include "Wall.h"
#include "constants.h"

using byte = unsigned char;

kkit::Wall::Wall(const std::vector<byte>& bytes, byte p_flags) {
	byte l_wall_type = (p_flags & 7);

	if (l_wall_type == 0)
		wall_type = Wall_type::Cube;
	else if (l_wall_type == 1)
		wall_type = Wall_type::Plane;
	else
		wall_type = Wall_type::Direction;

	inside = (p_flags >> 3) & 1;
	blast = (p_flags >> 4) & 1;

	for (std::size_t i{ 0 }; i < c::WALL_IMG_H; ++i)
		image.push_back(std::vector<byte>(begin(bytes) + c::WALL_IMG_W * i, begin(bytes) + c::WALL_IMG_W * (i + 1)));
}

byte kkit::Wall::get_palette_index(int p_x, int p_y) const {
	return this->image.at(p_x).at(p_y);
}
