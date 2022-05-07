#include <filesystem>
#include <iostream>
#include <iomanip>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include "../../../klabkit-sdl/source/klabkit/compression.h"
#include "../../../klabkit-sdl/source/klib/file.h"
#include "kkit_cli_constants.h"
#include "kkit_cli_util.h"

std::string kkit_cli::folder_combine(const std::string& p_folder, const std::string& p_subfolder) {
	return p_folder + "/" + p_subfolder;
}

std::string kkit_cli::full_file_path(const std::string& p_folder, const std::string& p_file_ext, const std::string& p_filename, int p_frame_no) {
	std::string l_filename = p_filename;

	if (p_frame_no >= 0) {
		std::string l_frame_str = std::to_string(p_frame_no + 1);
		l_filename += "-" + std::string(3 - l_frame_str.size(), '0') + l_frame_str;
	}

	return folder_combine(folder_combine(p_folder, p_file_ext), l_filename + "." + p_file_ext);
}

std::string kkit_cli::file_name(const std::string& p_filename, const std::string& p_extension) {
	return p_filename + "." + p_extension;
}


void kkit_cli::decompress_walls_walken(const std::string& p_folder, int p_wall_count) {
	std::cout << "Decompressing " << kkit_cli::c::FILE_NAME_WALLS << "...";

	auto l_in_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_WALLS, kkit_cli::c::FILE_EXT_KZP));
	auto l_out_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_WALLS, kkit_cli::c::FILE_EXT_DAT));

	auto l_in_bytes = klib::file::read_file_as_bytes(l_in_file);
	auto l_out_bytes = kkit::compression::decompress_walls_kzp_walken(l_in_bytes, p_wall_count);

	klib::file::write_bytes_to_file(l_out_bytes, l_out_file);

	std::cout << "Decompression complete. " << l_out_bytes.size() << " bytes written to disk.\n";
}

void kkit_cli::compress_walls_walken(const std::string& p_folder, int p_wall_count) {
	std::cout << "Compressing " << kkit_cli::c::FILE_NAME_WALLS << "...";

	auto l_in_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_WALLS, kkit_cli::c::FILE_EXT_DAT));
	auto l_out_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_WALLS, kkit_cli::c::FILE_EXT_KZP));

	auto l_in_bytes = klib::file::read_file_as_bytes(l_in_file);
	auto l_out_bytes = kkit::compression::compress_walls_kzp_walken(l_in_bytes, p_wall_count);

	klib::file::write_bytes_to_file(l_out_bytes, l_out_file);
	std::cout << "Compression complete. " << l_out_bytes.size() << std::fixed << std::setprecision(2) << " bytes written to disk. (compression ratio: " << 100 * l_out_bytes.size() / static_cast<float>(l_in_bytes.size()) << "%)\n";
}

void kkit_cli::compress_boards(const std::string& p_folder, int p_board_count, bool p_v1) {
	std::cout << "Compressing " << kkit_cli::c::FILE_NAME_BOARDS << "...";

	auto l_in_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_BOARDS, kkit_cli::c::FILE_EXT_DAT));
	auto l_out_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_BOARDS, kkit_cli::c::FILE_EXT_KZP));

	auto l_in_bytes = klib::file::read_file_as_bytes(l_in_file);
	auto l_out_bytes = kkit::compression::compress_boards_kzp(l_in_bytes, p_board_count, p_v1);

	klib::file::write_bytes_to_file(l_out_bytes, l_out_file);

	std::cout << "Compression complete. " << l_out_bytes.size() << std::fixed << std::setprecision(2) << " bytes written to disk. (compression ratio: " << 100 * l_out_bytes.size() / static_cast<float>(l_in_bytes.size()) << "%)\n";
}

void kkit_cli::decompress_boards(const std::string& p_folder, int p_board_count, bool p_v1) {
	std::cout << "Decompressing " << kkit_cli::c::FILE_NAME_BOARDS << "...";

	auto l_in_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_BOARDS, kkit_cli::c::FILE_EXT_KZP));
	auto l_out_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_BOARDS, kkit_cli::c::FILE_EXT_DAT));

	auto l_in_bytes = klib::file::read_file_as_bytes(l_in_file);
	auto l_out_bytes = kkit::compression::decompress_boards_kzp(l_in_bytes, p_board_count, p_v1);

	klib::file::write_bytes_to_file(l_out_bytes, l_out_file);

	std::cout << "Decompression complete. " << l_out_bytes.size() << " bytes written to disk.\n";
}

