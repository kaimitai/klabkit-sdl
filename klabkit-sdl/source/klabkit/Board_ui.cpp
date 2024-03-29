#include <algorithm>
#include <cmath>
#include <filesystem>

#include "./Board_ui.h"
#include "./../imgui/imgui.h"
#include "./../imgui/imgui_impl_sdl.h"
#include "./../imgui/imgui_impl_sdlrenderer.h"
#include "imgui_helper.h"
#include "./../klib/gfx.h"
#include "./../klib/klib_util.h"
#include "./kkit_gfx.h"

constexpr float ZOOM_MAX{ 3.0f };
constexpr float ZOOM_MIN{ 0.125f };

kkit::Board_ui::Board_ui(SDL_Renderer* p_rnd, const Project_config& p_config) :
	m_texture{ SDL_CreateTexture(p_rnd, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, c::WALL_IMG_W * c::MAP_W, c::WALL_IMG_H * c::MAP_H) },
	m_minimap_texture{ SDL_CreateTexture(p_rnd, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, c::MAP_W, c::MAP_H) },
	m_board_ind{ 0 }, m_cam_x{ 0 }, m_cam_y{ 0 }, m_cam_zoom{ 1.0f },
	m_mouse_drag_active{ false }, m_mouse_drag_pos{ std::make_pair(0,0) },
	m_toggles{ std::vector<bool>(5, false) },
	m_sel_tp_tile_no{ -1 },
	m_sel_save_file{ 0 }, m_sel_hiscore{ 0 },
	m_show_meta_editor{ false }, m_show_save_editor{ false }, m_show_hiscore_editor{ false }
{
	// tile flash timer
	m_timers.push_back(klib::Timer(70, 10, true));
	// pulsating color timer
	m_timers.push_back(klib::Timer(255, 1, true));

	// directions should be on by default
	m_toggles[0] = true;
}

void kkit::Board_ui::draw(SDL_Renderer* p_rnd,
	const klib::User_input& p_input, kkit::Project& p_project,
	kkit::Project_gfx& p_gfx, int p_w, int p_h) {
	generate_board_texture(p_rnd, p_project, p_gfx);

	int l_s_pixels_w{ static_cast<int>(p_w * m_cam_zoom) };
	int l_s_pixels_h{ static_cast<int>(p_h * m_cam_zoom) };

	/* minimap stuff */
	SDL_SetRenderTarget(p_rnd, m_minimap_texture);
	SDL_Color l_floor_col = p_gfx.get_floor_color();
	SDL_SetRenderDrawColor(p_rnd, l_floor_col.r, l_floor_col.g, l_floor_col.b, 0);
	SDL_RenderClear(p_rnd);
	klib::gfx::blit(p_rnd, p_gfx.get_texture(c::INDEX_MM_TEXTURES, m_board_ind), 0, 0);

	klib::gfx::draw_rect(p_rnd, m_cam_x / c::MAP_W, m_cam_y / c::MAP_H, l_s_pixels_w / c::MAP_W, l_s_pixels_h / c::MAP_H, c::COL_YELLOW, 1);
	/* minimap stuff end */

	SDL_SetRenderTarget(p_rnd, nullptr);

	SDL_SetRenderDrawColor(p_rnd, 126, 126, 255, 0);
	SDL_RenderClear(p_rnd);

	int l_rest_w = std::max(0, c::MAP_GFX_W - m_cam_x);
	int l_rest_h = std::max(0, c::MAP_GFX_H - m_cam_y);

	l_rest_w = std::min(l_rest_w, l_s_pixels_w);
	l_rest_h = std::min(l_rest_h, l_s_pixels_h);

	float ratio_w = static_cast<float>(l_rest_w) / static_cast<float>(l_s_pixels_w);
	float ratio_h = static_cast<float>(l_rest_h) / static_cast<float>(l_s_pixels_h);
	int l_ts_w = static_cast<int>(ratio_w * p_w);
	int l_ts_h = static_cast<int>(ratio_h * p_h);

	klib::gfx::blit_full_spec(p_rnd,
		m_texture, 0, 0, l_ts_w, l_ts_h,
		m_cam_x, m_cam_y, l_rest_w, l_rest_h);

	draw_ui(p_rnd, p_input, p_project, p_gfx, p_w, p_h);
}

SDL_Texture* kkit::Board_ui::get_tp_tile_texture(const kkit::Project_gfx& p_gfx, int p_index) const {
	if (p_index == -2)
		return  p_gfx.get_texture(c::INDEX_APP_TEXTURES, 2);
	else if (p_index >= 0)
		return p_gfx.get_texture(c::INDEX_WALL_TEXTURES, p_index);
	else
		return nullptr;
}

