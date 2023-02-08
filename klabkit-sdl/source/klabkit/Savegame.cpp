#include "Savegame.h"
#include "constants.h"
#include <stdexcept>

std::vector<kkit::Savegame_variable> kkit::Savegame::m_variables;

void kkit::Savegame::set_variables(const std::vector<kkit::Savegame_variable>& p_variables) {
	kkit::Savegame::m_variables = p_variables;
}

kkit::Savegame::Savegame(const std::vector<byte>& p_bytes) {
	std::size_t l_offset{ 0 };
	for (const auto& l_variable : m_variables) {
		const auto& l_key{ l_variable.m_var_name };
		std::size_t l_byte_size{ l_variable.m_byte_size };
		const std::string& l_count_str{ l_variable.m_count };
		std::size_t l_count{ l_variable.is_numeric_count() ?
		atoi(l_count_str.c_str()) : get_variable_value(l_key) };

		// parse name
		if (l_key == c::SAVE_CODE_HISCORENAME) {
			for (std::size_t i{ 0 }; i < l_byte_size && p_bytes.at(l_offset + i) != 0x00; ++i)
				m_hiscore_name.push_back(p_bytes.at(l_offset + i));
			l_offset += l_byte_size;
		}
		// parse board
		else if (l_key == c::SAVE_CODE_BOARD) {
			m_board = kkit::Board(std::vector<byte>(
				begin(p_bytes) + l_offset,
				begin(p_bytes) + l_offset + l_byte_size));
			l_offset += l_byte_size;
		}
		// parse numeric variables
		else
			read_variable(l_key, p_bytes, l_offset, l_byte_size, l_count);
	}

	// store unknown bytes (probably inconsequential garbage memory)
	m_unknown_bytes = std::vector<byte>(begin(p_bytes) + l_offset, end(p_bytes));

	// set the board start position based on the board start-variables in the savefile
	int l_px{ static_cast<int>(get_variable_value("startx") / 1024) };
	int l_py{ static_cast<int>(get_variable_value("starty") / 1024) };
	int l_startang{ static_cast<int>(get_variable_value("startang")) };
	kkit::Player_direction l_dir{ kkit::Player_direction::Up };
	if (l_startang < 512)
		l_dir = kkit::Player_direction::Right;
	else if (l_startang < 512 * 2)
		l_dir = kkit::Player_direction::Down;
	else if (l_startang < 512 * 3)
		l_dir = kkit::Player_direction::Left;
	m_board.set_player_start_position(l_px, l_py, l_dir);
}

kkit::Savegame::Savegame(const std::string& p_hiscore_name,
	const std::map<std::string, std::vector<unsigned int>>& p_var_values,
	const kkit::Board& p_board,
	const std::vector<byte>& p_unknown_bytes) :
	m_hiscore_name{ p_hiscore_name },
	m_variable_values{ p_var_values },
	m_board{ p_board },
	m_unknown_bytes{ p_unknown_bytes }
{ }

void kkit::Savegame::write_uint_le(std::vector<byte>& p_bytes,
	unsigned int p_value, std::size_t p_byte_size) {
	unsigned int mod{ 256 };

	for (std::size_t i{ 0 }; i < p_byte_size; ++i) {
		p_bytes.push_back(static_cast<byte>(
			p_value % 256
			));
		p_value /= 256;
	}
}

