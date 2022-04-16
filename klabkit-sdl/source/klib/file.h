#ifndef KLIB_FILE_H
#define KLIB_FILE_H

#include <string>
#include <vector>

namespace klib {
	namespace file {

		using byte = unsigned char;

		std::vector<byte> read_file_as_bytes(const std::string& p_file_name);
		void create_directory(const std::string& p_path);
	}
}

#endif
