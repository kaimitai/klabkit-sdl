#include <algorithm>
#include <cmath>

#include "Board_window.h"
#include "./../klib/file.h"
#include "./../klib/klib_util.h"
#include "./../klib/gfx.h"

kkit::Board_window::Board_window(SDL_Renderer* p_rnd) : toggles(std::vector<bool>(4, false)) {
	grid_texture = SDL_CreateTexture(p_rnd, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 4096, 4096);
	timers.push_back(klib::Timer(70, 10, true));

	// board tile metadata toggle buttons

	// buttons for destructible/clip/direction
	for (int i{ 0 }; i < 3; ++i)
		buttons.push_back(klib::Button("", BW_SBT_LBL_DESTR_X, BW_SBT_LBL_DESTR_Y + i * BW_SBT_LBL_DESTR_H, BW_SBT_LBL_DESTR_W, BW_SBT_LBL_DESTR_H));

	// toggle buttons
	buttons.push_back(klib::Button("Dir", BW_FBX, BW_FBY, BW_FBW, BW_FBH, klib::gc::COL_BLACK, klib::gc::COL_GREEN));
	buttons.push_back(klib::Button("Destr", BW_FBX + 1 * (BW_FBW + BW_FB_SPACING), BW_FBY, BW_FBW, BW_FBH));
	buttons.push_back(klib::Button("Clip", BW_FBX + 2 * (BW_FBW + BW_FB_SPACING), BW_FBY, BW_FBW, BW_FBH));
	buttons.push_back(klib::Button("Tile", BW_FBX + 3 * (BW_FBW + BW_FB_SPACING), BW_FBY, BW_FBW, BW_FBH));

	// direction indicator is turned on by default
	toggles[0] = true;

}

void kkit::Board_window::draw_selected_board_tile(SDL_Renderer* p_rnd, const klib::User_input& p_input, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx) const {
	int l_sel_tile_no = p_project.get_board(board_ind).get_tile_no(sel_tile_x, sel_tile_y);
	bool l_is_start_tile = p_project.get_board(board_ind).is_start_tile(sel_tile_x, sel_tile_y);

	klib::gfx::draw_window(p_rnd, p_gfx.get_font(),
		"tile @ (" + std::to_string(sel_tile_x) + "," + std::to_string(sel_tile_y) + "): " + (l_sel_tile_no == -1 ? (l_is_start_tile ? "Start" : "None") : "#" + std::to_string(l_sel_tile_no)),
		BW_SBTX - 1, BW_SBTY - klib::gc::BUTTON_H - 1, BW_SBTW + 2, BW_SBTH + 4 + klib::gc::BUTTON_H);

	klib::gfx::draw_rect(p_rnd, BW_SBTX, BW_SBTY, 128, 128, BG_COLOR, 0);

	if (l_sel_tile_no >= 0) {
		klib::gfx::blit_full_spec(p_rnd, p_gfx.get_tile_texture(l_sel_tile_no), BW_SBTX, BW_SBTY, 128, 128, 0, 0, 128, 128);

		bool l_blast = p_project.get_board(board_ind).is_blast(sel_tile_x, sel_tile_y);
		buttons[0].draw(p_rnd, p_gfx.get_font(), p_input,
			l_blast ? "Destruct" : "Nodestruct",
			l_blast ? klib::gc::COL_GREEN : klib::gc::COL_RED);

		bool l_inside = p_project.get_board(board_ind).is_inside(sel_tile_x, sel_tile_y);
		buttons[1].draw(p_rnd, p_gfx.get_font(), p_input,
			l_inside ? "Clip" : "Noclip",
			l_inside ? klib::gc::COL_GREEN : klib::gc::COL_RED);

		bool l_is_dir = (p_project.get_wall_type(l_sel_tile_no) == kkit::Wall_type::Direction);
		if (l_is_dir) {
			bool l_vertical = p_project.get_board(board_ind).is_vertical(sel_tile_x, sel_tile_y);

			buttons[2].draw(p_rnd, p_gfx.get_font(), p_input,
				l_vertical ? "Vertical" : "Horizontal",
				klib::gc::COL_ORANGE);
		}

	}
	else if (l_is_start_tile)
		buttons[2].draw(p_rnd, p_gfx.get_font(), p_input,
			"Face: " + p_project.get_board(board_ind).get_player_direction_as_string(),
			klib::gc::COL_ORANGE);
}

