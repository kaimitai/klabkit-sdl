#ifndef KLABKIT_WALL_H
#define KLABKIT_WALL_H

#include <vector>

namespace kkit {

	enum class Wall_type { Cube, Plane, Direction };

	using byte = unsigned char;

	class Wall {

		Wall_type wall_type;
		bool blast, inside;
		std::vector<std::vector<byte>> image;

	public:
		Wall(const std::vector<byte>& p_bytes, byte p_flags);
		byte get_palette_index(int p_x, int p_y) const;

		// getters
		const std::vector<std::vector<byte>>& get_image(void) const;

		// logical getters
		byte get_header_byte(void) const;
		std::vector<byte> get_image_bytes(void) const;
	};

}

#endif
