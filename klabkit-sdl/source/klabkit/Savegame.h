#ifndef KKIT_SAVEGAME_H
#define KKIT_SAVEGAME_H

#include <string>
#include <vector>
#include "./Board.h"

using byte = unsigned char;

namespace kkit {

	class Savegame {
		std::vector<byte> m_unknown_bytes;

		// variables in the order they appear in a savefile
		std::string hiscorenam;
		unsigned int hiscorenamstat, boardnum, scorecount, scoreclock;
		kkit::Board m_board;
		unsigned int skilevel, life, death, lifevests, lightnings, firepowers_0, firepowers_1, firepowers_2, bulchoose, keys, coins, compass, cheated, animate2, animate3, animate4, oscillate3, oscillate5, animate6, animate7, animate8, animate10, animate11, animate15, statusbar, statusbargoal, posx, posy, posz, ang, startx, starty, startang, angvel, vel, mxvel, myvel, svel, hvel, oldposx, oldposy;
		// bullets
		unsigned int bulnum;
		std::vector<unsigned int> bultype_todo, bulang, bulx, buly, bulstat;
		unsigned int lastbulshoot;
		// enemies
		unsigned int mnum;
		std::vector<unsigned int> mposx, mposy, mgolx, mgoly, moldx, moldy, mstat, mshock, mshot;
		unsigned int doorx, doory, doorstat,
			numwarps,
			justwarped;
		// warps (count=numwarps)
		std::vector<unsigned int> xwarp, ywarp;

		unsigned int totalclock, purpletime, greentime, capetime_0, capetime_1, musicstatus,
			clockspeed, count, countstop, nownote, junk;

		std::vector<unsigned int> chanage, chanfreq;

		unsigned int midiinst, mute, namrememberstat, fadewarpval, fadehurtval,
			slottime, slotpos_0, slotpos_1, slotpos_2, owecoins, owecoinwait;

		static void write_uint_le(std::vector<byte>& p_bytes,
			unsigned int p_value, std::size_t p_byte_size);
		static void write_uint16_le(std::vector<byte>& p_bytes,
			unsigned int p_value);

		static unsigned int read_uint16_le(const std::vector<byte>& p_bytes, std::size_t& p_offset);
		static unsigned int read_uint_le(const std::vector<byte>& p_bytes,
			std::size_t p_byte_size,
			std::size_t& p_offset);

		static void write_vector(std::vector<byte>& p_bytes,
			const std::vector<unsigned int>& p_values, unsigned int p_data_size);
		static void read_vector(const std::vector<byte>& p_bytes, std::size_t& p_offset,
			std::vector<unsigned int>& p_values, unsigned int p_count, unsigned int p_data_size);

	public:
		Savegame(const std::vector<byte>& p_bytes);

		Board get_board(void) const;
		std::vector<byte> get_bytes(void) const;
	};
}

#endif
