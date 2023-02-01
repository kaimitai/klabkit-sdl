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
	m_texture{ SDL_CreateTexture(p_rnd, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 4096, 4096) },
	m_board_ind{ 0 }, m_cam_x{ 0 }, m_cam_y{ 0 }, m_cam_zoom{ 1.0f },
	m_mouse_drag_active{ false }, m_mouse_drag_pos{ std::make_pair(0,0) },
	m_toggles{ std::vector<bool>(4, false) }
{
	// tile flash timer
	m_timers.push_back(klib::Timer(70, 10, true));
	// pulsating color timer
	m_timers.push_back(klib::Timer(256, 5));

	// directions should be on by default
	m_toggles[0] = true;
}

void kkit::Board_ui::draw(SDL_Renderer* p_rnd,
	const klib::User_input& p_input, kkit::Project& p_project,
	kkit::Project_gfx& p_gfx, int p_w, int p_h) {
	generate_board_texture(p_rnd, p_project, p_gfx);

	SDL_SetRenderDrawColor(p_rnd, 126, 126, 255, 0);
	SDL_RenderClear(p_rnd);

	int l_s_pixels_w{ static_cast<int>(p_w * m_cam_zoom) };
	int l_s_pixels_h{ static_cast<int>(p_h * m_cam_zoom) };

	int l_rest_w = std::max(0, 4096 - m_cam_x);
	int l_rest_h = std::max(0, 4096 - m_cam_y);

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

void kkit::Board_ui::generate_board_texture(SDL_Renderer* p_rnd,
	const kkit::Project& p_project, const kkit::Project_gfx& p_gfx) const {

	int l_sel_board_tile_no{ get_selected_board_tile_no(p_project) };
	const auto& board = p_project.get_board(this->m_board_ind);
	float l_shrink_factor = 0.25f + static_cast<float>(m_timers[0].get_frame()) / 100.0f;

	SDL_SetRenderTarget(p_rnd, m_texture);
	SDL_Color l_floor_col = p_gfx.get_floor_color();
	SDL_SetRenderDrawColor(p_rnd, l_floor_col.r, l_floor_col.g, l_floor_col.b, 0);
	SDL_RenderClear(p_rnd);

	for (int i{ 0 }; i < 64; ++i)
		for (int j{ 0 }; j < 64; ++j) {
			int l_tile_no = board.get_tile_no(i, j);

			bool l_flash = m_toggles[1] && board.is_blast(i, j);

			if (l_tile_no >= 0)
				l_flash |= m_toggles[2] && board.is_inside(i, j) && (!p_project.is_inside(l_tile_no) || p_project.is_clip_override(l_tile_no));
			else
				l_flash |= m_toggles[2] && !board.is_inside(i, j);

			l_flash |= m_toggles[3] && (l_tile_no == l_sel_board_tile_no);

			if (l_tile_no >= 0)
				klib::gfx::blit(p_rnd, p_gfx.get_texture(c::INDEX_WALL_TEXTURES, l_tile_no), 64 * i, 64 * j);

			if (m_toggles[0] && p_project.is_directional(l_tile_no))
				klib::gfx::blit_factor(p_rnd, p_gfx.get_texture(c::INDEX_APP_TEXTURES, board.is_vertical(i, j) ? 1 : 0), 64 * i + 32, 64 * j + 32, l_shrink_factor);
			if (l_flash)
				klib::gfx::blit_factor(p_rnd, p_gfx.get_texture(c::INDEX_APP_TEXTURES, 6), 64 * i + 32, 64 * j + 32, l_shrink_factor);
		}

	// draw player start
	int l_px = 64 * board.get_player_start_x() + 32;
	int l_py = 64 * board.get_player_start_y() + 32;
	kkit::Player_direction l_pdir = board.get_player_start_direction();
	int l_pstart_sprite_no{ 2 };
	if (l_pdir == kkit::Player_direction::Left)
		l_pstart_sprite_no = 3;
	else if (l_pdir == kkit::Player_direction::Down)
		l_pstart_sprite_no = 4;
	else if (l_pdir == kkit::Player_direction::Right)
		l_pstart_sprite_no = 5;

	klib::gfx::blit_factor(p_rnd, p_gfx.get_texture(c::INDEX_APP_TEXTURES, l_pstart_sprite_no), l_px, l_py, l_shrink_factor);


	// draw selected tile
	klib::gfx::draw_rect(p_rnd, m_sel_tile_x * 64, m_sel_tile_y * 64, 64, 64,
		kkit::gfx::get_pulse_color(0, m_timers[1].get_frame()),
		4);

	// draw full selection
	if (m_sel_tile_2_x >= 0)
		klib::gfx::draw_rect(p_rnd,
			std::min(m_sel_tile_x, m_sel_tile_2_x) * 64,
			std::min(m_sel_tile_y, m_sel_tile_2_y) * 64,
			64 * (1 + abs(m_sel_tile_x - m_sel_tile_2_x)),
			64 * (1 + abs(m_sel_tile_y - m_sel_tile_2_y)),
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
				add_cam_x(-64, p_w);
			else
				add_cam_y(64, p_h);
		}
		else if (p_input.mw_up()) {
			if (l_ctrl) {
				if (m_cam_zoom > ZOOM_MIN)
					zoom_camera(l_mx, l_my, -0.1f, p_w, p_h);
			}
			else if (l_shift)
				add_cam_x(64, p_w);
			else
				add_cam_y(-64, p_h);
		}

	}

	if (!ImGui::GetIO().WantCaptureKeyboard) {
		if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_C))
			copy_to_clipboard(p_project);
		else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_V))
			paste_from_clipboard(p_project);
		else if (l_shift && p_input.is_pressed(SDL_SCANCODE_V))
			show_selection_rectangle();
		else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_X))
			cut_selection(p_project);
		// ctrl+H: center on player start position
		else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_H))
			this->center_offset(p_project.get_player_start_pos(m_board_ind), p_w, p_h);
		// shift+H: set selected board tile to player start position
		else if (l_shift && p_input.is_pressed(SDL_SCANCODE_H))
			p_project.set_player_start_position(m_board_ind, m_sel_tile_x, m_sel_tile_y);
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
	return std::max(4096 - l_pixel_w, 0);
}

