#include "klib_util.h"

bool klib::util::is_p_in_rect(int p_x, int p_y, int r_x, int r_y, int r_w, int r_h) {
	return (p_x >= r_x && p_x < r_x + r_w && p_y >= r_y && p_y < r_y + r_h);
}