void kkit::Board_window::draw(SDL_Renderer* p_rnd, const klib::User_input& p_input, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx) const {
	// the board tile metadata buttons will be drawn dynamically in the selected tile drawing procedure
	for (std::size_t i{ 3 }; i < buttons.size(); ++i)
		buttons[i].draw(p_rnd, p_gfx.get_font(), p_input);

	bool l_mouse_over_tile_picker{ klib::util::is_p_in_rect(p_input.mx(), p_input.my(), BW_TPX, BW_TPY, BW_TPW, BW_TPH) };

	klib::gfx::draw_window(p_rnd, p_gfx.get_font(), "Board " + std::to_string(board_ind + 1) + "/" + std::to_string(p_project.get_board_count()), BW_BX - 1, BW_BY - klib::gc::BUTTON_H - 1, BW_BW + 2, BW_BW + 4 + klib::gc::BUTTON_H);
	draw_board(p_rnd, p_project, p_gfx, BW_BX, BW_BY);

	klib::gfx::draw_window(p_rnd, p_gfx.get_font(), "Minimap", BW_MX - 1, BW_MY - klib::gc::BUTTON_H - 1, BW_MW + 2, BW_MW + 4 + klib::gc::BUTTON_H);
	draw_minimap(p_rnd, BW_MX, BW_MY);

	klib::gfx::draw_window(p_rnd, p_gfx.get_font(), "Tile Picker", BW_TPX - 1, BW_TPY - klib::gc::BUTTON_H - 1, BW_TPW + 2, BW_TPH + 4 + klib::gc::BUTTON_H);
	draw_tile_picker(p_rnd, p_project, p_gfx, BW_TPX, BW_TPY);

	draw_selected_board_tile(p_rnd, p_input, p_project, p_gfx);


	if (l_mouse_over_tile_picker) {
		int l_tile_index = get_mouseover_tile_no(p_input.mx() - BW_TPX, p_input.my() - BW_TPY);

		if (l_tile_index >= -2) {
			int l_x = BW_BX + BW_BW - 2 * c::WALL_IMG_W - 4;
			int l_y = BW_BY + 4;
			int l_w = 2 * c::WALL_IMG_W;
			int l_h = 2 * c::WALL_IMG_H;

			klib::gfx::draw_rect(p_rnd, l_x - 1, l_y - 1, l_w + 2, l_h + 2, klib::gc::COL_WHITE, 1);
			klib::gfx::draw_rect(p_rnd, l_x, l_y, l_w, l_h, BG_COLOR, 0);

			if (l_tile_index >= 0 || l_tile_index == -2)
				klib::gfx::blit_p2_scale(p_rnd, l_tile_index >= 0 ? p_gfx.get_tile_texture(l_tile_index) : p_gfx.get_app_texture(2), l_x, l_y, 1);
		}
	}
}

void kkit::Board_window::button_click(std::size_t p_button_no, kkit::Project& p_project) {
	if (p_button_no == 0)
		p_project.toggle_mt_blast(board_ind, sel_tile_x, sel_tile_y);
	else if (p_button_no == 1)
		p_project.toggle_mt_inside(board_ind, sel_tile_x, sel_tile_y);
	else if (p_button_no == 2)
		p_project.toggle_mt_direction(board_ind, sel_tile_x, sel_tile_y);

	if (p_button_no >= 3) {
		bool l_toggle = !toggles[p_button_no - 3];
		toggles[p_button_no - 3] = l_toggle;
		buttons[p_button_no].set_bg_color(l_toggle ? klib::gc::COL_GREEN : klib::gc::COL_GRAY);
	}
}

