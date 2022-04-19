#ifndef KLABKIT_COMPRESSION_H
#define KLABKIT_COMPRESSION_H

#include <vector>

namespace kkit {

	using byte = unsigned char;

	namespace compression {

		// core compression routines
		std::vector<byte> decompress_lzw_block(const std::vector<byte>& p_input);

		// utility functions
		int get_file_offset(const std::vector<int> p_comp_lengths, int p_block_no);

		// concrete implementations based on file type we're handling
		std::vector<byte> decompress_walls_kzp(const std::vector<byte>& p_bytes);
		std::vector<byte> decompress_boards_kzp(const std::vector<byte>& p_bytes);

		std::vector<byte> decompress_file_contents(const std::vector<byte>& p_bytes, int p_block_count, int header_size = 0, int out_header_size = 0);
	}

}

#endif
