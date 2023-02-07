#ifndef KKIT_SAVEGAME_H
#define KKIT_SAVEGAME_H

#include <map>
#include <string>
#include <utility>
#include <vector>
#include "./Board.h"

using byte = unsigned char;

namespace kkit {

	class Savegame {
		std::string m_hiscore_name;
		std::vector<byte> m_unknown_bytes;
		std::map<std::string, std::vector<unsigned int>> m_variable_values;
		static std::vector<std::pair<std::string, std::size_t>> m_variable_sizes;
		kkit::Board m_board;

		static void write_uint_le(std::vector<byte>& p_bytes,
			unsigned int p_value, std::size_t p_byte_size);

		static unsigned int read_uint_le(const std::vector<byte>& p_bytes,
			std::size_t p_byte_size,
			std::size_t& p_offset);

		unsigned int get_variable_value(const std::string& p_key, std::size_t p_slot = 0) const;
		void read_variable(const std::string& p_key, const std::vector<byte>& p_bytes, std::size_t& p_offset, std::size_t p_byte_size, std::size_t p_count = 1);
		void read_variable_range(std::size_t p_start, std::size_t p_end, const std::vector<byte>& p_bytes, std::size_t& p_offset, std::size_t p_count = 1);
		void write_variable_range(std::size_t p_start, std::size_t p_end, std::vector<byte>& p_bytes) const;

	public:
		Savegame(const std::vector<byte>& p_bytes);
		Savegame(const std::string& p_hiscore_name,
			const std::map<std::string, std::vector<unsigned int>>& p_var_values,
			const kkit::Board& p_board,
			const std::vector<byte>& p_unknown_bytes);

		void set_board(const kkit::Board& p_board);

		const std::map<std::string, std::vector<unsigned int>>& get_variable_values(void) const;
		const std::vector<byte>& get_unknown_bytes(void) const;
		static std::vector<std::string> get_variable_order(void);

		Board get_board(void) const;
		const std::string& get_hiscore_name(void) const;
		unsigned int get_board_num(void) const;
		std::vector<byte> get_bytes(void) const;
	};
}

#endif