void kkit_cli::compress_walls(const std::string& p_folder, int p_wall_count, bool p_v1) {
	std::cout << "Compressing " << kkit_cli::c::FILE_NAME_WALLS << "...";

	auto l_in_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_WALLS, kkit_cli::c::FILE_EXT_DAT));
	auto l_out_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_WALLS, kkit_cli::c::FILE_EXT_KZP));

	auto l_in_bytes = klib::file::read_file_as_bytes(l_in_file);
	auto l_out_bytes = kkit::compression::compress_walls_kzp(l_in_bytes, p_wall_count, p_v1);

	klib::file::write_bytes_to_file(l_out_bytes, l_out_file);

	std::cout << "Compression complete. " << l_out_bytes.size() << std::fixed << std::setprecision(2) << " bytes written to disk. (compression ratio: " << 100 * l_out_bytes.size() / static_cast<float>(l_in_bytes.size()) << "%)\n";
}

void kkit_cli::decompress_walls(const std::string& p_folder, int p_wall_count, bool p_v1) {
	std::cout << "Decompressing " << kkit_cli::c::FILE_NAME_WALLS << "...";

	auto l_in_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_WALLS, kkit_cli::c::FILE_EXT_KZP));
	auto l_out_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_WALLS, kkit_cli::c::FILE_EXT_DAT));

	auto l_in_bytes = klib::file::read_file_as_bytes(l_in_file);
	auto l_out_bytes = kkit::compression::decompress_walls_kzp(l_in_bytes, p_wall_count, p_v1);

	klib::file::write_bytes_to_file(l_out_bytes, l_out_file);

	std::cout << "Decompression complete. " << l_out_bytes.size() << " bytes written to disk.\n";
}

void kkit_cli::compress_lab3d(const std::string& p_folder) {
	std::cout << "Creating archive " << kkit_cli::c::FILE_NAME_LAB3D << "...";

	std::vector<std::vector<byte>> l_bytes;

	std::set<std::string> l_in_files;

	for (int i{ 0 }; std::filesystem::exists(kkit_cli::full_file_path(p_folder,
		kkit_cli::c::FILE_EXT_GIF,
		kkit_cli::c::FILE_NAME_LAB3D,
		i)); ++i) {

		l_bytes.push_back(klib::file::read_file_as_bytes(kkit_cli::full_file_path(p_folder,
			kkit_cli::c::FILE_EXT_GIF,
			kkit_cli::c::FILE_NAME_LAB3D,
			i)));
	}

	auto l_out_bytes = kkit::compression::compress_lab3d_kzp(l_bytes);
	auto l_out_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_LAB3D, kkit_cli::c::FILE_EXT_KZP));

	klib::file::write_bytes_to_file(l_out_bytes, l_out_file);

	std::cout << "Archived " + std::to_string(l_bytes.size()) + " gif files.\n";
}

void kkit_cli::decompress_lab3d(const std::string& p_folder) {
	std::cout << "Extracting " << kkit_cli::c::FILE_NAME_LAB3D << "...";

	// decompress LAB3D.KZP
	auto l_in_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_LAB3D, kkit_cli::c::FILE_EXT_KZP));

	auto l_bytes = klib::file::read_file_as_bytes(l_in_file);
	auto l_out_files = kkit::compression::decompress_lab3d_kzp(l_bytes);
	auto l_out_folder = kkit_cli::folder_combine(p_folder, kkit_cli::c::FILE_EXT_GIF);

	std::filesystem::create_directory(l_out_folder);

	for (std::size_t i{ 0 }; i < l_out_files.size(); ++i) {
		auto l_out_file = kkit_cli::full_file_path(p_folder,
			kkit_cli::c::FILE_EXT_GIF,
			kkit_cli::c::FILE_NAME_LAB3D,
			static_cast<int>(i));

		klib::file::write_bytes_to_file(l_out_files[i], l_out_file);
	}

	std::cout << "Extracted " + std::to_string(l_out_files.size()) + " gif files.\n";
}

void kkit_cli::compress_story(const std::string& p_folder) {
	std::cout << "Encrypting " << kkit_cli::c::FILE_NAME_STORY << "...";

	auto l_in_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_STORY, kkit_cli::c::FILE_EXT_DAT));

	auto l_bytes = kkit::compression::compress_story_kzp(klib::file::read_file_as_bytes(l_in_file));

	auto l_out_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_STORY, kkit_cli::c::FILE_EXT_KZP));

	klib::file::write_bytes_to_file(l_bytes, l_out_file);

	std::cout << "Encryption complete\n";
}

void kkit_cli::decompress_story(const std::string& p_folder) {
	std::cout << "Decrypting " << kkit_cli::c::FILE_NAME_STORY << "...";

	auto l_in_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_STORY, kkit_cli::c::FILE_EXT_KZP));

	auto l_bytes = kkit::compression::decompress_story_kzp(klib::file::read_file_as_bytes(l_in_file));

	auto l_out_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_STORY, kkit_cli::c::FILE_EXT_DAT));

	klib::file::write_bytes_to_file(l_bytes, l_out_file);

	std::cout << "Decryption complete\n";
}

