#include "Gfx_window.h"
#include "kkit_gfx.h"
#include "xml_handler.h"
#include "./../klib/gfx.h"
#include "./../klib/klib_util.h"

kkit::Gfx_window::Gfx_window(void) : tile_row{ 0 }, tile_x{ 0 }, tile_y{ 0 } {
	// pulse color timer
	timers.push_back(klib::Timer(256, 5));

	for (int i{ 0 }; i < 3; ++i)
		buttons.push_back(klib::Button("", GW_AB_X, GW_AB_Y + i * GW_AB_H + (i + 1) * GW_AB_SPACING, GW_AB_W, GW_AB_H));

	buttons.push_back(klib::Button("Exp BMP", GW_BMP_X, GW_BMP_Y, GW_BMP_W));
	buttons.push_back(klib::Button("Imp BMP", GW_AB_X + GW_AB_W - GW_BMP_W, GW_BMP_Y, GW_BMP_W));
	buttons.push_back(klib::Button("Exp XML", GW_BMP_X, GW_BMP_Y + GW_BMP_H + GW_AB_SPACING, GW_BMP_W));
	buttons.push_back(klib::Button("Imp XML", GW_AB_X + GW_AB_W - GW_BMP_W, GW_BMP_Y + GW_BMP_H + GW_AB_SPACING, GW_BMP_W));

	buttons.push_back(klib::Button("Save KZP", GW_SAV_X, GW_SAV_Y, GW_SAV_W));
	buttons.push_back(klib::Button("Save DAT", GW_SAV_X, GW_SAV_Y + GW_SAV_H + GW_AB_SPACING, GW_SAV_W));
}

void kkit::Gfx_window::move(SDL_Renderer* p_rnd, const klib::User_input& p_input, int p_delta_ms, kkit::Project& p_project, kkit::Project_gfx& p_gfx) {
	bool l_ctrl = p_input.is_ctrl_pressed();

	for (auto& timer : timers)
		timer.move(p_delta_ms);

	if (p_input.mouse_clicked()) {
		for (std::size_t i{ 0 }; i < buttons.size(); ++i)
			if (buttons[i].is_hit(p_input.mx(), p_input.my())) {
				try {
					this->button_click(p_rnd, i, p_project, p_gfx);
				}
				catch (const std::exception& ex) {
					p_gfx.add_toast_error(ex.what());
				}
				catch (...) {
					p_gfx.add_toast_error("Unknown error occurred");
				}

				return;
			}
	}

	if (p_input.mw_down())
		tile_row = klib::util::validate(tile_row + (l_ctrl ? 5 : 1), 0, c_max_tile_row(p_project.get_wall_image_count()));
	else if (p_input.mw_up())
		tile_row = klib::util::validate(tile_row - (l_ctrl ? 5 : 1), 0, c_max_tile_row(p_project.get_wall_image_count()));
	else if (p_input.mouse_held() &&
		klib::util::is_p_in_rect(p_input.mx(), p_input.my(), GW_TX, GW_TY, GW_TH, GW_TW)) {
		int l_x = (p_input.mx() - GW_TX) / c::WALL_IMG_W;
		int l_y = (p_input.my() - GW_TY) / c::WALL_IMG_H;

		int l_new_index = (l_y + tile_row) * GW_TTPR + l_x;
		if (l_new_index < p_project.get_wall_image_count()) {
			tile_x = l_x;
			tile_y = l_y + tile_row;
		}
	}
	// save kzp
	else if (p_input.is_ctrl_pressed() && p_input.is_pressed(SDL_SCANCODE_S)) {
		this->button_click(p_rnd, 7, p_project, p_gfx);
	}
}

void kkit::Gfx_window::draw(SDL_Renderer* p_rnd, const klib::User_input& p_input, const kkit::Project& p_project, const kkit::Project_gfx& p_gfx) const {
	int l_selected_tile_no = get_selected_tile_no();
	bool l_inside = p_project.is_inside(l_selected_tile_no);
	bool l_blast = p_project.is_blast(l_selected_tile_no);
	std::string l_wall_type = p_project.get_block_type_as_string(l_selected_tile_no);

	buttons[0].draw(p_rnd, p_gfx.get_font(), p_input, l_wall_type, klib::gc::COL_ORANGE);
	buttons[1].draw(p_rnd, p_gfx.get_font(), p_input,
		l_inside ? "Clip" : "Noclip",
		l_inside ? klib::gc::COL_GREEN : klib::gc::COL_RED);
	buttons[2].draw(p_rnd, p_gfx.get_font(), p_input,
		l_blast ? "Destruct" : "Nodestruct",
		l_blast ? klib::gc::COL_GREEN : klib::gc::COL_RED);

	for (std::size_t i{ 3 }; i < buttons.size(); ++i)
		buttons[i].draw(p_rnd, p_gfx.get_font(), p_input);

	// draw tile selector window
	klib::gfx::draw_window(p_rnd, p_gfx.get_font(), "Tiles", GW_TX - 1, GW_TY - (1 + klib::gc::BUTTON_H), GW_TW + 2, GW_TH + 4 + klib::gc::BUTTON_H);

	for (int j{ 0 }; j < GW_TTPC; ++j)
		for (int i{ 0 }; i < GW_TTPR; ++i) {
			int l_index = (tile_row + j) * GW_TTPR + i;
			if (l_index >= p_project.get_wall_image_count())
				break;
			else {
				klib::gfx::blit(p_rnd, p_gfx.get_tile_texture(l_index),
					GW_TX + i * c::WALL_IMG_W, GW_TY + j * c::WALL_IMG_W);
			}
		}

	// draw selected tile
	klib::gfx::draw_window(p_rnd, p_gfx.get_font(),
		"Tile #" + std::to_string(l_selected_tile_no + 1),
		GW_SX - 1, GW_SY - (1 + klib::gc::BUTTON_H), GW_SW + 2, GW_SH + 4 + klib::gc::BUTTON_H);

	klib::gfx::draw_rect(p_rnd, GW_SX, GW_SY, GW_SW, GW_SH,
		kkit::gfx::get_pulse_color(2, timers[0].get_frame()),
		0);

	klib::gfx::blit_p2_scale(p_rnd, p_gfx.get_tile_texture(l_selected_tile_no),
		GW_SX, GW_SY, 2);

	if (tile_y >= tile_row && tile_y < tile_row + GW_TTPC) {
		int l_rel_y = tile_y - tile_row;
		klib::gfx::draw_rect(p_rnd, GW_TX + tile_x * c::WALL_IMG_W, GW_TY + l_rel_y * c::WALL_IMG_H, c::WALL_IMG_W, c::WALL_IMG_H,
			kkit::gfx::get_pulse_color(0, timers[0].get_frame()),
			3);
	}
}

