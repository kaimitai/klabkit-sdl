#include <iostream>

#include "./klabkit/Project.h"
#include "./klabkit/kkit_gfx.h"

int main(int argc, char* args[]) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
	}

	kkit::Project project("C:/Users/Kai/Downloads/klabkit");

	//kkit::gfx::project_walls_to_bmps(project);

	for (int i{ 0 }; i < 30; ++i)
		kkit::gfx::project_map_to_bmp(project, i);

	return 0;
}
