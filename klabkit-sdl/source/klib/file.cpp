#include "file.h"

#include <fstream>

using byte = unsigned char;

std::vector<byte> klib::file::read_file_as_bytes(const std::string& p_file_name) {
	std::vector<byte> result;

	std::ifstream input(p_file_name, std::ios::binary | std::ios::in);

	if (!input.fail())
		return std::vector<byte>((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
	else
		throw std::exception(("Could not open " + p_file_name).c_str());

	return result;
}

int klib::file::uint16_le(byte p_b1, byte p_b2) {
	return p_b2 * 256 + p_b1;
}
