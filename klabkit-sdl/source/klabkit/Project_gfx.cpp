#include "Project_gfx.h"
#include "kkit_gfx.h"

kkit::Project_gfx::Project_gfx(SDL_Renderer* p_rnd, const kkit::Project& p_project) : font(p_rnd, std::vector<byte>(begin(FONT_BYTES), end(FONT_BYTES)), 16, 22) {
	this->textures[INDEX_WALL_TEXTURES] = kkit::gfx::get_project_textures(p_rnd, p_project);
	this->textures[INDEX_APP_TEXTURES] = kkit::gfx::get_program_textures(p_rnd, p_project);
	this->textures[INDEX_MM_TEXTURES] = kkit::gfx::get_minimap_textures(p_rnd, p_project);
	this->load_background_textures(p_rnd);
}

kkit::Project_gfx::~Project_gfx(void) {

	// destroy all textures
	for (auto& kv : this->textures)
		for (auto& txt : kv.second)
			if (txt != nullptr)
				SDL_DestroyTexture(txt);
}

void kkit::Project_gfx::load_background_textures(SDL_Renderer* p_rnd) {
	this->textures[INDEX_BG_TEXTURES] = kkit::gfx::create_bg_textures(p_rnd);
}

void kkit::Project_gfx::reload_texture(SDL_Renderer* p_rnd, const kkit::Project& p_project, int p_frame_no) {
	auto l_texture = kkit::gfx::get_project_texture(p_rnd, p_project, p_frame_no);
	if (this->textures[0].at(p_frame_no) != nullptr)
		SDL_DestroyTexture(this->textures[0].at(p_frame_no));
	this->textures[0].at(p_frame_no) = l_texture;
}

void kkit::Project_gfx::reload_minimap_texture(SDL_Renderer* p_rnd, const kkit::Project& p_project, int p_frame_no) {
	auto l_texture = kkit::gfx::create_board_minimap_texture(p_rnd, p_project, p_frame_no);
	if (this->textures[INDEX_MM_TEXTURES].at(p_frame_no) != nullptr)
		SDL_DestroyTexture(this->textures[INDEX_MM_TEXTURES].at(p_frame_no));

	this->textures[INDEX_MM_TEXTURES].at(p_frame_no) = l_texture;
}

SDL_Texture* kkit::Project_gfx::get_tile_texture(int p_frame_no) const {
	return this->textures.at(INDEX_WALL_TEXTURES).at(p_frame_no);
}

SDL_Texture* kkit::Project_gfx::get_app_texture(int p_frame_no) const {
	return this->textures.at(INDEX_APP_TEXTURES).at(p_frame_no);
}

SDL_Texture* kkit::Project_gfx::get_bg_texture(int p_frame_no) const {
	return this->textures.at(INDEX_BG_TEXTURES).at(p_frame_no);
}

SDL_Texture* kkit::Project_gfx::get_minimap_texture(int p_frame_no) const {
	return this->textures.at(INDEX_MM_TEXTURES).at(p_frame_no);
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
