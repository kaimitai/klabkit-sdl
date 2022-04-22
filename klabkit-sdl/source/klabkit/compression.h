#ifndef KLABKIT_COMPRESSION_H
#define KLABKIT_COMPRESSION_H

#include <array>
#include <string>
#include <utility>
#include <vector>

namespace kkit {

	using byte = unsigned char;

	namespace compression {

		constexpr int LZW_DICT_SIZE{ 4096 };
		constexpr int LZW_UNCOMPRESSED_BLOCK_SIZE{ 4096 };
		constexpr std::array<byte, 6> HEADER_LAB3D{ { 'L', 'A', 'B', '3', 'D', '!'} };
		constexpr std::array<byte, 6> HEADER_GIF{ {'G','I','F','8','7','a'} };

		// core compression routines
		std::vector<byte> decompress_lzw_block(const std::vector<byte>& p_input);
		std::vector<byte> compress_lzw_block(const std::vector<byte>& p_input);

		// utility functions
		int get_file_offset(const std::vector<int> p_comp_lengths, int p_block_no);
		std::vector<std::vector<byte>> replace_lab3d_headers(const std::vector<byte>& p_bytes, const std::vector<byte>& p_from_header, const std::vector<byte>& to_header);

		// concrete implementations based on file type we're handling
		// decompression / extraction / decryption
		std::vector<byte> decompress_walls_kzp(const std::vector<byte>& p_bytes);
		std::vector<byte> decompress_boards_kzp(const std::vector<byte>& p_bytes);
		std::vector<std::vector<byte>> decompress_lab3d_kzp(const std::vector<byte>& p_bytes);
		std::vector<byte> decompress_story_kzp(const std::vector<byte>& p_bytes);
		std::vector<std::pair<std::string, std::vector<byte>>> decompress_songs_kzp(const std::vector<byte>& p_bytes);

		// compression / archiving / encryption
		std::vector<byte> compress_boards_kzp(const std::vector<byte>& p_bytes);
		std::vector<byte> compress_lab3d_kzp(const std::vector<std::vector<byte>>& p_file_bytes);
		std::vector<byte> compress_story_kzp(const std::vector<byte>& p_bytes);
		std::vector<byte> compress_songs_kzp(const std::vector<std::pair<std::string, std::vector<byte>>>& p_file_bytes);

		// lzw helper
		std::vector<byte> decompress_file_contents(const std::vector<byte>& p_bytes, int p_block_count, int header_size = 0, int out_header_size = 0);
	}

}

#endif