int kkit::Gfx_window::get_selected_tile_no(void) const {
	return (tile_x + tile_y * GW_TTPR);
}

int kkit::Gfx_window::c_max_tile_row(int p_total_tile_count) const {
	int l_max_y = (p_total_tile_count / GW_TTPR) - GW_TTPC;
	if (p_total_tile_count % GW_TTPR != 0)
		l_max_y += 1;

	return (l_max_y < 0 ? 0 : l_max_y);
}

void kkit::Gfx_window::button_click(SDL_Renderer* p_rnd, std::size_t p_button_no, kkit::Project& p_project, kkit::Project_gfx& p_gfx) {
	int l_tile_no = this->get_selected_tile_no();

	if (p_button_no == 0)
		p_project.toggle_wt_type(l_tile_no);
	else if (p_button_no == 1)
		p_project.toggle_wt_inside(l_tile_no);
	else if (p_button_no == 2)
		p_project.toggle_wt_blast(l_tile_no);
	else if (p_button_no == 3) {
		auto l_out_file = p_project.get_bmp_file_path(c::FILE_WALLS, l_tile_no);
		if (gfx::wall_to_bmp(p_project.get_wall(l_tile_no).get_image(),
			p_project.get_palette(),
			p_project.get_bmp_folder(),
			l_out_file))
			p_gfx.add_toast_ok("Saved " + l_out_file);
		else
			p_gfx.add_toast_error("Could not save " + l_out_file);
	}
	else if (p_button_no == 4) {
		auto l_in_file = p_project.get_bmp_file_path(c::FILE_WALLS, l_tile_no);
		auto l_img = kkit::gfx::load_bmp(p_project.get_palette(), l_in_file);
		p_project.set_wall_image(l_tile_no, l_img);
		p_gfx.reload_texture(p_rnd, p_project, l_tile_no);
		p_gfx.add_toast_ok("Loaded " + l_in_file);
	}
	// export XML
	else if (p_button_no == 5) {
		auto l_dir = p_project.get_file_directory(c::FILE_EXT_XML, l_tile_no);
		auto l_file = p_project.get_file_name(c::FILE_WALLS, c::FILE_EXT_XML, l_tile_no);
		kkit::xml::save_wall_xml(p_project.get_wall(this->get_selected_tile_no()), l_dir, l_file);
		p_gfx.add_toast_ok("Wall tile saved to " + l_file);
	}
	// import xml
	else if (p_button_no == 6) {
		p_project.reload_wall_from_xml(l_tile_no);
		p_gfx.reload_texture(p_rnd, p_project, l_tile_no);
		p_gfx.add_toast_ok("Loaded " + p_project.get_file_name(c::FILE_WALLS, c::FILE_EXT_XML, l_tile_no));
	}
	// save kzp
	else if (p_button_no == 7) {
		int l_bytes = p_project.save_walls_kzp();
		int l_wall_count(p_project.get_wall_image_count());
		int l_original_bytes = l_wall_count * c::WALL_IMG_W * c::WALL_IMG_H;
		p_gfx.add_toast_ok(std::to_string(l_wall_count) + " wall tiles saved to KZP (" +
			std::to_string(l_bytes) + " bytes, " + std::to_string(l_original_bytes) + " original)");
	}
	// save dat
	else if (p_button_no == 8) {
		int l_bytes = p_project.save_walls_kzp(false);
		int l_wall_count(p_project.get_wall_image_count());
		int l_original_bytes = l_wall_count * c::WALL_IMG_W * c::WALL_IMG_H;
		p_gfx.add_toast_ok(std::to_string(l_wall_count) + " wall tiles saved to DAT (" +
			std::to_string(l_bytes) + " bytes)");
	}
}
