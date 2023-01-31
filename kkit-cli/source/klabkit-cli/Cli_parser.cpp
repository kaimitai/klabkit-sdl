#include <stdexcept>
#include <map>
#include "Cli_parser.h"

kkit_cli::Cli_parser::Cli_parser(const std::vector<std::string>& p_args)
	: version{ 0 }, wall_count{ 0 }, board_count{ 0 }, compress{ false }
{

	for (int i{ 0 }; i < p_args.size(); ++i) {

		if (this->is_command(p_args[i])) {

			auto l_cmd = this->get_command(p_args[i]);

			// commands d and c do not require an argument value (no argument value means compress or decompress all)
			if (l_cmd == "c" || l_cmd == "d") {
				if (i < p_args.size() - 1)
					if (this->is_command(p_args[i + 1]))
						arg_dict.insert(std::make_pair(l_cmd, "all"));
					else
						arg_dict.insert(std::make_pair(l_cmd, p_args[++i]));
				else
					arg_dict.insert(std::make_pair(l_cmd, "all"));
			}
			// other commands require a value
			else {
				if (i == p_args.size() - 1 || this->is_command(p_args[i + 1]))
					throw_exception("Missing command line value for switch -" + p_args[i]);
				else
					arg_dict.insert(std::make_pair(l_cmd, p_args[++i]));
			}
		}
		else {
			throw_exception("Unexpected value: " + p_args[i]);
		}
	}

	this->fill_values();
	arg_dict.clear();
}

void  kkit_cli::Cli_parser::fill_values(void) {
	// check that we either compress, or decompress
	if ((arg_dict.find("c") != end(arg_dict) && arg_dict.find("d") != end(arg_dict))
		|| (arg_dict.find("c") == end(arg_dict) && arg_dict.find("d") == end(arg_dict)))
		throw_exception("One of the switches -c and -d must be given.");

	this->compress = (arg_dict.find("c") != end(arg_dict));
	if (this->compress)
		this->compress_type = arg_dict["c"];
	else
		this->compress_type = arg_dict["d"];

	if (arg_dict.find("v") == end(arg_dict))
		arg_dict.insert(std::make_pair("v", "21"));

	this->version = std::atoi(arg_dict["v"].c_str());
	if (version != 21 && version != 20 && version != 11 && version != 10 && version != 0)
		throw_exception("Invalid version: " + std::to_string(version));

	if (arg_dict.find("f") == end(arg_dict))
		this->folder = ".";
	else
		this->folder = arg_dict["f"];

	if (arg_dict.find("b") == end(arg_dict)) {
		if (this->version == 21 || this->version == 20)
			this->board_count = 30;
		else if (this->version == 11)
			this->board_count = 27;
		else
			this->board_count = 5; // walken or 1.0
	}
	else
		this->board_count = std::atoi(arg_dict["b"].c_str());

	if (arg_dict.find("b") == end(arg_dict)) {
		if (this->version == 21 || this->version == 20)
			this->wall_count = 448;
		else if (this->version == 11)
			this->wall_count = 224;
		else if (this->version == 10)
			this->wall_count = 192;
		else
			this->wall_count = 80; // walken
	}
	else
		this->wall_count = std::atoi(arg_dict["w"].c_str());

}

bool kkit_cli::Cli_parser::is_command(const std::string& p_arg) const {
	return p_arg.at(0) == '-';
}

std::string kkit_cli::Cli_parser::get_command(const std::string& p_arg) const {
	std::string result = p_arg.substr(1);

	if (COMMANDS.find(result) == end(COMMANDS))
		throw_exception("Invalid command switch -" + result);
	else
		return result;
}

void kkit_cli::Cli_parser::throw_exception(const std::string& p_err_msg) const {
	throw std::runtime_error(p_err_msg);
}

bool kkit_cli::Cli_parser::is_compress(void) const {
	return this->compress;
}

int kkit_cli::Cli_parser::get_board_count(void) const {
	return this->board_count;
}

int kkit_cli::Cli_parser::get_wall_count(void) const {
	return this->wall_count;
}

int kkit_cli::Cli_parser::get_version(void) const {
	return this->version;
}

std::string kkit_cli::Cli_parser::get_compress_type(void) const {
	return this->compress_type;
}

std::string kkit_cli::Cli_parser::get_folder(void) const {
	return this->folder;
}