int kkit::Board_ui::get_cam_y_max(int p_h) const {
	int l_pixel_h{ static_cast<int>(m_cam_zoom * p_h) };
	return std::max(4096 - l_pixel_h, 0);
}

std::pair<int, int> kkit::Board_ui::get_tile_pos_from_mouse_coords(int p_mx, int p_my) const {
	int l_mx = m_cam_x + static_cast<int>(m_cam_zoom * p_mx);
	int l_my = m_cam_y + static_cast<int>(m_cam_zoom * p_my);

	return std::make_pair(l_mx / 64, l_my / 64);
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
	return p_pos.first >= 0 && p_pos.second >= 0 &&
		p_pos.first < 64 && p_pos.second < 64;
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
		for (int j{ 0 }; j < m_clipboard.size() && (m_sel_tile_x + j < 64); ++j)
			for (int i{ 0 }; i < m_clipboard[j].size() && (m_sel_tile_y + i < 64); ++i) {
				p_project.set_tile(m_board_ind, m_sel_tile_x + j, m_sel_tile_y + i, m_clipboard[j][i]);
			}
	}
}

bool kkit::Board_ui::selection_fits(void) const {
	return (m_sel_tile_x + m_clipboard.size() <= 64) && (m_clipboard.size() == 0 || (m_sel_tile_y + m_clipboard[0].size() <= 64));
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

void kkit::Board_ui::rotate_selection(const kkit::Project& p_project, bool p_clockwise) {
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
			std::to_string(l_bytes) + " bytes, " + std::to_string(l_original_bytes) + " original)");
	else
		p_project.add_message(std::to_string(l_board_count) + " boards saved to DAT (" +
			std::to_string(l_bytes) + " bytes)");
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
	int l_tile_no = get_selected_board_tile_no(p_project); // p_tp_tile ? this->get_selected_tile_no(p_project) : this->get_selected_board_tile_no(p_project);
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
	int l_tile_no = get_selected_board_tile_no(p_project); // p_tp_tile ? this->get_selected_tile_no(p_project) : this->get_selected_board_tile_no(p_project);
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
