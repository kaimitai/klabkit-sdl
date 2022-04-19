#include "compression.h"

using byte = unsigned char;

std::vector<byte> kkit::compression::decompress_walls_kzp(const std::vector<byte>& p_bytes) {
	// the compressed files containt no metadata, so the number of blocks need to be known in advance
	// for walls.kzp - the image metadata header makes it even more difficult
	// each image consists of one compressed block
	return decompress_file_contents(p_bytes, 448, 448, 1024);
}

std::vector<byte> kkit::compression::decompress_boards_kzp(const std::vector<byte>& p_bytes) {
	// the compressed files containt no metadata, so the number of blocks need to be known in advance
	// each board is divided into two compressed blocks
	return decompress_file_contents(p_bytes, 30 * 2);
}

std::vector<byte> kkit::compression::decompress_file_contents(const std::vector<byte>& p_bytes, int p_block_count, int p_header_size, int p_out_header_size) {
	std::vector<byte> result(std::vector<byte>(begin(p_bytes), begin(p_bytes) + p_header_size));
	while (result.size() < p_out_header_size)
		result.push_back(0);

	auto compressed_data{ std::vector<byte>(begin(p_bytes) + p_header_size, end(p_bytes)) };
	std::vector<int> l_comp_lengths;

	// first bytes of the compressed data is the compressed lengths of each block, 1 int16 per
	for (int i{ 0 }; i < p_block_count; ++i) {
		int l_len = compressed_data.at(2 * i);
		l_len += 256 * compressed_data.at(2 * i + 1);
		l_comp_lengths.push_back(l_len);
	}

	for (int i{ 0 }; i < p_block_count; ++i) {
		auto px = decompress_lzw_block(std::vector<byte>(begin(compressed_data) + get_file_offset(l_comp_lengths, i), begin(compressed_data) + get_file_offset(l_comp_lengths, i + 1)));
		result.insert(end(result), begin(px), end(px));
	}

	return result;
}

int kkit::compression::get_file_offset(const std::vector<int> p_comp_lengths, int p_block_no) {
	// start at 2xblock number, the first bytes of the file are a header containing an int16 of compression lengths per block

	int result{ static_cast<int>(p_comp_lengths.size()) * 2 };

	for (int i = 0; i < p_block_no; ++i)
		result += p_comp_lengths[i] + 2; // add two, since the individual blocks also have a 2-byte header

	return result;
}

std::vector<byte> kkit::compression::decompress_lzw_block(const std::vector<byte>& p_input) {
	std::vector<byte> result(4096, 0);
	std::vector<int> lzwbuf(4096, 0);
	std::vector<int> lzwbuf2(4096, 0);
	std::vector<int> stack(4096, 0);
	std::vector<byte> tempbuf(std::vector<byte>(begin(p_input) + 2, end(p_input)));
	tempbuf.push_back(0);
	tempbuf.push_back(0);
	tempbuf.push_back(0);

	for (int i = 1; i <= 256; i++) {
		lzwbuf[i] = i & 255;
		lzwbuf2[i] = i;
	}

	int strtot = p_input.at(0) + p_input.at(1) * 256;
	int compleng = static_cast<int>(tempbuf.size());
	int bytecnt1{ 0 }, bytecnt2{ 0 }, bitcnt{ 0 }, stackp{ 0 }, currstr{ 256 }, goalstr{ 512 }, numbits{ 9 }, dat{ 0 };

	do {

		dat = (((tempbuf[bytecnt2]) |
			((tempbuf[bytecnt2 + 1])) << 8) >> bitcnt) &
			((1 << numbits) - 1);

		if (bitcnt + numbits > 16) {
			dat += ((tempbuf[bytecnt2 + 2]) &
				((1 << ((bitcnt + numbits) & 15)) - 1)) <<
				(16 - bitcnt);
		}

		bitcnt += numbits;
		bytecnt2 += bitcnt >> 3;
		bitcnt &= 7;

		lzwbuf2[currstr] = dat;

		while (dat >= 256) {
			stack[stackp++] = lzwbuf[dat];
			dat = lzwbuf2[dat];
		}

		lzwbuf[currstr - 1] = dat;
		lzwbuf[currstr] = dat;

		dat = lzwbuf2[dat];
		stack[stackp++] = dat;

		while (stackp > 0) {
			stackp--;
			if (bytecnt1 < 4096)
				result[bytecnt1++] = static_cast<byte>(stack[stackp]);
		}

		currstr++;
		if (currstr == goalstr) {
			numbits++;
			goalstr = (goalstr << 1);
		}

	} while (currstr <= strtot);

	return result;
}