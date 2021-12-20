#pragma once
#include <memory>

struct delete_if_not_cout
{
	void operator ()(std::ostream* stream);
};

std::unique_ptr<std::fstream> make_input_stream(int argc, const char* argv[]);

std::unique_ptr<std::ostream, delete_if_not_cout> make_output_stream(int argc, const char* argv[]);

int main(int argc, const char* argv[]);