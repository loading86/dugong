#include "logger.h"
#include <inttypes.h>
namespace raftcore
{
    class VolatileState
    {
    public:
        VolatileState();
        uint64_t LastIndex();
        uint64_t TermOf(uint64_t index);
        bool Compact(uint64_t index, uint64_t term);
        bool ClearSnapshot(uint64_t index);
        void Append(const std::deque<raftpb::Entry>& entries);
        void Load(std::shared_ptr<raftpb::Snapshot> snapshot);
    private:
        std::shared_ptr<raftpb::Snapshot> m_snapshot;
        std::deque<raftpb::Entry> m_ents;
        uint64_t m_offset;
    };
}