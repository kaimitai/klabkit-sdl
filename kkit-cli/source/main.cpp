#include <iostream>
#include "klabkit-cli/kkit_cli_constants.h"
#include "klabkit-cli/kkit_cli_util.h"

int main(int argc, char* argv[]) {
	const std::string l_folder{ "C:/Users/Kai/Downloads/klabkit" };
	const std::string l_walken_folder{ "C:/Users/Kai/Downloads/klabkit/versions/walken" };

	kkit_cli::decompress_walls_walken(l_walken_folder, 80);

	//compress_boards(l_folder, 30, false);
	//decompress_boards(l_folder, 30, false);
	//compress_walls(l_folder, 448, false);
	//decompress_walls(l_folder, 448, false);
	//compress_lab3d(l_folder);
	//decompress_lab3d(l_folder);
	//compress_story(l_folder);
}
