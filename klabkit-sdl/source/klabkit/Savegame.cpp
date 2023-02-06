#include "Savegame.h"
#include <stdexcept>

kkit::Savegame::Savegame(const std::vector<byte>& p_bytes) :
	m_board{ std::vector<byte>(begin(p_bytes) + 27, begin(p_bytes) + 27 + 8192) }
{
	hiscorenam = std::string(begin(p_bytes), begin(p_bytes) + 16);
	std::size_t l_offset{ 16 };

	hiscorenamstat = read_uint_le(p_bytes, 1, l_offset);
	boardnum = read_uint16_le(p_bytes, l_offset);
	scorecount = read_uint_le(p_bytes, 4, l_offset);
	scoreclock = read_uint_le(p_bytes, 4, l_offset);

	l_offset += 8192; // skip board data

	skilevel = read_uint16_le(p_bytes, l_offset);
	life = read_uint16_le(p_bytes, l_offset);
	death = read_uint16_le(p_bytes, l_offset);
	lifevests = read_uint16_le(p_bytes, l_offset);
	lightnings = read_uint16_le(p_bytes, l_offset);
	firepowers_0 = read_uint16_le(p_bytes, l_offset);
	firepowers_1 = read_uint16_le(p_bytes, l_offset);
	firepowers_2 = read_uint16_le(p_bytes, l_offset);
	bulchoose = read_uint16_le(p_bytes, l_offset);
	keys = read_uint_le(p_bytes, 4, l_offset);
	coins = read_uint16_le(p_bytes, l_offset);
	compass = read_uint16_le(p_bytes, l_offset);
	cheated = read_uint16_le(p_bytes, l_offset);
	animate2 = read_uint16_le(p_bytes, l_offset);
	animate3 = read_uint16_le(p_bytes, l_offset);
	animate4 = read_uint16_le(p_bytes, l_offset);
	oscillate3 = read_uint16_le(p_bytes, l_offset);
	oscillate5 = read_uint16_le(p_bytes, l_offset);
	animate6 = read_uint16_le(p_bytes, l_offset);
	animate7 = read_uint16_le(p_bytes, l_offset);
	animate8 = read_uint16_le(p_bytes, l_offset);
	animate10 = read_uint16_le(p_bytes, l_offset);
	animate11 = read_uint16_le(p_bytes, l_offset);
	animate15 = read_uint16_le(p_bytes, l_offset);
	statusbar = read_uint16_le(p_bytes, l_offset);
	statusbargoal = read_uint16_le(p_bytes, l_offset);
	posx = read_uint16_le(p_bytes, l_offset);
	posy = read_uint16_le(p_bytes, l_offset);
	posz = read_uint16_le(p_bytes, l_offset);
	ang = read_uint16_le(p_bytes, l_offset);
	startx = read_uint16_le(p_bytes, l_offset);
	starty = read_uint16_le(p_bytes, l_offset);
	startang = read_uint16_le(p_bytes, l_offset);
	angvel = read_uint16_le(p_bytes, l_offset);
	vel = read_uint16_le(p_bytes, l_offset);
	mxvel = read_uint16_le(p_bytes, l_offset);
	myvel = read_uint16_le(p_bytes, l_offset);
	svel = read_uint16_le(p_bytes, l_offset);
	hvel = read_uint16_le(p_bytes, l_offset);
	oldposx = read_uint16_le(p_bytes, l_offset);
	oldposy = read_uint16_le(p_bytes, l_offset);

	// bullets
	bulnum = read_uint16_le(p_bytes, l_offset);

	read_vector(p_bytes, l_offset, bultype_todo, bulnum, 2);
	read_vector(p_bytes, l_offset, bulang, bulnum, 2);
	read_vector(p_bytes, l_offset, bulx, bulnum, 2);
	read_vector(p_bytes, l_offset, buly, bulnum, 2);
	read_vector(p_bytes, l_offset, bulstat, bulnum, 4);

	lastbulshoot = read_uint_le(p_bytes, 4, l_offset);

	// enemies
	mnum = read_uint16_le(p_bytes, l_offset);

	read_vector(p_bytes, l_offset, mposx, mnum, 2);
	read_vector(p_bytes, l_offset, mposy, mnum, 2);
	read_vector(p_bytes, l_offset, mgolx, mnum, 2);
	read_vector(p_bytes, l_offset, mgolx, mnum, 2);
	read_vector(p_bytes, l_offset, moldx, mnum, 2);
	read_vector(p_bytes, l_offset, moldy, mnum, 2);
	read_vector(p_bytes, l_offset, mstat, mnum, 2);
	read_vector(p_bytes, l_offset, mshock, mnum, 2);
	read_vector(p_bytes, l_offset, mshot, mnum, 1);

	doorx = read_uint16_le(p_bytes, l_offset);
	doory = read_uint16_le(p_bytes, l_offset);
	doorstat = read_uint16_le(p_bytes, l_offset);

	// warps
	numwarps = read_uint_le(p_bytes, 1, l_offset);
	justwarped = read_uint_le(p_bytes, 1, l_offset);
	read_vector(p_bytes, l_offset, xwarp, numwarps, 1);
	read_vector(p_bytes, l_offset, ywarp, numwarps, 1);

	totalclock = read_uint_le(p_bytes, 4, l_offset);
	purpletime = read_uint_le(p_bytes, 4, l_offset);
	greentime = read_uint_le(p_bytes, 4, l_offset);
	capetime_0 = read_uint_le(p_bytes, 4, l_offset);
	capetime_1 = read_uint_le(p_bytes, 4, l_offset);
	musicstatus = read_uint_le(p_bytes, 4, l_offset);
	clockspeed = read_uint16_le(p_bytes, l_offset);
	count = read_uint_le(p_bytes, 4, l_offset);
	countstop = read_uint_le(p_bytes, 4, l_offset);
	nownote = read_uint16_le(p_bytes, l_offset);
	junk = read_uint16_le(p_bytes, l_offset);

	// channels
	read_vector(p_bytes, l_offset, chanage, 18, 4);
	read_vector(p_bytes, l_offset, chanfreq, 18, 1);

	midiinst = read_uint16_le(p_bytes, l_offset);
	mute = read_uint16_le(p_bytes, l_offset);
	namrememberstat = read_uint_le(p_bytes, 1, l_offset);
	fadewarpval = read_uint16_le(p_bytes, l_offset);
	fadehurtval = read_uint16_le(p_bytes, l_offset);
	slottime = read_uint16_le(p_bytes, l_offset);
	slotpos_0 = read_uint16_le(p_bytes, l_offset);
	slotpos_1 = read_uint16_le(p_bytes, l_offset);
	slotpos_2 = read_uint16_le(p_bytes, l_offset);
	owecoins = read_uint16_le(p_bytes, l_offset);
	owecoinwait = read_uint16_le(p_bytes, l_offset);

	// store extra unmapped bytes
	m_unknown_bytes = std::vector<byte>(begin(p_bytes) + l_offset,
		end(p_bytes));

	// set the board start position based on the board start-variables in the savefile
	int l_px{ static_cast<int>(startx / 1024) };
	int l_py{ static_cast<int>(starty / 1024) };
	kkit::Player_direction l_dir{ kkit::Player_direction::Up };
	if (startang < 512)
		l_dir = kkit::Player_direction::Right;
	else if (startang < 1024)
		l_dir = kkit::Player_direction::Down;
	else if (startang < 1536)
		l_dir = kkit::Player_direction::Left;
	m_board.set_player_start_position(l_px, l_py, l_dir);
}