void kkit::Board_ui::generate_board_texture(SDL_Renderer* p_rnd,
	const kkit::Project& p_project, const kkit::Project_gfx& p_gfx) const {

	int l_sel_board_tile_no{ get_selected_board_tile_no(p_project) };
	const auto& board = p_project.get_board(this->m_board_ind);
	float l_shrink_factor = 0.25f + static_cast<float>(m_timers[0].get_frame()) / 100.0f;

	SDL_SetRenderTarget(p_rnd, m_texture);
	SDL_Color l_floor_col = p_gfx.get_floor_color();
	SDL_SetRenderDrawColor(p_rnd, l_floor_col.r, l_floor_col.g, l_floor_col.b, 0);
	SDL_RenderClear(p_rnd);

	const auto& lr_tile_overlays{ p_project.get_config().get_tile_overlays() };

	for (int i{ 0 }; i < c::MAP_W; ++i)
		for (int j{ 0 }; j < c::MAP_H; ++j) {
			int l_tile_no = board.get_tile_no(i, j);

			bool l_flash = m_toggles[1] && board.is_blast(i, j);

			if (l_tile_no >= 0)
				l_flash |= m_toggles[2] && board.is_inside(i, j) && (!p_project.is_inside(l_tile_no) || p_project.is_clip_override(l_tile_no));
			else
				l_flash |= m_toggles[2] && !board.is_inside(i, j);

			l_flash |= m_toggles[3] && (l_tile_no == m_sel_tp_tile_no);

			if (l_tile_no >= 0) {
				klib::gfx::blit(p_rnd, p_gfx.get_texture(c::INDEX_WALL_TEXTURES, l_tile_no), c::WALL_IMG_W * i, c::WALL_IMG_H * j);
				auto iter{ lr_tile_overlays.find(l_tile_no) };
				if (iter != end(lr_tile_overlays))
					klib::gfx::blit(p_rnd, p_gfx.get_texture(c::INDEX_WALL_TEXTURES, iter->second), c::WALL_IMG_W * i, c::WALL_IMG_H * j);
			}

			if (m_toggles[0] && p_project.is_directional(l_tile_no))
				klib::gfx::blit_factor(p_rnd, p_gfx.get_texture(c::INDEX_APP_TEXTURES, board.is_vertical(i, j) ? 1 : 0), c::WALL_IMG_W * i + 32, c::WALL_IMG_H * j + 32, l_shrink_factor);
			if (l_flash)
				klib::gfx::blit_factor(p_rnd, p_gfx.get_texture(c::INDEX_APP_TEXTURES, 6), c::WALL_IMG_W * i + 32, c::WALL_IMG_H * j + 32, l_shrink_factor);
		}

	// draw player start
	int l_px = c::WALL_IMG_W * board.get_player_start_x() + 32;
	int l_py = c::WALL_IMG_H * board.get_player_start_y() + 32;
	kkit::Player_direction l_pdir = board.get_player_start_direction();
	int l_pstart_sprite_no{ 2 };
	if (l_pdir == kkit::Player_direction::Left)
		l_pstart_sprite_no = 3;
	else if (l_pdir == kkit::Player_direction::Down)
		l_pstart_sprite_no = 4;
	else if (l_pdir == kkit::Player_direction::Right)
		l_pstart_sprite_no = 5;

	klib::gfx::blit_factor(p_rnd, p_gfx.get_texture(c::INDEX_APP_TEXTURES, l_pstart_sprite_no), l_px, l_py, l_shrink_factor);

	// draw gridlines
	if (m_toggles[4]) {
		auto l_pulse_color{ klib::gfx::pulse_color(c::COL_WHITE, c::COL_GOLD, m_timers[1].get_frame()) };

		SDL_SetRenderDrawColor(p_rnd, l_pulse_color.r,
			l_pulse_color.g, l_pulse_color.b, 0);

		for (int i{ 1 }; i < c::MAP_W; ++i)
			SDL_RenderDrawLine(p_rnd, i * c::WALL_IMG_W, 0, i * c::WALL_IMG_W, c::MAP_H * c::WALL_IMG_H);
		for (int i{ 1 }; i < c::MAP_H; ++i)
			SDL_RenderDrawLine(p_rnd, 0, i * c::WALL_IMG_H, c::MAP_W * c::WALL_IMG_W, i * c::WALL_IMG_H);
	}

	// draw selected tile
	klib::gfx::draw_rect(p_rnd, m_sel_tile_x * c::WALL_IMG_W, m_sel_tile_y * c::WALL_IMG_H,
		c::WALL_IMG_W, c::WALL_IMG_H,
		kkit::gfx::get_pulse_color(0, m_timers[1].get_frame()),
		4);

	// draw full selection
	if (m_sel_tile_2_x >= 0)
		klib::gfx::draw_rect(p_rnd,
			std::min(m_sel_tile_x, m_sel_tile_2_x) * c::WALL_IMG_W,
			std::min(m_sel_tile_y, m_sel_tile_2_y) * c::WALL_IMG_H,
			c::WALL_IMG_W * (1 + abs(m_sel_tile_x - m_sel_tile_2_x)),
			c::WALL_IMG_H * (1 + abs(m_sel_tile_y - m_sel_tile_2_y)),
			klib::gc::COL_ORANGE, 4);

	SDL_SetRenderTarget(p_rnd, nullptr);
}

