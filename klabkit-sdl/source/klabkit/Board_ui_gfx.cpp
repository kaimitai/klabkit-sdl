#include <algorithm>
#include <cmath>
#include <filesystem>
#include "./Board_ui.h"
#include "./../imgui/imgui.h"
#include "./../imgui/imgui_impl_sdl.h"
#include "./../imgui/imgui_impl_sdlrenderer.h"
#include "imgui_helper.h"
#include "./xml_handler.h"
#include "./constants_color.h"
#include "./../klib/gfx.h"
#include "./../klib/klib_util.h"
#include "./kkit_gfx.h"

bool kkit::Board_ui::bmp_export(kkit::Project& p_project, int p_wall_no) const {
	auto l_out_file = p_project.get_file_full_path(c::FILE_WALLS, c::FILE_EXT_BMP, p_wall_no);
	return gfx::wall_to_bmp(p_project.get_wall(p_wall_no).get_image(),
		p_project.get_palette(),
		p_project.get_bmp_folder(),
		l_out_file);
}

bool kkit::Board_ui::bmp_import(SDL_Renderer* p_rnd, kkit::Project& p_project, kkit::Project_gfx& p_gfx, int p_wall_no) const {
	auto l_in_file = p_project.get_file_full_path(c::FILE_WALLS, c::FILE_EXT_BMP, p_wall_no);

	if (!std::filesystem::exists(l_in_file))
		return false;
	else {
		auto l_img = kkit::gfx::load_bmp(p_project.get_palette(), l_in_file);
		p_project.set_wall_image(p_wall_no, l_img);
		p_gfx.reload_texture(p_rnd, p_project, p_wall_no);
		return true;
	}
}

void kkit::Board_ui::xml_export_wall(kkit::Project& p_project, int p_wall_no) const {
	auto l_dir = p_project.get_file_directory(c::FILE_EXT_XML, p_wall_no);
	auto l_file = p_project.get_file_name(c::FILE_WALLS, c::FILE_EXT_XML, p_wall_no);
	kkit::xml::save_wall_xml(p_project.get_wall(p_wall_no), l_dir, l_file, p_project.get_config_label());
}

bool kkit::Board_ui::xml_import_wall(SDL_Renderer* p_rnd, kkit::Project& p_project, kkit::Project_gfx& p_gfx, int p_wall_no) const {
	auto l_in_file = p_project.get_file_full_path(c::FILE_WALLS, c::FILE_EXT_XML, p_wall_no);

	if (!std::filesystem::exists(l_in_file))
		return false;
	else {
		p_project.reload_wall_from_xml(p_wall_no);
		p_gfx.reload_texture(p_rnd, p_project, p_wall_no);
		return true;
	}
}
