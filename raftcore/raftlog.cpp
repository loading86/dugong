#include "raftlog.h"

namespace raftcore
{
    RaftLog::RaftLog(std::shared_ptr<MemoryStorage> storage, std::shared_ptr<Logger> logger)
    {
        m_storage = storage;
        m_logger = logger;
        m_volatile_state = std::shared_ptr<VolatileState>(new VolatileState());
    }
}