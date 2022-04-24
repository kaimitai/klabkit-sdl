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
		std::vector<byte> get_wall_bytes(void) const;
		kkit::Map_tile gen_map_tile(int p_tile_no) const;
		std::string get_block_type_as_string(int p_tile_no) const;
		std::pair<int, int> get_player_start_pos(int p_board_no) const;

		// getters
		std::string get_bmp_folder(void) const;
		std::string get_bmp_file_path(const std::string& p_file_prefix, int p_frame_no) const;
		int get_board_count(void) const;

		// wall attribute getters
		bool is_blast(int p_wall_no) const;
		bool is_inside(int p_wall_no) const;
		bool is_directional(int p_wall_no) const;
		kkit::Wall_type get_wall_type(int p_wall_no) const;

		// gfx
		const std::vector<std::tuple<byte, byte, byte>>& get_palette(void) const;
		int get_wall_image_count(void) const;
		const kkit::Wall& get_wall(int p_frame_no) const;
		const kkit::Board& get_board(int p_board_no) const;

		// manipulators
		// map tiles
		void clear_tile(int p_board_no, int p_x, int p_y);
		void set_tile(int p_board_no, int p_x, int p_y, const kkit::Map_tile& p_tile);
		void toggle_mt_direction(int p_board_no, int p_x, int p_y);
		void toggle_mt_blast(int p_board_no, int p_x, int p_y);
		void toggle_mt_inside(int p_board_no, int p_x, int p_y);
		void set_player_start_position(int p_board_no, int p_x, int p_y, kkit::Player_direction p_direction = kkit::Player_direction::Up);

		// save and load
		int save_boards_kzp(void) const;
	};

}

#endif
