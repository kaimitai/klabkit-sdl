#ifndef KLABKIT_PROJECT_H
#define KLABKIT_PROJECT_H

#include <deque>
#include <string>
#include <tuple>
#include <optional>
#include <utility>
#include <vector>

#include "Wall.h"
#include "Board.h"
#include "Savegame.h"
#include "Hiscore.h"
#include "Project_config.h"

namespace kkit {

	class Project {
		Project_config config;

		std::vector<Wall> walls;
		std::vector<Board> maps;
		std::vector<std::optional<Savegame>> m_saves;
		std::optional<kkit::Hiscore> m_hiscore;
		std::vector<std::tuple<byte, byte, byte>> palette;

		// user messages
		std::deque<std::pair<std::string, int>> m_messages;

		// walls loaded from kzp or dat?
		bool kzp_walls, kzp_boards;

		// initializers
		void initialize_palette(void);
		void initialize_walls(void);
		void initialize_maps(void);

		// walken-specific initializers
		void initialize_walls_walken(void);
		void initialize_maps_walken(void);

		// walken-specific save
		int save_boards_dat_walken() const;
		int save_walls_kzp_walken(bool p_compress = true) const;

		// utility functions
		std::string get_file_path(const std::string& p_subfolder, const std::string& p_file_prefix, const std::string& p_file_ext, int p_frame_no = 0) const;
		std::string get_file_path(const std::string& p_file_prefix, const std::string& p_file_ext, int p_frame_no = 0) const;
		std::string get_dat_file_name(const std::string& p_filename) const;

	public:
		Project(const kkit::Project_config& p_config);
		void add_message(const std::string& p_message, int p_status_code = 0);

		// logical getters
		std::vector<byte> get_board_bytes(void) const;
		std::vector<byte> get_wall_bytes(void) const;
		kkit::Map_tile gen_map_tile(int p_tile_no) const;
		std::string get_block_type_as_string(int p_tile_no) const;
		std::pair<int, int> get_player_start_pos(int p_board_no) const;
		bool is_clip_override(int p_tile_no) const;
		bool is_klab_v_1(void) const;
		bool is_walken(void) const;
		bool is_walls_kzp(void) const;
		bool is_boards_kzp(void) const;

		std::string get_file_directory(const std::string& p_extension, int p_frame_no = -1) const;
		std::string get_file_name(const std::string& p_filename, const std::string& p_extension, int p_frame_no = -1) const;
		std::string get_file_full_path(const std::string& p_filename, const std::string& p_extension, int p_frame_no = -1) const;
		std::string get_savegame_filename(std::size_t p_slot_no);

		// getters
		std::string get_bmp_folder(void) const;
		std::string get_bmp_file_path(const std::string& p_file_prefix, int p_frame_no) const;
		int get_board_count(void) const;
		std::string get_config_label(void) const;
		std::tuple<byte, byte, byte> get_floor_color(void) const;
		const Project_config& get_config(void) const;
		const std::deque<std::pair<std::string, int>>& get_messages(void) const;
		const std::string& get_savegame_player_name(std::size_t p_slot) const;
		unsigned int get_savegame_board_num(std::size_t p_slot) const;

		// wall attribute getters
		bool is_blast(int p_wall_no) const;
		bool is_inside(int p_wall_no) const;
		bool is_directional(int p_wall_no) const;
		kkit::Wall_type get_wall_type(int p_wall_no) const;
		const std::vector<std::vector<byte>> get_image_as_2dv(int p_wall_no) const;

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
		void flip_vertical(int p_board_no, int p_x, int p_y, int p_w, int p_h);
		void flip_horizontal(int p_board_no, int p_x, int p_y, int p_w, int p_h);

		// entire map
		void reload_map_from_xml(int p_board_no);
		// entire wall
		void reload_wall_from_xml(int p_wall_no);

		// wall tile metadata
		void toggle_wt_type(int p_wall_no);
		void toggle_wt_blast(int p_wall_no);
		void toggle_wt_inside(int p_wall_no);
		void set_wall_image(int p_wall_no, const std::vector<std::vector<byte>>& p_bytes);

		// savegames
		bool has_savegame(std::size_t p_slot) const;
		const kkit::Savegame& get_savegame(std::size_t p_slot) const;
		void load_saveboard(std::size_t p_board_slot, std::size_t p_save_slot);
		void export_board_to_save(std::size_t p_board_slot, std::size_t p_save_slot);
		void load_savefile_dat(std::size_t p_save_slot);
		void save_savefile_dat(std::size_t p_save_slot);
		void load_savefile_xml(std::size_t p_save_slot);
		void save_savefile_xml(std::size_t p_save_slot);

		// hiscore
		bool has_hiscore(void) const;
		const kkit::Hiscore& get_hiscore(void) const;
		void load_hiscore_dat(void);
		void save_hiscore_dat(void);
		void load_hiscore_xml(void);
		void save_hiscore_xml(void);

		// save and load
		int save_boards_kzp(bool p_compress = true) const;
		int save_walls_kzp(bool p_compress = true) const;
		void save_wall_xml(int p_wall_no) const;
		void save_board_xml(int p_board_no) const;
	};

}

#endif
