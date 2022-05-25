#include <algorithm>
#include <cmath>
#include <filesystem>

#include "Board_window.h"
#include "./../klib/file.h"
#include "./../klib/klib_util.h"
#include "./../klib/gfx.h"
#include "kkit_gfx.h"

kkit::Board_window::Board_window(SDL_Renderer* p_rnd, bool p_is_klab_v2) : toggles(std::vector<bool>(4, false)) {
	grid_texture = SDL_CreateTexture(p_rnd, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 4096, 4096);

	// tile flash timer
	timers.push_back(klib::Timer(70, 10, true));
	// pulsating color timer
	timers.push_back(klib::Timer(256, 5));

	// board tile metadata toggle buttons

	// buttons for destructible/clip/direction
	for (int i{ 0 }; i < 3; ++i)
		buttons.push_back(klib::Button("", BW_SBT_LBL_DESTR_X, BW_SBT_LBL_DESTR_Y + i * BW_SBT_LBL_DESTR_H, BW_SBT_LBL_DESTR_W, BW_SBT_LBL_DESTR_H));

	// toggle buttons
	buttons.push_back(klib::Button("Dir", BW_FBX, BW_FBY, BW_FBW, BW_FBH, klib::gc::COL_BLACK, klib::gc::COL_GREEN));
	buttons.push_back(klib::Button("Destr", BW_FBX + 1 * (BW_FBW + BW_FB_SPACING), BW_FBY, BW_FBW, BW_FBH));
	buttons.push_back(klib::Button("Clip", BW_FBX + 2 * (BW_FBW + BW_FB_SPACING), BW_FBY, BW_FBW, BW_FBH));
	buttons.push_back(klib::Button("Tile", BW_FBX + 3 * (BW_FBW + BW_FB_SPACING), BW_FBY, BW_FBW, BW_FBH));

	// logic buttons
	buttons.push_back(klib::Button("Prev", BW_FBX, BW_FBY + BW_FBH + BW_FB_SPACING, BW_FBW, BW_FBH));
	buttons.push_back(klib::Button("Next", BW_FBX + (BW_FBW + BW_FB_SPACING), BW_FBY + BW_FBH + BW_FB_SPACING, BW_FBW, BW_FBH));
	buttons.push_back(klib::Button("Count", BW_FBX + 2 * (BW_FBW + BW_FB_SPACING), BW_FBY + BW_FBH + BW_FB_SPACING, BW_FBW, BW_FBH));

	// save, load, export/import
	buttons.push_back(klib::Button("Export XML", BW_EXML_BTN_X, BW_EXML_BTN_Y, BW_EXML_BTN_W, BW_EXML_BTN_H));
	buttons.push_back(klib::Button("Import XML", BW_TPX + BW_TPW - BW_EXML_BTN_W, BW_EXML_BTN_Y, BW_EXML_BTN_W, BW_EXML_BTN_H));

	buttons.push_back(klib::Button("Save KZP", BW_EXML_BTN_X, BW_EXML_BTN_Y + BW_EXML_BTN_H + BW_FB_SPACING, BW_TPW, BW_EXML_BTN_H, klib::gc::COL_BLACK, p_is_klab_v2 ? klib::gc::COL_GRAY : klib::gc::COL_RED));
	buttons.push_back(klib::Button("Save DAT", BW_EXML_BTN_X, BW_EXML_BTN_Y + 2 * (BW_EXML_BTN_H + BW_FB_SPACING), BW_TPW, BW_EXML_BTN_H));
	buttons.push_back(klib::Button("Save BMP", BW_EXML_BTN_X, BW_EXML_BTN_Y + 3 * (BW_EXML_BTN_H + BW_FB_SPACING), BW_TPW, BW_EXML_BTN_H));

	// direction indicator is turned on by default
	toggles[0] = true;

}

