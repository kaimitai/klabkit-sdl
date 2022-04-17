#include <iostream>

#include "./klabkit/Project.h"
#include "./klabkit/kkit_gfx.h"
#include "klib/file.h"

int main(int argc, char* args[]) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
	}

	kkit::Project project("C:/Users/Kai/Downloads/klabkit");

	//kkit::gfx::project_walls_to_bmps(project);

	//for (int i{ 0 }; i < 30; ++i) kkit::gfx::project_map_to_bmp(project, i);

	//auto v = project.get_board_bytes();

	auto v = project.get_wall_bytes();
	klib::file::write_bytes_to_file(v, "C:/Users/Kai/Downloads/klabkit/out.dat");

	return 0;
}
