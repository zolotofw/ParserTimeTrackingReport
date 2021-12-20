#include "ParseCheckList.hpp"
#include <iostream>
#include <algorithm>
#include <functional>
#include <limits>
#include <iomanip>
#include <ctime>


void stream_ignore_next_tokens(std::fstream& stream, char delim, size_t how_much);

Profile parse_profile(std::fstream& stream, char delim);

bool if_equal_name_month (const Profile& first, const Profile& second)
{
	return first.name == second.name && first.month == second.month;
}

check_list parse_check_list(std::fstream& stream_in, char delim)
{
	if (!stream_in.is_open())
	{
		throw std::runtime_error{ "enter stream not good" };
	}

	using namespace std::placeholders;
	check_list report;

	while (!stream_in.eof())
	{
		Profile new_profile = parse_profile(stream_in, delim);

		if (!report.contains(new_profile.month))
		{
			report[new_profile.month].push_back(new_profile);

			continue;
		}

		auto find = std::ranges::find_if(report[new_profile.month], std::bind(if_equal_name_month, _1, new_profile));

		if (find != report[new_profile.month].end())
		{
			int prev_time = std::stoi((*find).hours);
			int current_time = std::stoi(new_profile.hours);
			(*find).hours = std::to_string(prev_time + current_time);

			continue;
		}

		report[new_profile.month].push_back(new_profile);
	}

	return report;
}

char detect_delim(const std::string& header)
{
	auto find_delim = std::find_if(header.begin(), header.end(), std::ispunct);

	if (find_delim != header.end())
	{
		return *find_delim;
	}

	return '\0';
}

void stream_ignore_next_tokens(std::fstream& stream, char delim, size_t how_much)
{
	for (size_t i = 0; i < how_much; ++i)
	{
		stream.ignore(std::numeric_limits<std::streamsize>::max(), delim);
	}
}

std::string profile_to_string(const Profile& profile, char delim)
{
	return profile.name + delim + profile.month + " " + profile.year + delim + profile.hours + '\n';
}

void sent_check_list(std::ostream& stream_out, const check_list& storage, char delim)
{
	using namespace std::placeholders;
	
	std::string header{ "Name;Month;Total hours" };
	stream_out << header << std::endl;

	for (const std::pair<std::string, const std::vector<Profile>&> pair : storage)
	{
		if (!stream_out.good())
		{
			throw std::runtime_error{ "Failed write check list" };
		}

		std::ranges::transform(pair.second, std::ostream_iterator<std::string>{stream_out}, std::bind(profile_to_string, _1, delim));
	}
}

Profile parse_profile(std::fstream& stream, char delim)
{
	Profile new_profile;
	std::getline(stream, new_profile.name, delim);

	stream_ignore_next_tokens(stream, delim, 5);
	std::tm date;
	stream >> std::get_time(&date, "%Y-%m-%d;%H");

	char buff[10];
	std::strftime(buff, 10, "%B", &date);
	new_profile.month = buff;
	std::strftime(buff, 10, "%Y", &date);
	new_profile.year = buff;
	std::strftime(buff, 10, "%H", &date);
	new_profile.hours = buff;

	stream.get();
	return new_profile;
}