void kkit_cli::compress_sounds(const std::string& p_folder) {
	std::cout << "Creating archive " << kkit_cli::c::FILE_NAME_SOUNDS << "...";

	std::vector<std::vector<byte>> l_bytes;

	std::set<std::string> l_in_files;

	for (int i{ 0 }; std::filesystem::exists(kkit_cli::full_file_path(p_folder,
		kkit_cli::c::FILE_EXT_WAV,
		kkit_cli::c::FILE_NAME_SOUNDS,
		i)); ++i) {

		l_bytes.push_back(klib::file::read_file_as_bytes(kkit_cli::full_file_path(p_folder,
			kkit_cli::c::FILE_EXT_WAV,
			kkit_cli::c::FILE_NAME_SOUNDS,
			i)));
	}

	auto l_out_bytes = kkit::compression::compress_sounds_kzp(l_bytes);
	auto l_out_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_SOUNDS, kkit_cli::c::FILE_EXT_KZP));

	klib::file::write_bytes_to_file(l_out_bytes, l_out_file);

	std::cout << "Archived " + std::to_string(l_bytes.size()) + " wav files.\n";
}

void kkit_cli::decompress_sounds(const std::string& p_folder) {
	std::cout << "Extracting " << kkit_cli::c::FILE_NAME_SOUNDS << "...";

	// decompress SOUNDS.KZP
	auto l_in_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_SOUNDS, kkit_cli::c::FILE_EXT_KZP));

	auto l_bytes = klib::file::read_file_as_bytes(l_in_file);
	auto l_out_files = kkit::compression::decompress_sounds_kzp(l_bytes);
	auto l_out_folder = kkit_cli::folder_combine(p_folder, kkit_cli::c::FILE_EXT_WAV);

	std::filesystem::create_directory(l_out_folder);

	for (std::size_t i{ 0 }; i < l_out_files.size(); ++i) {
		auto l_out_file = kkit_cli::full_file_path(p_folder,
			kkit_cli::c::FILE_EXT_WAV,
			kkit_cli::c::FILE_NAME_SOUNDS,
			static_cast<int>(i));

		klib::file::write_bytes_to_file(l_out_files[i], l_out_file);
	}

	std::cout << "Extracted " + std::to_string(l_out_files.size()) + " wav files.\n";
}

void kkit_cli::compress_songs(const std::string& p_folder) {
	std::cout << "Creating archive " << kkit_cli::c::FILE_NAME_SONGS << "...";

	std::vector<std::pair<std::string, std::vector<byte>>> l_bytes;

	std::set<std::string> l_in_files;

	for (const auto& l_ksm_file : std::filesystem::directory_iterator(kkit_cli::folder_combine(p_folder, kkit_cli::c::FILE_EXT_KSM)))
		if (l_ksm_file.path().extension() == ".KSM") {
			auto l_fn = l_ksm_file.path().filename().string();
			l_in_files.insert(l_fn.substr(0, l_fn.find_last_of(".")));
		}

	for (const auto& l_filename : l_in_files) {
		auto l_ksm_file = kkit_cli::full_file_path(p_folder, kkit_cli::c::FILE_EXT_KSM, l_filename);
		l_bytes.push_back(std::make_pair(l_filename, klib::file::read_file_as_bytes(l_ksm_file)));
	}

	auto l_out_bytes = kkit::compression::compress_songs_kzp(l_bytes);
	auto l_out_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_SONGS, kkit_cli::c::FILE_EXT_KZP));

	klib::file::write_bytes_to_file(l_out_bytes, l_out_file);

	std::cout << "Archived " + std::to_string(l_bytes.size()) + " KSM files.\n";
}

void kkit_cli::decompress_songs(const std::string& p_folder) {
	std::cout << "Extracting " << kkit_cli::c::FILE_NAME_SONGS << "...";

	// decompress SONGS.KZP
	auto l_in_file = kkit_cli::folder_combine(p_folder,
		kkit_cli::file_name(kkit_cli::c::FILE_NAME_SONGS, kkit_cli::c::FILE_EXT_KZP));

	auto l_bytes = klib::file::read_file_as_bytes(l_in_file);
	auto l_out_files = kkit::compression::decompress_songs_kzp(l_bytes);
	auto l_out_folder = kkit_cli::folder_combine(p_folder, kkit_cli::c::FILE_EXT_KSM);

	std::filesystem::create_directory(l_out_folder);

	for (const auto& l_file : l_out_files) {
		auto l_out_file = kkit_cli::full_file_path(p_folder,
			kkit_cli::c::FILE_EXT_KSM,
			l_file.first);

		klib::file::write_bytes_to_file(l_file.second, l_out_file);
	}

	std::cout << "Extracted " + std::to_string(l_out_files.size()) + " KSM files.\n";
}