void kkit::Board_ui::move(SDL_Renderer* p_rnd, const klib::User_input& p_input, int p_delta_ms, kkit::Project& p_project, kkit::Project_gfx& p_gfx, int p_w, int p_h) {
	bool l_shift{ p_input.is_shift_pressed() };
	bool l_ctrl{ p_input.is_ctrl_pressed() };
	int l_mx{ p_input.mx() };
	int l_my{ p_input.my() };

	for (auto& l_timer : m_timers)
		l_timer.move(p_delta_ms);

	if (!ImGui::GetIO().WantCaptureMouse) {

		if (!l_ctrl || !p_input.mouse_held())
			m_mouse_drag_active = false;

		if (l_ctrl && p_input.mouse_held()) {
			if (m_mouse_drag_active) {
				add_cam_x(m_mouse_drag_pos.first - l_mx, p_w);
				add_cam_y(m_mouse_drag_pos.second - l_my, p_h);
			}
			else {
				m_mouse_drag_active = true;
			}
			m_mouse_drag_pos = std::make_pair(l_mx, l_my);
		}
		else if (p_input.mouse_held()) {
			auto l_tile = get_tile_pos_from_mouse_coords(l_mx, l_my);
			if (is_valid_tile_pos(l_tile)) {
				if (l_shift) {
					m_sel_tile_2_x = l_tile.first;
					m_sel_tile_2_y = l_tile.second;
				}
				else {
					m_sel_tile_x = l_tile.first;
					m_sel_tile_y = l_tile.second;
					m_sel_tile_2_x = -1;
				}
			}
		}
		else if (p_input.mw_down()) {
			if (l_ctrl) {
				if (m_cam_zoom < ZOOM_MAX)
					zoom_camera(l_mx, l_my, 0.1f, p_w, p_h);
			}
			else if (l_shift)
				add_cam_x(-c::WALL_IMG_W, p_w);
			else
				add_cam_y(c::WALL_IMG_H, p_h);
		}
		else if (p_input.mw_up()) {
			if (l_ctrl) {
				if (m_cam_zoom > ZOOM_MIN)
					zoom_camera(l_mx, l_my, -0.1f, p_w, p_h);
			}
			else if (l_shift)
				add_cam_x(c::WALL_IMG_W, p_w);
			else
				add_cam_y(-c::WALL_IMG_H, p_h);
		}
		// right click on board grid: paint (set board grid tile to selected tile picker tile)
		else if (p_input.mouse_held(false)) {
			auto l_tcoords = get_tile_pos_from_mouse_coords(l_mx, l_my);
			if (is_valid_tile_pos(l_tcoords)) {

				if (l_ctrl) {
					m_sel_tp_tile_no = p_project.get_board(m_board_ind).get_tile_no(l_tcoords.first, l_tcoords.second);
				}
				else {

					int t_tile_no = p_project.get_board(m_board_ind).get_tile_no(l_tcoords.first, l_tcoords.second);

					if (m_sel_tp_tile_no == -2)
						p_project.set_player_start_position(m_board_ind, l_tcoords.first, l_tcoords.second);
					else if (m_sel_tp_tile_no != t_tile_no) {
						p_project.set_tile(m_board_ind, l_tcoords.first, l_tcoords.second, this->get_selected_tile(p_project, m_sel_tp_tile_no));
						this->board_changed(p_rnd, p_project, p_gfx);
					}
				}
			}
		}
	}

	if (!ImGui::GetIO().WantCaptureKeyboard) {
		// shift + home: to top of tile picker
		if (l_shift && p_input.is_pressed(SDL_SCANCODE_HOME)) {
			//tile_row = 0;
		}
		// home: go to first board
		else if (p_input.is_pressed(SDL_SCANCODE_HOME))
			m_board_ind = 0;
		// shift + end: to bottom of tile picker
		else if (l_shift && p_input.is_pressed(SDL_SCANCODE_END)) {
			//tile_row = c_tile_row_max(p_project);
		}
		// end: go to last board
		else if (p_input.is_pressed(SDL_SCANCODE_END))
			m_board_ind = p_project.get_board_count() - 1;
		// shift + PgDown: tile picker next page
		else if (l_shift && p_input.is_pressed(SDL_SCANCODE_PAGEDOWN)) {
			//tile_row = klib::util::validate(tile_row + BW_TPC, 0, c_tile_row_max(p_project));
		}
		// shift + PgUp: tile picker previous page
		else if (l_shift && p_input.is_pressed(SDL_SCANCODE_PAGEUP)) {
			//tile_row = klib::util::validate(tile_row - BW_TPC, 0, c_tile_row_max(p_project));
		}
		else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_C))
			copy_to_clipboard(p_project);
		else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_V)) {
			paste_from_clipboard(p_project);
			board_changed(p_rnd, p_project, p_gfx);
		}
		// ctrl+A: select all
		else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_A)) {
			this->m_sel_tile_x = 0;
			this->m_sel_tile_y = 0;
			this->m_sel_tile_2_x = c::MAP_W - 1;
			this->m_sel_tile_2_y = c::MAP_H - 1;
		}
		// shift+V: show where clipboard WOULD BE pasted
		else if (l_shift && p_input.is_pressed(SDL_SCANCODE_V))
			show_selection_rectangle();
		// delete: set all tiles in selection to 0 (internal -1)
		else if (p_input.is_pressed(SDL_SCANCODE_DELETE)) {
			this->clear_selection(p_project);
			this->board_changed(p_rnd, p_project, p_gfx);
		}
		// shift+Minus / PgDown: previous board
		else if ((l_shift && p_input.is_pressed(SDL_SCANCODE_KP_MINUS)) || p_input.is_pressed(SDL_SCANCODE_PAGEDOWN))
			m_board_ind = klib::util::validate(m_board_ind - 1, 0, p_project.get_board_count() - 1);
		// shift+Plus / PgUp: next board
		else if ((l_shift && p_input.is_pressed(SDL_SCANCODE_KP_PLUS)) || p_input.is_pressed(SDL_SCANCODE_PAGEUP))
			m_board_ind = klib::util::validate(m_board_ind + 1, 0, p_project.get_board_count() - 1);
		// ctrl+F: find selected board tile
		else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_F))
			this->center_offset(std::make_pair(m_sel_tile_x, m_sel_tile_y), p_w, p_h);
		// ctrl+P: go to previous tile of selected type on board (if shift held, previous tile of tile picker's type)
		else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_P))
			this->prev_tile(p_project, l_shift, p_w, p_h);
		// ctrl+H: center on player start position
		else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_H))
			this->center_offset(p_project.get_player_start_pos(m_board_ind), p_w, p_h);
		// shift+H: set selected board tile to player start position
		else if (l_shift && p_input.is_pressed(SDL_SCANCODE_H))
			p_project.set_player_start_position(m_board_ind, m_sel_tile_x, m_sel_tile_y);
		else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_X)) {
			cut_selection(p_project);
			board_changed(p_rnd, p_project, p_gfx);
		}
		// esc: clear selection rectangle
		else if (p_input.is_pressed(SDL_SCANCODE_ESCAPE))
			this->clear_secondary_selection();
		// ctrl+S: save to kzp (if shift held too; save to DAT)
		else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_S))
			this->save_boards_kzp(p_project, !l_shift);
		// ctrl+N: next tile of selected type (if shift held, next tile of tile picker's type - if any)
		else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_N))
			this->next_tile(p_project, l_shift, p_w, p_h);
		// B: toggle selected board tile desctructible property ([B]last)
		else if (p_input.is_pressed(SDL_SCANCODE_B))
			p_project.toggle_mt_blast(m_board_ind, m_sel_tile_x, m_sel_tile_y);
		// C: toggle selected board tile clip property ([C]lip)
		else if (p_input.is_pressed(SDL_SCANCODE_C))
			p_project.toggle_mt_inside(m_board_ind, m_sel_tile_x, m_sel_tile_y);
		// D: toggle selected board tile direction (horizontal/vertical), or if player start: n/e/s/w
		else if (p_input.is_pressed(SDL_SCANCODE_D))
			p_project.toggle_mt_direction(m_board_ind, m_sel_tile_x, m_sel_tile_y);
		// F: flip selection: horizontal flip, shift+F: vertical flip
		else if (p_input.is_pressed(SDL_SCANCODE_F)) {
			auto l_rect = this->get_selection_rectangle();
			if (l_shift)
				p_project.flip_vertical(m_board_ind, std::get<0>(l_rect), std::get<1>(l_rect), std::get<2>(l_rect), std::get<3>(l_rect));
			else
				p_project.flip_horizontal(m_board_ind, std::get<0>(l_rect), std::get<1>(l_rect), std::get<2>(l_rect), std::get<3>(l_rect));

			board_changed(p_rnd, p_project, p_gfx);
		}
		// R: rotate clipboard counter-clockwise, if shift held: rotate clockwise
		else if (p_input.is_pressed(SDL_SCANCODE_R))
			this->rotate_selection(p_project, l_shift);
		// ctrl+Plus/Minus: Zoom in/out (centered)
		else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_KP_MINUS)) {
			if (m_cam_zoom < ZOOM_MAX)
				zoom_camera(p_w / 2, p_h / 2, 0.1f, p_w, p_h);
		}
		else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_KP_PLUS)) {
			if (m_cam_zoom > ZOOM_MIN)
				zoom_camera(p_w / 2, p_h / 2, -0.1f, p_w, p_h);
		}
		// keyboard arrows - navigate
		else if (p_input.is_pressed(SDL_SCANCODE_UP))
			add_cam_y(-c::WALL_IMG_H * (l_ctrl ? 4 : 1), p_h);
		else if (p_input.is_pressed(SDL_SCANCODE_DOWN))
			add_cam_y(c::WALL_IMG_H * (l_ctrl ? 4 : 1), p_h);
		else if (p_input.is_pressed(SDL_SCANCODE_LEFT))
			add_cam_x(-c::WALL_IMG_W * (l_ctrl ? 4 : 1), p_w);
		else if (p_input.is_pressed(SDL_SCANCODE_RIGHT))
			add_cam_x(c::WALL_IMG_W * (l_ctrl ? 4 : 1), p_w);
	}
}

