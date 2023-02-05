#ifndef KKIT_SAVEGAME_H
#define KKIT_SAVEGAME_H

#include <vector>
#include "./Board.h"

using byte = unsigned char;

namespace kkit {

	class Savegame {
		std::vector<byte> m_bytes;

		static unsigned int read_uint16_le(const std::vector<byte>& p_bytes, std::size_t p_offset);

	public:
		Savegame(const std::vector<byte>& p_bytes);
		Board get_board(void) const;
	};
}

#endif
