#include "Hiscore.h"
#include "./../klib/klib_util.h"

kkit::Hiscore::Hiscore(const std::vector<byte>& p_bytes) {

	for (std::size_t l_offset{ 0 }; l_offset < p_bytes.size();) {
		std::vector<std::pair<std::string, unsigned int>> l_board_scores;

		for (std::size_t i{ 0 }; i < 8; ++i) {
			auto l_board_score{ std::make_pair<std::string, unsigned int>(
				parse_name(p_bytes, l_offset), parse_score(p_bytes, l_offset)
			) };
			if (!l_board_score.first.empty())
				l_board_scores.push_back(l_board_score);
			l_offset += 16;
		}

		m_scores.push_back(l_board_scores);
	}
}

kkit::Hiscore::Hiscore(const std::vector<std::vector<std::pair<std::string, unsigned int>>>& p_scores) :
	m_scores{ p_scores }
{}

std::vector<byte> kkit::Hiscore::get_bytes(void) const {
	std::vector<byte> result;

	for (const auto& l_brd : m_scores) {
		for (std::size_t l_s{ 0 }; l_s < l_brd.size(); ++l_s) {
			const auto& l_sco{ l_brd[l_s] };

			for (std::size_t i{ 0 }; i < l_sco.first.size() && i < 12; ++i)
				result.push_back(l_sco.first[i]);
			for (std::size_t i{ l_sco.first.size() }; i < 12; ++i)
				result.push_back(0x00);

			klib::util::write_uint_le(result, l_brd[l_s].second, 4);
		}

		for (std::size_t i{ l_brd.size() }; i < 8; ++i)
			for (int j{ 0 }; j < 16; ++j)
				result.push_back(0x00);
	}

	return result;
}

const std::pair<std::string, unsigned int>& kkit::Hiscore::get_score(std::size_t p_board_no,
	std::size_t p_score_no) const {
	return m_scores.at(p_board_no).at(p_score_no);
}

bool kkit::Hiscore::empty(void) const {
	return m_scores.empty();
}

std::size_t kkit::Hiscore::size(void) const {
	return m_scores.size();
}

std::size_t kkit::Hiscore::size(std::size_t p_board_no) const {
	return p_board_no < m_scores.size() ? m_scores[p_board_no].size() : 0;
}

// static functions
std::string kkit::Hiscore::parse_name(const std::vector<byte>& p_bytes, std::size_t p_offset) {
	std::string result;

	for (std::size_t i{ 0 }; i < 12 && p_bytes.at(p_offset + i) != 0x00; ++i)
		result.push_back(p_bytes.at(p_offset + i));

	return result;
}

unsigned int kkit::Hiscore::parse_score(const std::vector<byte>& p_bytes, std::size_t p_offset) {
	return klib::util::read_uint_le(p_bytes, 4, p_offset + 12);
}
