#include <iostream>
#include <string>
#include "main.hpp"
#include "ParseCheckList.hpp"


int main(int argc, const char* argv[])
{
	try
	{
		std::unique_ptr<std::fstream> inutput_stream_ptr = make_input_stream(argc, argv);
		std::unique_ptr<std::ostream, delete_if_not_cout> output_stream_ptr = make_output_stream(argc, argv);

		std::string header;
		std::getline(*inutput_stream_ptr, header);

		char delim = detect_delim(header);

		if (delim == '\0')
		{
			throw std::logic_error{ "can't find delim" };
		}

		check_list report = parse_check_list(*inutput_stream_ptr, delim);
		sent_check_list(*output_stream_ptr, report, delim);
	}
	catch (const std::exception& except)
	{
		std::cerr << except.what();
	}

    return 0;
}

void delete_if_not_cout::operator ()(std::ostream* stream)
{
	using namespace std;

	if (stream != &std::cout)
	{
		delete stream;
	}
}

std::unique_ptr<std::fstream> make_input_stream(int argc, const char* argv[])
{
	if (argc < 2)
	{
		throw std::runtime_error{ "Input file doesn't set" };
	}

	std::string path{ argv[1] };
	std::unique_ptr<std::fstream> fstream_ptr(new std::fstream{ path, std::ios_base::in });

	if (!fstream_ptr->is_open())
	{
		throw std::runtime_error{ "failed to open file " + path };
	}

	return std::unique_ptr<std::fstream> { fstream_ptr.release() };
}

std::unique_ptr<std::ostream, delete_if_not_cout> make_output_stream(int argc, const char* argv[])
{
	if (argc < 3)
	{
		return std::unique_ptr<std::ostream, delete_if_not_cout> {&std::cout};
	}

	std::string path{ argv[2] };
	std::unique_ptr<std::ofstream> ofstream_ptr(new std::ofstream{ path, std::ios_base::trunc });

	if (!ofstream_ptr->is_open())
	{
		throw std::runtime_error{ "failed to open file " + path };
	}

	return std::unique_ptr<std::ostream, delete_if_not_cout> { ofstream_ptr.release() };
}