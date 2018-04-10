#include <inttypes.h>
#include "protocol.h"
#include "logger.h"

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
        void Append(const std::deque<Entry>& entries);
        void Load(std::shared_ptr<Snapshot> snapshot);
    private:
        std::shared_ptr<Snapshot> m_snapshot;
        std::deque<Entry> m_ents;
        uint64_t m_offset;
    };
}