kkit::Map_tile kkit::Board_ui::get_selected_tile(const kkit::Project& p_project, int p_tile_no) const {
	return p_project.gen_map_tile(p_tile_no);
}

void kkit::Board_ui::set_cam_x(int p_x, int p_w) {
	int l_max_cam{ get_cam_x_max(p_w) };

	if (p_x >= m_cam_x && m_cam_x >= l_max_cam)
		return;
	else
		m_cam_x = p_x;

	m_cam_x = std::max(0, m_cam_x);
}

void kkit::Board_ui::set_cam_y(int p_y, int p_h) {
	int l_max_cam{ get_cam_y_max(p_h) };

	if (p_y >= m_cam_y && m_cam_y >= l_max_cam)
		return;
	else
		m_cam_y = p_y;

	m_cam_y = std::max(0, m_cam_y);
}

void kkit::Board_ui::add_cam_x(int p_dx, int p_w) {
	set_cam_x(m_cam_x + p_dx, p_w);
}

void kkit::Board_ui::add_cam_y(int p_dy, int p_h) {
	set_cam_y(m_cam_y + p_dy, p_h);
}

void kkit::Board_ui::add_cam_zoom(float p_dz) {
	m_cam_zoom = std::min(ZOOM_MAX, m_cam_zoom + p_dz);
	m_cam_zoom = std::max(ZOOM_MIN, m_cam_zoom);
}

