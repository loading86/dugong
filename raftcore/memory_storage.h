#pragma once
#include <inttypes.h>
#include <memory>
#include "logger.h"
#include "protocol.h"
namespace raftcore
{
    class MemoryStorage
    {
    public:
        MemoryStorage();
        void SetLogger(std::shared_ptr<Logger> logger){ m_logger = logger;}
        HardState GetHardState();
        ConfState GetConfState();
        bool ApplySnapshot(std::shared_ptr<Snapshot> snapshot);
        uint64_t FirstIndex();
        uint64_t LastIndex();
        bool TermOf(uint64_t index, uint64_t& term);
        
    private:
        std::shared_ptr<Snapshot> m_snapshot;
        HardState m_hard_state;
        std::deque<Entry> m_entries;
        std::shared_ptr<Logger> m_logger;
    };
}
