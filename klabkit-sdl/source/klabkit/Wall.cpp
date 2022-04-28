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

kkit::Wall::Wall(const std::vector<std::vector<byte>>& p_image, kkit::Wall_type p_wall_type, bool p_blast, bool p_inside) :
	image{ p_image }, wall_type{ p_wall_type }, blast{ p_blast }, inside{ p_inside }
{}

byte kkit::Wall::get_palette_index(int p_x, int p_y) const {
	return this->image.at(p_x).at(p_y);
}

const std::vector<std::vector<byte>>& kkit::Wall::get_image(void) const {
	return this->image;
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

bool kkit::Wall::is_blast(void) const {
	return this->blast;
}

bool kkit::Wall::is_inside(void) const {
	return this->inside;
}

kkit::Wall_type kkit::Wall::get_wall_type(void) const {
	return this->wall_type;
}

void kkit::Wall::toggle_blast(void) {
	this->blast = !this->blast;
}

void kkit::Wall::toggle_inside(void) {
	this->inside = !this->inside;
}

void kkit::Wall::toggle_type(void) {
	if (this->wall_type == kkit::Wall_type::Cube)
		this->wall_type = kkit::Wall_type::Direction;
	else if (this->wall_type == kkit::Wall_type::Direction)
		this->wall_type = kkit::Wall_type::Plane;
	else
		this->wall_type = kkit::Wall_type::Cube;
}

void kkit::Wall::set_image(const std::vector<std::vector<byte>>& p_image) {
	this->image = p_image;
}
