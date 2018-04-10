#pragma once
#include <inttypes.h>
#include <memory>
#include "logger.h"
namespace raftcore
{
    class MemoryStorage
    {
    public:
        MemoryStorage();
    private:
        std::shared_ptr<Snapshot> m_snapshot;
        std::deque<Entry> m_ents;
        std::shared_ptr<Logger> m_logger;
    };
}