std::vector<byte> kkit::Savegame::get_bytes(void) const {
	std::vector<byte> result(begin(hiscorenam), end(hiscorenam));

	write_uint_le(result, hiscorenamstat, 1);
	write_uint16_le(result, boardnum);
	write_uint_le(result, scorecount, 4);
	write_uint_le(result, scoreclock, 4);

	// get board bytes, but remove player start
	auto l_board_bytes{ m_board.get_bytes(false) };
	result.insert(end(result),
		begin(l_board_bytes), end(l_board_bytes));

	write_uint16_le(result, skilevel);
	write_uint16_le(result, life);
	write_uint16_le(result, death);
	write_uint16_le(result, lifevests);
	write_uint16_le(result, lightnings);
	write_uint16_le(result, firepowers_0);
	write_uint16_le(result, firepowers_1);
	write_uint16_le(result, firepowers_2);
	write_uint16_le(result, bulchoose);
	write_uint_le(result, keys, 4);
	write_uint16_le(result, coins);
	write_uint16_le(result, compass);
	write_uint16_le(result, cheated);
	write_uint16_le(result, animate2);
	write_uint16_le(result, animate3);
	write_uint16_le(result, animate4);
	write_uint16_le(result, oscillate3);
	write_uint16_le(result, oscillate5);
	write_uint16_le(result, animate6);
	write_uint16_le(result, animate7);
	write_uint16_le(result, animate8);
	write_uint16_le(result, animate10);
	write_uint16_le(result, animate11);
	write_uint16_le(result, animate15);
	write_uint16_le(result, statusbar);
	write_uint16_le(result, statusbargoal);
	write_uint16_le(result, posx);
	write_uint16_le(result, posy);
	write_uint16_le(result, posz);
	write_uint16_le(result, ang);
	write_uint16_le(result, startx);
	write_uint16_le(result, starty);
	write_uint16_le(result, startang);
	write_uint16_le(result, angvel);
	write_uint16_le(result, vel);
	write_uint16_le(result, mxvel);
	write_uint16_le(result, myvel);
	write_uint16_le(result, svel);
	write_uint16_le(result, hvel);
	write_uint16_le(result, oldposx);
	write_uint16_le(result, oldposy);

	write_uint16_le(result, bulnum);
	write_vector(result, bultype_todo, 2);
	write_vector(result, bulang, 2);
	write_vector(result, bulx, 2);
	write_vector(result, buly, 2);
	write_vector(result, bulstat, 4);

	write_uint_le(result, lastbulshoot, 4);

	write_uint16_le(result, mnum);
	write_vector(result, mposx, 2);
	write_vector(result, mposy, 2);
	write_vector(result, mgolx, 2);
	write_vector(result, mgoly, 2);
	write_vector(result, moldx, 2);
	write_vector(result, moldy, 2);
	write_vector(result, mstat, 2);
	write_vector(result, mshock, 2);
	write_vector(result, mshot, 1);

	write_uint16_le(result, doorx);
	write_uint16_le(result, doory);
	write_uint16_le(result, doorstat);

	write_uint_le(result, numwarps, 1);
	write_uint_le(result, justwarped, 1);

	write_vector(result, xwarp, 1);
	write_vector(result, ywarp, 1);

	write_uint_le(result, totalclock, 4);
	write_uint_le(result, purpletime, 4);
	write_uint_le(result, greentime, 4);
	write_uint_le(result, capetime_0, 4);
	write_uint_le(result, capetime_1, 4);
	write_uint_le(result, musicstatus, 4);
	write_uint16_le(result, clockspeed);
	write_uint_le(result, count, 4);
	write_uint_le(result, countstop, 4);
	write_uint16_le(result, nownote);
	write_uint16_le(result, junk);

	write_vector(result, chanage, 4);
	write_vector(result, chanfreq, 1);

	write_uint16_le(result, midiinst);
	write_uint16_le(result, mute);
	write_uint_le(result, namrememberstat, 1);
	write_uint16_le(result, fadewarpval);
	write_uint16_le(result, fadehurtval);
	write_uint16_le(result, slottime);
	write_uint16_le(result, slotpos_0);
	write_uint16_le(result, slotpos_1);
	write_uint16_le(result, slotpos_2);
	write_uint16_le(result, owecoins);
	write_uint16_le(result, owecoinwait);

	result.insert(end(result),
		begin(m_unknown_bytes), end(m_unknown_bytes));

	return result;
}

