#ifndef KLIB_FILE_H
#define KLIB_FILE_H

#include <string>
#include <vector>

namespace klib {
	namespace file {

		using byte = unsigned char;

		std::vector<byte> read_file_as_bytes(const std::string& p_file_name);

		// bytes
		int uint16_le(byte p_b1, byte p_b2);
	}
}

#endif