int kkit::Board_ui::get_cam_x_max(int p_w) const {
	int l_pixel_w{ static_cast<int>(m_cam_zoom * p_w) };
	return std::max(c::MAP_GFX_W - l_pixel_w, 0);
}

int kkit::Board_ui::get_cam_y_max(int p_h) const {
	int l_pixel_h{ static_cast<int>(m_cam_zoom * p_h) };
	return std::max(c::MAP_GFX_H - l_pixel_h, 0);
}

std::pair<int, int> kkit::Board_ui::get_tile_pos_from_mouse_coords(int p_mx, int p_my) const {
	int l_mx = m_cam_x + static_cast<int>(m_cam_zoom * p_mx);
	int l_my = m_cam_y + static_cast<int>(m_cam_zoom * p_my);

	return std::make_pair(l_mx / c::WALL_IMG_W, l_my / c::WALL_IMG_H);
}

std::pair<int, int> kkit::Board_ui::get_map_pixel_pos_from_mouse_coords(int p_mx, int p_my) const {
	int l_mx = m_cam_x + static_cast<int>(m_cam_zoom * p_mx);
	int l_my = m_cam_y + static_cast<int>(m_cam_zoom * p_my);

	return std::make_pair(l_mx, l_my);
}

std::pair<int, int> kkit::Board_ui::get_screen_coords_from_map_pos(int p_bx, int p_by) const {
	int l_sx = static_cast<int>(p_bx / m_cam_zoom);
	int l_sy = static_cast<int>(p_by / m_cam_zoom);
	return std::make_pair(l_sx, l_sy);
}

