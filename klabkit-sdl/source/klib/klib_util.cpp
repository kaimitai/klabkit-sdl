#include "klib_util.h"

using byte = unsigned char;

void klib::util::write_uint_le(std::vector<byte>& p_bytes,
	unsigned int p_value, std::size_t p_byte_size) {
	unsigned int mod{ 256 };

	for (std::size_t i{ 0 }; i < p_byte_size; ++i) {
		p_bytes.push_back(static_cast<byte>(
			p_value % 256
			));
		p_value /= 256;
	}
}

unsigned int klib::util::read_uint_le(const std::vector<byte>& p_bytes,
	std::size_t p_byte_size,
	std::size_t p_offset) {
	unsigned int result{ 0 };
	unsigned int l_mult{ 1 };

	for (std::size_t i{ 0 }; i < p_byte_size; ++i) {
		result += l_mult * static_cast<unsigned int>(p_bytes.at(p_offset++));
		l_mult *= 256;
	}

	return result;
}

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

int klib::util::uint_le(const std::vector<byte>& p_bytes, std::size_t p_offset, int p_num_bytes) {
	int result{ 0 };
	for (int i{ p_num_bytes - 1 }; i >= 0; --i)
		result = result * 256 + p_bytes.at(p_offset + i);

	return result;
}

std::vector<byte> klib::util::from_uint_le(int p_int, int p_byte_count) {
	std::vector<byte> result;

	while (p_int > 0) {
		result.push_back(static_cast<byte>(p_int % 256));
		p_int /= 256;
	}

	while (static_cast<int>(result.size()) < p_byte_count)
		result.push_back(0);

	return result;
}

void klib::util::append_uint_le(std::vector<byte>& p_bytes, int p_int, int p_byte_count) {
	auto l_bytes = from_uint_le(p_int, p_byte_count);
	p_bytes.insert(end(p_bytes), begin(l_bytes), end(l_bytes));
}
