#ifndef KKIT_SAVEGAME_H
#define KKIT_SAVEGAME_H

#include <map>
#include <string>
#include <utility>
#include <vector>
#include "./Board.h"
#include "./Wall.h"

using byte = unsigned char;

namespace kkit {

	struct Savegame_variable {

		std::string m_var_name, m_count;
		std::size_t m_byte_size;

		Savegame_variable(const std::string& p_var_name,
			std::size_t p_byte_size,
			const std::string& p_count);

		bool is_numeric_count(void) const;
	};

	class Savegame {
		static std::vector<kkit::Savegame_variable> m_variables;
		std::string m_hiscore_name;
		std::vector<byte> m_unknown_bytes;
		std::map<std::string, std::vector<unsigned int>> m_variable_values;
		kkit::Board m_board;

		static void write_uint_le(std::vector<byte>& p_bytes,
			unsigned int p_value, std::size_t p_byte_size);

		static unsigned int read_uint_le(const std::vector<byte>& p_bytes,
			std::size_t p_byte_size,
			std::size_t& p_offset);

		unsigned int get_variable_value(const std::string& p_key, std::size_t p_slot = 0) const;
		void read_variable(const std::string& p_key, const std::vector<byte>& p_bytes, std::size_t& p_offset, std::size_t p_byte_size, std::size_t p_count = 1);

		void parse_block(const std::vector<byte>& p_bytes, const Savegame_variable& p_var, std::size_t& p_offset);
		void post_load(const std::vector<byte>& p_bytes, std::size_t p_offset);

		void get_block_bytes(std::vector<byte>& p_bytes, const Savegame_variable& p_var) const;

	public:
		Savegame(const std::vector<byte>& p_bytes);
		Savegame(const std::vector<byte>& p_bytes, const std::vector<kkit::Wall>& p_walls);
		Savegame(const std::string& p_hiscore_name,
			const std::map<std::string, std::vector<unsigned int>>& p_var_values,
			const kkit::Board& p_board,
			const std::vector<byte>& p_unknown_bytes);

		void set_board(const kkit::Board& p_board);

		static void set_variables(const std::vector<kkit::Savegame_variable>& p_variables);
		static const std::vector<Savegame_variable>& get_variables(void);
		const std::vector<byte>& get_unknown_bytes(void) const;
		static std::vector<std::string> get_variable_order(void);
		const std::map<std::string, std::vector<unsigned int>>& get_variable_values(void) const;

		Board get_board(void) const;
		const std::string& get_hiscore_name(void) const;
		unsigned int get_board_num(void) const;
		std::vector<byte> get_bytes(void) const;
		std::vector<byte> get_bytes(const std::vector<kkit::Wall>& p_walls) const;
	};
}

#endif