bool kkit::Board_ui::is_valid_tile_pos(const std::pair<int, int>& p_pos) {
	return p_pos.first >= 0 &&
		p_pos.second >= 0 &&
		p_pos.first < c::MAP_W
		&& p_pos.second < c::MAP_H;
}

void kkit::Board_ui::center_offset(int p_w, int p_h) {
	int l_bpw = static_cast<int>(p_w * m_cam_zoom);
	int l_bph = static_cast<int>(p_h * m_cam_zoom);

	set_cam_x(m_cam_x - l_bpw / 2, p_w);
	set_cam_y(m_cam_y - l_bph / 2, p_h);
}

void kkit::Board_ui::center_offset(std::pair<int, int> p_coords, int p_w, int p_h) {
	m_cam_x = c::WALL_IMG_W * p_coords.first;
	m_cam_y = c::WALL_IMG_W * p_coords.second;
	this->center_offset(p_w, p_h);
}

// window position (p_sx, p_sy) should point to the same pixel on the board grid
// after adding p_dz to the zoom factor
void kkit::Board_ui::zoom_camera(int p_sx, int p_sy, float p_dz, int p_w, int p_h) {
	auto l_tg = get_map_pixel_pos_from_mouse_coords(p_sx, p_sy);

	add_cam_zoom(p_dz);

	float l_fac = 1.0f * m_cam_zoom;
	float l_tx = l_fac * p_sx;
	float l_ty = l_fac * p_sy;

	//m_cam_x = l_tg.first - l_tx;
	//m_cam_y = l_tg.second - l_ty;
	set_cam_x(static_cast<int>(l_tg.first - l_tx), p_w);
	set_cam_y(static_cast<int>(l_tg.second - l_ty), p_h);
}

/***********************
* Selection operations *
************************/

bool kkit::Board_ui::is_empty_selection(const kkit::Project& p_project) const {
	auto l_rect = this->get_selection_rectangle();
	const auto& l_brd = p_project.get_board(m_board_ind);

	int l_x = std::get<0>(l_rect);
	int l_y = std::get<1>(l_rect);
	int l_w = std::get<2>(l_rect);
	int l_h = std::get<3>(l_rect);

	for (int x{ l_x }; x < l_x + l_w; ++x)
		for (int y{ l_y }; y < l_y + l_h; ++y)
			if (l_brd.get_tile_no(x, y) != -1)
				return false;

	return true;
}

std::tuple<int, int, int, int> kkit::Board_ui::get_selection_rectangle(void) const {
	// we only have one tile selected
	if (m_sel_tile_2_x < 0)
		return std::make_tuple(m_sel_tile_x, m_sel_tile_y, 1, 1);
	// we have a rectangular selection
	else {
		int l_x = std::min(m_sel_tile_x, m_sel_tile_2_x);
		int l_y = std::min(m_sel_tile_y, m_sel_tile_2_y);
		int l_w = abs(m_sel_tile_x - m_sel_tile_2_x) + 1;
		int l_h = abs(m_sel_tile_y - m_sel_tile_2_y) + 1;
		return std::make_tuple(l_x, l_y, l_w, l_h);
	}
}

void kkit::Board_ui::copy_to_clipboard(const kkit::Project& p_project, bool p_clear_secondary) {
	auto l_rect = this->get_selection_rectangle();

	this->m_clipboard = p_project.get_board(m_board_ind).get_rectangle(std::get<0>(l_rect), std::get<1>(l_rect), std::get<2>(l_rect), std::get<3>(l_rect));
	if (p_clear_secondary)
		this->clear_secondary_selection();
}

void kkit::Board_ui::cut_selection(kkit::Project& p_project) {
	if (!this->is_empty_selection(p_project)) {
		this->copy_to_clipboard(p_project, false);
		this->clear_selection(p_project);
		this->clear_secondary_selection();
	}
}

void kkit::Board_ui::paste_from_clipboard(kkit::Project& p_project) {
	if (this->selection_fits()) {
		for (int j{ 0 }; j < m_clipboard.size() && (m_sel_tile_x + j < c::MAP_W); ++j)
			for (int i{ 0 }; i < m_clipboard[j].size() && (m_sel_tile_y + i < c::MAP_H); ++i) {
				p_project.set_tile(m_board_ind, m_sel_tile_x + j, m_sel_tile_y + i, m_clipboard[j][i]);
			}

		this->show_selection_rectangle();
	}
}