void kkit::Board_window::move(const klib::User_input& p_input, int p_delta_ms, kkit::Project& p_project, kkit::Project_gfx& p_gfx) {
	bool l_shift = p_input.is_shift_pressed();
	bool l_ctrl = p_input.is_ctrl_pressed();

	for (auto& timer : timers)
		timer.move(p_delta_ms);

	if (p_input.mouse_clicked())
		for (std::size_t i{ 0 }; i < buttons.size(); ++i)
			if (buttons[i].is_hit(p_input.mx(), p_input.my())) {
				button_click(i, p_project);
				return;
			}

	if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_C)) {
		this->copy_to_clipboard(p_project);
		return;
	}
	else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_V)) {
		this->paste_from_clipboard(p_project);
		return;
	}
	else if (l_shift && p_input.is_pressed(SDL_SCANCODE_V)) {
		this->show_selection_rectangle();
		return;
	}
	else if (p_input.is_pressed(SDL_SCANCODE_DELETE)) {
		this->clear_selection(p_project);
		return;
	}

	bool mouse_over_board_grid{ klib::util::is_p_in_rect(p_input.mx(), p_input.my(), BW_BX, BW_BY, BW_BW, BW_BW) };
	bool mouse_over_tile_picker{ klib::util::is_p_in_rect(p_input.mx(), p_input.my(), BW_TPX, BW_TPY, BW_TPW, BW_TPH) };

	if (mouse_over_board_grid && l_ctrl && p_input.mw_up()) {
		if (zoom_factor >= ZOOM_MAX)
			return;
		auto l_tcoords = this->get_pixel_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		this->move_grid_zoom(0.2f);
		//this->center_offset(l_tcoords);
		this->translate_grid_offset(l_tcoords.first, l_tcoords.second, p_input.mx() - BW_BX, p_input.my() - BW_BY);
	}
	else if (mouse_over_board_grid && l_ctrl && p_input.mw_down()) {
		if (zoom_factor <= ZOOM_MIN)
			return;
		auto l_tcoords = this->get_pixel_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		this->move_grid_zoom(-0.2f);
		//this->center_offset(l_tcoords);
		this->translate_grid_offset(l_tcoords.first, l_tcoords.second, p_input.mx() - BW_BX, p_input.my() - BW_BY);
	}
	else if (p_input.mw_up() && mouse_over_tile_picker)
		tile_row = klib::util::validate(tile_row - 1, 0, c_tile_row_max());
	else if (p_input.mw_down() && mouse_over_tile_picker)
		tile_row = klib::util::validate(tile_row + 1, 0, c_tile_row_max());
	else if (p_input.is_pressed(SDL_SCANCODE_UP) || (!l_shift && p_input.mw_up()))
		this->move_grid_offset_y(l_ctrl ? -4 * 64 : -64);
	else if (p_input.is_pressed(SDL_SCANCODE_DOWN) || (!l_shift && p_input.mw_down()))
		this->move_grid_offset_y(l_ctrl ? 4 * 64 : 64);
	else if (p_input.is_pressed(SDL_SCANCODE_LEFT) || (l_shift && p_input.mw_down()))
		this->move_grid_offset_x(l_ctrl ? -4 * 64 : -1 * 64);
	else if (p_input.is_pressed(SDL_SCANCODE_RIGHT) || (l_shift && p_input.mw_up()))
		this->move_grid_offset_x(l_ctrl ? 4 * 64 : 1 * 64);
	else if (p_input.is_pressed(SDL_SCANCODE_KP_MINUS))
		this->move_grid_zoom(-1);
	else if (p_input.is_pressed(SDL_SCANCODE_KP_PLUS))
		this->move_grid_zoom(1);
	else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_S)) {
		int l_bytes = p_project.save_boards_kzp();
		int l_board_count(p_project.get_board_count());
		int l_original_bytes = l_board_count * 64 * 64 * 2;
		p_gfx.add_toast_ok(std::to_string(l_board_count) + " boards saved to KZP (" +
			std::to_string(l_bytes) + " bytes, " + std::to_string(l_original_bytes) + " original)");
	}
	else if (p_input.is_pressed(SDL_SCANCODE_PAGEDOWN) && board_ind > 0)
		--board_ind;
	else if (p_input.is_pressed(SDL_SCANCODE_PAGEUP) && board_ind < p_project.get_board_count() - 1)
		++board_ind;

	if (p_input.mouse_held(false) && mouse_over_board_grid) {
		auto l_tcoords = this->get_tile_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		if (p_project.get_player_start_pos(board_ind) != l_tcoords) {
			int l_stile_no{ this->get_selected_tile_no() };
			if (l_stile_no == -2)
				p_project.set_player_start_position(board_ind, l_tcoords.first, l_tcoords.second);
			else
				p_project.set_tile(this->board_ind, l_tcoords.first, l_tcoords.second, this->get_selected_tile(p_project, l_stile_no));
		}
	}
	else if (mouse_over_board_grid && p_input.mouse_held(true) && !l_shift) {
		auto l_tcoords = this->get_tile_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		sel_tile_x = l_tcoords.first;
		sel_tile_y = l_tcoords.second;
		sel_tile_2_x = -1;	// clear selection
	}
	else if (mouse_over_board_grid && p_input.mouse_held(true) && l_shift) {
		auto l_tcoords = this->get_tile_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		sel_tile_2_x = l_tcoords.first;
		sel_tile_2_y = l_tcoords.second;
	}
	else if (p_input.mouse_held() &&
		klib::util::is_p_in_rect(p_input.mx(), p_input.my(), BW_MX, BW_MY, BW_MW, BW_MW)) {
		this->click_minimap(p_input.mx() - BW_MX, p_input.my() - BW_MY);
	}
	else if (p_input.mouse_held() && mouse_over_tile_picker) {
		this->click_tile_picker(p_input.mx() - BW_TPX, p_input.my() - BW_TPY);
	}
	else if (p_input.is_pressed(SDL_SCANCODE_B))
		p_project.toggle_mt_blast(board_ind, sel_tile_x, sel_tile_y);
	else if (p_input.is_pressed(SDL_SCANCODE_C))
		p_project.toggle_mt_inside(board_ind, sel_tile_x, sel_tile_y);
	else if (p_input.is_pressed(SDL_SCANCODE_D))
		p_project.toggle_mt_direction(board_ind, sel_tile_x, sel_tile_y);
	else if (p_input.is_pressed(SDL_SCANCODE_F)) {
		auto l_rect = this->get_selection_rectangle();
		if (l_shift)
			p_project.flip_vertical(board_ind, std::get<0>(l_rect), std::get<1>(l_rect), std::get<2>(l_rect), std::get<3>(l_rect));
		else
			p_project.flip_horizontal(board_ind, std::get<0>(l_rect), std::get<1>(l_rect), std::get<2>(l_rect), std::get<3>(l_rect));
	}
	else if (p_input.is_pressed(SDL_SCANCODE_R))
		this->rotate_selection(l_shift);
}

