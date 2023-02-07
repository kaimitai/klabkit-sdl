#include "Savegame.h"
#include <stdexcept>

constexpr char SAVE_CODE_BOARDNUM[]{ "boardnum" };

std::vector<std::pair<std::string, std::size_t>> kkit::Savegame::m_variable_sizes = { {"hiscorenamstat", 1}, {SAVE_CODE_BOARDNUM, 2}, {"scorecount", 4}, {"scoreclock", 4},
		{"skilevel", 2}, {"life", 2}, {"death", 2}, {"lifevests", 2}, {"lightnings", 2}, {"firepowers_0", 2}, {"firepowers_1", 2}, {"firepowers_2", 2}, {"bulchoose", 2}, {"keys", 4}, {"coins", 2}, {"compass", 2}, {"cheated", 2}, {"animate2", 2}, {"animate3", 2}, {"animate4", 2}, {"oscillate3", 2}, {"oscillate5", 2}, {"animate6", 2}, {"animate7", 2}, {"animate8", 2}, {"animate10", 2}, {"animate11", 2}, {"animate15", 2}, {"statusbar", 2}, {"statusbargoal", 2}, {"posx", 2}, {"posy", 2}, {"posz", 2}, {"ang", 2}, {"startx", 2}, {"starty", 2}, {"startang", 2}, {"angvel", 2}, {"vel", 2}, {"mxvel", 2}, {"myvel", 2}, {"svel", 2}, {"hvel", 2}, {"oldposx", 2}, {"oldposy", 2},
		{"bulnum", 2}, {"bultype_todo", 2}, {"bulang", 2}, {"bulx", 2}, {"buly", 2}, {"bulstat", 4},
		{"lastbulshoot", 4},
		{"mnum", 2}, {"mposx", 2}, {"mposy", 2}, {"mgolx", 2}, {"mgoly", 2}, {"moldx", 2}, {"moldy", 2}, {"mstat", 2}, {"mshock", 2}, {"mshot", 1},
		{"doorx", 2}, {"doory", 2}, {"doorstat", 2},
		{"numwarps", 1}, {"justwarped", 1}, {"xwarp", 1}, {"ywarp", 1},
		{"totalclock", 4}, {"purpletime", 4}, {"greentime", 4}, {"capetime_0", 4}, {"capetime_1", 4}, {"musicstatus", 4}, {"clockspeed", 2}, {"count", 4}, {"countstop", 4}, {"nownote", 2}, {"junk", 2},
		{"chanage", 4}, {"chanfreq", 1},
		{"midiinst", 2}, {"mute", 2}, {"namrememberstat", 1}, {"fadewarpval", 2}, {"fadehurtval", 2}, {"slottime", 2}, {"slotpos_0", 2}, {"slotpos_1", 2}, {"slotpos_2", 2}, {"owecoins", 2}, {"owecoinwait", 2} };

void kkit::Savegame::read_variable_range(std::size_t p_start, std::size_t p_end, const std::vector<byte>& p_bytes, std::size_t& p_offset, std::size_t p_count) {
	for (std::size_t i{ p_start }; i <= p_end; ++i)
		read_variable(m_variable_sizes[i].first, p_bytes, p_offset, m_variable_sizes[i].second, p_count);
}

kkit::Savegame::Savegame(const std::vector<byte>& p_bytes) :
	m_board{ std::vector<byte>(begin(p_bytes) + 27, begin(p_bytes) + 27 + 8192) }
{
	for (std::size_t i{ 0 }; i < 16 && p_bytes.at(i) != 0x00; ++i)
		m_hiscore_name.push_back(p_bytes.at(i));

	// skip hiscore name
	std::size_t l_offset{ 16 };

	read_variable_range(0, 3, p_bytes, l_offset);

	// skip board data
	l_offset += 8192;

	read_variable_range(4, 45, p_bytes, l_offset);

	// bullets
	read_variable_range(46, 50, p_bytes, l_offset, get_variable_value("bulnum"));
	read_variable_range(51, 52, p_bytes, l_offset);

	// enemies
	read_variable_range(53, 61, p_bytes, l_offset, get_variable_value("mnum"));
	read_variable_range(62, 66, p_bytes, l_offset);

	// warps
	read_variable_range(67, 68, p_bytes, l_offset, get_variable_value("numwarps"));
	read_variable_range(69, 79, p_bytes, l_offset);

	// channels
	read_variable_range(80, 81, p_bytes, l_offset, 18);
	read_variable_range(82, 92, p_bytes, l_offset);

	// store extra unmapped bytes
	m_unknown_bytes = std::vector<byte>(begin(p_bytes) + l_offset,
		end(p_bytes));

	// set the board start position based on the board start-variables in the savefile
	int l_px{ static_cast<int>(get_variable_value("startx") / 1024) };
	int l_py{ static_cast<int>(get_variable_value("starty") / 1024) };
	int l_startang{ static_cast<int>(get_variable_value("startang")) };
	kkit::Player_direction l_dir{ kkit::Player_direction::Up };
	if (l_startang < 512)
		l_dir = kkit::Player_direction::Right;
	else if (l_startang < 512 * 2)
		l_dir = kkit::Player_direction::Down;
	else if (l_startang < 512 * 3)
		l_dir = kkit::Player_direction::Left;
	m_board.set_player_start_position(l_px, l_py, l_dir);
}

