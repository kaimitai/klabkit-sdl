#include "Map_tile.h"

using byte = unsigned char;

kkit::Map_tile::Map_tile(int p_tile_no, bool p_inside, bool p_blast, bool p_vertical) :
	tile_no{ p_tile_no }, inside{ p_inside }, blast{ p_blast }, vertical{ p_vertical }
{ }

bool kkit::Map_tile::empty(void) const {
	return this->tile_no == -1;
}

int kkit::Map_tile::get_tile_no(void) const {
	return this->tile_no;
}

bool kkit::Map_tile::is_inside(void) const {
	return this->inside;
}

bool kkit::Map_tile::is_blast(void) const {
	return this->blast;
}

bool kkit::Map_tile::is_vertical(void) const {
	return this->vertical;
}

// manipulators
void kkit::Map_tile::toggle_direction(void) {
	this->vertical = !this->vertical;
}

void kkit::Map_tile::toggle_blast(void) {
	this->blast = !this->blast;
}

void kkit::Map_tile::toggle_inside(void) {
	this->inside = !this->inside;
}