// internal calculations
int kkit::Board_window::c_max_offset(void) const {
	return static_cast<int>(4096.0f - static_cast<float>(BW_BW) / this->zoom_factor);
}

void kkit::Board_window::validate_grid_offset(void) {
	zoom_factor = klib::util::validate(zoom_factor, ZOOM_MIN, ZOOM_MAX);
	board_px = klib::util::validate(board_px, 0, c_max_offset());
	board_py = klib::util::validate(board_py, 0, c_max_offset());
}

// (p_gx, p_gy) is a given pixel on the board map (0<p_gx,p_gy<4096)
// (p_sx, p_sy) is a given pixel on the visible grid (0<p_sx,p_sy<BW_BW)
// update the top left grid offset so that (p_gx, p_gy) will be drawn at (p_sx, p_sy) - if possible
void kkit::Board_window::translate_grid_offset(int p_gx, int p_gy, int p_sx, int p_sy) {
	float l_fac = 1.0f / zoom_factor;
	float l_tx = l_fac * p_sx;
	float l_ty = l_fac * p_sy;
	this->set_grid_offset(static_cast<int>(p_gx - l_tx), static_cast<int>(p_gy - l_ty));
}

void kkit::Board_window::move_grid_offset_x(int p_dx) {
	this->board_px += p_dx;
	this->validate_grid_offset();
}

void kkit::Board_window::move_grid_offset_y(int p_dy) {
	this->board_py += p_dy;
	this->validate_grid_offset();
}

void kkit::Board_window::set_grid_offset(int p_x, int p_y) {
	this->board_px = p_x;
	this->board_py = p_y;
	this->validate_grid_offset();
}

void kkit::Board_window::click_minimap(int p_x, int p_y) {
	float f_x = p_x / static_cast<float>(BW_MW);
	float f_y = p_y / static_cast<float>(BW_MW);
	this->set_grid_offset(p_x * 32 - c_bb_pixel_width() / 2, p_y * 32 - c_bb_pixel_width() / 2);
}

