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

byte kkit::Wall::get_header_byte(void) const {
	byte result{ 0 };
	if (this->wall_type == Wall_type::Plane)
		result = 1;
	else if (this->wall_type == Wall_type::Direction)
		result = 2;

	if (inside)
		result |= 0b1000;
	if (blast)
		result |= 0b10000;

	return result;
}

std::vector<byte> kkit::Wall::get_image_bytes(void) const {
	std::vector<byte> result;

	for (int i{ 0 }; i < this->image.size(); ++i)
		result.insert(end(result), begin(image[i]), end(image[i]));

	return result;
}
