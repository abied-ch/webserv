#ifndef CONFIG_HPP
#define CONFIG_HPP
#pragma once
#include "Headers.hpp"

// typedef std::map <std::string, std::map <std::string, std::map <std::string, std::map <std::string, std::vector <std::string> > > > > config_map;
class Config 
{
	public:
	
		Config(const std::string& path="config/webserv.conf");
		~Config();

		void												load_config_from_file(const std::string& config_path);
		void												parse_config_from_vector(const std::vector < std::pair <std::string, int> >& config);
		void												handle_opening_brace(const std::pair <std::string, int>& prev_line);
		void												handle_closing_brace(const std::pair <std::string, int>& prev_line);
		void												store_key_value_pairs(const std::pair <std::string, int> prev_line);
		void												validate_nesting(int line_count);
		void												validate_config_header(const std::vector <std::pair <std::string, int> >& config);
		void												validate_config(const std::string& config);
		std::string											remove_comments(const std::string& config);

		std::string											error_on_line(const std::string& issue, int line_count);
		std::string											error(const std::string& message);

		std::map <std::string, std::vector <std::string> >	get_config() const;

	private:
	
		std::string											_config_file_path;
		std::stack <std::string> 							_nesting_level;		// keeps track of nesting level
		std::map <std::string, std::vector <std::string> >	_config;

		Config(const Config& rhs);
		Config&	operator=(const Config& rhs);
};

std::ostream& operator<<(std::ostream& os, const Config& config);

#endif