void kkit::Board_window::center_offset(void) {
	// TODO: Fix this function, the problem is centering tiles near the right and bottom edges
	//int l_dx = std::min(c_tile_cnt() / 2, (64 - board_x) / c_tile_cnt());
	//int l_dy = std::min(c_tile_cnt() / 2, (64 - board_y) / c_tile_cnt());
	int l_dx = 0;
	int l_dy = 0;

	this->set_grid_offset(board_px - c_bb_pixel_width() / 2, board_py - c_bb_pixel_width() / 2);
}

void kkit::Board_window::center_offset(std::pair<int, int> p_coords) {
	this->set_grid_offset(p_coords.first, p_coords.second);
	this->center_offset();
}

void kkit::Board_window::move_grid_zoom(float p_dz) {
	this->zoom_factor += p_dz;
	this->validate_grid_offset();
}

void kkit::Board_window::draw_board(SDL_Renderer* p_rnd, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx, int p_x, int p_y) const {
	const auto& board = p_project.get_board(this->board_ind);
	float l_shrink_factor = 0.25f + static_cast<float>(timers[0].get_frame()) / 100.0f;

	SDL_SetRenderTarget(p_rnd, grid_texture);
	SDL_SetRenderDrawColor(p_rnd, BG_COLOR.r, BG_COLOR.g, BG_COLOR.b, 0);
	SDL_RenderClear(p_rnd);

	for (int i{ 0 }; i < 64; ++i)
		for (int j{ 0 }; j < 64; ++j)
			if (!board.is_empty_tile(i, j)) {
				int l_tile_no = board.get_tile_no(i, j);

				bool l_flash = toggles[1] && board.is_blast(i, j);
				l_flash |= toggles[2] && board.is_inside(i, j) && (!p_project.is_inside(l_tile_no) || c::CLIP_OVERRIDE.find(l_tile_no) != end(c::CLIP_OVERRIDE));
				l_flash |= toggles[3] && (l_tile_no == get_selected_tile_no());

				bool l_directional = p_project.is_directional(l_tile_no);

				klib::gfx::blit(p_rnd, p_gfx.get_tile_texture(l_tile_no), 64 * i, 64 * j);
				if (toggles[0] && l_directional)
					klib::gfx::blit_factor(p_rnd, p_gfx.get_app_texture(board.is_vertical(i, j) ? 1 : 0), 64 * i + 32, 64 * j + 32, l_shrink_factor);
				if (l_flash)
					klib::gfx::blit_factor(p_rnd, p_gfx.get_app_texture(6), 64 * i + 32, 64 * j + 32, l_shrink_factor);
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

	klib::gfx::blit_factor(p_rnd, p_gfx.get_app_texture(l_pstart_sprite_no), l_px, l_py, l_shrink_factor);

	// draw selected tile
	klib::gfx::draw_rect(p_rnd, sel_tile_x * 64, sel_tile_y * 64, 64, 64, SDL_Color{ 255,255,0 }, 4);

	// draw full selection
	if (sel_tile_2_x >= 0)
		klib::gfx::draw_rect(p_rnd,
			std::min(sel_tile_x, sel_tile_2_x) * 64,
			std::min(sel_tile_y, sel_tile_2_y) * 64,
			64 * (1 + abs(sel_tile_x - sel_tile_2_x)),
			64 * (1 + abs(sel_tile_y - sel_tile_2_y)),
			klib::gc::COL_ORANGE, 4);

	SDL_SetRenderTarget(p_rnd, nullptr);

	klib::gfx::blit_full_spec(p_rnd, this->grid_texture, BW_BX, BW_BY, BW_BW, BW_BW, board_px, board_py, static_cast<int>(BW_BW / zoom_factor), static_cast<int>(BW_BW / zoom_factor));

	// draw outline
	//klib::gfx::draw_rect(p_rnd, p_x, p_y, BOARD_PW, BOARD_PW, SDL_Color{ 255,255,255 }, 2);
}


void kkit::Board_window::draw_minimap(SDL_Renderer* p_rnd, int p_x, int p_y) const {
	klib::gfx::draw_rect(p_rnd, p_x, p_y, BW_MW, BW_MW, SDL_Color{ 0,0,0 }, 0);

	int l_sel_factor = static_cast<int>(16.0f / zoom_factor);

	int l_x = p_x + static_cast<int>(board_px * (128.0f / 4096.0f));
	int l_y = p_y + static_cast<int>(board_py * (128.0f / 4096.0f));

	klib::gfx::draw_rect(p_rnd, l_x, l_y, l_sel_factor, l_sel_factor, SDL_Color{ 255,255,0 }, 2);
}

void kkit::Board_window::draw_tile_picker(SDL_Renderer* p_rnd, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx, int p_x, int p_y) const {
	klib::gfx::draw_rect(p_rnd, p_x, p_y, BW_TPW, BW_TPH, SDL_Color{ 0,0,0 }, 0);

	for (int i{ tile_row * BW_TPR }; i < static_cast<int>(c::TILES.size()) && i < (BW_TPR * (BW_TPC + tile_row)); ++i) {
		int l_y = (i - tile_row * BW_TPR) / BW_TPR;
		int l_x = (i - tile_row * BW_TPR) % BW_TPR;
		int l_index = c::TILES[i];
		if (l_index >= 0)
			klib::gfx::blit_p2_scale(p_rnd, p_gfx.get_tile_texture(l_index), p_x + l_x * BW_TP_TW, p_y + l_y * BW_TP_TW, -1);
		else if (l_index == -2)
			klib::gfx::blit_p2_scale(p_rnd, p_gfx.get_app_texture(2), p_x + l_x * BW_TP_TW, p_y + l_y * BW_TP_TW, -1);
	}

	if (tile_y >= tile_row && tile_y < tile_row + BW_TPC)
		klib::gfx::draw_rect(p_rnd, p_x + 32 * tile_x, p_y + 32 * (tile_y - tile_row), 32, 32, SDL_Color{ 255, 255, 0 }, 2);

	std::string l_tile_md;
	int l_tile_no = c::TILES.at(tile_y * BW_TPR + tile_x);

	if (l_tile_no >= 0) {
		bool l_clip = p_project.is_inside(l_tile_no);
		bool l_blast = p_project.is_blast(l_tile_no);
		l_tile_md = std::to_string(l_tile_no) + ":" + (l_blast ? "Destruct" : "Nodestruct") + "," + (l_clip ? "Clip" : "Noclip") + "," + p_project.get_block_type_as_string(l_tile_no);
	}
	else if (l_tile_no == -1)
		l_tile_md = "Empty tile";
	else if (l_tile_no == -2)
		l_tile_md = "Start Position";

	klib::gfx::draw_label(p_rnd, p_gfx.get_font(), l_tile_md, p_x - 1, p_y + BW_TPH, BW_TPW + 2, klib::gc::BUTTON_H, klib::gc::COL_WHITE, klib::gc::COL_BLUE);
}

// get selected tile
// need the project to fetch metadata
kkit::Map_tile kkit::Board_window::get_selected_tile(const kkit::Project& p_project, int p_tile_no) const {
	return p_project.gen_map_tile(p_tile_no);
}

int kkit::Board_window::get_selected_tile_no(void) const {
	return c::TILES.at(tile_y * BW_TPR + tile_x);
}

int kkit::Board_window::get_mouseover_tile_no(int p_x, int p_y) const {
	int l_x = p_x / BW_TP_TW;
	int l_y = p_y / BW_TP_TW;
	int l_index = (l_y + tile_row) * BW_TPR + l_x;

	if (l_index < c::TILES.size())
		return c::TILES[l_index];
	else
		return -3;
}

void kkit::Board_window::click_tile_picker(int p_x, int p_y) {
	int l_x = p_x / BW_TP_TW;
	int l_y = p_y / BW_TP_TW + tile_row;
	int l_index = l_y * BW_TPR + l_x;
	if (l_index < c::TILES.size() && c::TILES[l_index] >= -2) {
		this->tile_x = l_x;
		this->tile_y = l_y;
	}
}

int  kkit::Board_window::c_bb_pixel_width(void) const {
	return static_cast<int>(8.0f * 64.0f / zoom_factor);
}

int kkit::Board_window::c_tile_row_max(void) const {
	return std::max(0, (static_cast<int>(c::TILES.size()) % BW_TPC == 0 ? 0 : 1) + static_cast<int>(c::TILES.size()) / BW_TPR - BW_TPC);
}

// the board was clicked, get its global coordinates from the pixels
// input pixel coordinates must be relative to the top left of the grid
// and do not call this function if clicking outside the actual level grid
std::pair<int, int> kkit::Board_window::get_tile_pos(int p_x, int p_y) const {
	int l_tx = board_px + static_cast<int>(p_x / zoom_factor);
	int l_ty = board_py + static_cast<int>(p_y / zoom_factor);
	return std::make_pair(l_tx / 64, l_ty / 64);
}

std::pair<int, int> kkit::Board_window::get_pixel_pos(int p_x, int p_y) const {
	int l_tx = board_px + static_cast<int>(p_x / zoom_factor);
	int l_ty = board_py + static_cast<int>(p_y / zoom_factor);
	return std::make_pair(l_tx, l_ty);
}

/***********************
* Selection operations *
************************/

std::tuple<int, int, int, int> kkit::Board_window::get_selection_rectangle(void) const {
	// we only have one tile selected
	if (sel_tile_2_x < 0)
		return std::make_tuple(sel_tile_x, sel_tile_y, 1, 1);
	// we have a rectangular selection
	else {
		int l_x = std::min(sel_tile_x, sel_tile_2_x);
		int l_y = std::min(sel_tile_y, sel_tile_2_y);
		int l_w = abs(sel_tile_x - sel_tile_2_x) + 1;
		int l_h = abs(sel_tile_y - sel_tile_2_y) + 1;
		return std::make_tuple(l_x, l_y, l_w, l_h);
	}
}

void kkit::Board_window::copy_to_clipboard(const kkit::Project& p_project) {
	auto l_rect = this->get_selection_rectangle();

	this->clipboard = p_project.get_board(board_ind).get_rectangle(std::get<0>(l_rect), std::get<1>(l_rect), std::get<2>(l_rect), std::get<3>(l_rect));
}

void kkit::Board_window::paste_from_clipboard(kkit::Project& p_project) {
	if (this->selection_fits()) {
		for (int j{ 0 }; j < clipboard.size() && (sel_tile_x + j < 64); ++j)
			for (int i{ 0 }; i < clipboard[j].size() && (sel_tile_y + i < 64); ++i) {
				p_project.set_tile(board_ind, sel_tile_x + j, sel_tile_y + i, clipboard[j][i]);
			}
	}
}

bool kkit::Board_window::selection_fits(void) const {
	return (sel_tile_x + clipboard.size() <= 64) && (clipboard.size() == 0 || (sel_tile_y + clipboard[0].size() <= 64));
}

void kkit::Board_window::show_selection_rectangle(void) {
	if (this->selection_fits() && clipboard.size() > 0) {
		sel_tile_2_x = sel_tile_x + static_cast<int>(clipboard.size()) - 1;
		sel_tile_2_y = sel_tile_y + static_cast<int>(clipboard[0].size()) - 1;
	}
}

void kkit::Board_window::clear_selection(kkit::Project& p_project) {
	auto l_rect = this->get_selection_rectangle();
	for (int i{ std::get<0>(l_rect) }; i < std::get<0>(l_rect) + std::get<2>(l_rect); ++i)
		for (int j{ std::get<1>(l_rect) }; j < std::get<1>(l_rect) + std::get<3>(l_rect); ++j)
			p_project.clear_tile(board_ind, i, j);
}

void kkit::Board_window::rotate_selection(bool p_clockwise) {
	std::vector<std::vector<kkit::Map_tile>> result;

	if (!p_clockwise) {
		for (int j{ static_cast<int>(clipboard[0].size()) - 1 }; j >= 0; --j) {
			std::vector<kkit::Map_tile> l_row;
			for (int i{ 0 }; i < clipboard.size(); ++i) {
				l_row.push_back(clipboard[i][j]);
			}
			result.push_back(l_row);
		}
	}
	else {
		for (int j{ 0 }; j < static_cast<int>(clipboard[0].size()); ++j) {
			std::vector<kkit::Map_tile> l_row;
			for (int i{ 0 }; i < clipboard.size(); ++i) {
				l_row.push_back(clipboard[i][j]);
			}
			result.push_back(l_row);
		}
	}

	for (auto& l_col : result)
		for (auto& l_tile : l_col)
			l_tile.toggle_direction();

	clipboard = result;
}
