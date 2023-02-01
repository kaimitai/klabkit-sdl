#ifndef KKIT_BOARD_UI_H
#define KKIT_BOARD_UI_H

#include <SDL.h>
#include <cstddef>
#include <utility>
#include "./Project.h"
#include "./Project_config.h"
#include "./Project_gfx.h"
#include "./../klib/User_input.h"
#include "./../klib/Timer.h"

namespace kkit {

	class Board_ui {
		// full board texture
		SDL_Texture* m_texture;

		// internal variables for level board position
		int m_board_ind, m_cam_x, m_cam_y;
		float m_cam_zoom;

		// internal variables for selected tile position
		int m_sel_tile_x{ 0 }, m_sel_tile_y{ 0 };
		int m_sel_tile_2_x{ -1 }, m_sel_tile_2_y{ 0 };
		std::vector<std::vector<kkit::Map_tile>> m_clipboard;
		std::vector<bool> m_toggles;

		// gameboard dragging variables
		bool m_mouse_drag_active;
		std::pair<int, int> m_mouse_drag_pos;

		// timers
		std::vector<klib::Timer> m_timers;

		// project data getters
		kkit::Map_tile get_selected_tile(const kkit::Project& p_project, int p_tile_no) const;

		// linear algebra
		std::pair<int, int> get_map_pixel_pos_from_mouse_coords(int p_mx, int p_my) const;
		std::pair<int, int> get_screen_coords_from_map_pos(int p_mx, int p_my) const;
		std::pair<int, int> get_tile_pos_from_mouse_coords(int p_mx, int p_my) const;
		static bool is_valid_tile_pos(const std::pair<int, int>& p_pos);
		void center_offset(int p_w, int p_h);
		void center_offset(std::pair<int, int> p_coords, int p_w, int p_h);

		// drawing routines
		void generate_board_texture(SDL_Renderer* p_rnd, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx) const;
		void draw_ui(SDL_Renderer* p_rnd,
			const klib::User_input& p_input, kkit::Project& p_project,
			kkit::Project_gfx& p_gfx, int p_w, int p_h);
		void draw_ui_main(SDL_Renderer* p_rnd,
			const klib::User_input& p_input, kkit::Project& p_project,
			kkit::Project_gfx& p_gfx, int p_w, int p_h);
		void draw_ui_minimap(SDL_Renderer* p_rnd,
			const klib::User_input& p_input, kkit::Project& p_project,
			kkit::Project_gfx& p_gfx);
		void draw_ui_selected_board_tile(SDL_Renderer* p_rnd,
			const klib::User_input& p_input, kkit::Project& p_project,
			kkit::Project_gfx& p_gfx);

		// file UI
		void xml_export(kkit::Project& p_project, int p_board_no) const;
		bool xml_import(kkit::Project& p_project, int p_board_no) const;
		void save_boards_kzp(kkit::Project& p_project, bool p_compress) const;

		// camera routines
		int get_cam_x_max(int p_w) const;
		int get_cam_y_max(int p_h) const;
		void add_cam_x(int p_dx, int p_h);
		void add_cam_y(int p_dy, int p_w);
		void add_cam_zoom(float p_dz);
		void set_cam_x(int p_x, int p_w);
		void set_cam_y(int p_y, int p_h);
		void zoom_camera(int p_sx, int p_sy, float p_dz, int p_w, int p_h);

		// selection operations, copy, paste, cut, flip, rotate etc
		bool is_empty_selection(const kkit::Project& p_project) const;
		std::tuple<int, int, int, int> get_selection_rectangle(void) const;
		void copy_to_clipboard(const kkit::Project& p_project, bool p_clear_secondary = false);
		void cut_selection(kkit::Project& p_project);
		void paste_from_clipboard(kkit::Project& p_project);
		bool selection_fits(void) const;
		void show_selection_rectangle(void);
		void clear_selection(kkit::Project& p_project);
		void rotate_selection(const kkit::Project& p_project, bool p_clockwise = false);
		void clear_secondary_selection(void);
		int get_selected_board_tile_no(const kkit::Project& p_project) const;

		// logic
		int count_tiles(const kkit::Project& p_project, int p_tile_no, bool p_all_boards = false) const;
		void next_tile(const kkit::Project& p_project, bool p_tp_tile, int p_w, int p_h, bool p_wrap = false);
		void prev_tile(const kkit::Project& p_project, bool p_tp_tile, int p_w, int p_h, bool p_wrap = false);

	public:
		Board_ui(SDL_Renderer* p_rnd, const Project_config& p_config);
		void draw(SDL_Renderer* p_rnd,
			const klib::User_input& p_input, kkit::Project& p_project,
			kkit::Project_gfx& p_gfx, int p_w, int p_h);
		void move(SDL_Renderer* p_rnd, const klib::User_input& p_input, int p_delta_ms,
			kkit::Project& p_project, kkit::Project_gfx& p_gfx, int p_w, int p_h);
	};

}

#endif
