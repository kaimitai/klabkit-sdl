#ifndef KLIB_UTIL_H
#define KLIB_UTIL_H

namespace klib {

	namespace util {

		// check if (px, py) is in rect (rx, ry, rx+rw,rx+rh)
		bool is_p_in_rect(int p_x, int p_y, int r_x, int r_y, int r_w, int r_h);

	}

}

#endif

