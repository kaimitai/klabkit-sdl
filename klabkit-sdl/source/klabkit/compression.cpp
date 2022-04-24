#include "compression.h"
#include "../klib/klib_util.h"

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

std::vector<byte> kkit::compression::decompress_story_kzp(const std::vector<byte>& p_bytes) {
	std::vector<byte> result;

	std::vector<int> l_offsets;
	for (int i{ 0 }; i < 128; ++i)
		l_offsets.push_back(p_bytes.at(2 * i) + 256 * p_bytes.at(2 * i + 1));
	l_offsets.push_back(0);

	bool l_last{ false };
	int l_length{ 0 };
	byte dat{ 0 }, dat_old{ 0 };

	for (int l_story_no{ 0 }; l_story_no < static_cast<int>(l_offsets.size()) - 1; ++l_story_no) {

		if (l_offsets[l_story_no + 1] == 0) {
			l_last = true;
			l_length = static_cast<int>(p_bytes.size()) - l_offsets[l_story_no];
		}
		else
			l_length = l_offsets[l_story_no + 1] - l_offsets[l_story_no];

		std::vector<byte> c_story(begin(p_bytes) + l_offsets[l_story_no], begin(p_bytes) + l_offsets[l_story_no] + l_length);
		dat = 0;

		for (int i{ 0 }; i < l_length - 1; ++i) {
			dat_old = dat;
			dat = c_story[i];
			byte l_out_byte{ static_cast<byte>(dat_old ^ dat) };

			result.push_back(l_out_byte);
			if (l_out_byte == 13)
				result.push_back(10);
		}

		if (l_last)
			break;

		result.push_back('\\');
		result.push_back(13);
		result.push_back(10);
	}

	return result;
}

