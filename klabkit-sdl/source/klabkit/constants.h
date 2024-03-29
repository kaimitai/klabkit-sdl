#ifndef KLABKIT_CONSTANTS_H
#define KLABKIT_CONSTANTS_H

#include <array>
#include <string>

namespace kkit {

	namespace c {

		std::string get_application_window_caption(void);

		// application constants
		constexpr char APP_TITLE[]{ "KKIT/SDL - Unofficial Ken's Labyrinth Editor's Toolkit" };
		constexpr char APP_VERSION[]{ "0.9" };
		constexpr int APP_W{ 1024 };
		constexpr int APP_H{ 768 };
		constexpr char CONF_FILE_NAME[]{ "kkit-sdl-config.xml" };

		// text constants
		constexpr char TXT_SAVE_KZP[]{ "Save KZP" };
		constexpr char TXT_SAVE_DAT[]{ "Save DAT" };
		constexpr char TXT_SAVE_GF_BOARDS[]{ "Save boards data file used by the game" };
		constexpr char TXT_SAVE_GF_WALLS[]{ "Save graphics data file used by the game" };
		constexpr char TXT_EXPORT_XML[]{ "Export xml" };
		constexpr char TXT_IMPORT_XML[]{ "Import xml" };
		constexpr char TXT_EXPORT_BMP[]{ "Export bmp" };
		constexpr char TXT_IMPORT_BMP[]{ "Import bmp" };
		constexpr char TXT_SHIFT_BOARDS[]{ "Hold shift to perform operation for all boards" };
		constexpr char TXT_SHIFT_WALLS[]{ "Hold shift to perform operation for all tiles" };

		constexpr char TXT_COUNT_TILES[]{ "Count tiles of selected type (hold Ctrl to count for Tile Picker type, hold Shift to count for all boards)" };
		constexpr char TXT_NEXT_TILE[]{ "Go to next tile of selected type (hold Ctrl to go to next Tile Picker type)" };
		constexpr char TXT_PREV_TILE[]{ "Go to previous tile of selected type (hold Ctrl to go to next Tile Picker tile)" };

		constexpr char TXT_CLIP[]{ "Noclip" };
		constexpr char TXT_DESTR[]{ "Destructible" };
		constexpr char TXT_HORIZONTAL[]{ "Horizontal" };
		constexpr char TXT_VERTICAL[]{ "Vertical" };

		using byte = unsigned char;

		// filename constants
		constexpr char FILE_WALLS[]{ "WALLS" };
		constexpr char FILE_BOARDS[]{ "BOARDS" };
		constexpr char FILE_SAVEGAME[]{ "SAVGAME" };
		constexpr char FILE_HISCORE[]{ "HISCORE" };
		constexpr char FILE_EXT_DAT[]{ "DAT" };
		constexpr char FILE_EXT_KZP[]{ "KZP" };
		constexpr char FILE_EXT_BMP[]{ "bmp" };
		constexpr char FILE_EXT_XML[]{ "xml" };

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

		// palette generator for lab3d v1
		constexpr std::array<std::array<byte, 3>, 16> PAL_GEN_V1{ {
			{0, 30, 63},
			{ 28,34,60 },
			{ 0,50,20 },
			{ 15,60,30 },
			{ 63,63,25 },
			{ 63,63,63 },
			{ 63,20,20 },
			{ 63,0,63 },
			{ 63,32,0 },
			{ 63,40,25 },
			{ 63,48,48 },
			{ 45,63,45 },
			{ 0,0,63 },
			{ 63,40,63 },
			{ 63,30,20 },
			{ 63,63,63 }
		} };

		constexpr std::tuple<byte, byte, byte> TRANS_COL_RGB{ 204, 92, 112 };
		constexpr std::tuple<byte, byte, byte> FLOOR_COL_RGB{ 72, 56, 28 };

		// wall files
		constexpr int WALL_IMG_W{ 64 };
		constexpr int WALL_IMG_H{ 64 };
		constexpr int WALL_IMG_BYTES{ WALL_IMG_W * WALL_IMG_H };

		constexpr int WALL_DATA_OFFSET{ 1024 };

		// level files
		constexpr int MAP_W{ 64 };
		constexpr int MAP_H{ 64 };
		constexpr int MAP_GFX_W{ MAP_W * WALL_IMG_W };
		constexpr int MAP_GFX_H{ MAP_H * WALL_IMG_H };
		constexpr int MAP_BYTES_PER_TILE{ 2 };
		constexpr int MAP_BYTES_WALKEN{ MAP_W * MAP_H };
		constexpr int MAP_BYTES{ MAP_W * MAP_H * MAP_BYTES_PER_TILE };

		// gfx constants - indexes to project graphic map entries
		constexpr int INDEX_WALL_TEXTURES{ 0 };
		constexpr int INDEX_APP_TEXTURES{ INDEX_WALL_TEXTURES + 1 };
		constexpr int INDEX_BG_TEXTURES{ INDEX_APP_TEXTURES + 1 };
		constexpr int INDEX_MM_TEXTURES{ INDEX_BG_TEXTURES + 1 }; // minimaps

		// savefile variable codes
		constexpr char SAVE_CODE_BOARD[]{ "board" };
		constexpr char SAVE_CODE_BOARDNUM[]{ "boardnum" };
		constexpr char SAVE_CODE_HISCORENAME[]{ "hiscorename" };

		// window position and size constants
		constexpr int WIN_MAIN_X{ 5 };
		constexpr int WIN_MAIN_Y{ 5 };
		constexpr int WIN_MAIN_W{ 400 };
		constexpr int WIN_MAIN_H{ 615 };

		constexpr int WIN_MINIMAP_X{ 5 };
		constexpr int WIN_MINIMAP_Y{ 625 };
		constexpr int WIN_MINIMAP_W{ 120 };
		constexpr int WIN_MINIMAP_H{ 134 };

		constexpr int WIN_TP_X{ 615 };
		constexpr int WIN_TP_Y{ 250 };
		constexpr int WIN_TP_W{ 405 };
		constexpr int WIN_TP_H{ 510 };

		constexpr int WIN_SBT_X{ 755 };
		constexpr int WIN_SBT_Y{ 5 };
		constexpr int WIN_SBT_W{ 265 };
		constexpr int WIN_SBT_H{ 240 };

		constexpr int WIN_META_X{ 415 };
		constexpr int WIN_META_Y{ 5 };
		constexpr int WIN_META_W{ 340 };
		constexpr int WIN_META_H{ 330 };

		// savefile editor window
		constexpr int WIN_SF_X{ 415 };
		constexpr int WIN_SF_Y{ 5 };
		constexpr int WIN_SF_W{ 385 };
		constexpr int WIN_SF_H{ 235 };

		// hiscores editor window
		constexpr int WIN_HS_X{ 415 };
		constexpr int WIN_HS_Y{ 5 };
		constexpr int WIN_HS_W{ 245 };
		constexpr int WIN_HS_H{ 280 };

		// message codes
		constexpr int MSG_CODE_INFO{ 0 };
		constexpr int MSG_CODE_WARNING{ MSG_CODE_INFO + 1 };
		constexpr int MSG_CODE_SUCCESS{ MSG_CODE_WARNING + 1 };
		constexpr int MSG_CODE_ERROR{ MSG_CODE_SUCCESS + 1 };
	}
}

#endif