void kkit::Board_window::draw_selected_board_tile(SDL_Renderer* p_rnd, const klib::User_input& p_input, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx) const {
	int l_sel_tile_no = p_project.get_board(board_ind).get_tile_no(sel_tile_x, sel_tile_y);
	bool l_is_start_tile = p_project.get_board(board_ind).is_start_tile(sel_tile_x, sel_tile_y);

	klib::gfx::draw_window(p_rnd, p_gfx.get_font(),
		"tile @ (" + std::to_string(sel_tile_x) + "," + std::to_string(sel_tile_y) + "): " + (l_sel_tile_no == -1 ? (l_is_start_tile ? "Start" : "None") : "#" + std::to_string(l_sel_tile_no + 1)),
		BW_SBTX - 1, BW_SBTY - klib::gc::BUTTON_H - 1, BW_SBTW + 2, BW_SBTH + 4 + klib::gc::BUTTON_H);

	klib::gfx::draw_rect(p_rnd, BW_SBTX, BW_SBTY, 128, 128,
		kkit::gfx::get_pulse_color(2, timers[1].get_frame()),
		0);

	if (l_is_start_tile)
		buttons[2].draw(p_rnd, p_gfx.get_font(), p_input,
			"Face: " + p_project.get_board(board_ind).get_player_direction_as_string(),
			klib::gc::COL_ORANGE);
	else {

		if (l_sel_tile_no >= 0) {
			klib::gfx::blit_full_spec(p_rnd, p_gfx.get_tile_texture(l_sel_tile_no), BW_SBTX, BW_SBTY, 128, 128, 0, 0, 128, 128);

			bool l_is_dir = (p_project.get_wall_type(l_sel_tile_no) == kkit::Wall_type::Direction);
			if (l_is_dir) {
				bool l_vertical = p_project.get_board(board_ind).is_vertical(sel_tile_x, sel_tile_y);

				buttons[2].draw(p_rnd, p_gfx.get_font(), p_input,
					l_vertical ? "Vertical" : "Horizontal",
					klib::gc::COL_ORANGE);
			}
		}

		bool l_blast = p_project.get_board(board_ind).is_blast(sel_tile_x, sel_tile_y);
		buttons[0].draw(p_rnd, p_gfx.get_font(), p_input,
			l_blast ? "Destruct" : "Nodestruct",
			l_blast ? klib::gc::COL_GREEN : klib::gc::COL_RED);

		bool l_inside = p_project.get_board(board_ind).is_inside(sel_tile_x, sel_tile_y);
		buttons[1].draw(p_rnd, p_gfx.get_font(), p_input,
			l_inside ? "Clip" : "Noclip",
			l_inside ? klib::gc::COL_GREEN : klib::gc::COL_RED);
	}
}