bool kkit::Board_ui::selection_fits(void) const {
	return (m_sel_tile_x + m_clipboard.size() <= c::MAP_W) && (m_clipboard.size() == 0 || (m_sel_tile_y + m_clipboard[0].size() <= c::MAP_H));
}

void kkit::Board_ui::show_selection_rectangle(void) {
	if (this->selection_fits() && m_clipboard.size() > 0) {
		m_sel_tile_2_x = m_sel_tile_x + static_cast<int>(m_clipboard.size()) - 1;
		m_sel_tile_2_y = m_sel_tile_y + static_cast<int>(m_clipboard[0].size()) - 1;
	}
}

void kkit::Board_ui::clear_selection(kkit::Project& p_project) {
	auto l_rect = this->get_selection_rectangle();
	for (int i{ std::get<0>(l_rect) }; i < std::get<0>(l_rect) + std::get<2>(l_rect); ++i)
		for (int j{ std::get<1>(l_rect) }; j < std::get<1>(l_rect) + std::get<3>(l_rect); ++j)
			p_project.clear_tile(m_board_ind, i, j);
}

void kkit::Board_ui::clear_secondary_selection(void) {
	this->m_sel_tile_2_x = -1;
}

void kkit::Board_ui::rotate_selection(kkit::Project& p_project, bool p_clockwise) {
	if (this->m_clipboard.empty())
		return;

	std::vector<std::vector<kkit::Map_tile>> result;

	if (p_clockwise) {
		for (int j{ static_cast<int>(m_clipboard[0].size()) - 1 }; j >= 0; --j) {
			std::vector<kkit::Map_tile> l_row;
			for (int i{ 0 }; i < m_clipboard.size(); ++i) {
				l_row.push_back(m_clipboard[i][j]);
			}
			result.push_back(l_row);
		}
	}
	else {
		for (int j{ 0 }; j < static_cast<int>(m_clipboard[0].size()); ++j) {
			std::vector<kkit::Map_tile> l_row;
			for (int i{ 0 }; i < m_clipboard.size(); ++i) {
				l_row.push_back(m_clipboard[static_cast<int>(m_clipboard.size()) - i - 1][j]);
			}
			result.push_back(l_row);
		}
	}

	for (auto& l_col : result)
		for (auto& l_tile : l_col)
			if (l_tile.get_tile_no() >= 0 && p_project.is_directional(l_tile.get_tile_no()))
				l_tile.toggle_direction();

	m_clipboard = result;
	p_project.add_message("Clipboard rotated " + std::string(p_clockwise ? "clockwise" : "counter-clockwise"));
}

int kkit::Board_ui::get_selected_board_tile_no(const kkit::Project& p_project) const {
	return p_project.get_board(m_board_ind).get_tile_no(m_sel_tile_x, m_sel_tile_y);
}

// save/load
bool kkit::Board_ui::xml_import(kkit::Project& p_project, int p_board_no) const {
	auto l_in_file = p_project.get_file_full_path(c::FILE_BOARDS, c::FILE_EXT_XML, p_board_no);

	if (!std::filesystem::exists(l_in_file))
		return false;
	else {
		p_project.reload_map_from_xml(p_board_no);
		return true;
	}
}

void kkit::Board_ui::xml_export(kkit::Project& p_project, int p_board_no) const {
	p_project.save_board_xml(p_board_no);
}

void kkit::Board_ui::save_boards_kzp(kkit::Project& p_project, bool p_compress) const {
	bool l_compress = (p_compress && !p_project.is_walken() && !p_project.is_klab_v_1());

	int l_bytes = p_project.save_boards_kzp(l_compress);
	int l_board_count(p_project.get_board_count());
	int l_original_bytes = l_board_count * (p_project.is_walken() ? 1 : 2) * c::MAP_W * c::MAP_H;

	// only v2.x boards should be saved as kzp - there is no real point otherwise
	if (l_compress)
		p_project.add_message(std::to_string(l_board_count) + " boards saved to KZP (" +
			std::to_string(l_bytes) + " bytes, " + std::to_string(l_original_bytes) + " original)",
			c::MSG_CODE_SUCCESS);
	else
		p_project.add_message(std::to_string(l_board_count) + " boards saved to DAT (" +
			std::to_string(l_bytes) + " bytes)",
			c::MSG_CODE_SUCCESS);
}

