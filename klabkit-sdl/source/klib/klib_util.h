#ifndef KLIB_UTIL_H
#define KLIB_UTIL_H

#include <vector>

namespace klib {

	namespace util {

		using byte = unsigned char;

		// check if (px, py) is in rect (rx, ry, rx+rw,rx+rh)
		bool is_p_in_rect(int p_x, int p_y, int r_x, int r_y, int r_w, int r_h);

		// equivalent of std::string::find for vector<byte> - look for headers etc
		std::size_t find_subvector(const std::vector<byte>& p_bytes, const std::vector<byte>& p_search, std::size_t p_offset);

		template <class T>
		T validate(T p_input, T p_min_val, T p_max_val) {
			if (p_input < p_min_val)
				return p_min_val;
			else if (p_input > p_max_val)
				return p_max_val;
			else
				return p_input;
		}
	}

}

#endif