void kkit::Board_window::draw(SDL_Renderer* p_rnd, const klib::User_input& p_input, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx) const {
	// the board tile metadata buttons will be drawn dynamically in the selected tile drawing procedure
	for (std::size_t i{ 3 }; i < buttons.size(); ++i)
		buttons[i].draw(p_rnd, p_gfx.get_font(), p_input);

	bool l_mouse_over_board_grid{ klib::util::is_p_in_rect(p_input.mx(), p_input.my(), BW_BX, BW_BY, BW_BW, BW_BW) };
	bool l_mouse_over_tile_picker{ klib::util::is_p_in_rect(p_input.mx(), p_input.my(), BW_TPX, BW_TPY, BW_TPW, BW_TPH) };

	klib::gfx::draw_window(p_rnd, p_gfx.get_font(), "Board " + std::to_string(board_ind + 1) + "/" + std::to_string(p_project.get_board_count()), BW_BX - 1, BW_BY - klib::gc::BUTTON_H - 1, BW_BW + 2, BW_BW + 4 + klib::gc::BUTTON_H,
		klib::gc::BUTTON_H,
		l_mouse_over_board_grid ? klib::gc::COL_BLACK : klib::gc::COL_WHITE,
		l_mouse_over_board_grid ? gfx::get_pulse_color(0, timers[1].get_frame()) : klib::gc::COL_BLUE);
	draw_board(p_rnd, p_project, p_gfx, BW_BX, BW_BY);

	klib::gfx::draw_window(p_rnd, p_gfx.get_font(), "Minimap", BW_MX - 1, BW_MY - klib::gc::BUTTON_H - 1, BW_MW + 2, BW_MW + 4 + klib::gc::BUTTON_H);
	klib::gfx::blit_p2_scale(p_rnd, p_gfx.get_minimap_texture(board_ind), BW_MX, BW_MY, 1);
	draw_minimap(p_rnd, BW_MX, BW_MY);

	klib::gfx::draw_window(p_rnd, p_gfx.get_font(), "Tile Picker", BW_TPX - 1, BW_TPY - klib::gc::BUTTON_H - 1, BW_TPW + 2, BW_TPH + 4 + klib::gc::BUTTON_H,
		klib::gc::BUTTON_H,
		l_mouse_over_tile_picker ? klib::gc::COL_BLACK : klib::gc::COL_WHITE,
		l_mouse_over_tile_picker ? gfx::get_pulse_color(0, timers[1].get_frame()) : klib::gc::COL_BLUE);
	draw_tile_picker(p_rnd, p_project, p_gfx, BW_TPX, BW_TPY);

	draw_selected_board_tile(p_rnd, p_input, p_project, p_gfx);


	if (l_mouse_over_tile_picker) {
		int l_tile_index = get_mouseover_tile_no(p_project, p_input.mx() - BW_TPX, p_input.my() - BW_TPY);

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

void kkit::Board_window::button_click(std::size_t p_button_no, SDL_Renderer* p_rnd, kkit::Project& p_project, kkit::Project_gfx& p_gfx, const klib::User_input& p_input) {
	bool l_shift_held = p_input.is_shift_pressed();
	bool l_ctrl_held = p_input.is_ctrl_pressed();

	// toggle board tile: destructible
	if (p_button_no == 0)
		p_project.toggle_mt_blast(board_ind, sel_tile_x, sel_tile_y);
	// toggle board tile: clip/noclip
	else if (p_button_no == 1)
		p_project.toggle_mt_inside(board_ind, sel_tile_x, sel_tile_y);
	// toggle board tile: direction
	else if (p_button_no == 2)
		p_project.toggle_mt_direction(board_ind, sel_tile_x, sel_tile_y);
	// previous board tile of same type (if shift held, prev tile of tile picker's selected type)
	else if (p_button_no == BW_BTN_LOGIC)
		this->prev_tile(p_project, l_shift_held);
	// next board tile of same type (if shift held, next tile of tile picker's selected type)
	else if (p_button_no == BW_BTN_LOGIC + 1)
		this->next_tile(p_project, l_shift_held);
	else if (p_button_no == BW_BTN_LOGIC + 2) {
		int l_tile_no = (l_ctrl_held ? this->get_selected_tile_no(p_project) : this->get_selected_board_tile_no(p_project));
		int l_tile_cnt = this->count_tiles(p_project, l_tile_no, l_shift_held);

		std::string l_msg{ "Count selected " };
		l_msg += (l_ctrl_held ? "tile picker" : "board");
		l_msg += " tile (#" + std::to_string(l_tile_no + 1) + ", ";
		l_msg += (l_shift_held ? "all boards" : "board " + std::to_string(board_ind + 1));
		l_msg += "): " + std::to_string(l_tile_cnt);

		p_gfx.add_toast_info(l_msg);
	}
	// export board(s) to xml
	else if (p_button_no == BW_BTN_SL_IND) {
		int l_exported{ 0 };

		for (int i{ l_shift_held ? 0 : board_ind }; i < (l_shift_held ? p_project.get_board_count() : board_ind + 1); ++i) {
			this->xml_export(p_project, i);
			++l_exported;
		}
		p_gfx.add_toast_ok(std::to_string(l_exported) + " xml file(s) exported");
	}
	// import board(s) from xml
	else if (p_button_no == BW_BTN_SL_IND + 1) {
		int l_imported{ 0 };

		for (int i{ l_shift_held ? 0 : board_ind }; i < (l_shift_held ? p_project.get_board_count() : board_ind + 1); ++i)
			if (this->xml_import(p_project, i)) {
				++l_imported;
				p_gfx.reload_minimap_texture(p_rnd, p_project, i);
			}

		if (l_imported > 0)
			p_gfx.add_toast_ok(std::to_string(l_imported) + " xml file(s) imported");
		else
			p_gfx.add_toast_error("No xml file(s) found");
	}
	// save kzp
	else if (p_button_no == BW_BTN_SL_IND + 2)
		this->save_boards_kzp(p_project, p_gfx, true);
	// save dat	
	else if (p_button_no == BW_BTN_SL_IND + 3)
		this->save_boards_kzp(p_project, p_gfx, false);
	// save board as bmp file
	else if (p_button_no == BW_BTN_SL_IND + 4) {

		int l_exported{ 0 };

		for (int i{ l_shift_held ? 0 : board_ind }; i < (l_shift_held ? p_project.get_board_count() : board_ind + 1); ++i) {
			kkit::gfx::project_map_to_bmp(p_project, i);
			++l_exported;
		}
		p_gfx.add_toast_ok(std::to_string(l_exported) + " bmp file(s) saved");

	}
	// flash toggles
	else if (p_button_no >= 3) {
		bool l_toggle = !toggles[p_button_no - 3];
		toggles[p_button_no - 3] = l_toggle;
		buttons[p_button_no].set_bg_color(l_toggle ? klib::gc::COL_GREEN : klib::gc::COL_GRAY);
	}
}

void kkit::Board_window::move(SDL_Renderer* p_rnd, const klib::User_input& p_input, int p_delta_ms, kkit::Project& p_project, kkit::Project_gfx& p_gfx) {
	bool l_shift = p_input.is_shift_pressed();
	bool l_ctrl = p_input.is_ctrl_pressed();

	for (auto& timer : timers)
		timer.move(p_delta_ms);

	// check if any button was clicked
	if (p_input.mouse_clicked())
		for (std::size_t i{ 0 }; i < buttons.size(); ++i)
			if (buttons[i].is_hit(p_input.mx(), p_input.my())) {
				try {
					button_click(i, p_rnd, p_project, p_gfx, p_input);
				}
				catch (const std::exception& ex) {
					p_gfx.add_toast_error(ex.what());
				}
				catch (...) {
					p_gfx.add_toast_error("Unknown error occurred");
				}
				return;
			}

	bool mouse_over_board_grid{ klib::util::is_p_in_rect(p_input.mx(), p_input.my(), BW_BX, BW_BY, BW_BW, BW_BW) };
	bool mouse_over_tile_picker{ klib::util::is_p_in_rect(p_input.mx(), p_input.my(), BW_TPX, BW_TPY, BW_TPW, BW_TPH) };

	// shift + home: to top of tile picker
	if (l_shift && p_input.is_pressed(SDL_SCANCODE_HOME))
		tile_row = 0;
	// home: go to first board
	else if (p_input.is_pressed(SDL_SCANCODE_HOME))
		board_ind = 0;
	// shift + end: to bottom of tile picker
	else if (l_shift && p_input.is_pressed(SDL_SCANCODE_END))
		tile_row = c_tile_row_max(p_project);
	// end: go to last board
	else if (p_input.is_pressed(SDL_SCANCODE_END))
		board_ind = p_project.get_board_count() - 1;
	// shift + PgDown: tile picker next page
	else if (l_shift && p_input.is_pressed(SDL_SCANCODE_PAGEDOWN))
		tile_row = klib::util::validate(tile_row + BW_TPC, 0, c_tile_row_max(p_project));
	// shift + PgUp: tile picker previous page
	else if (l_shift && p_input.is_pressed(SDL_SCANCODE_PAGEUP))
		tile_row = klib::util::validate(tile_row - BW_TPC, 0, c_tile_row_max(p_project));
	// ctrl+C: copy selection to clipboard
	else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_C))
		this->copy_to_clipboard(p_project);
	// ctrl+V: paste from clipboard
	else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_V)) {
		this->paste_from_clipboard(p_project);
		this->board_changed(p_rnd, p_project, p_gfx);
	}
	// ctrl+A: select all
	else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_A)) {
		this->sel_tile_x = 0;
		this->sel_tile_y = 0;
		this->sel_tile_2_x = c::MAP_W - 1;
		this->sel_tile_2_y = c::MAP_H - 1;
	}
	// shift+V: show where clipboard WOULD BE pasted
	else if (l_shift && p_input.is_pressed(SDL_SCANCODE_V))
		this->show_selection_rectangle();
	// delete: set all tiles in selection to 0 (internal -1)
	else if (p_input.is_pressed(SDL_SCANCODE_DELETE)) {
		this->clear_selection(p_project);
		this->board_changed(p_rnd, p_project, p_gfx);
	}
	// shift+Minus / PgDown: previous board
	else if ((l_shift && p_input.is_pressed(SDL_SCANCODE_KP_MINUS)) || p_input.is_pressed(SDL_SCANCODE_PAGEDOWN))
		board_ind = klib::util::validate(board_ind - 1, 0, p_project.get_board_count() - 1);
	// shift+Plus / PgUp: next board
	else if ((l_shift && p_input.is_pressed(SDL_SCANCODE_KP_PLUS)) || p_input.is_pressed(SDL_SCANCODE_PAGEUP))
		board_ind = klib::util::validate(board_ind + 1, 0, p_project.get_board_count() - 1);
	// ctrl+F: find selected board tile
	else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_F))
		this->center_offset(std::make_pair(sel_tile_x, sel_tile_y));
	// ctrl+P: go to previous tile of selected type on board (if shift held, previous tile of tile picker's type)
	else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_P))
		this->prev_tile(p_project, l_shift);
	// ctrl+H: center on player start position
	else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_H))
		this->center_offset(p_project.get_player_start_pos(board_ind));
	// shift+H: set selected board tile to player start position
	else if (l_shift && p_input.is_pressed(SDL_SCANCODE_H))
		p_project.set_player_start_position(board_ind, sel_tile_x, sel_tile_y);
	// ctrl+X: cut selection
	else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_X)) {
		this->cut_selection(p_project);
		this->board_changed(p_rnd, p_project, p_gfx);
	}
	// esc: clear selection rectangle
	else if (p_input.is_pressed(SDL_SCANCODE_ESCAPE))
		this->clear_secondary_selection();
	// ctrl+S: save to kzp (if shift held too; save to DAT)
	else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_S))
		this->save_boards_kzp(p_project, p_gfx, !l_shift);
	// ctrl+N: next tile of selected type (if shift held, next tile of tile picker's type - if any)
	else if (l_ctrl && p_input.is_pressed(SDL_SCANCODE_N))
		this->next_tile(p_project, l_shift);
	// B: toggle selected board tile desctructible property ([B]last)
	else if (p_input.is_pressed(SDL_SCANCODE_B))
		p_project.toggle_mt_blast(board_ind, sel_tile_x, sel_tile_y);
	// C: toggle selected board tile clip property ([C]lip)
	else if (p_input.is_pressed(SDL_SCANCODE_C))
		p_project.toggle_mt_inside(board_ind, sel_tile_x, sel_tile_y);
	// D: toggle selected board tile direction (horizontal/vertical), or if player start: n/e/s/w
	else if (p_input.is_pressed(SDL_SCANCODE_D))
		p_project.toggle_mt_direction(board_ind, sel_tile_x, sel_tile_y);
	// F: flip selection: horizontal flip, shift+F: vertical flip
	else if (p_input.is_pressed(SDL_SCANCODE_F)) {
		auto l_rect = this->get_selection_rectangle();
		if (l_shift)
			p_project.flip_vertical(board_ind, std::get<0>(l_rect), std::get<1>(l_rect), std::get<2>(l_rect), std::get<3>(l_rect));
		else
			p_project.flip_horizontal(board_ind, std::get<0>(l_rect), std::get<1>(l_rect), std::get<2>(l_rect), std::get<3>(l_rect));

		this->board_changed(p_rnd, p_project, p_gfx);
	}
	// R: rotate clipboard counter-clockwise, if shift held: rotate clockwise
	else if (p_input.is_pressed(SDL_SCANCODE_R)) {
		this->rotate_selection(p_project, l_shift);
		p_gfx.add_toast_ok("Clipboard rotated " + std::string(l_shift ? "clockwise" : "counter-clockwise"));
	}
	// ctrl+Plus / ctrl+mousewheel up on board grid: zoom in
	else if ((l_ctrl && p_input.is_pressed(SDL_SCANCODE_KP_PLUS)) || (mouse_over_board_grid && l_ctrl && p_input.mw_up())) {
		bool l_kp{ p_input.is_pressed(SDL_SCANCODE_KP_PLUS) };
		if (zoom_factor >= ZOOM_MAX)
			return;
		auto l_tcoords = l_kp ?
			this->get_pixel_pos(BW_BW / 2, BW_BW / 2) :
			this->get_pixel_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		this->move_grid_zoom(0.1f);
		this->translate_grid_offset(l_tcoords.first, l_tcoords.second,
			l_kp ? (BW_BW / 2) : (p_input.mx() - BW_BX),
			l_kp ? (BW_BW / 2) : (p_input.my() - BW_BY));
	}
	// ctrl+Minus / ctrl+mousewheel down on board grid: zoom out
	else if ((l_ctrl && p_input.is_pressed(SDL_SCANCODE_KP_MINUS)) || (mouse_over_board_grid && l_ctrl && p_input.mw_down())) {
		bool l_kp{ p_input.is_pressed(SDL_SCANCODE_KP_MINUS) };
		if (zoom_factor <= ZOOM_MIN)
			return;
		std::pair<int, int> l_tcoords = l_kp ?
			this->get_pixel_pos(BW_BW / 2, BW_BW / 2) :
			this->get_pixel_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		this->move_grid_zoom(-0.1f);
		this->translate_grid_offset(l_tcoords.first, l_tcoords.second,
			l_kp ? (BW_BW / 2) : (p_input.mx() - BW_BX),
			l_kp ? (BW_BW / 2) : (p_input.my() - BW_BY));
	}
	// mousewheel up over tile picker: scroll up
	else if (p_input.mw_up() && mouse_over_tile_picker)
		tile_row = klib::util::validate(tile_row - (l_ctrl ? 4 : 1), 0, c_tile_row_max(p_project));
	// mousewheel down over tile picker: scroll down
	else if (p_input.mw_down() && mouse_over_tile_picker)
		tile_row = klib::util::validate(tile_row + (l_ctrl ? 4 : 1), 0, c_tile_row_max(p_project));
	// up arrow / mousewheel up on board grid, scroll board grid up (ctrl for faster scroll)
	else if (p_input.is_pressed(SDL_SCANCODE_UP) || (mouse_over_board_grid && !l_shift && p_input.mw_up()))
		this->move_grid_offset_y(l_ctrl ? -4 * 64 : -64);
	// down arrow / mousewheel down on board grid, scroll board grid down (ctrl for faster scroll)
	else if (p_input.is_pressed(SDL_SCANCODE_DOWN) || (mouse_over_board_grid && !l_shift && p_input.mw_down()))
		this->move_grid_offset_y(l_ctrl ? 4 * 64 : 64);
	// left arrow / shift + mousewheel down on board grid, scroll board grid left (ctrl for faster scroll)
	else if (p_input.is_pressed(SDL_SCANCODE_LEFT) || (l_shift && p_input.mw_down()))
		this->move_grid_offset_x(l_ctrl ? -4 * 64 : -1 * 64);
	// right arrow / shift + mousewheel up on board grid, scroll board grid right (ctrl for faster scroll)
	else if (p_input.is_pressed(SDL_SCANCODE_RIGHT) || (l_shift && p_input.mw_up()))
		this->move_grid_offset_x(l_ctrl ? 4 * 64 : 1 * 64);

	// right click on board grid: paint (set board grid tile to selected tile picker tile)
	else if (p_input.mouse_held(false) && mouse_over_board_grid) {
		auto l_tcoords = this->get_tile_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);

		int l_stile_no{ this->get_selected_tile_no(p_project) };
		int t_tile_no = p_project.get_board(board_ind).get_tile_no(l_tcoords.first, l_tcoords.second);

		if (l_stile_no == -2)
			p_project.set_player_start_position(board_ind, l_tcoords.first, l_tcoords.second);
		else if (l_stile_no != t_tile_no) {
			p_project.set_tile(this->board_ind, l_tcoords.first, l_tcoords.second, this->get_selected_tile(p_project, l_stile_no));
			this->board_changed(p_rnd, p_project, p_gfx);
		}
	}
	// mouse down on board grid, no shift: select clicked tile
	// if ctrl held: "color picker" (make tile picker tile equal to selected board tile)
	else if (mouse_over_board_grid && p_input.mouse_held(true) && !l_shift) {
		auto l_tcoords = this->get_tile_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		sel_tile_x = l_tcoords.first;
		sel_tile_y = l_tcoords.second;
		this->clear_secondary_selection();

		if (l_ctrl) {
			int l_sel_tile_no = p_project.get_board(board_ind).get_tile_no(sel_tile_x, sel_tile_y);
			int l_index = p_project.get_tile_picker_index(l_sel_tile_no);

			if (l_index >= 0) {
				this->tile_x = l_index % 14; // TODO: Replace with constants
				this->tile_y = l_index / 14;
			}
		}

	}
	// shift + board grid mouse down; make selection rectangle
	else if (mouse_over_board_grid && p_input.mouse_held(true) && l_shift) {
		auto l_tcoords = this->get_tile_pos(p_input.mx() - BW_BX, p_input.my() - BW_BY);
		sel_tile_2_x = l_tcoords.first;
		sel_tile_2_y = l_tcoords.second;
	}
	// minimap clicked, change board grid offset to match
	else if (p_input.mouse_held() &&
		klib::util::is_p_in_rect(p_input.mx(), p_input.my(), BW_MX, BW_MY, BW_MW, BW_MW))
		this->click_minimap(p_input.mx() - BW_MX, p_input.my() - BW_MY);
	// tile picker clicked, select tile
	else if (p_input.mouse_held() && mouse_over_tile_picker)
		this->click_tile_picker(p_project, p_input.mx() - BW_TPX, p_input.my() - BW_TPY);

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
	int l_bpw = c_bb_pixel_width();

	bool l_bx = board_px >= c_max_offset() + l_bpw / 2;
	bool l_by = board_py >= c_max_offset() + l_bpw / 2;

	this->set_grid_offset(board_px - (l_bx ? 0 : l_bpw / 2), board_py - (l_by ? 0 : l_bpw / 2));
}

