#include "Project_gfx.h"
#include "kkit_gfx.h"

kkit::Project_gfx::Project_gfx(SDL_Renderer* p_rnd, const kkit::Project& p_project) : font(p_rnd, std::vector<byte>(begin(FONT_BYTES), end(FONT_BYTES)), 16, 22) {
	this->textures[INDEX_WALL_TEXTURES] = kkit::gfx::get_project_textures(p_rnd, p_project);
	this->textures[1] = kkit::gfx::get_program_textures(p_rnd, p_project);
}

kkit::Project_gfx::~Project_gfx(void) {

	// destroy all textures
	for (auto& kv : this->textures)
		for (auto& txt : kv.second)
			if (txt != nullptr)
				SDL_DestroyTexture(txt);
}

SDL_Texture* kkit::Project_gfx::get_tile_texture(int p_frame_no) const {
	return this->textures.at(INDEX_WALL_TEXTURES).at(p_frame_no);
}

SDL_Texture* kkit::Project_gfx::get_app_texture(int p_frame_no) const {
	return this->textures.at(INDEX_APP_TEXTURES).at(p_frame_no);
}

const klib::Font& kkit::Project_gfx::get_font(void) const {
	return this->font;
}

void kkit::Project_gfx::move(int p_delta_ms) {
	this->toasts.move(p_delta_ms);
}

void kkit::Project_gfx::draw(SDL_Renderer* p_rnd) const {
	this->toasts.draw(p_rnd, this->font);
}

void kkit::Project_gfx::add_toast_ok(const std::string& p_msg) {
	toasts.add_toast_ok(p_msg);
}

void kkit::Project_gfx::add_toast_info(const std::string& p_msg) {
	toasts.add_toast_info(p_msg);
}

void kkit::Project_gfx::add_toast_error(const std::string& p_msg) {
	toasts.add_toast_error(p_msg);
}
