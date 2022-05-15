#include <iostream>
#include "klabkit-cli/kkit_cli_constants.h"
#include "klabkit-cli/kkit_cli_util.h"
#include "klabkit-cli/Cli_parser.h"

void output_header(void);
void output_help(void);
std::string to_lowercase(const std::string& p_arg);

int main(int argc, char* argv[]) try {
	output_header();

	if (argc < 2) {
		output_help();
		return 0;
	}

	std::vector<std::string> l_arguments;
	for (int i = 1; i < argc; ++i)
		l_arguments.push_back(to_lowercase(argv[i]));

	kkit_cli::Cli_parser parser(l_arguments);

	std::string l_cmp_type = parser.get_compress_type();
	std::string l_folder = parser.get_folder();
	int l_bcount = parser.get_board_count();
	int l_wcount = parser.get_wall_count();
	int l_version = parser.get_version();
	bool l_compress = parser.is_compress();

	bool l_is_klab_v1 = (l_version == 10 || l_version == 11);
	bool l_is_walken = (l_version == 0);

	std::cout << "Using the following configuration:\nVersion: " << l_version
		<< "\nFolder: " << l_folder
		<< "\nBoard count: " << l_bcount
		<< "\nTile count: " << l_wcount
		<< "\n\n" << (l_compress ? "Compress " : "Decompress ") << l_cmp_type << "\n\n";

	const std::string ARG_ALL{ "all" };
	const std::string ARG_BOARDS{ "boards" };
	const std::string ARG_WALLS{ "walls" };
	const std::string ARG_STORY{ "story" };
	const std::string ARG_LAB3D{ "lab3d" };
	const std::string ARG_SOUNDS{ "sounds" };
	const std::string ARG_SONGS{ "songs" };

	// compress one or all
	if (l_compress) {
		// compress walls - special case for walken
		if (l_cmp_type == ARG_ALL || l_cmp_type == ARG_WALLS)
			if (!l_is_walken)
				kkit_cli::compress_walls(l_folder, l_wcount, l_is_klab_v1);
			else
				kkit_cli::compress_walls_walken(l_folder, l_wcount);

		// nothing else can be compressed for walken
		if (!l_is_walken) {

			// compress songs
			if (!l_is_walken && (l_cmp_type == ARG_ALL || l_cmp_type == ARG_SONGS))
				kkit_cli::compress_songs(l_folder);

			// compress sounds
			if (!l_is_walken && (l_cmp_type == ARG_ALL || l_cmp_type == ARG_SOUNDS))
				kkit_cli::compress_sounds(l_folder);

			// compress story
			if (!l_is_walken && (l_cmp_type == ARG_ALL || l_cmp_type == ARG_STORY))
				kkit_cli::compress_story(l_folder);

			// nothing else can be compressed for klab v1
			if (!l_is_klab_v1) {

				// compress boards
				if (l_cmp_type == ARG_ALL || l_cmp_type == ARG_BOARDS)
					kkit_cli::compress_boards(l_folder, l_bcount, l_is_klab_v1);

				// compress lab3d
				if (l_cmp_type == ARG_ALL || l_cmp_type == ARG_LAB3D)
					kkit_cli::compress_lab3d(l_folder);

			}
		}
	}
	// decompress one or all
	else {

		// decompress walls - special case for walken
		if (l_cmp_type == ARG_ALL || l_cmp_type == ARG_WALLS)
			if (!l_is_walken)
				kkit_cli::decompress_walls(l_folder, l_wcount, l_is_klab_v1);
			else
				kkit_cli::decompress_walls_walken(l_folder, l_wcount);

		// nothing else can be decompressed for walken
		if (!l_is_walken) {

			// decompress songs
			if (!l_is_walken && (l_cmp_type == ARG_ALL || l_cmp_type == ARG_SONGS))
				kkit_cli::decompress_songs(l_folder);

			// decompress sounds
			if (!l_is_walken && (l_cmp_type == ARG_ALL || l_cmp_type == ARG_SOUNDS))
				kkit_cli::decompress_sounds(l_folder);

			// decompress story
			if (!l_is_walken && (l_cmp_type == ARG_ALL || l_cmp_type == ARG_STORY))
				kkit_cli::decompress_story(l_folder);

			// nothing else can be decompressed for klab v1
			if (!l_is_klab_v1) {

				// decompress boards
				if (l_cmp_type == ARG_ALL || l_cmp_type == ARG_BOARDS)
					kkit_cli::decompress_boards(l_folder, l_bcount, l_is_klab_v1);

				// decompress lab3d
				if (l_cmp_type == ARG_ALL || l_cmp_type == ARG_LAB3D)
					kkit_cli::decompress_lab3d(l_folder);

			}
		}
	}

}
catch (const std::exception& ex) {
	std::cout << "\nError occurred: " << ex.what();
}
catch (...) {
	std::cout << "Unknown error occurred";
}

void output_header(void) {
	std::cout << std::string(69, '-') << "\n"
		<< "KKIT/CLI - Unofficial Ken's Labyrinth Editor's Toolkit Command Line Interface\n"
		<< "By \"kaimitai\" (https://github.com/kaimitai/klabkit-sdl)\n"
		<< "Build date: " + std::string(__DATE__) + " at " + std::string(__TIME__) + " CET\n"
		<< std::string(69, '-') << "\n\n";
}

void output_help(void) {
	std::cout << "Switches:\n-d: decompress\n-c: compress\n"
		<< "Optional parameter for -d and -c: walls, boards, story, lab3d, sounds, songs, all. Default: all\n\n"
		<< "-v: version (valid values 21, 20, 11, 10, and 0 for Walken - default: 21)\n"
		<< "-f: folder (if the folder has spaces, enclose the value within quotes) Default: current directory\n\n"
		<< "-b: board count (if using a non-standard value - do not give unless you know what you are doing)\n"
		<< "-w: tile count (if using a non-standard value - do not give unless you know what you are doing)\n\n"
		<< "Example: kkit-cli -d walls -v 11 -f \"c:\\games\\ken3d_1_1\"";
}

std::string to_lowercase(const std::string& p_arg) {
	std::string result;

	for (const char c : p_arg)
		result.push_back((c >= 'A' && c <= 'Z') ? 'a' + (c - 'A') : c);

	return result;
}
