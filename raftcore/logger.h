#pragma once
#include "spdlog/spdlog.h"
namespace raftcore
{
enum class LogLevel
{
	TRACE,
	DEBUG,
	INFO,
	WARN,
	ERROR,
	CRITICAL,
	OFF
};
class Logger
{
public:
	Logger();
	void SetLevel(LogLevel lvl);
	template<typename... Params>
	void Info(const std::string& format, Params&& ...params)
	{
		m_logger->info(format.c_str(), params...);
	}
	
	template<typename... Params>
	void Debug(const std::string& format, Params&& ...params)
	{
		m_logger->debug(format.c_str(), params...);
	}
	
	template<typename... Params>
	void Warn(const std::string& format, Params&& ...params)
	{
		m_logger->warn(format.c_str(), params...);
	}
	
	template<typename... Params>
	void Error(const std::string& format, Params&& ...params)
	{
		m_logger->error(format.c_str(), params...);
	}
	
	template<typename... Params>
	void Trace(const std::string& format, Params&& ...params)
	{
		m_logger->trace(format.c_str(), params...);
	}

	template<typename... Params>
	void Panic(const std::string& format, Params&& ...params)
	{
		m_logger->trace(format.c_str(), params...);
		exit(0);
	}
private:
	std::shared_ptr<spdlog::logger> m_logger;
};	
}

