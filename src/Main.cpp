#include <iostream>

#include <cstdio>

#include "Core/Appplication.hpp"

int main(const int argc, const char* argv[])
{
	if (argc < 2)
	{
		std::cout << "1st parameter expects the example number:\n\n"
				  << ogl::Application::getHelp() << std::flush;
		return 0;
	}

	return ogl::Application::runProgramNumber(::atoi(argv[1]));
}
