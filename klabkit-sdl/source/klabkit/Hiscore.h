#ifndef KKIT_HISCORE_H
#define KKIT_HISCORE_H

#include <string>
#include <utility>
#include <vector>

using byte = unsigned char;

namespace kkit {

	class Hiscore {

		static std::string parse_name(const std::vector<byte>& p_bytes, std::size_t p_offset);
		static unsigned int parse_score(const std::vector<byte>& p_bytes, std::size_t p_offset);
		std::vector<std::vector<std::pair<std::string, unsigned int>>> m_scores;

	public:
		Hiscore(const std::vector<byte>& p_bytes);
		Hiscore(const std::vector<std::vector<std::pair<std::string, unsigned int>>>& p_scores);
		std::vector<byte> get_bytes(void) const;

		bool empty(void) const;
		std::size_t size(void) const;
		std::size_t size(std::size_t p_board_no) const;

		const std::pair<std::string, unsigned int>& get_score(std::size_t p_board_no,
			std::size_t p_score_no) const;
	};

}

#endif
