#pragma once
#include <inttypes.h>
#include <memory>
#include "protocol.h"
#include "logger.h"

namespace raftcore
{
    class VolatileState
    {
    public:
        VolatileState(std::shared_ptr<Logger> logger);
        bool LastIndex(uint64_t& index);
        bool TermOf(uint64_t index, uint64_t& term);
        bool ClearEntriesBefore(uint64_t index, uint64_t term);
        bool ClearSnapshot(uint64_t index);
        void Append(const std::deque<Entry>& entries, uint64_t offset);
        void Load(std::shared_ptr<Snapshot> snapshot);
        bool FirstUnstableIndex(uint64_t& index);
        void SetOffset(uint64_t offset){m_offset = offset;}
    private:
        std::shared_ptr<Snapshot> m_snapshot;
        std::deque<Entry> m_entries;
        uint64_t m_offset;
        std::shared_ptr<Logger> m_logger;
    };
}