#pragma once
#include <fstream>
#include <ostream>
#include <string>
#include <unordered_map>

struct Profile
{
	std::string name;
	std::string month;
	std::string year;
	std::string hours;
};

using check_list = std::unordered_map<std::string, std::vector<Profile>>;

check_list parse_check_list(std::fstream& stream_in, char delim);

void sent_check_list(std::ostream& stream_out, const check_list& storage, char delim);

char detect_delim(const std::string& header);