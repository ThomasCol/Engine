#ifndef _LOG_H_
#define _LOG_H_

#include <string>

namespace Core::Debug
{
	class Log
	{
	public:
		/* enum for log type */
		enum class Severity
		{
			DEFAULT = 0,
			INFO,
			DEBUG,
			WARNING,
			ERROR
		};

	private:
		/* constructor */
		Log() = default;
		Log(const Log& log) = delete;
		Log(Log&& log) = default;
		/* destructor */
		~Log() = default;

		/* singleton */
		static Log	m_instance;

	public:
		static Log&	Instance();

		/* treat log type */
		void 		LogType(Severity severity, std::string message = "\0");

		/* send log */
		void 		Info(std::string info, std::string time);
		void 		Debug(std::string debug, std::string time);
		void 		Warning(std::string warning, std::string time);
		void 		Error(std::string error, std::string time);
	};

	/* write log on log file */
	void 			WriteLog(std::string log);

	/* clear log file */
	void			ClearLog();

	/* send log time */
	std::string		GetTime();
}

/* macro to stop log */
#ifdef NDEBUG
	#define LOG(severity, message) ((void)0)
	#define	ClearLOG() ((void)0)
#else
	/* macro to call log */
	#define LOG(severity, message)\
	Core::Debug::Log::Instance().LogType(Core::Debug::Log::Severity::severity,\
																		message)
	#define	ClearLOG() Core::Debug::ClearLog()
#endif

#endif //_LOG_H_
