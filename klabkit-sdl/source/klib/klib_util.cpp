#include "klib_util.h"

using byte = unsigned char;

bool klib::util::is_p_in_rect(int p_x, int p_y, int r_x, int r_y, int r_w, int r_h) {
	return (p_x >= r_x && p_x < r_x + r_w && p_y >= r_y && p_y < r_y + r_h);
}

std::size_t klib::util::find_subvector(const std::vector<byte>& total, const std::vector<byte>& part, std::size_t start_index) {
	for (std::size_t i{ start_index }; i < total.size() - part.size(); ++i) {
		bool is_match{ true };
		for (std::size_t j{ 0 }; j < part.size(); ++j)
			if (total[i + j] != part[j]) {
				is_match = false;
				break;
			}

		if (is_match)
			return i;
	}

	return total.size();
}
