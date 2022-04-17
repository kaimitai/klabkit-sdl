#ifndef KLABKIT_MAP_TILE_H
#define KLABKIT_MAP_TILE_H

namespace kkit {

	using byte = unsigned char;

	class Map_tile {

		bool inside, blast, vertical;
		int tile_no;

	public:
		Map_tile(int p_tile_no, bool p_inside, bool p_blast, bool p_vertical);
		
		// logical getters
		bool empty(void) const;

		// getters
		int get_tile_no(void) const;
		bool is_inside(void) const;
		bool is_blast(void) const;
		bool is_vertical(void) const;
	};

}

#endif
