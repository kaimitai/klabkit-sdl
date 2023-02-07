#ifndef KLIB_UTIL_H
#define KLIB_UTIL_H

#include <string>
#include <vector>

namespace klib {

	namespace util {

		using byte = unsigned char;

		// read/write little-endiad ints from/to vectors
		void write_uint_le(std::vector<byte>& p_bytes,
			unsigned int p_value, std::size_t p_byte_size);
		unsigned int read_uint_le(const std::vector<byte>& p_bytes,
			std::size_t p_byte_size,
			std::size_t p_offset);

		// check if (px, py) is in rect (rx, ry, rx+rw,rx+rh)
		bool is_p_in_rect(int p_x, int p_y, int r_x, int r_y, int r_w, int r_h);

		// equivalent of std::string::find for vector<byte> - look for headers etc
		std::size_t find_subvector(const std::vector<byte>& p_bytes, const std::vector<byte>& p_search, std::size_t p_offset);

		// get uint_le value from a vector starting at a certain offset, and of a certain byte length
		int uint_le(const std::vector<byte>& p_bytes, std::size_t p_offset, int p_byte_count);

		std::vector<byte> from_uint_le(int p_int, int p_byte_no);
		void append_uint_le(std::vector<byte>& p_bytes, int p_int, int p_byte_count);

		template<class T>
		void rot_sq_matrix_ccw(std::vector<std::vector<T>>& p_input) {

			auto cyclic_roll = [](T& a, T& b, T& c, T& d) {
				T temp = a;
				a = b;
				b = c;
				c = d;
				d = temp;
			};

			int n{ static_cast<int>(p_input.size()) };

			for (int i{ 0 }; i < n / 2; i++)
				for (int j{ 0 }; j < (n + 1) / 2; j++)
					cyclic_roll(p_input[i][j], p_input[n - 1 - j][i], p_input[n - 1 - i][n - 1 - j], p_input[j][n - 1 - i]);
		}

		template <class T>
		T validate(T p_input, T p_min_val, T p_max_val) {
			if (p_input < p_min_val)
				return p_min_val;
			else if (p_input > p_max_val)
				return p_max_val;
			else
				return p_input;
		}

		template <class T>
		std::vector<std::vector<T>> sparsify(std::vector<std::vector<T>> p_v, std::size_t p_step) {
			std::vector<std::vector<T>> result;

			for (std::size_t j{ 0 }; j < p_v.size(); j += p_step) {
				std::vector<T> l_row;
				for (std::size_t i{ 0 }; i < p_v[j].size(); i += p_step)
					l_row.push_back(p_v[j][i]);
				result.push_back(l_row);
			}

			return result;
		}

		template <class T>
		std::vector<T> string_split(const std::string& p_values, char p_delimeter) {
			std::vector<T> result;

			std::size_t l_last = 0;
			std::size_t l_next = p_values.find(p_delimeter);

			while (l_next != std::string::npos) {
				auto l_next_num = p_values.substr(l_last, l_next - l_last);
				result.push_back(static_cast<T>(atoi(l_next_num.c_str())));

				l_last = l_next + 1;
				l_next = p_values.find(p_delimeter, l_last);
			}
			auto l_next_num = p_values.substr(l_last, l_next - l_last);
			if (!l_next_num.empty())
				result.push_back(static_cast<T>(atoi(l_next_num.c_str())));

			return result;
		}

		template <class T>
		std::string string_join(const std::vector<T>& p_values, char p_delimiter) {
			std::string result;

			for (std::size_t i{ 0 }; i < p_values.size(); ++i)
				result += std::to_string(p_values[i]) + p_delimiter;

			result.pop_back();

			return result;
		}

	}

}

#endif