void kkit::Board_ui::save_walls_kzp(kkit::Project& p_project, kkit::Project_gfx& p_gfx, bool p_compress) const {
	int l_bytes = p_project.save_walls_kzp(p_compress);
	int l_wall_count(p_project.get_wall_image_count());
	int l_original_bytes = l_wall_count * c::WALL_IMG_W * c::WALL_IMG_H;

	bool l_pref_kzp{ p_project.get_config().get_ext_walls() == Data_ext::KZP };

	if (p_compress && l_pref_kzp)
		p_project.add_message(std::to_string(l_wall_count) + " wall tiles saved to KZP (" +
			std::to_string(l_bytes) + " bytes, " + std::to_string(l_original_bytes) + " original)",
			c::MSG_CODE_SUCCESS);
	else
		p_project.add_message(std::to_string(l_wall_count) + " wall tiles saved to DAT (" +
			std::to_string(l_bytes) + " bytes)",
			c::MSG_CODE_SUCCESS);
}

// logic
int kkit::Board_ui::count_tiles(const kkit::Project& p_project, int p_tile_no, bool p_all_boards) const {
	int result{ 0 };

	for (int i{ p_all_boards ? 0 : m_board_ind }; i < (p_all_boards ? p_project.get_board_count() : m_board_ind + 1); ++i) {
		const auto& l_brd = p_project.get_board(i);
		for (int x{ 0 }; x < c::MAP_W; ++x)
			for (int y{ 0 }; y < c::MAP_H; ++y)
				if (l_brd.get_tile_no(x, y) == p_tile_no)
					++result;
	}

	return result;
}

void kkit::Board_ui::next_tile(const kkit::Project& p_project, bool p_tp_tile, int p_w, int p_h, bool p_wrap) {
	const auto& l_brd{ p_project.get_board(m_board_ind) };
	int l_tile_no = p_tp_tile ? m_sel_tp_tile_no : get_selected_board_tile_no(p_project);
	bool l_first{ true };

	for (int x{ p_wrap ? 0 : m_sel_tile_x }; x < c::MAP_W; ++x) {
		for (int y{ l_first && !p_wrap ? m_sel_tile_y + 1 : 0 }; y < c::MAP_H; ++y)
			if (l_brd.get_tile_no(x, y) == l_tile_no) {
				m_sel_tile_x = x;
				m_sel_tile_y = y;
				this->clear_secondary_selection();
				this->center_offset(std::make_pair(m_sel_tile_x, m_sel_tile_y), p_w, p_h);
				return;
			}
		l_first = false;
	}

	// no tile found, wrap around
	if (!p_wrap)
		this->next_tile(p_project, p_tp_tile, p_w, p_h, true);

	// no tile found, but wrap was already turned on, return with no changes
}

// pretty much a lazy copy/past of next_tile
void kkit::Board_ui::prev_tile(const kkit::Project& p_project, bool p_tp_tile, int p_w, int p_h, bool p_wrap) {
	const auto& l_brd{ p_project.get_board(m_board_ind) };
	int l_tile_no = p_tp_tile ? m_sel_tp_tile_no : get_selected_board_tile_no(p_project);
	bool l_first{ true };

	for (int x{ p_wrap ? c::MAP_W - 1 : m_sel_tile_x }; x >= 0; --x) {
		for (int y{ l_first && !p_wrap ? m_sel_tile_y - 1 : c::MAP_H - 1 }; y >= 0; --y)
			if (l_brd.get_tile_no(x, y) == l_tile_no) {
				m_sel_tile_x = x;
				m_sel_tile_y = y;
				this->clear_secondary_selection();
				this->center_offset(std::make_pair(m_sel_tile_x, m_sel_tile_y), p_w, p_h);
				return;
			}
		l_first = false;
	}

	// no tile found, wrap around
	if (!p_wrap)
		this->prev_tile(p_project, p_tp_tile, p_w, p_h, true);

	// no tile found, but wrap was already turned on, return with no changes
}

std::string kkit::Board_ui::get_tp_tile_description(int p_index) {
	if (p_index == -2)
		return "Start Position";
	else if (p_index == -1)
		return "Empty Tile";
	else
		return "Tile #" + std::to_string(p_index + 1);
}

std::string kkit::Board_ui::get_wall_metadata_string(kkit::Wall_type p_type) {
	if (p_type == kkit::Wall_type::Cube)
		return "Cube";
	else if (p_type == kkit::Wall_type::Direction)
		return "Directional";
	else
		return "Plane";
}

void kkit::Board_ui::board_changed(SDL_Renderer* p_rnd, const kkit::Project& p_project, kkit::Project_gfx& p_gfx) {
	p_gfx.reload_minimap_texture(p_rnd, p_project, m_board_ind);
}
