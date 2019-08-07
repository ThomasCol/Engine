#include "Log.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

namespace Core::Debug
{

	Log	Log::m_instance {Log()};

	Log&	Log::Instance()
	{
		return m_instance;
	}

	void 	Log::LogType(Severity severity, std::string message)
	{
		std::string	time = GetTime();

		switch (severity)
		{
			case Severity::INFO:
				Info(message, time);
				break;

			case Severity::DEBUG:
				Debug(message, time);
				break;

			case Severity::WARNING:
				Warning(message, time);
				break;

			case Severity::ERROR:
				Error(message, time);
				break;

			default:
				break;
		}
	}

	void 	Log::Info(std::string info, std::string time)
	{
		std::string	log = "[Info]  " + time + "\n\t> " + info + "\n";

		std::cout << log;

		WriteLog(log);
	}

	void 	Log::Debug(std::string debug, std::string time)
	{
		std::string	log = "[Debug]  " + time + "\n\t> " + debug + "\n";

		std::cout << log;

		WriteLog(log);
	}

	void 	Log::Warning(std::string warning, std::string time)
	{
		std::string	log = "[Warning]  " + time + "\n\t> " + warning + "\n";

		std::cout << log;

		WriteLog(log);
	}

	void 	Log::Error(std::string error, std::string time)
	{
		std::string	log = "[Error]  " + time + "\n\t> " + error + "\n";

		std::cout << log;

		WriteLog(log);
	}

	void 	ClearLog()
	{
		std::ofstream	logFile("ressource/log.txt", std::ofstream::trunc);

		if (logFile.is_open())
		{
			logFile.close();
		}
	}

	void 	WriteLog(std::string log)
	{
		std::ofstream	logFile("ressource/log.txt", std::ios::app);

		if (logFile.is_open())
		{
			logFile << log;
			logFile.close();
		}
	}

	std::string	GetTime()
	{
		time_t theTime = time(NULL);
		struct tm *aTime = localtime(&theTime);

		int	hour = aTime->tm_hour;
		int	min = aTime->tm_min;
		int	sec = aTime->tm_sec;

		std::string time = std::to_string(hour) + ":" +	std::to_string(min) + ":" +
		std::to_string(sec);

		return time;
	}
}
