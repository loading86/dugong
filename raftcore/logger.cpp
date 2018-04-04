#include "logger.h"
namespace raftcore
{
Logger::Logger()
{
	m_logger = spdlog::rotating_logger_mt("raft", "logs/raft_log", 10 * 1024 * 1024, 3);
}	

void Logger::SetLevel(LogLevel lvl)
{
	int int_lvl = static_cast<int>(lvl);
	m_logger->set_level(static_cast<spdlog::level::level_enum>(int_lvl));
}	
}