std::vector<std::pair<std::string, std::vector<byte>>> kkit::compression::decompress_songs_kzp(const std::vector<byte>& p_bytes) {
	std::vector<std::pair<std::string, std::vector<byte>>> result;
	std::vector<std::string> l_filenames;
	std::vector<int> l_offsets;

	std::size_t l_file_count = klib::util::uint_le(p_bytes, 0, 2);

	for (std::size_t i{ 0 }; i < l_file_count; ++i) {
		std::size_t l_offset{ 2 + 12 * i };
		std::string l_filename;
		auto l_fn = std::vector<byte>(begin(p_bytes) + l_offset, begin(p_bytes) + l_offset + 8);
		for (std::size_t j{ 0 }; j < l_fn.size() && l_fn[j] != 0; ++j)
			l_filename.push_back(l_fn[j]);
		l_filenames.push_back(l_filename);
		l_offsets.push_back(klib::util::uint_le(p_bytes, l_offset + 8, 4));
	}

	l_offsets.push_back(static_cast<int>(p_bytes.size()));

	for (std::size_t i{ 0 }; i < l_file_count; ++i)
		result.push_back(std::make_pair(l_filenames[i], std::vector<byte>(begin(p_bytes) + l_offsets[i], begin(p_bytes) + l_offsets[i + 1])));

	return result;
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
	std::vector<byte> result(LZW_UNCOMPRESSED_BLOCK_SIZE, 0);
	std::vector<int> lzwbuf(LZW_DICT_SIZE, 0);
	std::vector<int> lzwbuf2(LZW_DICT_SIZE, 0);
	std::vector<int> stack(LZW_DICT_SIZE, 0);
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
			if (bytecnt1 < LZW_UNCOMPRESSED_BLOCK_SIZE)
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

std::vector<byte> kkit::compression::compress_lzw_block(const std::vector<byte>& buf1) {
	std::vector<byte> result;
	std::vector<byte> value(4360, 0), buf1x(4096, 0), buf2(20000, 0);
	std::vector<uint16_t> child(4360, 0), sibly(4360, 0);

	for (std::size_t i{ 0 }; i < 256; ++i) {
		value[i] = static_cast<byte>(i);
		child[i] = 65535;
		sibly[i] = (i + 1) & 255;
	}

	int addrcnt{ 256 }, bytecnt1{ 0 }, bytecnt2{ 0 }, dat{ 0 }, numbits{ 9 }, addr{ 0 }, newaddr{ 0 };
	byte bitcnt{ 0 };

	do {
		addr = buf1[bytecnt1];
		do {
			++bytecnt1;
			if (bytecnt1 == LZW_UNCOMPRESSED_BLOCK_SIZE)
				break;
			if (child[addr] == 65535) {
				child[addr] = addrcnt;
				break;
			}
			newaddr = child[addr];
			while (value[newaddr] != buf1[bytecnt1])
			{
				if (sibly[newaddr] == 65535)
				{
					sibly[newaddr] = addrcnt;
					break;
				}
				newaddr = sibly[newaddr];
			}
			if (sibly[newaddr] == addrcnt)
				break;
			addr = newaddr;
		} while (addr >= 0);

		if (bytecnt1 < buf1.size())
			value[addrcnt] = buf1[bytecnt1];

		child[addrcnt] = 65535;
		sibly[addrcnt] = 65535;
		for (int i{ 0 }; i < numbits; i++)
		{
			if ((addr & (1 << i)) > 0)
				dat += (1 << bitcnt);
			bitcnt++;
			if (bitcnt == 8)
			{
				bitcnt = 0;
				buf2[bytecnt2] = dat;
				bytecnt2++;
				dat = 0;
			}
		}
		addrcnt++;
		if ((addrcnt == 512) || (addrcnt == 1024) || (addrcnt == 2048) || (addrcnt == 4096))
			numbits++;

	} while (bytecnt1 < 4096);

	for (int i{ 0 }; i < numbits; i++) {
		if ((addr & (1 << i)) > 0)
			dat += (1 << bitcnt);
		bitcnt++;
		if (bitcnt == 8) {
			bitcnt = 0;
			buf2[bytecnt2] = dat;
			bytecnt2++;
			dat = 0;
		}
	}

	result.push_back(addrcnt % 256);
	result.push_back(addrcnt / 256);

	if (bytecnt2 < 4096) {
		result.insert(end(result), begin(buf2), begin(buf2) + bytecnt2);
	}
	else {
		addrcnt = 0;
		bytecnt2 = LZW_UNCOMPRESSED_BLOCK_SIZE;
		result.insert(end(result), begin(buf1), begin(buf1) + bytecnt2);
	}

	return result;
}

std::vector<byte> kkit::compression::compress_boards_kzp(const std::vector<byte>& p_bytes) {
	return compress_file_contents(p_bytes);
}

std::vector<byte> kkit::compression::compress_walls_kzp(const std::vector<byte>& p_bytes) {
	return compress_file_contents(p_bytes, 1024);
}

// determine header size and block count (these are the same number when a header exists) from lzw compressed archives
std::pair<int, int> kkit::compression::calculate_lzw_block_count(const std::vector<byte>& p_bytes, bool p_has_header) {
	int l_block_count{ 0 };
	int target_size{ static_cast<int>(p_bytes.size()) };

	for (int l_header{ 0 }; l_header < 1024; ++l_header) {

		int l_file_size{ l_header };

		for (int l_blocks{ 0 }; ; ++l_blocks) {
			int l_comp_size = klib::util::uint_le(p_bytes, l_header + 2 * l_blocks, 2);
			l_file_size += l_comp_size + 4;

			if (l_file_size == target_size && (!p_has_header || (l_header == (l_blocks + 1))))
				return std::make_pair(l_header, l_blocks + 1);
			else if (l_file_size > target_size)
				break;
		}
	}

	throw std::exception("Could not determine lzw file metadata");

}

std::vector<byte> kkit::compression::compress_file_contents(const std::vector<byte>& p_bytes, int p_header_size) {
	int l_block_count = (static_cast<int>(p_bytes.size()) - p_header_size) / LZW_UNCOMPRESSED_BLOCK_SIZE;
	int l_out_header_size = (p_header_size == 0 ? 0 : l_block_count);
	std::vector<byte> result(l_out_header_size + l_block_count * 2, 0);
	std::copy(begin(p_bytes), begin(p_bytes) + l_block_count, begin(result));

	for (int i{ 0 }; i < l_block_count; ++i) {
		auto l_cmp_data = compress_lzw_block(std::vector<byte>(begin(p_bytes) + p_header_size + i * LZW_UNCOMPRESSED_BLOCK_SIZE, begin(p_bytes) + p_header_size + (i + 1) * LZW_UNCOMPRESSED_BLOCK_SIZE));
		result[l_out_header_size + 2 * i] = (l_cmp_data.size() - 2) % 256;
		result[l_out_header_size + 2 * i + 1] = static_cast<byte>((l_cmp_data.size() - 2) / 256);
		result.insert(end(result), begin(l_cmp_data), end(l_cmp_data));
	}

	return result;
}

std::vector<std::vector<byte>> kkit::compression::decompress_lab3d_kzp(const std::vector<byte>& p_bytes) {
	return replace_lab3d_headers(p_bytes, std::vector<byte>(begin(HEADER_LAB3D), end(HEADER_LAB3D)), std::vector<byte>(begin(HEADER_GIF), end(HEADER_GIF)));
}

// lab3d.kzp consists of consecutive gif-files, where the gif header has been replaced
// the offsets to the images are hard coded in the original ken3d executable so this file cannot easily be modified
// idea: update a source port to determine the offsets by looking for the LAB3D!-header instead of relying on hard coded offsets
// this function will do both "compression" and "decompression", based on which header to find and which to replace it with
std::vector<std::vector<byte>> kkit::compression::replace_lab3d_headers(const std::vector<byte>& p_bytes, const std::vector<byte>& p_from_header, const std::vector<byte>& to_header) {
	std::vector<std::vector<byte>> result;
	std::vector<std::size_t> l_offsets;
	std::size_t l_offset{ 0 };

	// find the offsets to the from-header in the file
	while (l_offset < p_bytes.size()) {
		std::size_t l_next_offset = klib::util::find_subvector(p_bytes, std::vector<byte>(begin(p_from_header), end(p_from_header)), l_offset);
		l_offsets.push_back(l_next_offset);
		l_offset = l_next_offset + 1;
	}

	for (std::size_t i{ 0 }; i < l_offsets.size() - 1; ++i) {
		result.push_back(std::vector<byte>(begin(p_bytes) + l_offsets[i], begin(p_bytes) + l_offsets[i + 1]));
		std::copy(begin(to_header), end(to_header), begin(result[i]));
	}

	return result;
}

std::vector<byte> kkit::compression::compress_lab3d_kzp(const std::vector<std::vector<byte>>& p_file_bytes) {
	std::vector<byte> result;

	for (std::vector<byte> p_bytes : p_file_bytes) {
		std::copy(begin(HEADER_LAB3D), end(HEADER_LAB3D), begin(p_bytes));
		result.insert(end(result), begin(p_bytes), end(p_bytes));
	}

	return result;
}

std::vector<byte> kkit::compression::compress_story_kzp(const std::vector<byte>& p_bytes) {
	std::vector<byte> result(256, 0);

	// initialize with first offset: 256
	result[0] = 0;
	result[1] = 1;


	byte dat_old{ 0 };
	int cnt{ 256 };
	std::size_t ind{ 0 }, j{ 2 };
	byte dat{ p_bytes.at(ind++) };
	bool l_read{ true };

	while (l_read) {

		if (dat != 10 && dat != 92) {
			dat ^= dat_old;
			result.push_back(dat);
			dat_old = dat;
			dat = 0;
			++cnt;
		}
		if (dat == 92) {
			dat = dat_old;
			result.push_back(dat);
			dat_old = 0;
			++cnt;
			result[j++] = cnt % 256;
			result[j++] = cnt / 256;
			++ind;
			++ind;
		}
		if (ind < p_bytes.size()) {
			dat = p_bytes.at(ind);
			l_read = true;
			++ind;
		}
		else
			l_read = false;
	}

	return result;
}

std::vector<byte> kkit::compression::compress_songs_kzp(const std::vector<std::pair<std::string, std::vector<byte>>>& p_file_bytes) {
	// start by writing the file count as uint16le
	int l_file_count = static_cast<int>(p_file_bytes.size());
	std::vector<byte> result(klib::util::from_uint_le(l_file_count, 2));

	// write file info; file names and file offsets
	int l_offset{ 2 + 12 * l_file_count };

	for (int i{ 0 }; i < l_file_count; ++i) {
		result.insert(end(result), begin(p_file_bytes[i].first), end(p_file_bytes[i].first));

		for (int l_pad{ 0 }; l_pad < 8 - static_cast<int>(p_file_bytes[i].first.size()); ++l_pad)
			result.push_back(0);

		auto l_offset_bytes = klib::util::from_uint_le(l_offset, 4);
		result.insert(end(result), begin(l_offset_bytes), end(l_offset_bytes));
		l_offset += static_cast<int>(p_file_bytes[i].second.size());
	}

	// write all actual file contents
	for (const auto& l_v : p_file_bytes)
		result.insert(end(result), begin(l_v.second), end(l_v.second));

	return result;
}
