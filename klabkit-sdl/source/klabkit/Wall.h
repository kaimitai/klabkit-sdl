#ifndef KLABKIT_WALL_H
#define KLABKIT_WALL_H

#include <vector>

namespace kkit {

	using byte = unsigned char;

	class Wall {

		std::vector<std::vector<byte>> image;

	public:
		Wall(const std::vector<byte>& p_bytes, byte p_flags);
		byte get_palette_index(int p_x, int p_y) const;
	};

}

#endif
