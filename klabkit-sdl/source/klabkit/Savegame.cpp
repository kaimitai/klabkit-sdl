#include "Savegame.h"

kkit::Savegame::Savegame(const std::vector<byte>& p_bytes) :
	m_bytes{ p_bytes }
{}

unsigned int kkit::Savegame::read_uint16_le(const std::vector<byte>& p_bytes, std::size_t p_offset) {
	return static_cast<unsigned int>(p_bytes.at(p_offset)) +
		256 * static_cast<unsigned int>(p_bytes.at(p_offset + 1));
}

kkit::Board kkit::Savegame::get_board(void) const {
	kkit::Board result(std::vector<byte>(begin(m_bytes) + 27, begin(m_bytes) + 27 + 8192));

	unsigned int l_x{ read_uint16_le(m_bytes, 8192 + 27 + 62) };
	unsigned int l_y{ read_uint16_le(m_bytes, 8192 + 27 + 62 + 2) };
	unsigned int l_ang{ read_uint16_le(m_bytes, 8192 + 27 + 62 + 4) };

	int l_px{ static_cast<int>(l_x - 512) / 1024 };
	int l_py{ static_cast<int>(l_y - 512) / 1024 };
	kkit::Player_direction l_dir{ kkit::Player_direction::Up };

	if (l_ang == 0)
		l_dir = kkit::Player_direction::Right;
	else if (l_ang == 512)
		l_dir = kkit::Player_direction::Down;
	else if (l_ang == 1024)
		l_dir = kkit::Player_direction::Left;

	result.set_player_start_position(l_px, l_py, l_dir);

	return result;
}
