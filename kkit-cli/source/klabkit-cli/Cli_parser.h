#ifndef KKIT_CLI_CLI_PARSER_H
#define KKIT_CLI_CLI_PARSER_H

#include <map>
#include <set>
#include <string>
#include <vector>

namespace kkit_cli {

	class Cli_parser {

		// [c]ompress, [d]ecompress, [v]ersion, [f]older, [b]oard count, [w]all count
		const std::set<std::string> COMMANDS{ "c", "d", "v", "f", "b", "w" };

		std::map<std::string, std::string> arg_dict;
		std::string folder;
		int board_count, wall_count, version;
		bool compress;
		std::string compress_type;

		bool is_command(const std::string& p_arg) const;
		std::string get_command(const std::string& p_arg) const;

		void fill_values(void);
		void throw_exception(const std::string& p_err_msg) const;

	public:
		Cli_parser(const std::vector<std::string>& p_arguments);

		static std::string to_lowercase(const std::string& p_arg);
		bool is_compress(void) const;
		int get_board_count(void) const;
		int get_wall_count(void) const;
		int get_version(void) const;
		std::string get_compress_type(void) const;
		std::string get_folder(void) const;
	};

}

#endif
