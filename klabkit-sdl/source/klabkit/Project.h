#ifndef KLABKIT_PROJECT_H
#define KLABKIT_PROJECT_H

#include <string>
#include <tuple>
#include <vector>

#include "Wall.h"
#include "Board.h"

namespace kkit {

	class Project {
		std::string project_folder;
		std::vector<Wall> walls;
		std::vector<Board> maps;
		std::vector<std::tuple<byte, byte, byte>> palette;

		// initializers
		void initialize_palette(void);
		void initialize_walls(void);
		void initialize_maps(void);

		// utility functions
		std::string get_file_path(const std::string& p_subfolder, const std::string& p_file_prefix, const std::string& p_file_ext, int p_frame_no = 0) const;
		std::string get_file_path(const std::string& p_file_prefix, const std::string& p_file_ext, int p_frame_no = 0) const;
		std::string get_dat_file_name(const std::string& p_filename) const;

	public:
		Project(const std::string&);

		// logical getters
		std::vector<byte> get_board_bytes(void) const;

		// getters
		std::string get_bmp_folder(void) const;
		std::string get_bmp_file_path(const std::string& p_file_prefix, int p_frame_no) const;

		// gfx
		const std::vector<std::tuple<byte, byte, byte>>& get_palette(void) const;
		int get_wall_image_count(void) const;
		const kkit::Wall& get_wall(int p_frame_no) const;
		const kkit::Board& get_board(int p_board_no) const;
	};

}

#endif
