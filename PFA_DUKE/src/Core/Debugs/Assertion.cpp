#include "Assertion.h"
#include "Log.h"

#include <iostream>

namespace Core::Debug
{
	void	StaticAssert(bool condition, std::string file, int line)
	{
		if (!condition)
		{
			std::string	time = GetTime();

			std::string log {"[Static Assert]  " + time + "\n\t> in file " + file +
			", line " +	std::to_string(line) + ".\n"};

			std::cout << log;

			WriteLog(log);

			exit(0);
		}
	}

	void	Assert(bool condition, std::string file, int line)
	{
		if (!condition)
		{
			std::string	time = GetTime();

			std::string log {"[Assert]  " + time + "\n\t> in file " + file +
			", line " +	std::to_string(line) + ".\n"};

			std::cout << log;

			WriteLog(log);
		}
	}
}