kkit::Savegame::Savegame(const std::string& p_hiscore_name,
	const std::map<std::string, std::vector<unsigned int>>& p_var_values,
	const kkit::Board& p_board,
	const std::vector<byte>& p_unknown_bytes) :
	m_hiscore_name{ p_hiscore_name },
	m_variable_values{ p_var_values },
	m_board{ p_board },
	m_unknown_bytes{ p_unknown_bytes }
{ }

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

void kkit::Savegame::write_variable_range(std::size_t p_start, std::size_t p_end, std::vector<byte>& p_bytes) const {
	for (std::size_t i{ p_start }; i <= p_end; ++i) {
		const auto& l_key = m_variable_sizes[i].first;
		std::size_t l_data_size = m_variable_sizes[i].second;

		if (m_variable_values.find(l_key) != end(m_variable_values))
			for (unsigned int n : m_variable_values.at(l_key))
				write_uint_le(p_bytes, n, l_data_size);
	}
}

std::vector<byte> kkit::Savegame::get_bytes(void) const {
	std::vector<byte> result{ std::vector<byte>(begin(m_hiscore_name), end(m_hiscore_name)) };

	while (result.size() < 16)
		result.push_back(0x00);

	write_variable_range(0, 3, result);

	// get board bytes, but remove player start
	auto l_board_bytes{ m_board.get_bytes(false) };
	result.insert(end(result),
		begin(l_board_bytes), end(l_board_bytes));

	write_variable_range(4, m_variable_sizes.size() - 1, result);

	result.insert(end(result),
		begin(m_unknown_bytes), end(m_unknown_bytes));

	return result;
}

unsigned int kkit::Savegame::get_variable_value(const std::string& p_key, std::size_t p_slot) const {
	auto iter{ m_variable_values.find(p_key) };
	if (iter == end(m_variable_values))
		return 0;
	else
		return iter->second.at(p_slot);
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

void kkit::Savegame::set_board(const kkit::Board& p_board) {
	m_board = p_board;

	m_variable_values["startx"] = { static_cast<unsigned int>(m_board.get_player_start_x()) * 1024 + 512 };
	m_variable_values["starty"] = { static_cast<unsigned int>(m_board.get_player_start_y()) * 1024 + 512 };

	auto l_dir{ m_board.get_player_start_direction() };
	unsigned int l_dir_angle{ 0 };

	if (l_dir == kkit::Player_direction::Down)
		l_dir_angle = 512;
	else if (l_dir == kkit::Player_direction::Left)
		l_dir_angle = 512 * 2;
	else if (l_dir == kkit::Player_direction::Up)
		l_dir_angle = 512 * 3;

	m_variable_values["startang"] = { l_dir_angle };

}

const std::map<std::string, std::vector<unsigned int>>& kkit::Savegame::get_variable_values(void) const {
	return m_variable_values;
}

const std::vector<byte>& kkit::Savegame::get_unknown_bytes(void) const {
	return m_unknown_bytes;
}

std::vector<std::string> kkit::Savegame::get_variable_order(void) {
	std::vector<std::string> result;
	for (const auto& kv : m_variable_sizes)
		result.push_back(kv.first);
	return result;
}

kkit::Board kkit::Savegame::get_board(void) const {
	return m_board;
}

const std::string& kkit::Savegame::get_hiscore_name(void) const {
	return m_hiscore_name;
}

unsigned int kkit::Savegame::get_board_num(void) const {
	return this->get_variable_value(SAVE_CODE_BOARDNUM);
}

void kkit::Savegame::read_variable(const std::string& p_key, const std::vector<byte>& p_bytes, std::size_t& p_offset, std::size_t p_byte_size, std::size_t p_count) {
	for (std::size_t i{ 0 }; i < p_count; ++i)
		m_variable_values[p_key].push_back(read_uint_le(p_bytes, p_byte_size, p_offset));
}