void kkit::Savegame::write_vector(std::vector<byte>& p_bytes,
	const std::vector<unsigned int>& p_values, unsigned int p_data_size) {
	for (auto n : p_values)
		write_uint_le(p_bytes, n, p_data_size);
}

void kkit::Savegame::read_vector(const std::vector<byte>& p_bytes, std::size_t& p_offset,
	std::vector<unsigned int>& p_values, unsigned int p_count, unsigned int p_data_size) {
	for (unsigned int i{ 0 }; i < p_count; ++i)
		p_values.push_back(read_uint_le(p_bytes, p_data_size, p_offset));
}

void kkit::Savegame::write_uint_le(std::vector<byte>& p_bytes,
	unsigned int p_value, std::size_t p_byte_size) {
	unsigned int mod{ 256 };

	for (std::size_t i{ 0 }; i < p_byte_size; ++i) {
		p_bytes.push_back(static_cast<byte>(
			p_value % 256
			));
		p_value /= 256;
	}
}

void kkit::Savegame::write_uint16_le(std::vector<byte>& p_bytes,
	unsigned int p_value) {
	write_uint_le(p_bytes, p_value, 2);
}

unsigned int kkit::Savegame::read_uint16_le(const std::vector<byte>& p_bytes, std::size_t& p_offset) {
	return read_uint_le(p_bytes, 2, p_offset);
}

unsigned int kkit::Savegame::read_uint_le(const std::vector<byte>& p_bytes,
	std::size_t p_byte_size,
	std::size_t& p_offset) {
	unsigned int result{ 0 };
	unsigned int l_mult{ 1 };

	for (std::size_t i{ 0 }; i < p_byte_size; ++i) {
		result += l_mult * static_cast<unsigned int>(p_bytes.at(p_offset++));
		l_mult *= 256;
	}

	return result;
}

kkit::Board kkit::Savegame::get_board(void) const {
	return m_board;
}