void kkit::Board_window::center_offset(std::pair<int, int> p_coords) {
	board_px = c::WALL_IMG_W * p_coords.first;
	board_py = c::WALL_IMG_W * p_coords.second;
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
		for (int j{ 0 }; j < 64; ++j) {
			//if (!board.is_empty_tile(i, j)) {
			int l_tile_no = board.get_tile_no(i, j);

			bool l_flash = toggles[1] && board.is_blast(i, j);

			if (l_tile_no >= 0)
				l_flash |= toggles[2] && board.is_inside(i, j) && (!p_project.is_inside(l_tile_no) || p_project.is_clip_override(l_tile_no));
			else
				l_flash |= toggles[2] && !board.is_inside(i, j);

			l_flash |= toggles[3] && (l_tile_no == get_selected_tile_no(p_project));

			if (l_tile_no >= 0)
				klib::gfx::blit(p_rnd, p_gfx.get_tile_texture(l_tile_no), 64 * i, 64 * j);

			if (toggles[0] && p_project.is_directional(l_tile_no))
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
	klib::gfx::draw_rect(p_rnd, sel_tile_x * 64, sel_tile_y * 64, 64, 64,
		kkit::gfx::get_pulse_color(0, timers[1].get_frame()),
		4);

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
	//klib::gfx::draw_rect(p_rnd, p_x, p_y, BW_MW, BW_MW, SDL_Color{ 0,0,0 }, 0);

	int l_sel_factor = static_cast<int>(16.0f / zoom_factor);

	int l_x = p_x + static_cast<int>(board_px * (128.0f / 4096.0f));
	int l_y = p_y + static_cast<int>(board_py * (128.0f / 4096.0f));

	klib::gfx::draw_rect(p_rnd, l_x, l_y, l_sel_factor, l_sel_factor, SDL_Color{ 255,255,0 }, 2);
}

void kkit::Board_window::draw_tile_picker(SDL_Renderer* p_rnd, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx, int p_x, int p_y) const {
	klib::gfx::draw_rect(p_rnd, p_x, p_y, BW_TPW, BW_TPH,
		kkit::gfx::get_pulse_color(2, timers[1].get_frame()),
		0);

	for (int i{ tile_row * BW_TPR }; i < static_cast<int>(p_project.get_tile_picker().size()) && i < (BW_TPR * (BW_TPC + tile_row)); ++i) {
		int l_y = (i - tile_row * BW_TPR) / BW_TPR;
		int l_x = (i - tile_row * BW_TPR) % BW_TPR;
		int l_index = p_project.get_tile_picker()[i];
		if (l_index >= 0)
			klib::gfx::blit_p2_scale(p_rnd, p_gfx.get_tile_texture(l_index), p_x + l_x * BW_TP_TW, p_y + l_y * BW_TP_TW, -1);
		else if (l_index == -2)
			klib::gfx::blit_p2_scale(p_rnd, p_gfx.get_app_texture(2), p_x + l_x * BW_TP_TW, p_y + l_y * BW_TP_TW, -1);
	}

	if (tile_y >= tile_row && tile_y < tile_row + BW_TPC)
		klib::gfx::draw_rect(p_rnd, p_x + 32 * tile_x, p_y + 32 * (tile_y - tile_row), 32, 32,
			kkit::gfx::get_pulse_color(4, timers[1].get_frame()),
			4);

	std::string l_tile_md;
	int l_tile_no = p_project.get_tile_picker().at(tile_y * BW_TPR + tile_x);

	if (l_tile_no >= 0) {
		bool l_clip = p_project.is_inside(l_tile_no);
		bool l_blast = p_project.is_blast(l_tile_no);
		l_tile_md = std::to_string(l_tile_no + 1) + ":" + (l_blast ? "Destruct" : "Nodestruct") + "," + (l_clip ? "Clip" : "Noclip") + "," + p_project.get_block_type_as_string(l_tile_no);
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

int kkit::Board_window::get_selected_tile_no(const kkit::Project& p_project) const {
	return p_project.get_tile_picker().at(tile_y * BW_TPR + tile_x);
}

int kkit::Board_window::get_selected_board_tile_no(const kkit::Project& p_project) const {
	return p_project.get_board(board_ind).get_tile_no(sel_tile_x, sel_tile_y);
}

int kkit::Board_window::get_mouseover_tile_no(const kkit::Project& p_project, int p_x, int p_y) const {
	int l_x = p_x / BW_TP_TW;
	int l_y = p_y / BW_TP_TW;
	int l_index = (l_y + tile_row) * BW_TPR + l_x;

	if (l_index < p_project.get_tile_picker().size())
		return p_project.get_tile_picker()[l_index];
	else
		return -3;
}

void kkit::Board_window::click_tile_picker(const kkit::Project& p_project, int p_x, int p_y) {
	int l_x = p_x / BW_TP_TW;
	int l_y = p_y / BW_TP_TW + tile_row;
	int l_index = l_y * BW_TPR + l_x;
	if (l_index < p_project.get_tile_picker().size() && p_project.get_tile_picker()[l_index] >= -2) {
		this->tile_x = l_x;
		this->tile_y = l_y;
	}
}

int  kkit::Board_window::c_bb_pixel_width(void) const {
	return static_cast<int>(8.0f * 64.0f / zoom_factor);
}

int kkit::Board_window::c_bb_tile_pixel_width(void) const {
	return static_cast<int>(64.0f / zoom_factor);
}

int kkit::Board_window::c_tile_row_max(const kkit::Project& p_project) const {
	return std::max(0, (static_cast<int>(p_project.get_tile_picker().size()) % BW_TPR == 0 ? 0 : 1) + static_cast<int>(p_project.get_tile_picker().size()) / BW_TPR - BW_TPC);
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

bool kkit::Board_window::is_empty_selection(const kkit::Project& p_project) const {
	auto l_rect = this->get_selection_rectangle();
	const auto& l_brd = p_project.get_board(board_ind);

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

void kkit::Board_window::copy_to_clipboard(const kkit::Project& p_project, bool p_clear_secondary) {
	auto l_rect = this->get_selection_rectangle();

	this->clipboard = p_project.get_board(board_ind).get_rectangle(std::get<0>(l_rect), std::get<1>(l_rect), std::get<2>(l_rect), std::get<3>(l_rect));
	if (p_clear_secondary)
		this->clear_secondary_selection();
}

void kkit::Board_window::cut_selection(kkit::Project& p_project) {
	if (!this->is_empty_selection(p_project)) {
		this->copy_to_clipboard(p_project, false);
		this->clear_selection(p_project);
		this->clear_secondary_selection();
	}
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

void kkit::Board_window::clear_secondary_selection(void) {
	this->sel_tile_2_x = -1;
}

void kkit::Board_window::rotate_selection(const kkit::Project& p_project, bool p_clockwise) {
	if (this->clipboard.empty())
		return;

	std::vector<std::vector<kkit::Map_tile>> result;

	if (p_clockwise) {
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
				l_row.push_back(clipboard[static_cast<int>(clipboard.size()) - i - 1][j]);
			}
			result.push_back(l_row);
		}
	}

	for (auto& l_col : result)
		for (auto& l_tile : l_col)
			if (l_tile.get_tile_no() >= 0 && p_project.is_directional(l_tile.get_tile_no()))
				l_tile.toggle_direction();

	clipboard = result;
}


// save/load
bool kkit::Board_window::bmp_export(kkit::Project& p_project, int p_board_no) const {
	try {
		kkit::gfx::project_map_to_bmp(p_project, p_board_no);
		return true;
	}
	catch (const std::exception&) {
		return false;
	}
}

bool kkit::Board_window::xml_import(kkit::Project& p_project, int p_board_no) const {
	auto l_in_file = p_project.get_file_full_path(c::FILE_BOARDS, c::FILE_EXT_XML, p_board_no);

	if (!std::filesystem::exists(l_in_file))
		return false;
	else {
		p_project.reload_map_from_xml(p_board_no);
		return true;
	}
}

void kkit::Board_window::xml_export(const kkit::Project& p_project, int p_board_no) const {
	p_project.save_board_xml(p_board_no);
}


void kkit::Board_window::save_boards_kzp(const kkit::Project& p_project, kkit::Project_gfx& p_gfx, bool p_compress) const {
	bool l_compress = (p_compress && !p_project.is_walken() && !p_project.is_klab_v_1());

	int l_bytes = p_project.save_boards_kzp(l_compress);
	int l_board_count(p_project.get_board_count());
	int l_original_bytes = l_board_count * (p_project.is_walken() ? 1 : 2) * c::MAP_W * c::MAP_H;

	// only v2.x boards should be saved as kzp - there is no real point otherwise
	if (l_compress)
		p_gfx.add_toast_ok(std::to_string(l_board_count) + " boards saved to KZP (" +
			std::to_string(l_bytes) + " bytes, " + std::to_string(l_original_bytes) + " original)");
	else
		p_gfx.add_toast_ok(std::to_string(l_board_count) + " boards saved to DAT (" +
			std::to_string(l_bytes) + " bytes)");
}

// board calculations
int kkit::Board_window::count_tiles(const kkit::Project& p_project, int p_tile_no, bool p_all_boards) const {
	int result{ 0 };

	for (int i{ p_all_boards ? 0 : board_ind }; i < (p_all_boards ? p_project.get_board_count() : board_ind + 1); ++i) {
		const auto& l_brd = p_project.get_board(i);
		for (int x{ 0 }; x < c::MAP_W; ++x)
			for (int y{ 0 }; y < c::MAP_H; ++y)
				if (l_brd.get_tile_no(x, y) == p_tile_no)
					++result;
	}

	return result;
}

void kkit::Board_window::next_tile(const kkit::Project& p_project, bool p_tp_tile, bool p_wrap) {
	const auto& l_brd{ p_project.get_board(board_ind) };
	int l_tile_no = p_tp_tile ? this->get_selected_tile_no(p_project) : this->get_selected_board_tile_no(p_project);
	bool l_first{ true };

	for (int x{ p_wrap ? 0 : sel_tile_x }; x < c::MAP_W; ++x) {
		for (int y{ l_first && !p_wrap ? sel_tile_y + 1 : 0 }; y < c::MAP_H; ++y)
			if (l_brd.get_tile_no(x, y) == l_tile_no) {
				sel_tile_x = x;
				sel_tile_y = y;
				this->clear_secondary_selection();
				this->center_offset(std::make_pair(sel_tile_x, sel_tile_y));
				return;
			}
		l_first = false;
	}

	// no tile found, wrap around
	if (!p_wrap)
		this->next_tile(p_project, p_tp_tile, true);

	// no tile found, but wrap was already turned on, return with no changes
}

// pretty much a lazy copy/past of next_tile
void kkit::Board_window::prev_tile(const kkit::Project& p_project, bool p_tp_tile, bool p_wrap) {
	const auto& l_brd{ p_project.get_board(board_ind) };
	int l_tile_no = p_tp_tile ? this->get_selected_tile_no(p_project) : this->get_selected_board_tile_no(p_project);
	bool l_first{ true };

	for (int x{ p_wrap ? c::MAP_W - 1 : sel_tile_x }; x >= 0; --x) {
		for (int y{ l_first && !p_wrap ? sel_tile_y - 1 : c::MAP_H - 1 }; y >= 0; --y)
			if (l_brd.get_tile_no(x, y) == l_tile_no) {
				sel_tile_x = x;
				sel_tile_y = y;
				this->clear_secondary_selection();
				this->center_offset(std::make_pair(sel_tile_x, sel_tile_y));
				return;
			}
		l_first = false;
	}

	// no tile found, wrap around
	if (!p_wrap)
		this->prev_tile(p_project, p_tp_tile, true);

	// no tile found, but wrap was already turned on, return with no changes
}

void kkit::Board_window::board_changed(SDL_Renderer* p_rnd, const kkit::Project& p_project, kkit::Project_gfx& p_gfx) {
	p_gfx.reload_minimap_texture(p_rnd, p_project, this->board_ind);
}
