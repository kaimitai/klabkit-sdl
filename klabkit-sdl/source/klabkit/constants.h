#ifndef KLABKIT_CONSTANTS_H
#define KLABKIT_CONSTANTS_H

#include <array>

namespace kkit {

	namespace c {

		using byte = unsigned char;

		// filename constants
		constexpr char FILE_WALLS[]{ "WALLS" };
		constexpr char FILE_BOARDS[]{ "BOARDS" };
		constexpr char FILE_EXT_DAT[]{ "DAT" };
		constexpr char FILE_EXT_BMP[]{ "bmp" };

		// palette constants
		constexpr int TRANSP_PAL_INDEX{ 255 };
		// palette generator values
		constexpr std::array<std::array<byte, 3>, 16> PAL_GEN{ {
			{0,30,63},
			{28,34,60},
			{0,50,20},
			{41,52,28},
			{63,63,25},
			{63,63,63},
			{63,20,20},
			{63,0,63},
			{63,48,27},
			{63,40,25},
			{63,48,48},
			{45,63,45},
			{55,55,63},
			{63,40,63},
			{63,30,20},
			{55,25,30}
			} };

		// wall files
		constexpr int WALL_IMG_W{ 64 };
		constexpr int WALL_IMG_H{ 64 };
		constexpr int WALL_IMG_BYTES{ WALL_IMG_W * WALL_IMG_H };

		constexpr int WALL_DATA_OFFSET{ 1024 };

		// level files
		constexpr int MAP_W{ 64 };
		constexpr int MAP_H{ 64 };
		constexpr int MAP_BYTES_PER_TILE{ 2 };
		constexpr int MAP_BYTES{ MAP_W * MAP_H * MAP_BYTES_PER_TILE };
	}
}

#endif