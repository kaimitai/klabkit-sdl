#ifndef KKIT_CLI_UTIL_H
#define KKIT_CLI_UTIL_H

#include "kkit_cli_constants.h"
#include <string>

namespace kkit_cli {

	using byte = unsigned char;

	std::string folder_combine(const std::string& p_folder, const std::string& p_subfolder);
	std::string full_file_path(const std::string& p_folder, const std::string& p_file_ext, const std::string& p_filename, int p_frame_no = -1);
	std::string file_name(const std::string& p_filename, const std::string& p_extension);
	
	// compression procedures
	void compress_boards(const std::string& p_folder, int p_board_count, bool p_v1);
	void compress_lab3d(const std::string& p_folder);
	void compress_songs(const std::string& p_folder);
	void compress_sounds(const std::string& p_folder);
	void compress_story(const std::string& p_folder);
	void compress_walls(const std::string& p_folder, int p_wall_count, bool p_v1);
	void compress_walls_walken(const std::string& p_folder, int p_wall_count);

	// decompression procedures
	void decompress_boards(const std::string& p_folder, int p_board_count, bool p_v1);
	void decompress_lab3d(const std::string& p_folder);
	void decompress_songs(const std::string& p_folder);
	void decompress_sounds(const std::string& p_folder);
	void decompress_story(const std::string& p_folder);
	void decompress_walls(const std::string& p_folder, int p_wall_count, bool p_v1);
	void decompress_walls_walken(const std::string& p_folder, int p_wall_count);
}

#endif