std::vector<byte> kkit::Savegame::get_bytes(void) const {
	std::vector<byte> result;

	for (const auto& l_variable : m_variables) {
		const auto& l_key{ l_variable.m_var_name };
		std::size_t l_byte_size{ l_variable.m_byte_size };

		if (l_key == c::SAVE_CODE_BOARD) {
			// get board bytes, but remove player start
			auto l_board_bytes{ m_board.get_bytes(false) };
			result.insert(end(result),
				begin(l_board_bytes), end(l_board_bytes));
		}
		else if (l_key == c::SAVE_CODE_HISCORENAME) {
			for (std::size_t i{ 0 }; i < l_byte_size && i < m_hiscore_name.size(); ++i)
				result.push_back(m_hiscore_name[i]);
			for (std::size_t i{ m_hiscore_name.size() }; i < l_byte_size; ++i)
				result.push_back(0x00);
		}
		else {
			if (m_variable_values.find(l_key) != end(m_variable_values))
				for (unsigned int n : m_variable_values.at(l_key))
					write_uint_le(result, n, l_byte_size);
		}

	}

	result.insert(end(result),
		begin(m_unknown_bytes), end(m_unknown_bytes));

	return result;
}

unsigned int kkit::Savegame::get_variable_value(const std::string& p_key, std::size_t p_slot) const {
	auto iter{ m_variable_values.find(p_key) };
	if (iter == end(m_variable_values))
		return 0;
	else
		return iter->second.at(p_slot);
}

unsigned int kkit::Savegame::read_uint_le(const std::vector<byte>& p_bytes,
	std::size_t p_byte_size,
	std::size_t& p_offset) {
	unsigned int result{ 0 };
	unsigned int l_mult{ 1 };

	for (std::size_t i{ 0 }; i < p_byte_size; ++i) {
		result += l_mult * static_cast<unsigned int>(p_bytes.at(p_offset++));
		l_mult *= 256;
	}

	return result;
}

void kkit::Savegame::set_board(const kkit::Board& p_board) {
	m_board = p_board;

	m_variable_values["startx"] = { static_cast<unsigned int>(m_board.get_player_start_x()) * 1024 + 512 };
	m_variable_values["starty"] = { static_cast<unsigned int>(m_board.get_player_start_y()) * 1024 + 512 };

	auto l_dir{ m_board.get_player_start_direction() };
	unsigned int l_dir_angle{ 0 };

	if (l_dir == kkit::Player_direction::Down)
		l_dir_angle = 512;
	else if (l_dir == kkit::Player_direction::Left)
		l_dir_angle = 512 * 2;
	else if (l_dir == kkit::Player_direction::Up)
		l_dir_angle = 512 * 3;

	m_variable_values["startang"] = { l_dir_angle };

}

const std::vector<kkit::Savegame_variable>& kkit::Savegame::get_variables(void) {
	return m_variables;
}

const std::vector<byte>& kkit::Savegame::get_unknown_bytes(void) const {
	return m_unknown_bytes;
}

std::vector<std::string> kkit::Savegame::get_variable_order(void) {
	std::vector<std::string> result;
	for (const auto& l_var : m_variables)
		result.push_back(l_var.m_var_name);
	return result;
}

const std::map<std::string, std::vector<unsigned int>>& kkit::Savegame::get_variable_values(void) const {
	return m_variable_values;
}

kkit::Board kkit::Savegame::get_board(void) const {
	return m_board;
}

const std::string& kkit::Savegame::get_hiscore_name(void) const {
	return m_hiscore_name;
}

unsigned int kkit::Savegame::get_board_num(void) const {
	return this->get_variable_value(c::SAVE_CODE_BOARDNUM);
}

void kkit::Savegame::read_variable(const std::string& p_key, const std::vector<byte>& p_bytes, std::size_t& p_offset, std::size_t p_byte_size, std::size_t p_count) {
	for (std::size_t i{ 0 }; i < p_count; ++i)
		m_variable_values[p_key].push_back(read_uint_le(p_bytes, p_byte_size, p_offset));
}

kkit::Savegame_variable::Savegame_variable(const std::string& p_var_name, std::size_t p_byte_size,
	const std::string& p_count) :
	m_var_name{ p_var_name }, m_byte_size{ p_byte_size }, m_count{ p_count }
{ }

bool kkit::Savegame_variable::is_numeric_count(void) const {
	for (char c : m_count)
		if (c < '0' || c>'9')
			return false;
	return true;
}
