#ifndef KLABKIT_MAP_TILE_H
#define KLABKIT_MAP_TILE_H

namespace kkit {

	using byte = unsigned char;

	class Map_tile {

		bool inside, blast, vertical;
		int tile_no;

	public:
		Map_tile(int p_tile_no, bool p_inside, bool p_blast, bool p_vertical);
		
		// getters
		bool empty(void) const;
		int get_tile_no(void) const;
	};

}

#endif
